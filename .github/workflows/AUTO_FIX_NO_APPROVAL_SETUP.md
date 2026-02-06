# Auto-Fix Workflow - No Approval Required Setup

## Overview

The auto-fix workflow is designed to run **automatically without requiring manual approval**, even for first-time contributors. This is achieved by using the `workflow_run` trigger.

## How It Works

### workflow_run Trigger
- The auto-fix workflow uses the `workflow_run` event trigger
- This trigger runs in the **default branch context** (not the PR branch context)
- Workflows triggered by `workflow_run` automatically have access to secrets and write permissions
- **They do NOT require manual approval** because they run in a trusted context

### Security Model
The workflow is secure because:
1. The workflow file itself comes from the default branch (main/master)
2. Even if a PR modifies the workflow file, the version from the default branch is used
3. The workflow has explicit checks to only process PRs from the same repository (not forks)
4. Fork PRs are automatically excluded by the condition:
   ```yaml
   github.event.workflow_run.head_repository.full_name == github.repository
   ```

## Verification

### Check if Approval is Required

If you're seeing approval requirements, check these settings:

1. **Repository Settings > Actions > General**
   - Go to: `https://github.com/ImagingTools/Acf/settings/actions`
   - Under "Fork pull request workflows from outside collaborators"
   - Setting should be: "Require approval for first-time contributors" or more permissive
   - **Note:** This setting does NOT affect `workflow_run` workflows!

2. **Workflow_run workflows are exempt**
   - Workflows triggered by `workflow_run` are **automatically exempt** from approval requirements
   - They run in the default branch context, not the PR context
   - They do not need any special repository settings to bypass approval

### Testing

To verify the auto-fix workflow runs without approval:

1. Create a test PR with an intentional build error
2. Wait for the TeamCity CI workflow to fail
3. The auto-fix workflow should start **automatically** within seconds
4. No manual approval should be required
5. Check the Actions tab to see the workflow running

### Expected Behavior

✅ **Correct Behavior:**
- TeamCity CI workflow fails
- Auto-fix workflow starts automatically (no approval needed)
- Auto-fix workflow waits for TeamCity builds to complete
- Auto-fix workflow analyzes errors and posts comment to PR

❌ **If approval is required:**
- This should NOT happen with `workflow_run` trigger
- If you see this, there may be a GitHub Actions bug or misconfiguration
- Contact GitHub Support or check GitHub Status page

## Troubleshooting

### Workflow Not Starting At All

If the auto-fix workflow doesn't start:

1. Check the workflow file exists in the **default branch** (main/master)
2. Verify the trigger workflow name matches exactly: `"TeamCity CI"`
3. Check workflow logs for the triggering workflow to ensure it completed
4. Verify the condition is met:
   - `github.event.workflow_run.conclusion == 'failure'`
   - `github.event.workflow_run.event == 'pull_request'`
   - `github.event.workflow_run.head_repository.full_name == github.repository`

### Workflow Skipping Steps

If the workflow starts but skips auto-fix:

1. Check the "Wait for TeamCity Builds to Complete" step
2. Verify TeamCity builds completed
3. Verify at least one TeamCity build failed
4. Check logs for the skip reason

## Alternative: Using pull_request_target (Not Recommended)

**We do NOT use `pull_request_target`** because:
- It's designed for different use cases (commenting on PRs)
- It still requires approval for first-time contributors in some configurations
- It's less secure for our use case
- `workflow_run` is the recommended approach for workflow chaining

## References

- [GitHub Docs: workflow_run Event](https://docs.github.com/en/actions/using-workflows/events-that-trigger-workflows#workflow_run)
- [GitHub Blog: Keeping your GitHub Actions workflows secure](https://securitylab.github.com/research/github-actions-preventing-pwn-requests/)
- [Workflow run approval](https://docs.github.com/en/actions/managing-workflow-runs/approving-workflow-runs-from-public-forks)

## Summary

✅ **The auto-fix workflow is already configured to run without approval**

The `workflow_run` trigger ensures:
- Automatic execution when TeamCity CI fails
- No manual approval needed
- Secure execution in default branch context
- Access to repository secrets and write permissions

No additional configuration should be needed. If you're experiencing approval requirements, please check the troubleshooting section or file an issue.
