# Analyze Build Errors Workflow - Fix Documentation

## Problem Statement

The "Analyze Build Errors" workflow was not working - it failed to extract TeamCity build IDs from GitHub Actions workflow logs, resulting in no build error analysis being performed.

## Root Cause Analysis

### What Was Happening

The workflow attempted to:
1. Find the TeamCity CI workflow run for a given PR
2. Fetch job logs using the GitHub API endpoint: `GET /repos/{owner}/{repo}/actions/jobs/{job_id}/logs`
3. Parse the logs to extract build IDs using regex: `/Build queued\. ID=(\d+)/`

### Why It Failed

The GitHub API endpoint `/repos/{owner}/{repo}/actions/jobs/{job_id}/logs` **returns a 302 redirect** to the actual log file URL (on Azure Blob Storage or similar), not the log content directly.

When using `github.request()` from the `actions/github-script` action:
- The request follows the redirect
- But returns the redirect response, not the file content
- The workflow tried to match regex on the redirect URL instead of log content
- Result: No build IDs extracted

### Evidence

From workflow run 21761341887:
```
Found TeamCity workflow run: 21760833496
BUILD_IDS=''
Analyzing TeamCity build IDs: 
No TeamCity build IDs found
```

## Solution Implemented

### New Approach: Use Artifacts

Instead of parsing logs, leverage the existing artifact upload in the TeamCity CI workflow:

```yaml
# teamcity-trigger.yml lines 137-145
- name: Upload Build ID Artifact
  uses: actions/upload-artifact@v4
  with:
    name: teamcity-build-info-${{ matrix.platform }}
    path: |
      teamcity-build-id-${{ matrix.platform }}.txt
      teamcity-build-url-${{ matrix.platform }}.txt
```

### Workflow Changes

#### Step 1: Get PR and Workflow Information
```yaml
- name: Get PR and TeamCity Workflow Information
  id: get_info
  uses: actions/github-script@v8
  with:
    script: |
      # Find TeamCity workflow run for the PR's commit
      # Return workflow_run_id for artifact download
```

**Changes:**
- Removed log fetching logic
- Returns `workflow_run_id` instead of `build_ids`
- Cleaner, simpler JavaScript code

#### Step 2: Download Artifacts
```yaml
- name: Download TeamCity Build Info Artifacts
  if: fromJSON(steps.get_info.outputs.result).workflow_run_id != null
  uses: actions/download-artifact@v4
  continue-on-error: true
  with:
    pattern: teamcity-build-info-*
    run-id: ${{ fromJSON(steps.get_info.outputs.result).workflow_run_id }}
    merge-multiple: true
    path: ./teamcity-artifacts
```

**Key points:**
- Uses `actions/download-artifact@v4` with cross-workflow artifact download
- `pattern: teamcity-build-info-*` matches both Windows and Linux artifacts
- `merge-multiple: true` combines artifacts into one directory
- `continue-on-error: true` gracefully handles missing artifacts

#### Step 3: Extract Build IDs
```yaml
- name: Extract Build IDs from Artifacts
  id: extract_build_ids
  shell: bash
  run: |
    BUILD_IDS=""
    
    if [ -d "./teamcity-artifacts" ]; then
      shopt -s nullglob  # Safe glob matching
      for id_file in ./teamcity-artifacts/teamcity-build-id-*.txt; do
        BUILD_ID=$(cat "$id_file" | tr -d '[:space:]')
        if [ -n "$BUILD_ID" ]; then
          BUILD_IDS="$BUILD_IDS $BUILD_ID"
        fi
      done
      shopt -u nullglob
      BUILD_IDS=$(echo "$BUILD_IDS" | xargs)
    fi
    
    echo "build_ids=$BUILD_IDS" >> "$GITHUB_OUTPUT"
```

**Key improvements:**
- Reads build IDs from text files (simple and reliable)
- Uses `nullglob` to prevent issues with non-matching patterns
- Proper whitespace handling
- Clear logging for debugging

#### Step 4: Fetch and Analyze Errors
```yaml
- name: Fetch and Analyze Build Errors
  env:
    TEAMCITY_URL: ${{ vars.TEAMCITY_URL }}
    TEAMCITY_TOKEN: ${{ vars.TEAMCITY_TOKEN }}
  shell: bash
  run: |
    BUILD_IDS='${{ steps.extract_build_ids.outputs.build_ids }}'
    # ... fetch and analyze TeamCity logs
```

**Changes:**
- Now uses `steps.extract_build_ids.outputs.build_ids` instead of JSON parsing
- Rest of the logic remains the same

## Benefits of the Fix

### 1. Reliability
- ✅ Uses official artifact mechanism (designed for cross-workflow data sharing)
- ✅ No dependency on log API redirect behavior
- ✅ Artifacts are designed to be downloaded and read

### 2. Simplicity
- ✅ Removed ~50 lines of complex JavaScript log parsing code
- ✅ Simple file reading instead of HTTP redirects
- ✅ Easier to debug and maintain

### 3. Performance
- ✅ Artifact download is optimized by GitHub
- ✅ No need to fetch entire job logs just to extract one line
- ✅ Faster execution

### 4. Robustness
- ✅ Handles missing artifacts gracefully (`continue-on-error: true`)
- ✅ Safe glob pattern matching (`nullglob`)
- ✅ Works for both Windows and Linux builds

## Testing

### Test Case 1: PR with TeamCity Builds
**PR:** #131 (this PR)  
**Workflow Run:** 21761376535  
**Expected:**
- Download artifacts: `teamcity-build-info-windows`, `teamcity-build-info-linux`
- Extract 2 build IDs
- Fetch and analyze TeamCity logs
- Post comment with errors

### Test Case 2: PR without TeamCity Builds
**Expected:**
- No workflow run found
- Skip artifact download
- Report "No TeamCity build IDs found"

### Test Case 3: PR with Incomplete TeamCity Builds
**Expected:**
- Workflow run exists but no artifacts (builds still running)
- `continue-on-error` prevents workflow failure
- Report "No TeamCity build IDs found"

## How to Use

1. Go to **Actions** → **Analyze Build Errors**
2. Click **"Run workflow"**
3. Enter the PR number
4. Click **"Run workflow"** button
5. Wait for completion
6. Check the PR for a comment with build error analysis

## Future Improvements

1. **Auto-trigger**: Consider triggering automatically when TeamCity CI fails
2. **Better error formatting**: Improve the error extraction and formatting
3. **Link to specific errors**: Add line numbers and file references where possible
4. **Artifact retention**: Ensure artifacts are retained long enough for analysis

## Related Files

- `.github/workflows/analyze-build-errors.yml` - The fixed workflow
- `.github/workflows/teamcity-trigger.yml` - TeamCity CI workflow (uploads artifacts)

## Commit History

1. `f2ac313` - Fix Analyze Build Errors workflow - use artifacts instead of parsing logs
2. `74c2a37` - Improve glob pattern handling in artifact extraction
