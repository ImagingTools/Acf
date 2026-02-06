#!/usr/bin/env node

/**
 * Test script to validate the wait-for-checks logic
 * This simulates the behavior of the "Wait for All PR Checks to Complete" step
 */

// Mock check runs data for testing
const mockCheckRuns = {
  scenario1: {
    name: "All checks completed, TeamCity failed",
    checks: [
      { name: "Attempt Auto-Fix", status: "in_progress", conclusion: null },
      { name: "Trigger TeamCity Build (windows)", status: "completed", conclusion: "failure" },
      { name: "Trigger TeamCity Build (linux)", status: "completed", conclusion: "success" },
      { name: "Security Scanning / Dependency Review", status: "completed", conclusion: "success" },
      { name: "Security Scanning / SBOM Validation", status: "completed", conclusion: "success" }
    ],
    expected: { proceed: true, reason: "All checks completed and at least one TeamCity build failed" }
  },
  scenario2: {
    name: "All checks completed, all TeamCity passed",
    checks: [
      { name: "Attempt Auto-Fix", status: "in_progress", conclusion: null },
      { name: "Trigger TeamCity Build (windows)", status: "completed", conclusion: "success" },
      { name: "Trigger TeamCity Build (linux)", status: "completed", conclusion: "success" },
      { name: "Security Scanning / Dependency Review", status: "completed", conclusion: "success" }
    ],
    expected: { proceed: false, reason: "No TeamCity builds failed" }
  },
  scenario3: {
    name: "Some checks still pending",
    checks: [
      { name: "Attempt Auto-Fix", status: "in_progress", conclusion: null },
      { name: "Trigger TeamCity Build (windows)", status: "completed", conclusion: "failure" },
      { name: "Trigger TeamCity Build (linux)", status: "in_progress", conclusion: null },
      { name: "Security Scanning / Dependency Review", status: "queued", conclusion: null }
    ],
    expected: { proceed: null, reason: "Should continue waiting" }
  },
  scenario4: {
    name: "Both TeamCity builds failed",
    checks: [
      { name: "Attempt Auto-Fix", status: "in_progress", conclusion: null },
      { name: "Trigger TeamCity Build (windows)", status: "completed", conclusion: "failure" },
      { name: "Trigger TeamCity Build (linux)", status: "completed", conclusion: "failure" },
      { name: "Security Scanning / Dependency Review", status: "completed", conclusion: "success" }
    ],
    expected: { proceed: true, reason: "All checks completed and multiple TeamCity builds failed" }
  }
};

function testWaitLogic(checkRuns) {
  // Filter out this workflow's own check runs
  const otherChecks = checkRuns.filter(check => 
    check.name !== 'Attempt Auto-Fix'
  );
  
  console.log(`  Monitoring ${otherChecks.length} checks (excluding this workflow)`);
  
  // Check if all checks are completed
  const pendingChecks = otherChecks.filter(check => 
    check.status !== 'completed'
  );
  
  if (pendingChecks.length === 0) {
    console.log('  ✅ All PR checks have completed');
    
    // Check if any TeamCity build failed
    const teamcityChecks = otherChecks.filter(check => 
      check.name.includes('Trigger TeamCity Build')
    );
    
    console.log(`  Found ${teamcityChecks.length} TeamCity check(s)`);
    
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
  
  console.log(`  ⏳ ${pendingChecks.length} check(s) still pending:`);
  for (const check of pendingChecks) {
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
