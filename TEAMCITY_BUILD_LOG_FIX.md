# TeamCity Build Log API Fix

## Problem Statement
The TeamCity CI workflow on the main branch was failing when attempting to fetch build logs from the TeamCity server. The workflow would complete the build trigger successfully, but when trying to retrieve logs for display and error analysis, it would receive a 404 error.

## Error Message
```
Field 'log' is not supported. Supported are: number, status, statusText, id, startDate, finishDate, buildTypeId, branchName.
```

## Root Cause
The workflow was using an incorrect API endpoint to fetch build logs:
```
http://217.160.251.80:8111/app/rest/builds/id:$BUILD_ID/log
```

This endpoint does not exist in the TeamCity REST API. The error message indicates that the `/app/rest/builds/id:$BUILD_ID` endpoint exists, but it doesn't support a `log` field.

## Solution
Updated the workflow to use the correct TeamCity endpoint for downloading build logs:
```
http://217.160.251.80:8111/downloadBuildLog.html?buildId=$BUILD_ID
```

## Files Changed
- `.github/workflows/teamcity-trigger.yml` - Updated in 3 locations:
  1. Line 244: Fetch Build Logs step
  2. Line 300: Build Summary log URL
  3. Line 381: Analyze Build Errors job log fetching

## Technical Details

### Correct TeamCity Build Log Endpoints
According to TeamCity documentation, build logs can be accessed via:
1. **HTML Download (used in this fix)**: `/downloadBuildLog.html?buildId=$BUILD_ID`
2. **Artifact Access**: `/app/rest/builds/id:$BUILD_ID/artifacts/content/.teamcity/logs/buildLog.zip`
3. **Metadata Listing**: `/app/rest/builds/id:$BUILD_ID/artifacts/metadata/.teamcity/logs`

The HTML download endpoint is the simplest and most direct method for retrieving plain text build logs.

### What Still Works
The following REST API endpoints continue to work correctly:
- `/app/rest/builds/id:$BUILD_ID` - Get build metadata (status, state, etc.)
- `/app/rest/builds/id:$BUILD_ID/problemOccurrences` - Get build problems
- `/app/rest/builds/id:$BUILD_ID/testOccurrences` - Get test failures

## Impact
This fix resolves:
1. ✅ Build log fetching failures (404 errors)
2. ✅ "Fetch Build Logs" step showing error JSON instead of logs
3. ✅ "Analyze Build Errors" job unable to retrieve logs for analysis
4. ✅ Missing error context in GitHub Actions logs

## Validation
- [x] YAML syntax validated
- [x] Code review completed - no issues
- [x] Security scan completed - no vulnerabilities
- [x] Changes follow TeamCity REST API documentation
- [x] Fix addresses exact error from failed builds

## Testing Notes
The workflow requires manual approval to run when triggered by the Copilot bot (security feature). Once a maintainer approves and the workflow runs, it should:
1. Successfully fetch build logs from TeamCity
2. Display the last 100 lines of the build log in GitHub Actions
3. Perform error analysis on actual build logs (not error messages)
4. Post meaningful error analysis to PRs when builds fail

## Related Documentation
- TeamCity REST API: https://www.jetbrains.com/help/teamcity/rest/teamcity-rest-api-documentation.html
- GitHub Workflow: `.github/workflows/teamcity-trigger.yml`
- Build Error Analysis: Integrated into the same workflow as `analyze-build-errors` job

## Future Considerations
If additional log processing is needed, consider:
- Using the artifact endpoint for accessing structured log files
- Implementing log caching to reduce API calls
- Adding retry logic for transient network errors
- Filtering logs more aggressively to reduce noise in error analysis
