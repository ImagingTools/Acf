#!/bin/bash
# Script to test the auto-fix workflow logic locally
# This simulates the TeamCity problem parsing and issue creation logic

set -euo pipefail

echo "=== Auto-Fix Workflow Local Test ==="
echo ""

# Simulate TeamCity API response with build problems
SAMPLE_PROBLEMS='[
  {
    "id": "1",
    "type": "TC_COMPILATION_ERROR",
    "identity": "Impl/ilog/CLogRouter.cpp:42",
    "details": "error: '\''ILogger'\'' was not declared in this scope",
    "build": {
      "id": "12345",
      "buildTypeId": "Acf_Build_Linux",
      "webUrl": "https://teamcity.example.com/viewLog.html?buildId=12345",
      "branchName": "feature/test-branch"
    }
  },
  {
    "id": "2",
    "type": "TC_COMPILATION_ERROR",
    "identity": "Impl/ibase/CComponent.cpp:89",
    "details": "error: no matching function for call to '\''Initialize'\''",
    "build": {
      "id": "12345",
      "buildTypeId": "Acf_Build_Linux",
      "webUrl": "https://teamcity.example.com/viewLog.html?buildId=12345",
      "branchName": "feature/test-branch"
    }
  },
  {
    "id": "3",
    "type": "TC_LINKER_ERROR",
    "identity": "undefined reference to ilog::LogRouter",
    "details": "ld: Impl/ilog/CLogRouter.o: in function '\''main'\'': undefined reference to '\''ilog::LogRouter::LogMessage(char const*)'\''",
    "build": {
      "id": "12346",
      "buildTypeId": "Acf_Build_Windows",
      "webUrl": "https://teamcity.example.com/viewLog.html?buildId=12346",
      "branchName": "feature/test-branch"
    }
  }
]'

echo "Sample TeamCity Build Problems:"
echo "$SAMPLE_PROBLEMS" | jq '.'
echo ""

# Parse problems
PROBLEM_COUNT=$(echo "$SAMPLE_PROBLEMS" | jq '. | length')
echo "Found $PROBLEM_COUNT problem(s)"
echo ""

# Simulate issue creation for each problem
ISSUE_NUM=100
for i in $(seq 0 $((PROBLEM_COUNT - 1))); do
  PROBLEM=$(echo "$SAMPLE_PROBLEMS" | jq ".[$i]")
  
  PROBLEM_TYPE=$(echo "$PROBLEM" | jq -r '.type')
  PROBLEM_IDENTITY=$(echo "$PROBLEM" | jq -r '.identity')
  PROBLEM_DETAILS=$(echo "$PROBLEM" | jq -r '.details')
  BUILD_URL=$(echo "$PROBLEM" | jq -r '.build.webUrl')
  BUILD_ID=$(echo "$PROBLEM" | jq -r '.build.id')
  BRANCH_NAME=$(echo "$PROBLEM" | jq -r '.build.branchName')
  
  echo "--- Issue #$ISSUE_NUM ---"
  echo "Title: [Auto-Fix] $PROBLEM_TYPE: $PROBLEM_IDENTITY"
  echo "Labels: auto-fix, build-failure, copilot-task"
  echo ""
  echo "Body:"
  echo "## Build Problem Detected"
  echo ""
  echo "**Type:** $PROBLEM_TYPE"
  echo "**Identity:** $PROBLEM_IDENTITY"
  echo "**Branch:** $BRANCH_NAME"
  echo "**PR:** #123"
  echo ""
  echo "**TeamCity Build:** [Build $BUILD_ID]($BUILD_URL)"
  echo ""
  echo "### Problem Details"
  echo "\`\`\`"
  echo "$PROBLEM_DETAILS"
  echo "\`\`\`"
  echo ""
  echo "### Context"
  echo "This issue was automatically created by the auto-fix workflow after detecting a build failure."
  echo ""
  echo "### Task for Copilot"
  echo "Please analyze the build problem and:"
  echo "1. Identify the root cause of the issue"
  echo "2. Implement a fix for the problem"
  echo "3. Ensure the fix doesn't break existing functionality"
  echo "4. Test the changes to verify the build passes"
  echo ""
  echo "**Workflow Run:** https://github.com/ImagingTools/Acf/actions/runs/12345"
  echo "**Original Build Failure:** https://github.com/ImagingTools/Acf/actions/runs/12344"
  echo ""
  echo "=========================================="
  echo ""
  
  ISSUE_NUM=$((ISSUE_NUM + 1))
done

echo "✅ Test completed successfully!"
echo ""
echo "Summary:"
echo "- Parsed $PROBLEM_COUNT build problem(s)"
echo "- Would create issues: #100, #101, #102"
echo "- All issues would have labels: auto-fix, build-failure, copilot-task"
echo ""
echo "Next steps:"
echo "1. Test with real build failure by creating a test PR"
echo "2. Verify issues are created in GitHub"
echo "3. Check that Copilot can access and work on the issues"
