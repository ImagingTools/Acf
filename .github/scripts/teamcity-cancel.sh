#!/usr/bin/env bash

# Helpers of the TeamCity CI workflow to cancel TeamCity builds. Source this file in a step.
# They need TEAMCITY_URL and TEAMCITY_TOKEN in the environment and jq on the path.

# Cancels one TeamCity build and fails when TeamCity does not accept the request.
# Usage: teamcity_cancel_build <build id> <comment>
teamcity_cancel_build()
{
  local payload
  payload=$(jq -n --arg comment "$2" '{ buildCancelRequest: { comment: $comment, readdIntoQueue: false } }')

  curl --fail-with-body -sS \
    -X POST \
    -H "Authorization: Bearer $TEAMCITY_TOKEN" \
    -H "Content-Type: application/json" \
    -H "Accept: application/json" \
    --data "$payload" \
    "$TEAMCITY_URL/app/rest/builds/id:$1" > /dev/null
}

# Cancels the queued and running builds of a branch. A failure is a warning, the caller goes on.
# The branch is matched here and not in the TeamCity locator, because a branch name such as
# "fix(ui),x" is a valid git ref but breaks the locator syntax.
# A branch name is unique for a build type: the pull request VCS roots of TeamCity fetch only the branches of this
# repository, and a pull request from a fork gets no secrets, so it never queues a build.
# The builds of main and master are never cancelled here, even when a pull request has such a head branch and the
# build type is shared with the pushes to main.
# Usage: teamcity_cancel_branch_builds <build type> <branch> <comment>
teamcity_cancel_branch_builds()
{
  local ids id

  case "$2" in
    main | master)
      echo "The builds of $2 are not cancelled by a pull request."
      return 0
      ;;
  esac

  ids=$(curl --fail-with-body -sS \
    --retry 5 --retry-delay 2 --retry-all-errors \
    -G \
    -H "Authorization: Bearer $TEAMCITY_TOKEN" \
    -H "Accept: application/json" \
    --data-urlencode "locator=buildType:$1,state:(queued:true,running:true),count:1000" \
    --data-urlencode "fields=build(id,branchName)" \
    "$TEAMCITY_URL/app/rest/builds" | jq -r --arg branch "$2" '.build[]? | select(.branchName == $branch) | .id') || {
    echo "::warning title=Builds not cancelled::Could not list the TeamCity builds of $2, they may still be queued."
    return 0
  }

  for id in $ids; do
    echo "Cancelling TeamCity build $id"
    teamcity_cancel_build "$id" "$3" || echo "::warning title=Build not cancelled::Could not cancel TeamCity build $id."
  done
}
