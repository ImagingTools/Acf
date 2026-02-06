# Build Fix Workflow - Test Execution Guide

**Date:** 2026-02-06  
**Workflow:** `analyze-build-errors.yml`  
**Purpose:** Step-by-step guide for executing tests

---

## Quick Start

### Run Automated Tests
```bash
cd /home/runner/work/Acf/Acf
bash .github/workflows/test-build-error-analysis.sh
```

This validates:
- ✅ Artifact extraction logic
- ✅ Missing artifacts handling
- ✅ Empty file handling
- ✅ Whitespace trimming
- ✅ Single platform scenarios
- ✅ Error pattern matching
- ✅ Nullglob behavior

---

## Manual Test Execution

### Prerequisites Checklist

Before running manual tests, verify:

1. **Repository Variables Configured**
   ```bash
   # Check if variables are set (requires GitHub CLI with proper permissions)
   gh variable list
   ```
   
   Required variables:
   - `TEAMCITY_URL`
   - `TEAMCITY_TOKEN`
   - `TEAMCITY_BUILD_TYPE_WINDOWS`
   - `TEAMCITY_BUILD_TYPE_LINUX`

2. **Test PR Available**
   - PR with build failures is ideal for testing
   - PR should have TeamCity CI workflow completed
   - Artifacts should be uploaded

3. **Permissions Verified**
   - Workflow has `pull-requests: write`
   - Workflow has `issues: write`
   - Workflow has `actions: read`

---

## Test Scenario 1: Full Build Error Analysis

**Purpose:** Verify complete workflow with failing builds

### Setup

1. Create a test PR with intentional build errors:
   ```bash
   git checkout -b test/build-error-analysis
   
   # Introduce a compilation error
   echo 'syntax error here' >> Include/icomp/CComponentBase.h
   
   git add .
   git commit -m "Test: introduce build error"
   git push origin test/build-error-analysis
   
   # Create PR
   gh pr create --title "Test: Build Error Analysis" \
                --body "Testing build error analysis workflow"
   ```

2. Wait for TeamCity CI to complete:
   ```bash
   # Check workflow status
   gh run list --workflow=teamcity-trigger.yml --limit 5
   
   # Wait for completion
   gh run watch <run-id>
   ```

3. Verify artifacts were uploaded:
   ```bash
   # Get the workflow run ID from above
   gh run view <run-id>
   
   # Check for artifacts
   gh api repos/ImagingTools/Acf/actions/runs/<run-id>/artifacts
   ```

### Execute Test

1. Navigate to GitHub Actions:
   ```
   https://github.com/ImagingTools/Acf/actions/workflows/analyze-build-errors.yml
   ```

2. Click "Run workflow"

3. Enter the PR number from above

4. Click "Run workflow" button

5. Monitor execution:
   ```bash
   # Get the analysis workflow run ID
   gh run list --workflow=analyze-build-errors.yml --limit 1
   
   # Watch the run
   gh run watch <analysis-run-id>
   ```

### Verify Results

1. **Check Workflow Logs**
   ```bash
   gh run view <analysis-run-id> --log
   ```
   
   Expected in logs:
   - ✅ "Found TeamCity workflow run: <id>"
   - ✅ "Artifacts directory found, extracting build IDs..."
   - ✅ "Found build ID: <id> (from teamcity-build-id-windows.txt)"
   - ✅ "Found build ID: <id> (from teamcity-build-id-linux.txt)"
   - ✅ "Analyzing TeamCity build IDs: <ids>"
   - ✅ "Fetching build log for build <id>..."

2. **Check PR Comment**
   ```bash
   gh pr view <pr-number> --comments
   ```
   
   Expected in comment:
   - ✅ Header: "## 🔍 Build Error Analysis"
   - ✅ Mention: "### @github-copilot"
   - ✅ Section: "**Build Errors Found:**"
   - ✅ Errors in code blocks: ```...```
   - ✅ TeamCity build links
   - ✅ Workflow run link

