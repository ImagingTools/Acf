# Auto-Fix Workflow Testing Guide

This guide explains how to test the auto-fix workflow and its new waiting mechanism.

## Prerequisites

Before testing, ensure:
- TeamCity integration is configured (TEAMCITY_URL, TEAMCITY_TOKEN, etc.)
- You have access to create pull requests in the repository
- You understand basic GitHub Actions workflows

## Unit Testing the Wait Logic

A test script is provided to validate the wait-for-checks logic locally.

**Run the test:**
```bash
node .github/workflows/test-wait-logic.js
```

This script tests various scenarios:
1. All checks completed with TeamCity failure → Should proceed
2. All checks completed with no TeamCity failure → Should skip
3. Some checks still pending → Should continue waiting
4. Multiple TeamCity builds failed → Should proceed

## Testing Scenarios

### Scenario 1: Test with Real Build Failure

This is the most realistic test but requires a real build failure.

**Steps:**
1. Create a new branch from main:
   ```bash
   git checkout -b test/auto-fix-workflow
   ```

2. Introduce a build error (e.g., syntax error in C++ code):
   ```cpp
   // In any .cpp file, add an intentional error
   this is not valid C++ code;
   ```

3. Commit and push:
   ```bash
   git add .
   git commit -m "Test: Intentional build error for auto-fix testing"
   git push origin test/auto-fix-workflow
   ```

4. Create a pull request from this branch

5. Wait for ALL PR checks to run and complete:
   - TeamCity CI (Windows and Linux)
   - Security Scanning
   - Any other required checks

6. Verify the auto-fix workflow behavior:
   - Go to Actions tab
   - Look for "Auto-Fix on Build Failure" workflow run
   - Check the "Wait for All PR Checks to Complete" step:
     - Should show polling attempts
     - Should list pending checks (if any)
     - Should detect TeamCity failure
     - Should proceed with auto-fix
   - Verify it only started AFTER all checks completed

7. Verify build error comment was posted:
   - Check the PR for a comment from github-actions bot
   - Should contain:
     - "❌ Build Failed" header
     - "@github-copilot" mention
     - Actual build errors from TeamCity logs
     - Links to TeamCity builds
   - Verify issue contains:
     - Problem type and details
     - Link to TeamCity build
     - Instructions for Copilot
     - Link back to the PR

8. Clean up:
   - Close the test PR
   - Delete the test branch

### Scenario 2: Test with All Checks Passing

Test that auto-fix correctly skips when no TeamCity builds fail.

**Steps:**
1. Create a PR with valid code changes (no build errors)
2. Wait for all checks to complete successfully
3. Verify the auto-fix workflow:
   - Runs and completes
   - The "Wait for All PR Checks to Complete" step shows all checks completed
   - The "Skip Auto-Fix" step runs with message about no TeamCity failures
   - No build error comment is posted to the PR

### Scenario 3: Test Timeout Behavior

Test that auto-fix handles timeouts gracefully.

**Note:** This is difficult to test in practice as it requires checks to run for >30 minutes.

**Expected behavior:**
- If checks don't complete within 30 minutes
- The wait loop should timeout
- The workflow should skip auto-fix with a timeout message
- No errors should be thrown

### Scenario 4: Test with One Platform Failing

Test with Windows build failing but Linux build passing (or vice versa).

**Steps:**
1. Introduce a platform-specific build error
2. Create a PR
3. Wait for both platform builds to complete
4. Verify auto-fix:
   - Waits for both platforms to finish
   - Detects the single platform failure
   - Proceeds with auto-fix
   - Comments with details about the failed platform

## Verifying Workflow Logs

When reviewing workflow runs, check these key indicators:

### "Wait for All PR Checks to Complete" Step

**Good indicators:**
- Shows polling attempts: "Attempt X/60"
- Lists found checks: "Found X check runs"
- Shows which checks are pending (if any)
- Eventually shows: "✅ All PR checks have completed"
- Shows TeamCity check detection
- Shows decision: "proceeding with auto-fix" or "skipping auto-fix"

**Problem indicators:**
- Times out before all checks complete
- Doesn't detect TeamCity checks correctly
- Doesn't filter out itself from check list

