# Auto-Fix Workflow Flow Diagram

## Current Implementation Flow

```
┌─────────────────────────────────────────────────────────────┐
│  PR Created/Updated                                         │
└────────────────┬────────────────────────────────────────────┘
                 │
                 ▼
┌─────────────────────────────────────────────────────────────┐
│  TeamCity CI Workflow Triggers                              │
│  - Windows Build (matrix)                                   │
│  - Linux Build (matrix)                                     │
└────────────────┬────────────────────────────────────────────┘
                 │
                 ▼
┌─────────────────────────────────────────────────────────────┐
│  Other PR Checks Run                                        │
│  - Security Scanning / Dependency Review                    │
│  - Security Scanning / SBOM Validation                      │
│  - Other configured checks                                  │
└────────────────┬────────────────────────────────────────────┘
                 │
                 ▼
┌─────────────────────────────────────────────────────────────┐
│  TeamCity CI Workflow Completes                             │
│  (Success or Failure)                                       │
└────────────────┬────────────────────────────────────────────┘
                 │
                 ▼
┌─────────────────────────────────────────────────────────────┐
│  Auto-Fix Workflow Triggers                                 │
│  (via workflow_run event)                                   │
└────────────────┬────────────────────────────────────────────┘
                 │
                 ▼
┌─────────────────────────────────────────────────────────────┐
│  Get PR Information                                         │
└────────────────┬────────────────────────────────────────────┘
                 │
                 ▼
┌─────────────────────────────────────────────────────────────┐
│  Wait for All PR Checks to Complete                         │
│  ┌───────────────────────────────────────────────────────┐  │
│  │  Poll every 30 seconds (max 30 minutes)              │  │
│  │                                                       │  │
│  │  For each attempt:                                   │  │
│  │  1. Get all check runs for PR commit                │  │
│  │  2. Filter out self (Attempt Auto-Fix)              │  │
│  │  3. Check if all are completed                      │  │
│  │                                                       │  │
│  │  If all completed:                                   │  │
│  │  - Find TeamCity checks                             │  │
│  │  - Check for failures                               │  │
│  │  - Return proceed: true/false                       │  │
│  └───────────────────────────────────────────────────────┘  │
└────────────────┬────────────────────────────────────────────┘
                 │
                 ▼
         ┌───────┴───────┐
         │               │
         ▼               ▼
┌──────────────┐  ┌──────────────────┐
│ proceed:     │  │ proceed:         │
│ false        │  │ true             │
└──────┬───────┘  └─────┬────────────┘
       │                │
       ▼                ▼
┌──────────────┐  ┌──────────────────┐
│ Skip         │  │ Checkout PR      │
│ Auto-Fix     │  │ branch           │
│              │  └─────┬────────────┘
│ (Log reason) │        │
└──────────────┘        ▼
                 ┌──────────────────┐
                 │ Analyze Build    │
                 │ Failure          │
                 │ - Get TeamCity   │
                 │   build IDs      │
                 │ - Fetch logs     │
                 │ - Parse errors   │
                 └─────┬────────────┘
                       │
                       ▼
                 ┌──────────────────┐
                 │ Attempt Common   │
                 │ Fixes            │
                 │ (Future impl.)   │
                 └─────┬────────────┘
                       │
                       ▼
                 ┌──────────────────┐
                 │ Comment on PR    │
                 │ - @github-copilot│
                 │ - Build errors   │
                 │ - TeamCity links │
                 └─────┬────────────┘
                       │
                       ▼
                 ┌──────────────────┐
                 │ Commit & Push    │
                 │ (if fixes made)  │
                 └──────────────────┘
```

## Decision Points

### Wait for Checks Decision
```
All checks completed?
├─ No  → Continue waiting (up to 30 min)
└─ Yes → Check TeamCity results
         ├─ Any TeamCity failed? → proceed: true
         └─ All passed?          → proceed: false
```

### Skip Reasons
- ❌ Timeout (30 minutes reached)
- ✅ All checks passed (no TeamCity failures)
- ⚠️ No PR found for branch

## Key Features

✅ **Complete Context**: Waits for ALL checks before acting
✅ **Smart Detection**: Only proceeds on actual TeamCity failures  
✅ **Graceful Handling**: Clear logging for skipped runs
✅ **Timeout Protection**: Max 30 minute wait with fallback
✅ **Self-Awareness**: Excludes own check from monitoring
