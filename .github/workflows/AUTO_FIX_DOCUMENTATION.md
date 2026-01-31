# Auto-Fix on Build Failure

This GitHub Actions workflow automatically attempts to fix common build errors when CI builds fail.

## Overview

When a TeamCity CI build fails for a pull request, the auto-fix workflow:
1. Analyzes the build failure
2. Attempts to apply common fixes
3. Commits and pushes changes if fixes are available
4. Comments on the PR with results

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
3. **Analyze**: Examines the build failure patterns and logs
4. **Fix**: Applies appropriate fixes based on the error type
5. **Commit**: Creates a commit with fixes (if any were applied)
6. **Comment**: Posts a comment on the PR explaining what was done

### Safety
- Only runs on pull requests (not main/master branches)
- Creates clear commit messages indicating auto-fix
- Comments on PR so reviewers can see what was changed
- Creates a check run with results

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
