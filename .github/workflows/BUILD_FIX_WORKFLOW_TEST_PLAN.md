# Build Fix Workflow - Test Plan

**Date:** 2026-02-06  
**Workflow:** `analyze-build-errors.yml`  
**Purpose:** Test and validate the simplified build error analysis workflow

---

## Overview

This document provides a comprehensive test plan for the "Analyze Build Errors" workflow, which was created as a simplified replacement for the complex auto-fix system. The workflow was fixed in PR #131 to use artifacts instead of parsing GitHub Actions logs.

---

## Test Objectives

1. **Verify artifact-based build ID extraction works correctly**
2. **Confirm TeamCity build logs are fetched and analyzed properly**
3. **Validate error detection and formatting**
4. **Ensure comments are posted to PRs correctly**
5. **Test graceful handling of edge cases**

---

## Test Environment Setup

### Prerequisites

1. Repository variables configured:
   - `TEAMCITY_URL` - TeamCity server URL
   - `TEAMCITY_TOKEN` - TeamCity authentication token
   - `TEAMCITY_BUILD_TYPE_WINDOWS` - Windows build configuration ID
   - `TEAMCITY_BUILD_TYPE_LINUX` - Linux build configuration ID

2. Permissions:
   - Workflow has `pull-requests: write` permission
   - Workflow has `issues: write` permission
   - Workflow has `actions: read` permission

3. Test PR with:
   - TeamCity CI workflow run completed
   - Build artifacts uploaded (`teamcity-build-info-windows`, `teamcity-build-info-linux`)
   - Build ID files present in artifacts

---

## Test Scenarios

### Test 1: Successful Build Error Analysis (Primary Path)

**Goal:** Verify the workflow extracts build IDs from artifacts and posts errors to PR

**Setup:**
1. Create a PR with code changes that cause build failures
2. Wait for TeamCity CI workflow to complete
3. Verify artifacts are uploaded:
   - `teamcity-build-info-windows` artifact exists
   - `teamcity-build-info-linux` artifact exists
   - Each artifact contains a `teamcity-build-id-*.txt` file

**Test Steps:**
1. Go to **Actions** → **Analyze Build Errors**
2. Click **"Run workflow"**
3. Enter the PR number
4. Click **"Run workflow"** button
5. Wait for workflow to complete

**Expected Results:**
- ✅ Workflow completes successfully (green checkmark)
- ✅ Step "Get PR and TeamCity Workflow Information" finds the workflow run
- ✅ Step "Download TeamCity Build Info Artifacts" downloads both artifacts
- ✅ Step "Extract Build IDs from Artifacts" extracts 2 build IDs
- ✅ Step "Fetch and Analyze Build Errors" retrieves TeamCity logs
- ✅ Step "Post Comment with Errors" creates a comment on the PR
- ✅ Comment contains:
  - `## 🔍 Build Error Analysis` header
  - `### @github-copilot` mention
  - Build errors in code blocks
  - Links to TeamCity builds
  - Link to workflow run

**Verification:**
```bash
# Check workflow run logs
gh run view <run-id> --log

# Verify artifacts were downloaded
# Look for: "Artifacts directory found, extracting build IDs..."
# Look for: "Found build ID: <id> (from teamcity-build-id-windows.txt)"

# Check PR comment
gh pr view <pr-number> --comments
```

---

### Test 2: PR Without TeamCity Builds

**Goal:** Verify graceful handling when no TeamCity builds exist

**Setup:**
1. Create a PR with only documentation changes (no TeamCity builds triggered)
2. Or use a PR where TeamCity builds haven't run yet

**Test Steps:**
1. Run the "Analyze Build Errors" workflow
2. Enter the PR number
3. Wait for completion

**Expected Results:**
- ✅ Workflow completes successfully
- ✅ Step "Get PR and TeamCity Workflow Information" reports "No TeamCity workflow runs found"
- ✅ Step "Download TeamCity Build Info Artifacts" is skipped (conditional)
- ✅ Comment posted to PR says:
  ```
  **Status:** No TeamCity build errors found for this PR.
  
  This could mean:
  - The builds have not run yet
  - The builds passed successfully
  - Build IDs could not be extracted from the workflow logs
  ```

---

### Test 3: PR With Successful Builds (No Errors)

