# Auto-Fix on Build Failure

This GitHub Actions workflow automatically posts build errors to PR discussions when CI builds fail.

## Overview

When a TeamCity CI build fails for a pull request, the auto-fix workflow:
1. Analyzes the build failure
2. Fetches detailed problem information from TeamCity
3. Posts build errors as comments in the PR discussion
4. Includes links to TeamCity builds and workflow runs
5. Attempts to apply common fixes (where implemented)
6. Commits and pushes changes if fixes are available

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
The workflow is triggered by the `workflow_run` event when the "TeamCity CI" workflow completes on a pull request.

### Process
1. **Get PR Information**: Identifies the pull request associated with the build
2. **Wait for TeamCity Builds**: Waits for TeamCity builds to complete (up to 30 minutes)
   - Monitors only TeamCity build check runs on the PR commit
   - Verifies that all TeamCity builds have reached a completed state
   - Checks if any TeamCity builds failed
   - Only proceeds if all TeamCity builds are done AND at least one failed
   - Other checks (Security Scanning, etc.) can still be running
3. **Checkout**: Checks out the PR branch with write permissions (if proceeding)
4. **Analyze**: Examines the build failure patterns and fetches detailed problems from TeamCity
5. **Fix**: Applies appropriate fixes based on the error type (where implemented)
6. **Comment**: Posts build errors as comments in the PR discussion with:
   - Error type and identity
   - Detailed error messages in code blocks
   - Links to TeamCity builds
   - Links to workflow runs
7. **Commit**: Creates a commit with fixes (if any were applied)

### Safety
- Only runs on pull requests (not main/master branches)
- Waits for TeamCity builds to complete before attempting fixes
- Does not wait for other checks (Security Scanning, etc.)
- Only runs if at least one TeamCity build failed
- Creates clear commit messages indicating auto-fix
- Comments on PR so reviewers can see what was changed
- Creates a check run with results

## Build Error Commenting

### Overview
The auto-fix workflow automatically posts build errors to the PR discussion when build failures are detected, with comments addressed directly to GitHub Copilot for AI-assisted fixing. The workflow analyzes **actual TeamCity build logs** to extract compilation errors, linker errors, and CMake errors, not just high-level problem occurrences. This provides Copilot with the detailed information needed to suggest meaningful fixes.

### When Comments Are Posted
Comments are posted when:
- A TeamCity build fails
- The auto-fix workflow successfully fetches and analyzes the build logs
- Compilation, linker, or build configuration errors are extracted from logs

### What Gets Analyzed
The workflow analyzes:
1. **TeamCity Build Logs**: Full build output (last 500 lines)
2. **Compilation Errors**: Lines matching `error:`, `Error:`, `ERROR:`, `fatal error:`
3. **Linker Errors**: Lines matching `undefined reference`, `unresolved external`, `cannot find -l`, etc.
4. **CMake Errors**: CMake configuration and generation errors
5. **Structured Problems**: TeamCity's problemOccurrences API (as supplementary data)

### Comment Format
Each comment includes:

**Header**: `## ❌ Build Failed`

**Copilot Address**: `### @github-copilot` - Directly addresses GitHub Copilot

**Summary Section**:
- Actual compilation/linker errors extracted from build logs
- First 10 most important error messages
- Clear, actionable error context for Copilot
- Request for Copilot to analyze and suggest specific fixes

**Detailed Errors Section** (Collapsible):
- Complete extracted compilation errors
- Complete extracted linker errors
- Complete extracted CMake errors
- Organized by build ID (for multi-platform builds)

**Structured Problems** (Collapsible):
- TeamCity's structured problem occurrences
- Problem type, identity, and details
- Links to TeamCity build logs

**Additional Information**:
- Direct links to TeamCity build logs
- Links to workflow runs
- Links to original build failures

### Workflow
1. Build fails in TeamCity
2. Auto-fix workflow:
   - Extracts TeamCity build IDs from GitHub Actions logs
   - Fetches actual build logs from TeamCity API (last 500 lines)
   - Parses logs for compilation errors using pattern matching
   - Parses logs for linker errors
   - Parses logs for CMake errors
   - Also fetches structured problemOccurrences as supplement
3. A comment is posted to the PR with:
   - Real error messages from compiler/linker
   - Error context (file, line, specific issue)
   - Comment addressed to @github-copilot
4. GitHub Copilot receives:
   - Actual error messages, not generic "build failed" messages
   - Specific file and line information where available
   - Error type context (compilation vs. linking vs. configuration)
5. Copilot can respond with targeted, specific fixes

### Benefits
- **Real Error Messages**: Actual compiler/linker errors, not generic summaries
- **AI-Assisted Fixing**: Copilot receives actionable error information
- **Automated Deep Analysis**: Build logs are parsed automatically
- **Prioritized Errors**: Most important errors shown first
- **Complete Context**: Full logs available in collapsible sections
- **Multi-Platform Support**: Analyzes errors from all failed builds (Windows/Linux)
- **No Manual Log Review**: Developers don't need to download TeamCity logs