### "Skip Auto-Fix" Step

Should run when:
- All checks passed (no TeamCity failures)
- Timeout occurred
- Shows clear reason for skipping

### "Comment on PR" Step

Should only run when proceeding with auto-fix.
Should post detailed error information from TeamCity.

## Reviewing Past Workflow Runs

If there have been previous build failures:

**Steps:**
1. Go to Actions tab
2. Filter by "Auto-Fix on Build Failure" workflow
3. Select a recent run
4. Review the logs for each step:
   - "Analyze Build Failure" - Check if TeamCity problems were fetched
   - "Create Copilot Tasks for Build Problems" - Check if issues were created
   - "Comment on PR" - Check if PR comments mention created issues

## What to Verify

### In the Workflow Logs

- [x] "Analyze Build Failure" step successfully fetches TeamCity build IDs
- [x] TeamCity API calls return problem information
- [x] Problems are parsed as JSON
- [x] "Create Copilot Tasks" step creates issues
- [x] Issue numbers are output correctly

### In the Created Issues

- [x] Title format: `[Auto-Fix] {Type}: {Identity}`
- [x] Labels include: `auto-fix`, `build-failure`, `copilot-task`
- [x] Body includes:
  - Problem type and identity
  - Branch name
  - PR reference
  - TeamCity build link
  - Problem details in code block
  - Clear instructions for Copilot
  - Links to workflow run and original failure

### In the PR Comments

- [x] Comment mentions detected issues
- [x] Comment shows issue type
- [x] Comment lists created Copilot tasks with issue numbers
- [x] Comment includes links to workflow run and build logs

## Expected Behavior

### When Build Fails with TeamCity Problems

1. Auto-fix workflow triggers
2. Workflow fetches detailed problems from TeamCity
3. For each problem, a GitHub issue is created
4. Issues are labeled appropriately
5. PR receives comment listing all created issues

### When Build Fails without Detailed Problems

1. Auto-fix workflow triggers
2. Workflow attempts to extract problems
3. Falls back to generic issue if problems can't be parsed
4. Single generic issue created with available information
5. PR receives comment about the generic issue

### When TeamCity Configuration is Missing

1. Auto-fix workflow triggers
2. Workflow skips TeamCity analysis
3. Generic comment posted to PR
4. No issues created (insufficient information)

## Troubleshooting

### No Issues Created

**Possible causes:**
- TeamCity configuration variables not set
- Build problems not extracted successfully
- Issue creation failed (check permissions)

**Solution:**
- Verify TEAMCITY_URL and TEAMCITY_TOKEN are configured
- Check workflow logs for error messages
- Verify workflow has `issues: write` permission

### Issues Created but Missing Information

**Possible causes:**
- TeamCity API response incomplete
- Problem object missing fields

**Solution:**
- Check TeamCity API response in workflow logs
- Verify TeamCity build problems API returns expected fields
- Update workflow to handle missing fields gracefully

### Workflow Doesn't Trigger

**Possible causes:**
- TeamCity CI workflow name doesn't match
- Build didn't actually fail
- Workflow triggered for fork PR (security restriction)

**Solution:**
- Verify workflow_run trigger matches "TeamCity CI"
- Confirm build conclusion is "failure"
- Ensure PR is from same repository (not a fork)

## Advanced Testing

### Test with Multiple Problems

Create multiple build errors in different files to test:
- Multiple issue creation
- Issue numbering and linking
- Performance with many problems

### Test with Different Problem Types

Introduce different types of errors:
- Compilation errors
- Linker errors  
- Test failures
- CMake configuration errors

Verify each creates an appropriate issue with correct categorization.

## Monitoring in Production

After deployment, monitor:
- Success rate of issue creation
- Quality of problem extraction from TeamCity
- Usefulness of created issues for Copilot
- False positives (issues created for non-issues)

## Feedback and Improvements

Track:
- How often Copilot successfully fixes issues
- Common problem types that can be automated
- Issues that require manual intervention
- Workflow performance and timing

Use this data to:
- Improve problem detection patterns
- Add more auto-fix logic
- Refine issue templates
- Optimize workflow efficiency
