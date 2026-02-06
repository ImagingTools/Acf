# Automatic Build Error Analysis Integration

## Overview
Automatic workflow that posts build error analysis to PR conversations when TeamCity CI fails.

## Problem Solved
The original `analyze-build-errors.yml` workflow was manual-only (workflow_dispatch) and had issues:
1. Required manual triggering with PR number
2. Artifact finding logic sometimes looked at wrong workflow runs
3. Not integrated into PR conversation flow

## Solution
Created new `post-build-analysis.yml` workflow that:
- **Automatically triggers** when TeamCity CI workflow fails
- **Uses workflow_run event** to capture the exact TeamCity CI workflow run
- **Downloads artifacts** directly from the failed TeamCity CI run
- **Posts to PR conversation** automatically addressing @github-copilot

## Workflow Details

### Trigger
```yaml
on:
  workflow_run:
    workflows: ["TeamCity CI"]
    types: [completed]
```

### Conditions
- Only runs when TeamCity CI conclusion is 'failure'
- Only runs for pull_request events (not main branch)

### Process
1. Extract PR number from workflow_run event
2. Download TeamCity build info artifacts from the failed workflow run
3. Extract build IDs from artifact files (teamcity-build-id-{platform}.txt)
4. Fetch build logs from TeamCity REST API
5. Parse errors (compilation, linker, CMake)
6. Post formatted comment to PR conversation

### Comment Format
- Includes @github-copilot mention for AI assistance
- Shows compilation, linker, and CMake errors
- Links to TeamCity build pages
- Includes link to re-run manual analysis if needed

## Files

### New Files
- `.github/workflows/post-build-analysis.yml` - Automatic analysis workflow

### Existing Files (unchanged)
- `.github/workflows/analyze-build-errors.yml` - Manual analysis workflow (still available)
- `.github/workflows/teamcity-trigger.yml` - TeamCity CI workflow (uploads artifacts)
- `.github/workflows/test-build-error-analysis.sh` - Test script

## Usage

### Automatic (Primary)
When TeamCity CI fails on a PR:
1. TeamCity CI workflow completes with failure
2. `post-build-analysis.yml` automatically triggers
3. Build errors are posted to PR conversation
4. @github-copilot can analyze and suggest fixes

### Manual (Fallback)
If automatic posting fails or you want to re-analyze:
1. Go to Actions → "Analyze Build Errors" workflow
2. Click "Run workflow"
3. Enter PR number
4. Workflow posts analysis to PR

## Benefits
1. **Automatic**: No manual intervention needed
2. **Fast**: Posts errors immediately after build failure
3. **Integrated**: Shows up in PR conversation naturally
4. **AI-assisted**: @github-copilot can provide fix suggestions
5. **Reliable**: Uses correct workflow run ID via workflow_run event

## Testing
The workflow will be tested when:
1. A PR is created or updated
2. TeamCity CI runs and fails
3. Automatic analysis posts to PR conversation

Check PR conversation for automatic build error analysis comments.

## Troubleshooting

### Artifacts not found
- Ensure TeamCity CI workflow uploaded artifacts correctly
- Check artifact names match pattern: `teamcity-build-info-{platform}`
- Verify artifact retention (default: 1 day)

### No comment posted
- Check workflow run logs in Actions tab
- Verify PR number was extracted from workflow_run event
- Ensure workflow has `pull-requests: write` permission

### Analysis incomplete
- Verify TeamCity REST API credentials (vars.TEAMCITY_URL, vars.TEAMCITY_TOKEN)
- Check build IDs are valid integers
- Ensure TeamCity builds generated logs

## Future Improvements
- Add build log caching to avoid repeated TeamCity API calls
- Support for multiple build attempts
- Enhanced error categorization and prioritization
- Integration with GitHub code annotations