3. **Validate Error Content**
   - Check that actual build errors are included
   - Verify compilation errors are present
   - Verify linker errors (if any) are present
   - Verify CMake errors (if any) are present

### Cleanup

```bash
# Close and delete test PR
gh pr close <pr-number>
git checkout main
git branch -D test/build-error-analysis
git push origin --delete test/build-error-analysis
```

---

## Test Scenario 2: No TeamCity Builds

**Purpose:** Verify handling when TeamCity hasn't run

### Setup

1. Create a PR with documentation-only changes:
   ```bash
   git checkout -b test/docs-only
   
   # Make a documentation change
   echo "Test update" >> README.md
   
   git add README.md
   git commit -m "Test: docs update"
   git push origin test/docs-only
   
   gh pr create --title "Test: Docs Only" \
                --body "Testing workflow with no TeamCity builds"
   ```

2. **Important:** Don't wait for TeamCity - trigger analysis immediately

### Execute Test

1. Run the workflow with the PR number

2. Expected behavior:
   - Workflow completes successfully
   - No artifacts found
   - Comment posts generic message

### Verify Results

```bash
gh pr view <pr-number> --comments
```

Expected comment content:
```
**Status:** No TeamCity build errors found for this PR.

This could mean:
- The builds have not run yet
- The builds passed successfully
- Build IDs could not be extracted from the workflow logs
```

---

## Test Scenario 3: Successful Builds

**Purpose:** Verify behavior when builds pass

### Setup

1. Create a PR with a simple, safe change:
   ```bash
   git checkout -b test/successful-build
   
   # Add a comment to a file
   echo "// Test comment" >> Include/icomp/IComponent.h
   
   git add .
   git commit -m "Test: safe change"
   git push origin test/successful-build
   
   gh pr create --title "Test: Successful Build" \
                --body "Testing workflow with passing builds"
   ```

2. Wait for TeamCity builds to complete successfully

### Execute Test

1. Run the workflow with the PR number

2. Expected behavior:
   - Build IDs extracted
   - TeamCity logs fetched
   - Minimal or no errors found
   - Comment posted with either:
     - "No significant errors found" or
     - Last 100 lines of build log (fallback)

---

## Test Scenario 4: Partial Build Failure

**Purpose:** Verify handling when only one platform fails

### Setup

1. Create a platform-specific error (e.g., Windows-only):
   ```bash
   git checkout -b test/windows-only-error
   
   # Add Windows-specific code with an error
   cat >> Include/icomp/test.h << 'EOF'
   #ifdef _WIN32
   syntax error here
   #endif
   EOF
   
   git add .
   git commit -m "Test: Windows-only error"
   git push origin test/windows-only-error
   
   gh pr create --title "Test: Partial Build Failure" \
                --body "Testing workflow with single platform failure"
   ```

2. Wait for TeamCity builds to complete

### Execute Test

1. Run the workflow with the PR number

2. Expected behavior:
   - Both build IDs extracted (or just one if only one ran)
   - Errors from failing platform included
   - Success from passing platform (no errors or minimal)

---

## Debugging Failed Tests

### Check Workflow Logs

```bash
# Get detailed logs
gh run view <run-id> --log > workflow-logs.txt

# Search for specific steps
grep -A 50 "Download TeamCity Build Info Artifacts" workflow-logs.txt
grep -A 50 "Extract Build IDs from Artifacts" workflow-logs.txt
grep -A 50 "Fetch and Analyze Build Errors" workflow-logs.txt
```

### Check Artifacts Manually

```bash
# Download artifacts from TeamCity workflow
gh run download <teamcity-run-id>

# Check contents
ls -la teamcity-build-info-*/
cat teamcity-build-info-*/teamcity-build-id-*.txt
```

### Test TeamCity API Manually

