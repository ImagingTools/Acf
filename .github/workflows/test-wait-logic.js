#!/usr/bin/env node

/**
 * Test script to validate the wait-for-TeamCity-builds logic
 * This simulates the behavior of the "Wait for TeamCity Builds to Complete" step
 */

// Mock check runs data for testing
const mockCheckRuns = {
  scenario1: {
    name: "TeamCity builds completed, one failed",
    checks: [
      { name: "Attempt Auto-Fix", status: "in_progress", conclusion: null },
      { name: "Trigger TeamCity Build (windows)", status: "completed", conclusion: "failure" },
      { name: "Trigger TeamCity Build (linux)", status: "completed", conclusion: "success" },
      { name: "Security Scanning / Dependency Review", status: "in_progress", conclusion: null },
      { name: "Security Scanning / SBOM Validation", status: "queued", conclusion: null }
    ],
    expected: { proceed: true, reason: "TeamCity builds completed and at least one failed" }
  },
  scenario2: {
    name: "TeamCity builds completed, all passed",
    checks: [
      { name: "Attempt Auto-Fix", status: "in_progress", conclusion: null },
      { name: "Trigger TeamCity Build (windows)", status: "completed", conclusion: "success" },
      { name: "Trigger TeamCity Build (linux)", status: "completed", conclusion: "success" },
      { name: "Security Scanning / Dependency Review", status: "in_progress", conclusion: null }
    ],
    expected: { proceed: false, reason: "No TeamCity builds failed" }
  },
  scenario3: {
    name: "TeamCity builds still pending",
    checks: [
      { name: "Attempt Auto-Fix", status: "in_progress", conclusion: null },
      { name: "Trigger TeamCity Build (windows)", status: "completed", conclusion: "failure" },
      { name: "Trigger TeamCity Build (linux)", status: "in_progress", conclusion: null },
      { name: "Security Scanning / Dependency Review", status: "completed", conclusion: "success" }
    ],
    expected: { proceed: null, reason: "Should continue waiting for TeamCity builds" }
  },
  scenario4: {
    name: "Both TeamCity builds failed",
    checks: [
      { name: "Attempt Auto-Fix", status: "in_progress", conclusion: null },
      { name: "Trigger TeamCity Build (windows)", status: "completed", conclusion: "failure" },
      { name: "Trigger TeamCity Build (linux)", status: "completed", conclusion: "failure" },
      { name: "Security Scanning / Dependency Review", status: "queued", conclusion: null }
    ],
    expected: { proceed: true, reason: "Multiple TeamCity builds failed" }
  }
};

function testWaitLogic(checkRuns) {
  // Filter to only TeamCity checks
  const teamcityChecks = checkRuns.filter(check => 
    check.name.includes('Trigger TeamCity Build')
  );
  
  console.log(`  Found ${teamcityChecks.length} TeamCity check(s)`);
  
  if (teamcityChecks.length === 0) {
    console.log('  ⚠️ No TeamCity checks found - skipping auto-fix');
    return { proceed: false };
  }
  
  // Check if all TeamCity builds are completed
  const pendingTeamcityChecks = teamcityChecks.filter(check => 
    check.status !== 'completed'
  );
  
  if (pendingTeamcityChecks.length === 0) {
    console.log('  ✅ All TeamCity builds have completed');
    
    // Check if any TeamCity build failed
    const failedTeamcityChecks = teamcityChecks.filter(check => 
      check.conclusion === 'failure'
    );
    
    if (failedTeamcityChecks.length > 0) {
      console.log(`  ✅ ${failedTeamcityChecks.length} TeamCity build(s) failed - proceeding with auto-fix`);
      for (const check of failedTeamcityChecks) {
        console.log(`    - ${check.name}: ${check.conclusion}`);
      }
      return { proceed: true };
    } else {
      console.log('  ⚠️ No TeamCity builds failed - skipping auto-fix');
      return { proceed: false };
    }
  }
  
  console.log(`  ⏳ ${pendingTeamcityChecks.length} TeamCity build(s) still pending:`);
  for (const check of pendingTeamcityChecks) {
    console.log(`    - ${check.name}: ${check.status}`);
  }
  
  return null; // Continue waiting
}

console.log("Testing Auto-Fix Wait Logic\n");
console.log("=".repeat(60));

let allTestsPassed = true;

for (const scenario of Object.values(mockCheckRuns)) {
  console.log(`\n${scenario.name}`);
  console.log("-".repeat(60));
  
  const result = testWaitLogic(scenario.checks);
  
  // Check if result matches expected
  const expectedResult = scenario.expected.proceed;
  const actualResult = result?.proceed ?? null;
  
  const passed = expectedResult === actualResult;
  
  console.log(`\n  Result: ${JSON.stringify(result)}`);
  console.log(`  Expected: proceed=${expectedResult}`);
  console.log(`  Status: ${passed ? '✅ PASS' : '❌ FAIL'}`);
  
  if (!passed) {
    allTestsPassed = false;
  }
}

console.log("\n" + "=".repeat(60));
console.log(allTestsPassed ? "\n✅ All tests passed!" : "\n❌ Some tests failed!");
console.log("=".repeat(60));

process.exit(allTestsPassed ? 0 : 1);
