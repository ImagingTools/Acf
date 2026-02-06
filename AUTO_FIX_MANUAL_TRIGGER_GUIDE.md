# Auto-Fix Workflow Manual Trigger Guide

**Date:** 2026-02-06  
**Workflow:** Auto-Fix on Build Failure  
**File:** `.github/workflows/auto-fix-on-failure.yml`

---

## Overview

The auto-fix workflow can now be triggered **manually** in addition to automatic triggering via check_run events. This is useful for:
- Testing the workflow
- Re-running analysis when automatic triggering fails
- Debugging workflow behavior
- Running analysis on older PRs

---

## How to Trigger Manually

### Via GitHub Web Interface

1. **Navigate to Actions:**
   - Go to: https://github.com/ImagingTools/Acf/actions/workflows/auto-fix-on-failure.yml

2. **Click "Run workflow":**
   - Click the "Run workflow" button in the top right

3. **Fill in Parameters:**
   - **Branch:** Select "main" (workflow must run from main branch)
   - **pr_number** (required): The pull request number to analyze (e.g., `128`)
   - **workflow_run_id** (optional): TeamCity CI workflow run ID if known
   - **head_sha** (optional): Specific commit SHA to analyze

4. **Click "Run workflow":**
   - The workflow will start immediately

### Example Parameters

**Minimal (Required Only):**
```
pr_number: 128
workflow_run_id: (leave empty)
head_sha: (leave empty)
```

**With Specific Workflow Run:**
```
pr_number: 128
workflow_run_id: 21760295776
head_sha: (leave empty)
```

**With Specific SHA:**
```
pr_number: 128
workflow_run_id: (leave empty)
head_sha: 75f99a24cd480484f479049baefd58c363c54e9d
```

---

## Workflow Parameters

### pr_number (Required)

**Description:** The pull request number to analyze  
**Type:** Number  
**Example:** `128`

**Usage:**
- The workflow will fetch PR details from GitHub API
- PR must exist and be accessible
- Determines which commit and branch to analyze

### workflow_run_id (Optional)

**Description:** TeamCity CI workflow run ID  
**Type:** String  
**Example:** `21760295776`

**Usage:**
- If provided, the workflow will use this specific run
- If not provided, the workflow will auto-detect the most recent TeamCity CI run for the PR's commit
- Useful when you want to analyze a specific build

**How to Find:**
1. Go to the PR's "Checks" tab
2. Click on "TeamCity CI"
3. The run ID is in the URL: `https://github.com/ImagingTools/Acf/actions/runs/{run_id}`

### head_sha (Optional)

**Description:** Specific commit SHA to analyze  
**Type:** String  
**Example:** `75f99a24cd480484f479049baefd58c363c54e9d`

**Usage:**
- If provided, uses this specific commit instead of the PR's head
- If not provided, uses the PR's current head commit
- Useful for analyzing a specific commit in the PR's history

**How to Find:**
1. Go to the PR's "Commits" tab
2. Click on the commit
3. The SHA is shown in the commit details (7-character short version or full 40-character SHA)

---

## How It Works

### Automatic Trigger (check_run event)

**When:**
- TeamCity CI check run completes with `failure` conclusion
- Check run is associated with a pull request
- Check run name contains "Trigger TeamCity Build"

**Flow:**
1. GitHub dispatches `check_run: completed` event
2. Auto-fix workflow evaluates conditional
3. If conditions met, workflow runs automatically
4. Fetches PR details from check_run event payload

### Manual Trigger (workflow_dispatch)

**When:**
- User manually triggers via GitHub UI or API

**Flow:**
1. User provides `pr_number` (and optionally `workflow_run_id`, `head_sha`)
2. Auto-fix workflow always runs (no conditional check for manual triggers)
3. Fetches PR details from GitHub API using provided `pr_number`
4. Uses provided `workflow_run_id` if given, otherwise auto-detects
5. Uses provided `head_sha` if given, otherwise uses PR's current head

### Workflow Steps (Both Triggers)

1. **Determine Event Source:** Identifies if automatic or manual trigger
2. **Get PR Information:** Fetches PR details (from event payload or API)
3. **Wait for TeamCity Builds:** Waits for TeamCity builds to complete
4. **Get TeamCity Workflow Run:** Finds the TeamCity CI workflow run ID
5. **Download Build Info:** Downloads TeamCity build artifacts
6. **Analyze Build Failure:** Fetches and analyzes build logs
7. **Post PR Comment:** Posts analysis results to the PR