```bash
# Set environment variables
export TEAMCITY_URL="<your-teamcity-url>"
export TEAMCITY_TOKEN="<your-token>"
export BUILD_ID="<build-id>"

# Fetch build status
curl -H "Authorization: Bearer $TEAMCITY_TOKEN" \
     "$TEAMCITY_URL/app/rest/builds/id:$BUILD_ID?fields=state,status,webUrl"

# Fetch build log (last 100 lines)
curl -H "Authorization: Bearer $TEAMCITY_TOKEN" \
     "$TEAMCITY_URL/app/rest/builds/id:$BUILD_ID/log" | tail -n 100
```

### Common Issues

**Issue: "No TeamCity workflow runs found"**
- PR doesn't have any TeamCity CI runs
- TeamCity workflow hasn't been triggered
- Wrong PR number provided

**Issue: "No artifacts directory found"**
- Artifacts haven't been uploaded yet (build still running)
- Artifacts expired (retention period passed)
- TeamCity workflow failed before artifact upload step

**Issue: "No build ID files found in artifacts directory"**
- Artifact structure changed
- File names don't match pattern `teamcity-build-id-*.txt`
- Files are empty or corrupted

**Issue: "No TeamCity build IDs found"**
- All of the above artifact issues
- Build IDs are empty or invalid

---

## Automated Test Script Details

The test script `test-build-error-analysis.sh` validates:

### Test 1: Artifact Extraction
- Creates mock artifact directory
- Places build ID files (windows, linux)
- Runs extraction logic
- Verifies both IDs extracted

### Test 2: Missing Artifacts
- Tests behavior when artifact directory doesn't exist
- Verifies no errors thrown
- Confirms empty result

### Test 3: Empty Build ID Files
- Creates empty files
- Creates whitespace-only files
- Verifies these are skipped

### Test 4: Whitespace Trimming
- Tests files with leading/trailing whitespace
- Tests files with newlines
- Verifies clean extraction

### Test 5: Single Platform
- Tests with only one build ID file
- Verifies single ID extracted correctly

### Test 6: Error Pattern Matching
- Tests regex patterns for:
  - Compilation errors
  - Linker errors
  - CMake errors
- Verifies all types detected

### Test 7: Nullglob Behavior
- Tests glob pattern with no matches
- Verifies no errors from non-matching patterns

---

## Success Criteria Summary

The workflow is considered fully tested and validated when:

- [x] Automated tests pass (7/7 tests)
- [ ] Manual Test 1: Full build error analysis completes successfully
- [ ] Manual Test 2: No TeamCity builds handled gracefully
- [ ] Manual Test 3: Successful builds post appropriate message
- [ ] Manual Test 4: Partial failures analyzed correctly
- [ ] All comments posted to PRs correctly
- [ ] All @github-copilot mentions work
- [ ] All TeamCity build links are valid
- [ ] All workflow run links are valid
- [ ] No errors in workflow execution
- [ ] Documentation is complete and accurate

---

## Next Steps After Testing

1. **Document Results**
   - Record all test executions in test log
   - Capture screenshots of PR comments
   - Save workflow run IDs for reference

2. **Update Documentation**
   - Add any new edge cases discovered
   - Document any workarounds needed
   - Update troubleshooting section

3. **Consider Improvements**
   - Evaluate if automatic triggering is needed
   - Consider better error formatting
   - Think about artifact retention period

4. **Monitor Usage**
   - Track how often workflow is used
   - Collect user feedback
   - Identify common use cases

---

## Related Files

- `.github/workflows/analyze-build-errors.yml` - The workflow
- `.github/workflows/BUILD_FIX_WORKFLOW_TEST_PLAN.md` - Detailed test plan
- `.github/workflows/test-build-error-analysis.sh` - Automated test script
- `.github/workflows/ANALYZE_BUILD_ERRORS_FIX.md` - Fix documentation

---

**Last Updated:** 2026-02-06
