#!/usr/bin/env bash

# Helper of the TeamCity CI workflow to pick the TeamCity build configuration of a run. Source this file in a step.
# The id comes from the repository variable TEAMCITY_BUILD_TYPE_<PLATFORM>_<PR|MAIN>, which the caller passes in the
# environment. When the variable is empty, the id is built as <prefix>_Build_<Platform>_<PR|Main>.

# Prints the id of the build configuration, or nothing when neither the variable nor the prefix is set.
# Usage: teamcity_build_type <platform: windows|linux> <kind: pr|main> <explicit id> <prefix>
teamcity_build_type()
{
  local platform="$1" kind="$2" explicit="$3" prefix="$4"

  if [ -n "$explicit" ]; then
    echo "$explicit"
    return 0
  fi

  if [ -z "$prefix" ]; then
    return 0
  fi

  local platform_name kind_name
  platform_name="$(tr '[:lower:]' '[:upper:]' <<< "${platform:0:1}")${platform:1}"
  kind_name="$(tr '[:lower:]' '[:upper:]' <<< "${kind:0:1}")${kind:1}"
  if [ "$kind" = "pr" ]; then
    kind_name="PR"
  fi

  echo "${prefix}_Build_${platform_name}_${kind_name}"
}
