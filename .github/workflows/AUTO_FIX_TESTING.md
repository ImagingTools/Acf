# Auto-Fix Workflow Testing Guide

This guide explains how to test the auto-fix workflow and Copilot task creation functionality.

## Prerequisites

Before testing, ensure:
- TeamCity integration is configured (TEAMCITY_URL, TEAMCITY_TOKEN, etc.)
- You have access to create pull requests in the repository
- You understand basic GitHub Actions workflows

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

5. Wait for TeamCity CI to run and fail

6. Verify the auto-fix workflow triggers:
   - Go to Actions tab
   - Look for "Auto-Fix on Build Failure" workflow run
   - Check that it completed successfully

7. Verify Copilot tasks were created:
   - Check the PR for comments about created issues
   - Go to Issues tab
   - Look for issues with labels: `auto-fix`, `build-failure`, `copilot-task`
   - Verify issue contains:
     - Problem type and details
     - Link to TeamCity build
     - Instructions for Copilot
     - Link back to the PR

8. Clean up:
   - Close the test PR
   - Close or delete the test issues
   - Delete the test branch

### Scenario 2: Test Workflow Logic with Mock Data

To test the workflow logic without triggering a real build:

**Steps:**
1. Go to the Actions tab in GitHub
2. Select the "Auto-Fix on Build Failure" workflow
3. Note: This workflow is triggered by `workflow_run` events, so manual dispatch is not available
4. You can review the workflow code to verify the logic

### Scenario 3: Review Past Workflow Runs

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
