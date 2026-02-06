# TeamCity CI Workflow - Integrated Build Error Analysis

## Overview
The TeamCity CI workflow now includes integrated build error analysis. When builds fail on pull requests, the workflow automatically collects logs, analyzes errors, and posts them directly to the PR conversation.

## Architecture

### Single Workflow Design
Instead of using separate workflows connected via `workflow_run` events, all functionality is integrated into `teamcity-trigger.yml`:

```
TeamCity CI Workflow
├── Job: trigger-teamcity (matrix: windows, linux)
│   ├── Trigger TeamCity builds
│   ├── Wait for completion
│   ├── Fetch build logs
│   └── Upload build ID artifacts
│
└── Job: analyze-build-errors
    ├── Download build ID artifacts
    ├── Extract build IDs from all platforms
    ├── Fetch and analyze build logs from TeamCity
    └── Post error analysis to PR conversation
```

### Flow
1. **Build Triggering**: Matrix job triggers both Windows and Linux builds on TeamCity
2. **Build Execution**: Workflow waits for builds to complete
3. **Artifact Upload**: Build IDs are saved as artifacts for each platform
4. **Error Analysis** (only if builds failed and it's a PR):
   - Downloads all build ID artifacts
   - Extracts build IDs from both platforms
   - Fetches build logs from TeamCity REST API
   - Parses errors (compilation, linker, CMake)
   - Posts formatted analysis to PR conversation
   - Addresses @github-copilot for assistance

## Key Features

### Automatic Execution
- Runs automatically on every PR push and main branch push
- Error analysis only runs for PRs when builds fail
- No manual intervention required

### Comprehensive Error Detection
- **Compilation errors**: Detects C/C++ compilation errors
- **Linker errors**: Finds undefined references and missing libraries
- **CMake errors**: Captures configuration issues
- **Fallback**: Shows last 100 lines if no specific errors found

### PR Integration
- Posts directly to PR conversation when builds fail
- Includes @github-copilot mention for AI assistance
- Links to TeamCity build pages for detailed logs
- Clean, formatted output for easy reading

## Benefits Over Previous Architecture

### Simplicity
- ✅ Single workflow file instead of two
- ✅ No `workflow_run` event complexity
- ✅ Easier to understand and maintain

### Reliability
- ✅ Build IDs available directly in the same workflow
- ✅ No cross-workflow artifact access issues
- ✅ Guaranteed to run after builds complete

### Performance
- ✅ Faster execution (no waiting for workflow_run event)
- ✅ Direct access to build artifacts
- ✅ Immediate error posting

## Permissions Required

The workflow requires these permissions:
```yaml
permissions:
  contents: read        # Read repository code
  statuses: write       # Update commit statuses
  checks: write         # Create check runs
  pull-requests: write  # Post comments to PRs
  issues: write         # Post comments (PRs are issues)
```

## Configuration

### Repository Variables
- `TEAMCITY_URL`: TeamCity server URL (e.g., http://217.160.251.80:8111)
- `TEAMCITY_TOKEN`: TeamCity access token
- `TEAMCITY_BUILD_TYPE_WINDOWS`: Windows build configuration ID
- `TEAMCITY_BUILD_TYPE_LINUX`: Linux build configuration ID

### Workflow Conditions
The error analysis job runs only when:
1. `always()` - Runs even if build job failed
2. `github.event_name == 'pull_request'` - Only for PRs
3. `needs.trigger-teamcity.result == 'failure'` - Only when builds failed

## Example Output

When a build fails, a comment is posted to the PR:

```markdown
## 🔍 TeamCity Build Failed - Error Analysis

### @github-copilot

**Status:** TeamCity CI build failed ❌

Please analyze the following build errors and suggest specific fixes:

```
=== Build 12345 - Compilation Errors ===
error: ILogger was not declared in this scope
error: no matching function for call to Initialize

=== Build 67890 - Linker Errors ===
ld: undefined reference to ilog::LogRouter::LogMessage
```

### TeamCity Build Links

- [Build 12345](http://217.160.251.80:8111/viewLog.html?buildId=12345)
- [Build 67890](http://217.160.251.80:8111/viewLog.html?buildId=67890)

---

*Posted automatically by [TeamCity CI Workflow](https://github.com/ImagingTools/Acf/actions/runs/...)*
```

## Files

### Modified
- `.github/workflows/teamcity-trigger.yml` - Integrated error analysis job

### Removed
- `.github/workflows/post-build-analysis.yml` - No longer needed (functionality moved to teamcity-trigger.yml)

### Unchanged
- `.github/workflows/analyze-build-errors.yml` - Manual analysis workflow (optional fallback)
- `.github/workflows/test-build-error-analysis.sh` - Test script (all tests pass)

## Testing

### Unit Tests
```bash
bash .github/workflows/test-build-error-analysis.sh
```
All 7 tests pass:
- Artifact Extraction
- Missing Artifacts Directory
- Empty Build ID Files
- Whitespace Trimming
- Single Platform
- Error Pattern Matching
- Nullglob Behavior

### Integration Testing
The workflow will be tested when:
1. A PR is created or updated
2. TeamCity builds are triggered
3. One or both builds fail
4. Error analysis is posted to PR conversation

## Troubleshooting

### No comment posted
- Check that it's a PR (not main branch push)
- Verify builds actually failed
- Check workflow run logs in Actions tab
- Ensure permissions are configured

### Artifacts not found
- Verify build ID artifacts were uploaded
- Check artifact names match pattern: `teamcity-build-info-{platform}`
- Confirm artifacts haven't expired (1 day retention)

### No errors extracted
- Verify TeamCity REST API credentials
- Check build IDs are valid
- Ensure TeamCity builds generated logs
- Review "Fetch and Analyze Build Errors" step logs

## Migration from Previous Architecture

### What Changed
- ❌ Removed: `post-build-analysis.yml` workflow
- ✅ Added: `analyze-build-errors` job in `teamcity-trigger.yml`
- ✅ Simplified: No more workflow_run events
- ✅ Improved: Direct artifact access

### What Stayed the Same
- Same error analysis logic
- Same comment format
- Same @copilot integration
- Same TeamCity REST API usage
- Manual workflow still available as fallback

## Future Enhancements
- Add error categorization and prioritization
- Support for multiple build attempts
- Integration with GitHub code annotations
- Build log caching to reduce TeamCity API calls