**Goal:** Verify behavior when TeamCity builds pass

**Setup:**
1. Create a PR with code changes
2. Wait for TeamCity builds to complete successfully
3. Verify builds have status "SUCCESS"

**Test Steps:**
1. Run the "Analyze Build Errors" workflow
2. Enter the PR number
3. Wait for completion

**Expected Results:**
- ✅ Workflow completes successfully
- ✅ Build IDs are extracted from artifacts
- ✅ TeamCity logs are fetched
- ✅ No errors found in logs (or minimal warnings)
- ✅ Comment posted with either:
  - "No significant errors found" or
  - Last 100 lines of build log (fallback)

---

### Test 4: PR With Partial Builds (One Platform Only)

**Goal:** Verify handling when only one platform build exists

**Setup:**
1. Create a scenario where only Windows or Linux build completed
2. Only one artifact is available

**Test Steps:**
1. Run the workflow
2. Enter the PR number

**Expected Results:**
- ✅ Workflow continues despite `continue-on-error: true`
- ✅ One build ID extracted
- ✅ Analysis performed for available build
- ✅ Comment posted with single build's errors

---

### Test 5: Artifact Download Failure

**Goal:** Verify robustness when artifacts are unavailable

**Setup:**
1. Use a PR where TeamCity workflow exists but artifacts expired
2. Or artifacts weren't uploaded due to workflow failure

**Test Steps:**
1. Run the workflow
2. Enter the PR number

**Expected Results:**
- ✅ Step "Download TeamCity Build Info Artifacts" fails but doesn't stop workflow (`continue-on-error: true`)
- ✅ Step "Extract Build IDs from Artifacts" finds no artifacts directory
- ✅ Reports "No artifacts directory found"
- ✅ Comment posted saying "No TeamCity build IDs found for this PR"

---

### Test 6: Build ID File Format Validation

**Goal:** Verify proper handling of build ID file content

**Setup:**
1. Verify artifact files contain only the build ID (no extra whitespace)
2. Check both Windows and Linux artifacts

**Test Steps:**
1. Download artifacts manually
2. Check file contents:
   ```bash
   cat teamcity-build-id-windows.txt
   cat teamcity-build-id-linux.txt
   ```

**Expected Results:**
- ✅ Files contain numeric build IDs only
- ✅ No trailing newlines or spaces
- ✅ Build IDs are valid TeamCity build IDs

---

### Test 7: Error Extraction Accuracy

**Goal:** Verify correct error extraction from TeamCity logs

**Setup:**
1. Create a PR with specific error types:
   - Compilation errors
   - Linker errors
   - CMake configuration errors

**Test Steps:**
1. Run the workflow
2. Check the posted comment

**Expected Results:**
- ✅ Comment includes section "=== Build <ID> - Compilation Errors ==="
- ✅ Comment includes section "=== Build <ID> - Linker Errors ==="
- ✅ Comment includes section "=== Build <ID> - CMake Errors ==="
- ✅ Errors are properly formatted in code blocks
- ✅ Error context is preserved (file names, line numbers)

---

### Test 8: Multiple Build Failures

**Goal:** Verify handling when both Windows and Linux builds fail

**Setup:**
1. Create a PR with errors that affect both platforms
2. Wait for both builds to fail

**Test Steps:**
1. Run the workflow
2. Enter the PR number

**Expected Results:**
- ✅ Two build IDs extracted
- ✅ Logs fetched for both builds
- ✅ Errors from both builds included in comment
- ✅ Clearly separated sections for each build
- ✅ Links to both TeamCity builds included

---

## Test Checklist

Use this checklist when performing manual testing:

### Pre-Test Setup
- [ ] Repository variables configured (TEAMCITY_URL, TEAMCITY_TOKEN, etc.)
- [ ] Test PR created with build failures
- [ ] TeamCity CI workflow completed on test PR
- [ ] Artifacts uploaded and verified

### Workflow Execution
- [ ] Navigate to Actions → Analyze Build Errors
- [ ] Click "Run workflow"
- [ ] Enter correct PR number
- [ ] Workflow starts successfully
- [ ] All steps complete (check for failures)

### Artifact Handling
- [ ] Artifacts downloaded successfully
- [ ] Build IDs extracted from files
- [ ] Multiple build IDs handled correctly
- [ ] No errors when artifacts missing (graceful degradation)