### Example Comment
```markdown
## ❌ Build Failed

### @github-copilot

**Build Error Summary:**

The TeamCity CI build has failed. Here are the key errors from the build log:

```
error: 'ILogger' was not declared in this scope
   42 |     ILogger* logger = GetLogger();
      |     ^~~~~~~
error: 'GetLogger' was not declared in this scope
   42 |     ILogger* logger = GetLogger();
      |                       ^~~~~~~~~
fatal error: ilog/ILogger.h: No such file or directory
   12 | #include <ilog/ILogger.h>
      |          ^~~~~~~~~~~~~~~~
```

**Request:** Please analyze these build errors and suggest specific fixes for this PR. Focus on the actual compilation/linker errors shown above.

---

### Complete Build Log Errors

<details>
<summary>Click to expand full build log errors</summary>

```
--- Build 12345 ---
=== Compilation Errors ===
Impl/ilog/CLogRouter.cpp:12:10: fatal error: ilog/ILogger.h: No such file or directory
   12 | #include <ilog/ILogger.h>
      |          ^~~~~~~~~~~~~~~~
compilation terminated.
Impl/ilog/CLogRouter.cpp:42:5: error: 'ILogger' was not declared in this scope
   42 |     ILogger* logger = GetLogger();
      |     ^~~~~~~
Impl/ilog/CLogRouter.cpp:42:23: error: 'GetLogger' was not declared in this scope
   42 |     ILogger* logger = GetLogger();
      |                       ^~~~~~~~~
```

</details>

### Structured Build Problems

<details>
<summary>Click to expand TeamCity problem occurrences</summary>

#### Error 1: TC_COMPILATION_ERROR

**Issue:** Compilation failed in Impl/ilog/

**TeamCity Build:** [Build 12345](https://teamcity.example.com/...)

**Details:**
```
Compilation errors detected
```

</details>

### Additional Information

- **Workflow Run:** [View Details](https://github.com/ImagingTools/Acf/actions/runs/123456)
- **Original Build Failure:** [View Build](https://github.com/ImagingTools/Acf/actions/runs/123455)
- **TeamCity Build 12345:** [https://teamcity.example.com/viewLog.html?buildId=12345](https://teamcity.example.com/viewLog.html?buildId=12345)
```

## Waiting for TeamCity Builds

### Overview
The auto-fix workflow intelligently waits for TeamCity builds to complete before attempting to fix build errors. This ensures that:
- All TeamCity builds (Windows, Linux, etc.) have finished
- Auto-fix only runs when there's a confirmed TeamCity build failure
- Other checks (Security Scanning, etc.) can continue running independently

### How It Works
1. **Initial Trigger**: The workflow starts when the TeamCity CI workflow completes (success or failure)
2. **Check Monitoring**: The workflow queries the GitHub API to get all check runs for the PR's head commit
3. **Wait Loop**: Every 30 seconds, it checks if TeamCity builds have completed
   - Maximum wait time: 30 minutes (60 attempts × 30 seconds)
   - Filters to only monitor TeamCity build checks (not Security Scanning or other checks)
4. **Decision Point**: Once all TeamCity builds complete:
   - If any TeamCity build failed: Proceed with auto-fix analysis and commenting
   - If all TeamCity builds passed: Skip auto-fix (no action needed)
   - If timeout reached: Skip auto-fix and log timeout message

### Benefits
- **Focused Waiting**: Only waits for TeamCity builds, not all checks
- **Faster Response**: Doesn't wait for unrelated checks like Security Scanning
- **Resource Efficient**: Only runs the fix logic when actually needed
- **Clear Logging**: Provides detailed progress information in the workflow logs

### Configuration
The wait behavior can be adjusted in the workflow file:
- `maxAttempts`: Number of check attempts (default: 60)
- `delayMs`: Milliseconds between checks (default: 30000 = 30 seconds)
- Total maximum wait time = maxAttempts × (delayMs / 1000) seconds

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
Possible causes:
- TeamCity builds didn't complete within the 30-minute timeout
- No TeamCity builds failed (all passed)
- The workflow file doesn't exist or has invalid YAML syntax
- The "TeamCity CI" workflow name doesn't match exactly
- The PR branch doesn't have necessary permissions
- Check the "Wait for TeamCity Builds to Complete" step logs for details

### Auto-Fix Skipped with "proceed: false"
This is expected behavior when:
- All TeamCity builds completed but none failed
- The timeout was reached before TeamCity builds completed
- No TeamCity build checks were found
- Check the workflow logs to see which condition triggered the skip

### Auto-Fix Timeout
If the workflow times out waiting for TeamCity builds:
- Increase `maxAttempts` in the "Wait for TeamCity Builds to Complete" step
- Check if TeamCity builds are stuck or taking too long
- Review which TeamCity builds are pending in the workflow logs

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
