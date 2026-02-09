# Auto-Fix Button Feature

**Date:** 2026-02-06  
**Feature:** Automatic "Execute Auto-Fix" button on PR when CI fails  
**Workflow:** `.github/workflows/post-autofix-button.yml`

---

## Overview

When TeamCity CI fails on a pull request, a comment is automatically posted with an **"Execute Auto-Fix"** button. This provides a one-click experience to trigger the auto-fix workflow.

---

## How It Works

### Automatic Trigger

**When:**
- TeamCity CI workflow completes with `failure` conclusion
- The workflow was triggered by a `pull_request` event

**What Happens:**
1. The "Post Auto-Fix Button" workflow triggers via `workflow_run` event
2. Finds the PR associated with the failed CI run
3. Posts a comment on the PR with:
   - A button/link to trigger auto-fix
   - Pre-populated parameter values (PR number, workflow run ID, commit SHA)
   - Quick reference section with copy-paste values

### Comment Structure

```markdown
## 🔧 Auto-Fix Available

The TeamCity CI build has failed. You can run the auto-fix workflow to analyze the build errors.

### Execute Auto-Fix

Click the button below to trigger the auto-fix workflow:

**[▶️ Execute Auto-Fix](https://github.com/ImagingTools/Acf/actions/workflows/auto-fix-on-failure.yml)**

When the workflow page opens:
1. Click "Run workflow"
2. Enter PR number: `128`
3. (Optional) Enter workflow run ID: `21760295776`
4. (Optional) Enter commit SHA: `75f99a24cd...`
5. Click the green "Run workflow" button

---
<details>
<summary>Quick reference values</summary>

- **PR Number:** `128`
- **Workflow Run ID:** `21760295776`
- **Commit SHA:** `75f99a24cd...`

</details>
```

---

## User Experience

### Step 1: CI Fails
- TeamCity CI runs and fails
- User receives notification about CI failure

### Step 2: Auto-Fix Button Appears
- Comment automatically posted on PR
- Comment includes "Execute Auto-Fix" button

### Step 3: One Click to Trigger
- User clicks the "Execute Auto-Fix" button
- GitHub Actions workflow page opens
- User clicks "Run workflow"
- Enters the pre-provided PR number (already shown in comment)
- Optionally enters workflow run ID and SHA (also provided)
- Clicks "Run workflow"

### Step 4: Results Posted
- Auto-fix workflow analyzes build logs
- Posts results as a new comment on the PR
- User gets actionable error information

---

## Features

### Smart Detection
- Only posts when CI actually fails (not on success or action_required)
- Only posts for pull_request-triggered workflows (not push events)
- Finds the correct PR even if multiple PRs exist for the same commit

### No Duplicates
- Checks for existing auto-fix button comments for the same workflow run
- Won't post duplicate buttons if run is re-triggered

### Pre-Populated Values
- Comment includes all necessary parameter values
- Copy-paste ready for quick execution
- Collapsible "Quick reference" section to avoid clutter

---

## Technical Details

### Workflow File
`.github/workflows/post-autofix-button.yml`

### Trigger
```yaml
on:
  workflow_run:
    workflows: ["TeamCity CI"]
    types:
      - completed
```

### Conditional
```yaml
if: |
  github.event.workflow_run.conclusion == 'failure' &&
  github.event.workflow_run.event == 'pull_request'
```

### Permissions Required
```yaml
permissions:
  pull-requests: write
  issues: write
```

---

## Limitations

### GitHub UI Constraints

**No Custom Buttons:**
- GitHub doesn't support adding actual buttons to PR UI
- Best alternative: Posted comment with markdown link
- Link opens workflow_dispatch page

**No Pre-Filled Forms:**
- GitHub workflow_dispatch doesn't support URL parameters to pre-fill inputs
- Users must still enter parameter values manually
- Comment provides the values to enter

### Workaround Provided

**The comment includes:**
1. Direct link to workflow_dispatch page
2. Clear instructions on what to enter
3. All parameter values prominently displayed
4. Collapsible quick reference section

**User Experience:**
- One click to open workflow page
- Copy-paste values from comment
- One more click to execute

**Total: 2-3 clicks** (vs navigating through Actions tab: 4-5 clicks)

---

## Alternative Approaches Considered

### 1. Repository Dispatch with Issue Comments
**How:** Listen for `/auto-fix` command in PR comments

**Pros:**
- True one-click experience (just type `/auto-fix`)
- No need to navigate away from PR

**Cons:**
- Requires setting up issue_comment webhooks
- Need to parse and validate commands
- More complex implementation

**Status:** Not implemented (can add later if needed)

### 2. GitHub App with Custom UI
**How:** Create a GitHub App that adds custom UI elements

**Pros:**
- Can add actual buttons to PR UI
- Professional integration

**Cons:**
- Requires GitHub App creation and installation
- OAuth setup and management
- Much more complex infrastructure

**Status:** Out of scope for current implementation

### 3. Direct API Call from Comment
**How:** Use GitHub Actions API to trigger workflow from comment event

**Pros:**
- Can trigger workflow directly
- No need to leave PR page

**Cons:**
- Still requires comment-based command
- Need to handle authentication and permissions

**Status:** Could be next iteration if current solution isn't sufficient

---

## Future Enhancements

### Potential Improvements

1. **Comment-Based Trigger:**
   - Add support for `/auto-fix` command in comments
   - Automatically trigger workflow when user comments
   - Reply with status when workflow starts

2. **Status Updates:**
   - Update the button comment when workflow starts
   - Show "Running..." status
   - Link to workflow run

3. **Results Integration:**
   - Include auto-fix results in the same comment thread
   - Use reactions to indicate status (✅ success, ❌ failure)

4. **Smart Defaults:**
   - If only one failed build, auto-select it
   - Reduce required user input

---

## Testing

### Manual Test

1. Create a PR with failing CI
2. Wait for TeamCity CI to complete with failure
3. Check PR for auto-fix button comment
4. Click the button
5. Verify workflow page opens
6. Verify parameter values match comment
7. Execute workflow
8. Verify results posted to PR

### Verification

**Comment Posted:** ✅ Check PR comments for "🔧 Auto-Fix Available"

**Values Correct:** ✅ Verify PR number, run ID, and SHA match actual values

**Link Works:** ✅ Click button and verify workflow page opens

**No Duplicates:** ✅ Re-run CI and verify no duplicate comments

---

## Summary

This feature provides a streamlined user experience for triggering auto-fix analysis when CI fails:

**Before:**
1. Navigate to Actions tab
2. Find auto-fix workflow
3. Click "Run workflow"
4. Manually look up PR number
5. Manually find workflow run ID
6. Enter values and run

**After:**
1. See comment with button on PR
2. Click "Execute Auto-Fix" button
3. Copy-paste provided values
4. Click "Run workflow"

**Result:** Reduced from 6 steps to 4 steps, with all values provided automatically.

---

**Status:** ✅ IMPLEMENTED  
**Testing:** ⏳ READY FOR TESTING  
**Documentation:** ✅ COMPLETE  
**Generated:** 2026-02-06 18:02 UTC  
**Agent:** GitHub Copilot Coding Agent