### TeamCity Integration
- [ ] TeamCity API authentication works
- [ ] Build logs fetched successfully
- [ ] Error patterns matched correctly
- [ ] Build URLs constructed properly

### Comment Posting
- [ ] Comment appears on PR
- [ ] Comment addresses @github-copilot
- [ ] Errors formatted in code blocks
- [ ] TeamCity build links present and working
- [ ] Workflow run link present and working

### Edge Cases
- [ ] No TeamCity builds - handled gracefully
- [ ] Successful builds - appropriate message
- [ ] Partial builds - single platform handled
- [ ] Expired artifacts - error message clear
- [ ] Invalid PR number - error message clear

---

## Verification Commands

### Check Workflow Run Status
```bash
# List recent workflow runs
gh run list --workflow=analyze-build-errors.yml --limit 5

# View specific run
gh run view <run-id> --log

# Download run logs
gh run download <run-id>
```

### Check PR Comments
```bash
# View PR comments
gh pr view <pr-number> --comments

# View PR with web browser
gh pr view <pr-number> --web
```

### Verify Artifacts
```bash
# List artifacts for a workflow run
gh api repos/ImagingTools/Acf/actions/runs/<run-id>/artifacts

# Download artifacts
gh run download <teamcity-workflow-run-id>
```

### Check TeamCity Build
```bash
# Fetch build status
curl -H "Authorization: Bearer $TEAMCITY_TOKEN" \
  "$TEAMCITY_URL/app/rest/builds/id:<build-id>?fields=state,status,webUrl"

# Fetch build log (last 100 lines)
curl -H "Authorization: Bearer $TEAMCITY_TOKEN" \
  "$TEAMCITY_URL/app/rest/builds/id:<build-id>/log" | tail -n 100
```

---

## Known Issues and Limitations

### Current Implementation
1. **Manual Trigger Only**: Workflow must be triggered manually, not automatic on build failure
2. **Artifact Retention**: Artifacts expire after 1 day (retention-days: 1)
3. **Error Pattern Matching**: Uses regex patterns that may not catch all error types
4. **Log Size**: Only fetches last 500 lines of TeamCity logs
5. **No Auto-Fix**: Only analyzes and posts errors, doesn't attempt fixes

### From PR #131 Fix
- ✅ **Fixed**: GitHub Actions log API redirect issue
- ✅ **Fixed**: Uses artifacts instead of parsing logs
- ✅ **Fixed**: Robust glob pattern handling with `nullglob`
- ✅ **Fixed**: Proper whitespace trimming in build IDs

---

## Success Criteria

The workflow passes all tests if:

1. ✅ **Artifact extraction works**: Build IDs extracted from artifacts in 100% of cases where artifacts exist
2. ✅ **Error analysis accurate**: All compilation, linker, and CMake errors detected and included
3. ✅ **Comments posted**: Comments appear on PRs with correct formatting
4. ✅ **Copilot mention works**: @github-copilot is properly tagged
5. ✅ **Graceful degradation**: No workflow failures on edge cases
6. ✅ **Clear error messages**: Users understand what went wrong when builds not found
7. ✅ **TeamCity links work**: All build URLs are valid and accessible

---

## Next Steps After Testing

1. **Document results**: Create test report with all scenarios
2. **Update documentation**: Add any new edge cases discovered
3. **Suggest improvements**: Based on testing findings
4. **Consider automation**: Evaluate if any scenarios should auto-trigger
5. **Monitor usage**: Track how workflow is used in practice

---

## Related Files

- `.github/workflows/analyze-build-errors.yml` - The workflow being tested
- `.github/workflows/teamcity-trigger.yml` - TeamCity CI workflow (uploads artifacts)
- `.github/workflows/ANALYZE_BUILD_ERRORS_FIX.md` - Fix documentation from PR #131
- `WORKFLOW_SIMPLIFICATION_SUMMARY.md` - Background on workflow simplification

---

## Test Execution Log

When running tests, document results here:

### Test Run 1: [Date]
- PR Number: 
- Workflow Run ID: 
- Result: 
- Notes: 

### Test Run 2: [Date]
- PR Number: 
- Workflow Run ID: 
- Result: 
- Notes: 

---

**Last Updated:** 2026-02-06
