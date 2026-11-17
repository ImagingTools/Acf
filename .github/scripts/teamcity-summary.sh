#!/usr/bin/env bash

# Helpers of the TeamCity CI workflow to describe failed TeamCity builds in a pull request comment.
# Source this file in a step. It needs TEAMCITY_URL and TEAMCITY_TOKEN in the environment and jq on the path.

MAX_PROBLEMS=10
MAX_FAILED_TESTS=20
MAX_ERROR_LINES=30
MAX_LINE_LENGTH=300

# Prints a TeamCity REST response as JSON.
# Usage: teamcity_get <path and query>
teamcity_get()
{
  curl -sS \
    -H "Authorization: Bearer $TEAMCITY_TOKEN" \
    -H "Accept: application/json" \
    "$TEAMCITY_URL/app/rest/$1"
}

# Prints the error lines of the build steps, without the timestamps, the step markers and repeated lines.
# Only the output of the build steps is read: a failing git mirror update during the checkout does not fail the build.
# The three lines after a CMake error are kept, because they say what is wrong.
# Finding no line is not an error, and neither is head closing the pipe: the function always succeeds.
# Usage: teamcity_step_errors <build id>
teamcity_step_errors()
{
  curl -sS \
    -H "Authorization: Bearer $TEAMCITY_TOKEN" \
    "$TEAMCITY_URL/downloadBuildLog.html?buildId=$1" \
    | grep -E '\[Step [0-9]+/[0-9]+\]' \
    | sed -E 's/^\[[0-9:]+\][A-Za-z ]?:[[:space:]]*(\[[^]]*\][[:space:]]*)*\[Step [0-9]+\/[0-9]+\][[:space:]]*//' \
    | grep -vE 'Process exited with code|Changed build status|is skipped because|^Step .* failed$|^Call Stack' \
    | awk '
        /CMake Error/ { print; context = 3; next }
        context > 0 { print; context--; next }
        /([Ee]rror|ERROR|[Ff]atal)[: ]|FAILED|undefined reference|ninja:/ { print }
      ' \
    | sed 's/```/'"'''"'/g' \
    | cut -c1-"$MAX_LINE_LENGTH" \
    | awk '!seen[$0]++' \
    | head -n "$MAX_ERROR_LINES" \
    || true
}

# Prints the failed tests of a build, each with the failure lines of its output.
# Usage: teamcity_failed_tests <build id>
teamcity_failed_tests()
{
  teamcity_get "testOccurrences?locator=build:(id:$1),status:FAILURE,count:$MAX_FAILED_TESTS&fields=testOccurrence(name,details)" \
    | jq -r '.testOccurrence[]?
      | "- " + (.name | sub("^\\(empty\\): "; ""))
        + ((.details // "") | split("\n")
          | map(select(test("^FAIL!|^   (Loc|Actual|Expected)")))
          | .[:4]
          | map("\n  - `" + (gsub("`"; "") | sub("^ +"; "") | .[0:200]) + "`")
          | join(""))'
}

# Prints the markdown section of one failed build and nothing for a build that passed or was cancelled.
# Usage: teamcity_build_summary <build id> <platform>
teamcity_build_summary()
{
  local build status state cancelled text url problems test_count errors

  build=$(teamcity_get "builds/id:$1?fields=state,status,statusText,webUrl,canceledInfo") || return 0
  state=$(echo "$build" | jq -r '.state // empty')
  status=$(echo "$build" | jq -r '.status // empty')
  cancelled=$(echo "$build" | jq -r 'if .canceledInfo then "yes" else "no" end')

  if [ "$state" != "finished" ] || [ "$status" = "SUCCESS" ] || [ "$cancelled" = "yes" ]; then
    return 0
  fi

  text=$(echo "$build" | jq -r '.statusText // "failed"')
  url=$(echo "$build" | jq -r '.webUrl // empty')
  echo "### $2: $text"
  echo
  echo "[Build $1]($url)"
  echo

  # The exit code and the failed tests are already in the status text and the list of tests below.
  problems=$(teamcity_get "builds/id:$1/problemOccurrences?fields=problemOccurrence(type,details)" \
    | jq -r --argjson max "$MAX_PROBLEMS" '[.problemOccurrence[]? | select(.type != "TC_EXIT_CODE" and .type != "TC_FAILED_TESTS") | "- " + (.details // .type)] | unique | .[:$max][]') || problems=""
  if [ -n "$problems" ]; then
    echo "$problems"
    echo
  fi

  # With failed tests the build log is full of test output, so the tests are shown instead of it.
  test_count=$(teamcity_get "testOccurrences?locator=build:(id:$1),status:FAILURE&fields=count" | jq -r '.count // 0') || test_count=0
  if [ "$test_count" -gt 0 ]; then
    echo "Failed tests ($test_count):"
    echo
    teamcity_failed_tests "$1" || true
    echo
    return 0
  fi

  errors=$(teamcity_step_errors "$1")
  if [ -n "$errors" ]; then
    echo '```'
    echo "$errors"
    echo '```'
    echo
  fi
}

# Prints the whole comment for the builds listed in the teamcity-build-id-<platform>.txt files of a directory.
# Prints nothing when none of the builds failed.
# Usage: teamcity_analysis_report <directory> <commit> <workflow run url>
teamcity_analysis_report()
{
  local file platform id section sections

  sections=""
  for file in "$1"/teamcity-build-id-*.txt; do
    [ -f "$file" ] || continue
    platform=${file##*teamcity-build-id-}
    platform=${platform%.txt}
    id=$(tr -d '[:space:]' < "$file")
    [ -n "$id" ] || continue
    section=$(teamcity_build_summary "$id" "${platform^}") || section=""
    if [ -n "$section" ]; then
      sections="$sections$section"$'\n\n'
    fi
  done

  if [ -z "$sections" ]; then
    return 0
  fi

  echo "## TeamCity build failed"
  echo
  echo "Commit \`${2:0:8}\`, [workflow run]($3)"
  echo
  printf '%s' "$sections"
}
