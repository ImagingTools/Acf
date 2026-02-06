# Workflow Approval Configuration

**Date:** 2026-02-06  
**Requirement:** Allow all workflows to run without manual approval

---

## Current Issue

GitHub Actions workflows triggered by pull requests from contributors require manual approval before they can run. This is a security feature that prevents potentially malicious code from executing in workflows.

**Current Behavior:**
- TeamCity CI workflow requires approval for PR #128
- This blocks the auto-fix workflow from being tested properly
- Manual approval is needed before each workflow run

---

## Required Configuration Changes

### Option 1: Disable Fork Pull Request Workflows Approval (Recommended for Testing)

**Location:** Repository Settings → Actions → General

**Setting to Change:**
- **"Fork pull request workflows from outside collaborators"**
- Change from: **"Require approval for first-time contributors"** or **"Require approval for all outside collaborators"**
- Change to: **"Run workflows from fork pull requests"** (no approval required)

**Steps:**
1. Go to: https://github.com/ImagingTools/Acf/settings/actions
2. Scroll to "Fork pull request workflows from outside collaborators"
3. Select: **"Require approval for first-time contributors who recently created their account"** (least restrictive while maintaining some security)
   - OR select: **"Run workflows from fork pull requests"** (no approval at all)

### Option 2: Add User as Collaborator

**Location:** Repository Settings → Collaborators

**Steps:**
1. Go to: https://github.com/ImagingTools/Acf/settings/access
2. Click "Add people"
3. Add the user (copilot-swe-agent or the testing account)
4. Grant appropriate permissions

**Effect:**
- Workflows from this user will no longer require approval
- User becomes a trusted contributor

---

## Security Considerations

### Risks of Disabling Approval

**Without approval, workflows can:**
- Access repository secrets
- Execute arbitrary code
- Make changes to the repository (if permissions allow)
- Consume GitHub Actions minutes

**Mitigation:**
1. **Review all workflow files** before disabling approval
2. **Limit workflow permissions** using `permissions:` blocks
3. **Monitor workflow runs** for suspicious activity
4. **Re-enable approval** after testing is complete

### Current Workflow Permissions

The auto-fix workflow has these permissions:
```yaml
permissions:
  contents: write
  pull-requests: write
  issues: write
  actions: read
  checks: read
```

These are necessary for the workflow to function but are limited in scope.

---

## Recommended Approach for This Testing

### For Immediate Testing (PR #128)

**Option A: Manual Approval** (Current)
- Maintainer approves each workflow run manually
- Safe but requires human intervention
- Link to approve: https://github.com/ImagingTools/Acf/actions/runs/21760295776

**Option B: Change Repository Settings** (Recommended)
1. Temporarily change fork PR workflow setting to "Require approval for first-time contributors who recently created their account"
2. This allows the copilot-swe-agent bot to run workflows without approval
3. Complete the testing
4. Revert the setting back to stricter approval if desired

### For Long-Term Production Use

**Recommendation:** Keep approval enabled for external contributors, but:
1. Add trusted bot accounts (like copilot-swe-agent) as collaborators
2. Or use "Require approval for first-time contributors who recently created their account" setting
3. This balances security with functionality

---

## How to Make the Change

### Via GitHub Web Interface

1. Navigate to: https://github.com/ImagingTools/Acf/settings/actions
2. Scroll to: **"Fork pull request workflows from outside collaborators"**
3. Select one of:
   - ☐ Require approval for all outside collaborators
   - ☐ Require approval for first-time contributors
   - ☑ **Require approval for first-time contributors who recently created their account** (RECOMMENDED)
   - ☐ Run workflows from fork pull requests
4. Click **Save**

### Verification

After changing the setting:
1. Push a new commit to PR #128
2. Verify TeamCity CI workflow starts automatically without "action_required" status
3. Verify auto-fix workflow can trigger when TeamCity CI fails

---

## Who Can Make This Change

**Required Permission:** Repository Admin or Organization Owner

**Current Maintainers:**
- @kirill-lepski (likely has admin access)
- Organization owners of ImagingTools

---

## Alternative: Approve Current Workflow Run

If you don't want to change repository settings, you can approve the current workflow run:

**Link:** https://github.com/ImagingTools/Acf/actions/runs/21760295776

**Effect:**
- Only this specific run will execute
- Future runs will still require approval
- Allows testing to continue without changing repository settings

---

## Summary

**To allow workflows to run without approval:**

1. **Go to:** https://github.com/ImagingTools/Acf/settings/actions
2. **Find:** "Fork pull request workflows from outside collaborators"
3. **Select:** "Require approval for first-time contributors who recently created their account"
4. **Click:** Save

This change requires **repository admin permissions** and balances security with testing needs.

---

**Status:** Awaiting repository admin to make configuration change  
**Action Required:** Repository settings modification (not code change)  
**Estimated Time:** < 2 minutes  
**Risk Level:** Low (can be reverted at any time)
