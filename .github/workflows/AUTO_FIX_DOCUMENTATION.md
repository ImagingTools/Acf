# Auto-Fix on Build Failure

This GitHub Actions workflow automatically attempts to fix common build errors when CI builds fail.

## Overview

When a TeamCity CI build fails for a pull request, the auto-fix workflow:
1. Analyzes the build failure
2. Fetches detailed problem information from TeamCity
3. Creates GitHub issues as tasks for Copilot to fix
4. Attempts to apply common fixes (where implemented)
5. Commits and pushes changes if fixes are available
6. Comments on the PR with results and links to Copilot tasks

## Supported Auto-Fixes

### 1. Missing Component Registration
**Problem:** Build error like `'BasePck::ComponentName' has not been declared`

**Auto-Fix:**
- Adds missing include in package header file
- Adds typedef for the component
- Adds I_EXPORT_COMPONENT in package .cpp file

### 2. Package ID Mismatches
**Problem:** Component used in .acc file but not registered in specified package

**Auto-Fix:**
- Updates PackageId in .acc file to match where component is actually registered
- Or adds component registration to the expected package

### 3. Common Build Configuration Issues
**Auto-Fix:**
- Fixes common CMake configuration problems
- Updates dependencies if obvious mismatches exist

## How It Works

### Trigger
The workflow is triggered by the `workflow_run` event when the "TeamCity CI" workflow completes with a failure status on a pull request.

### Process
1. **Get PR Information**: Identifies the pull request associated with the failed build
2. **Checkout**: Checks out the PR branch with write permissions
3. **Analyze**: Examines the build failure patterns and fetches detailed problems from TeamCity
4. **Create Copilot Tasks**: Creates GitHub issues for detected problems with comprehensive context
5. **Fix**: Applies appropriate fixes based on the error type (where implemented)
6. **Commit**: Creates a commit with fixes (if any were applied)
7. **Comment**: Posts a comment on the PR explaining what was done and linking to Copilot tasks

### Safety
- Only runs on pull requests (not main/master branches)
- Creates clear commit messages indicating auto-fix
- Comments on PR so reviewers can see what was changed
- Creates a check run with results

## Copilot Task Creation

### Overview
The auto-fix workflow automatically creates GitHub issues as tasks for Copilot when build problems are detected. This enables automated issue tracking and allows Copilot to work on fixing the detected problems.

### When Tasks Are Created
Tasks are created when:
- A TeamCity build fails with specific build problems
- The auto-fix workflow successfully analyzes the failure
- Detailed problem information is available from TeamCity

### Task Format
Each created issue includes:

**Title**: `[Auto-Fix] {ProblemType}: {ProblemIdentity}`

**Labels**:
- `auto-fix` - Indicates automatic creation
- `build-failure` - Marks as a build failure issue  
- `copilot-task` - Designates for Copilot to work on

**Content**:
- Problem type and identity
- Branch and PR reference
- Link to TeamCity build logs
- Detailed problem description
- Clear instructions for Copilot on what needs to be fixed
- Links to workflow runs

### Workflow
1. Build fails in TeamCity
2. Auto-fix workflow analyzes the failure
3. For each detected problem, a GitHub issue is created
4. Issue is labeled as a Copilot task
5. PR receives a comment with links to the created issues
6. Copilot can then work on fixing the issues

### Benefits
- **Automatic tracking**: No manual issue creation needed
- **Comprehensive context**: All debugging information included
- **Copilot ready**: Issues formatted for Copilot to work on
- **Visibility**: Team can see all build issues
- **Traceability**: Links between PRs, builds, and issues

### Example Issue
```markdown
## Build Problem Detected

**Type:** TC_COMPILATION_ERROR
**Identity:** Error in Impl/ilog/CLogRouter.cpp:42
**Branch:** feature/new-logging
**PR:** #123

**TeamCity Build:** [Build 45678](https://teamcity.example.com/...)

### Problem Details
```
error: 'ILogger' was not declared in this scope
```

### Context
This issue was automatically created by the auto-fix workflow after detecting a build failure.

### Task for Copilot
Please analyze the build problem and:
1. Identify the root cause of the issue
2. Implement a fix for the problem
3. Ensure the fix doesn't break existing functionality
4. Test the changes to verify the build passes
```

## Configuration

### Enabling/Disabling
The auto-fix workflow is enabled by default. To disable it:
1. Delete or rename `.github/workflows/auto-fix-on-failure.yml`
2. Or add a condition to skip certain branches/PRs

### Customization
To customize auto-fix behavior, edit the workflow file:
- Add new error patterns in the "Analyze Build Failure" step
- Add new fix logic in the "Attempt Common Fixes" step
- Modify commit message format
- Adjust PR comment templates

## Manual Override

If auto-fix makes incorrect changes:
1. The commit is clearly marked as auto-generated
2. You can revert it: `git revert <commit-hash>`
3. Or force push to remove it: `git reset --hard HEAD~1 && git push -f`

## Limitations

- Cannot fix all types of build errors
- Some complex issues require human judgment
- May not detect all error patterns from TeamCity logs
- Limited to problems that can be fixed by file modifications

## Best Practices

1. **Review Auto-Fixes**: Always review commits made by the auto-fix bot
2. **Test After Fix**: Wait for the new build to complete before merging
3. **Learn from Fixes**: Use auto-fix commits to understand common patterns
4. **Provide Feedback**: If auto-fix fails on a common issue, consider adding support for it

## Troubleshooting

### Auto-Fix Didn't Run
- Check that the workflow file exists and is valid YAML
- Verify the "TeamCity CI" workflow name matches exactly
- Ensure the PR branch has necessary permissions

### Auto-Fix Applied Wrong Changes
- Review the logic in "Attempt Common Fixes" step
- Add more specific error detection patterns
- Consider adding test mode to dry-run fixes first

### Build Still Fails After Auto-Fix
- Some issues cannot be automatically fixed
- Review the PR comment for guidance on manual fixes
- Check build logs for additional error details

## Examples

### Example 1: Missing Component Registration
**Error:**
```
error: 'BasePck::LogRouter' has not been declared
```

**Auto-Fix Applied:**
1. Added `#include <ilog/CLogRouterComp.h>` to BasePck.h
2. Added `typedef ilog::CLogRouterComp LogRouter;` to BasePck.h
3. Added I_EXPORT_COMPONENT for LogRouter in BasePck.cpp
4. Committed changes and pushed to PR

### Example 2: Package Mismatch
**Error:**
```
error: 'LogPck::LogRouter' has not been declared
```

**Auto-Fix Applied:**
1. Analyzed that LogRouter is registered in BasePck, not LogPck
2. Updated LogTest.acc to use `PackageId="BasePck"`
3. Committed changes and pushed to PR

## Future Enhancements

Potential improvements to the auto-fix system:
- Machine learning to detect patterns from historical fixes
- Integration with build log parsing for better error detection
- Support for more complex multi-file refactorings
- Interactive mode where bot asks for confirmation before fixing
- Statistics dashboard showing auto-fix success rates

## Related Documentation

- [TeamCity Integration](TEAMCITY_INTEGRATION.md)
- [Contributing Guidelines](../../CONTRIBUTING.md)
- [Security Policy](../../SECURITY.md)
