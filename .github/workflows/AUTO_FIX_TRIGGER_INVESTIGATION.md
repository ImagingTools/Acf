# Auto-Fix Workflow Trigger Investigation

## Problem Statement

The auto-fix workflow is NOT being triggered by the `workflow_run` event when TeamCity CI completes with failure.

## Evidence

### TeamCity CI Failures
- Run ID: 21755667295 - Completed at 15:18:49Z with `conclusion: failure`
- Run ID: 21755781339 - Completed at 15:24:22Z with `conclusion: failure`

###Auto-Fix Workflow Runs
Looking at the auto-fix workflow (ID: 229006910) runs:
- 2026-02-06T15:22:17Z - `event:push` (NOT workflow_run)
- 2026-02-06T15:17:55Z - `event:push` (NOT workflow_run)
- 2026-02-06T15:10:42Z - `event:workflow_run` ✓ (earlier successful run)
- 2026-02-06T15:08:01Z - `event:push` (NOT workflow_run)
- 2026-02-06T15:06:21Z - `event:workflow_run` ✓ (earlier run)

**NO `workflow_run` events were triggered after the recent TeamCity CI failures!**

## Configuration Check

### Auto-Fix Workflow on Main Branch (commit 1672136)

```yaml
name: Auto-Fix on Build Failure

on:
  workflow_run:
    workflows: ["TeamCity CI"]
    types:
      - completed
```

**Trigger condition:**
```yaml
if: |
  github.event.workflow_run.conclusion == 'failure' &&
  github.event.workflow_run.event == 'pull_request' &&
  github.event.workflow_run.head_repository.full_name == github.repository
```

### TeamCity CI Workflow on Main Branch (commit 1672136)

```yaml
name: TeamCity CI

on:
  pull_request:
  push:
    branches:
      - main
      - master
```

## Analysis

### What Should Happen

1. TeamCity CI workflow runs and completes with failure
2. GitHub Actions should trigger the `workflow_run` event
3. Auto-fix workflow should start with `event: workflow_run`
4. Auto-fix workflow checks the conditions (failure, pull_request, same repo)
5. If all conditions met, auto-fix proceeds

### What Actually Happens

1. TeamCity CI workflow runs and completes with failure ✓
2. **workflow_run event is NOT triggered** ❌
3. Auto-fix workflow does NOT start
4. **BUG: Auto-fix workflow starts with `event: push` instead** ❌

## Possible Causes

### 1. GitHub Actions Bug or Limitation

The `workflow_run` event may have issues or rate limits in the GitHub Actions system.

### 2. Workflow Name Mismatch

Although unlikely, there could be whitespace or invisible character issues in the workflow name.

Verification:
- TeamCity CI workflow name: "TeamCity CI" ✓
- Auto-fix trigger: `workflows: ["TeamCity CI"]` ✓
- **Names match exactly** ✓

### 3. Event Type Filter

The auto-fix workflow only triggers on `completed` type.

Verification:
- TeamCity CI status: `completed` ✓
- Auto-fix listens for: `types: [completed]` ✓
- **Event type matches** ✓

### 4. Branch/Repository Context

The workflow_run event runs in the default branch context.

Verification:
- Auto-fix workflow exists on main branch ✓
- TeamCity CI running from PR branch ✓
- **Context should be correct** ✓

### 5. Timing or Race Condition

There might be a timing issue where GitHub Actions doesn't properly dispatch the workflow_run event.

### 6. Push Event Configuration

**CRITICAL FINDING:** The auto-fix workflow is being triggered by `push` events even though the workflow file on main only has `workflow_run` trigger!

This suggests:
- Either GitHub Actions has a bug
- Or there's a cached/stale workflow definition
- Or workflow files are being read from PR branch despite documentation

## Recommendations

### Short Term (Testing)

1. ✅ Document this issue clearly
2. ⏳ Wait to see if workflow_run event eventually triggers (delayed)
3. ⏳ Try re-running the TeamCity CI workflow manually
4. ⏳ Check if there are any GitHub Actions status issues

### Medium Term (Workarounds)

1. Consider adding explicit `push` and `pull_request` triggers to auto-fix workflow
2. Add manual `workflow_dispatch` trigger for testing
3. Implement polling mechanism to check for failed builds

### Long Term (Fix)

1. Report issue to GitHub Actions if confirmed as a bug
2. Consider alternative trigger mechanisms
3. Investigate using GitHub Apps or webhooks instead

## Next Steps

1. Wait 5-10 minutes to see if workflow_run event is delayed
2. Check GitHub Actions status page for known issues
3. Try manual re-run of TeamCity CI
4. Consider adding additional trigger types as workaround

## References

- GitHub workflow_run documentation: https://docs.github.com/en/actions/using-workflows/events-that-trigger-workflows#workflow_run
- Repository memory: "workflow_run trigger ALWAYS executes from default branch (main)"
- PR #122: https://github.com/ImagingTools/Acf/pull/122
- TeamCity CI Run: https://github.com/ImagingTools/Acf/actions/runs/21755781339

## Test Date

Investigation Date: 2026-02-06 15:25 UTC
