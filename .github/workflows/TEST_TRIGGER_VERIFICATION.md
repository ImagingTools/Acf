# Auto-Fix Workflow Trigger Test

This file is created to trigger a new CI run and verify that the auto-fix workflow triggers correctly.

## Expected Behavior

When this commit is pushed:

1. **TeamCity CI workflow** should run
   - It will fail due to the intentional error in `Include/istd/CSystem.cpp`
   - The error: undefined type `UndefinedType` and missing semicolon

2. **Auto-Fix workflow** should trigger automatically via `workflow_run` event
   - It should wait for TeamCity builds to complete
   - It should detect the build failure
   - It should analyze the build logs
   - It should post a comment to the PR with error details

3. **Security Scanning** should run
   - It should complete successfully

## Verification Steps

1. Check that TeamCity CI runs and fails
2. Check that Auto-Fix workflow triggers (look for workflow_run event)
3. Check that Auto-Fix workflow analyzes the failure
4. Check that a comment is posted to PR #122

## Test Date

Created: 2026-02-06 15:20 UTC

## Related Files

- Build error: `Include/istd/CSystem.cpp` (line 530-534)
- Workflow file: `.github/workflows/auto-fix-on-failure.yml` (on main branch)
- Trigger workflow: `.github/workflows/teamcity-trigger.yml`