---

## Use Cases

### 1. Testing the Workflow

**Scenario:** You want to test if the auto-fix workflow works correctly

**Solution:**
1. Find a PR with a failed TeamCity CI build
2. Note the PR number (e.g., `128`)
3. Manually trigger the workflow with just the PR number
4. Workflow will auto-detect the failed build and analyze it

### 2. Re-running After Failure

**Scenario:** Auto-fix triggered automatically but failed due to a transient error

**Solution:**
1. Check the failed auto-fix run to get the PR number
2. Manually trigger with the same PR number
3. Workflow will re-analyze the build

### 3. Analyzing Specific Build

**Scenario:** Multiple TeamCity CI runs exist for a PR, and you want to analyze a specific one

**Solution:**
1. Find the workflow run ID from the "Actions" tab
2. Manually trigger with both `pr_number` and `workflow_run_id`
3. Workflow will analyze that specific run

### 4. Debugging

**Scenario:** You want to see what the workflow would do for a specific commit

**Solution:**
1. Get the commit SHA from the PR
2. Manually trigger with `pr_number` and `head_sha`
3. Workflow will analyze builds for that specific commit

---

## Troubleshooting

### Error: "PR number is required for manual trigger"

**Cause:** `pr_number` parameter was not provided or is invalid

**Solution:** Ensure you enter a valid PR number when triggering manually

### Error: "No TeamCity CI workflow run found"

**Cause:** No TeamCity CI run exists for the specified commit

**Solution:** 
- Check that TeamCity CI actually ran for the PR
- Verify the commit SHA is correct
- Ensure the workflow_run_id (if provided) is valid

### Workflow Runs but Does Nothing

**Cause:** TeamCity builds may not have failed, or builds are still pending

**Solution:**
- Check the "Wait for TeamCity Builds to Complete" step output
- Ensure at least one TeamCity build failed
- Wait for builds to complete before triggering

---

## Permissions Required

**To Trigger Manually:**
- Write access to the repository (collaborator or maintainer)

**To View Results:**
- Read access to the repository

---

## Examples

### Example 1: Simple Manual Trigger

**URL:** https://github.com/ImagingTools/Acf/actions/workflows/auto-fix-on-failure.yml

**Parameters:**
```
pr_number: 128
```

**Result:**
- Workflow fetches PR #128 details
- Auto-detects the latest TeamCity CI run for the PR's head commit
- Analyzes build failures and posts results

### Example 2: Specific Workflow Run

**URL:** https://github.com/ImagingTools/Acf/actions/workflows/auto-fix-on-failure.yml

**Parameters:**
```
pr_number: 128
workflow_run_id: 21760295776
```

**Result:**
- Workflow analyzes TeamCity CI run 21760295776 specifically
- Useful when multiple runs exist and you want a specific one

### Example 3: Specific Commit

**URL:** https://github.com/ImagingTools/Acf/actions/workflows/auto-fix-on-failure.yml

**Parameters:**
```
pr_number: 128
head_sha: 75f99a24cd480484f479049baefd58c363c54e9d
```

**Result:**
- Workflow analyzes builds for commit 75f99a24
- Useful for analyzing a specific commit in the PR's history

---

## Comparison: Automatic vs Manual

| Aspect | Automatic (check_run) | Manual (workflow_dispatch) |
|--------|----------------------|---------------------------|
| **Trigger** | GitHub check_run event | User initiates via UI/API |
| **When** | After TeamCity check fails | Anytime user chooses |
| **PR Info Source** | Event payload | GitHub API (via pr_number) |
| **Workflow Run ID** | Auto-detected from check_run | Provided or auto-detected |
| **Conditional** | Only if check failed | Always runs |
| **Use Case** | Production automation | Testing, debugging, re-runs |

---

## Summary

The auto-fix workflow now supports both automatic and manual triggering:

**Automatic:**
- Responds to TeamCity CI failures automatically
- No user intervention required
- Production use

**Manual:**
- User-initiated via GitHub UI
- Requires PR number
- Testing, debugging, and re-runs

Both trigger methods use the same workflow logic and produce the same results. The manual trigger provides flexibility for testing and handling edge cases where automatic triggering fails.

---

**Documentation Status:** ✅ COMPLETE  
**Feature Status:** ✅ IMPLEMENTED  
**Testing Status:** ⏳ READY FOR TESTING  
**Generated:** 2026-02-06 17:58 UTC  
**Agent:** GitHub Copilot Coding Agent
