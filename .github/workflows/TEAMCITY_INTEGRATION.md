# GitHub Actions - TeamCity Integration

All repositories trigger TeamCity through one reusable workflow, `ImagingTools/Acf/.github/workflows/teamcity-ci.yml`. A repository only holds a short workflow that calls it, so every project has the same build trigger and a fix made here reaches all of them.

## Overview

The reusable workflow, for each platform (Windows and Linux by default) in parallel:
1. Queues a TeamCity build for the pull request head, the merge queue commit or the pushed commit and sets a pending commit status `TeamCity CI (<platform>)`.
2. Cancels the older TeamCity builds of the same pull request branch, so a newer push supersedes them.
3. Waits for the build (60 minutes queued at most, 60 minutes running at most, 130 minutes for the job). A build over a limit is cancelled in TeamCity.
4. Cancels the TeamCity build when the GitHub run is cancelled, cancels the builds of a pull request when it is closed, and cancels the build of a merge queue group that the queue dropped.
5. Fetches the last 100 lines of the log, the build problems and the failed tests into the run log.
6. Reports the result as the commit status, which blocks the merge when it fails.
7. On a failed pull request build, posts an analysis comment and collapses the comments of earlier runs. After a successful run the old comments are collapsed too.

Files:
- `.github/workflows/teamcity-ci.yml`: the reusable workflow.
- `.github/scripts/teamcity-cancel.sh`, `teamcity-summary.sh`, `teamcity-comments.js`, `teamcity-build-type.sh`: its helpers. The workflow checks them out of this repository, so a calling repository does not carry them.
- `.github/workflows/teamcity-trigger.yml`: the call of this repository itself, also the model for the others.

## Using it in a repository

Add `.github/workflows/teamcity-trigger.yml`:

```yaml
name: TeamCity CI

on:
  pull_request:
    types: [opened, synchronize, reopened, closed]
  merge_group:
    types: [checks_requested]
  push:
    branches:
      - main
      - master

permissions:
  contents: read
  statuses: write
  checks: write
  pull-requests: write
  issues: write

jobs:
  teamcity:
    uses: ImagingTools/Acf/.github/workflows/teamcity-ci.yml@v1
    with:
      build-type-prefix: MyProject
    secrets: inherit
```

The `on:` and `permissions:` blocks cannot be shared, so they stay in each repository. Delete any older copy of the trigger and its `.github/scripts/teamcity-*` files.

### Inputs

| Input | Default | Meaning |
|-------|---------|---------|
| `platforms` | `["windows","linux"]` | JSON array of the platforms to build, for example `'["windows"]'`. |
| `build-type-prefix` | empty | Used only for a build configuration whose repository variable is not set. Builds the id as `<prefix>_Build_<Platform>_<PR or Main>`, for example `MyProject_Build_Windows_PR`. |
| `acf-ref` | `v1` | Ref of this repository the helper scripts are taken from. Keep it equal to the ref in the `uses` line. |

### Versioning

The ref after `@` in the `uses` line selects the version of the workflow, and `acf-ref` selects the version of its scripts. Keep the two equal.

| Goal | `uses: ...@` | `acf-ref` |
|------|--------------|-----------|
| Follow compatible fixes automatically | `v1` | leave out |
| Freeze a repository | a full commit SHA | the same SHA |
| Try a branch before it is released | the branch name | the same branch |

`v1` is a moving major tag. A compatible fix is released by moving it (`git tag -f v1 <commit>` and `git push -f origin v1`). A breaking change gets `v2`, and each repository moves to it by editing its `uses` line.

## Configuration

Set these in the repository settings (Settings → Secrets and variables → Actions) or once for the organization. The closest scope wins, so a repository overrides the organization value by defining the same name. Secrets are not passed to workflows of forks.

| Name | Kind | Description | Example |
|------|------|-------------|---------|
| `TEAMCITY_URL` | variable | TeamCity server URL without a trailing slash. | `https://teamcity.example.com` |
| `TEAMCITY_TOKEN` | secret | Token that may queue and cancel builds and read their results. | |
| `TEAMCITY_BUILD_TYPE_WINDOWS_PR` | variable | Build configuration id of Windows pull requests. | `Acf_Build_Windows` |
| `TEAMCITY_BUILD_TYPE_WINDOWS_MAIN` | variable | Windows `main`/`master` pushes. | `Acf_Build_Windows_Main` |
| `TEAMCITY_BUILD_TYPE_LINUX_PR` | variable | Linux pull requests. | `Acf_Build_Linux` |
| `TEAMCITY_BUILD_TYPE_LINUX_MAIN` | variable | Linux `main`/`master` pushes. | `Acf_Build_Linux_Main` |

A build-type variable wins over `build-type-prefix`. A repository whose TeamCity ids follow `<Project>_Build_<Platform>_<PR or Main>` can skip the four variables and give only the prefix. A repository that builds one platform sets `platforms` and needs only that platform's variables.

### How to get the TeamCity values

1. **Server URL**: the base URL of the TeamCity server.
2. **Access token**: in TeamCity open your profile, then **Access Tokens**, then **Create access token**. Name it (for example "GitHub Actions Integration") and give it the rights to queue builds, cancel builds and view results.
3. **Build configuration ids**: open the build configuration in TeamCity. The id is in the URL and in its settings, usually `ProjectId_BuildConfigurationId`. A repository needs one configuration per platform for pull requests and one for `main`/`master`.

## What is passed to TeamCity

- `branchName`: the Git branch name, used by TeamCity for the branch association.
- `env.GIT_BRANCH`: the Git branch name.
- `env.GIT_COMMIT`: the commit SHA. For a pull request this is the head SHA, not the merge commit, so TeamCity builds exactly what was pushed. In the merge queue it is the head of the queue branch, which is what main becomes.

## Timeouts

- Queued: 60 minutes at most. A build that stays queued longer is cancelled and the check fails.
- Running: 60 minutes at most. A build that runs longer is cancelled and the check fails.
- Job: 130 minutes at the GitHub level for the wait step.

The limits are `MAX_QUEUED_SECONDS`, `MAX_RUNNING_SECONDS` and `timeout-minutes` in `teamcity-ci.yml`. Change them there, for every repository at once.

## Required checks

Require the commit statuses the workflow writes, not the names of its jobs:
- `TeamCity CI (windows)`
- `TeamCity CI (linux)`

A repository that builds one platform requires only that one.

The names of the jobs depend on the calling workflow: a job of a reusable workflow is named `<caller job id> / <job name>`, for example `teamcity / Trigger TeamCity Build (linux)`. A required check with another name, such as the `Trigger TeamCity Build (linux)` of the earlier per-repository workflow, never reports and stays at "Expected - Waiting for status to be reported" although the build passed. The statuses have the same names in every repository, whatever the caller is called.

Change the required checks in the branch protection or the ruleset when a repository switches to the reusable workflow.

## Merge queue

Without a merge queue, a ruleset that requires branches to be up to date makes every pull request that waits for auto-merge fall behind when another one merges. Each of them is then updated and built again. A merge queue replaces that requirement: GitHub builds each queued pull request on a `gh-readonly-queue/main/pr-<number>-<sha>` branch that holds main and the pull requests ahead of it, and moves main to that commit when the required checks pass. The pull requests are built in parallel and nobody updates a branch.

The workflow builds the queue branch when the calling workflow has the `merge_group` trigger shown above:
- It uses the pull request build configuration, `TEAMCITY_BUILD_TYPE_<PLATFORM>_PR`.
- It writes the same `TeamCity CI (<platform>)` statuses, on the queue commit.
- When a pull request ahead fails, the queue drops the groups behind it, deletes their branches and builds new ones. The workflow checks its branch once a minute and cancels the TeamCity build of a dropped group, so the agent builds the new group instead.
- A failed build removes the pull request from the queue. Fix it, push, and turn auto-merge on again.

Every workflow that writes a required check needs the `merge_group` trigger, otherwise the queue waits for that check until its timeout. The pull request build configurations must accept the `gh-readonly-queue/*` branches, which a branch specification of `+:refs/heads/*` does.

Turn the queue on in this order:
1. Release the workflow (move `v1`) and add the `merge_group` trigger to the calling workflow.
2. In the ruleset of main, add **Require merge queue** and turn off **Require branches to be up to date before merging**, which the queue replaces.
3. Queue settings: merge method **Merge commit**, build concurrency 5, group size 1 to 5, wait 5 minutes, all entries must pass, and a status check timeout of **150 minutes**, more than the 130 minutes the job may take. The default of 60 minutes fails long builds.

## Troubleshooting

### "TeamCity configuration not found in repository variables"
- Ensure `TEAMCITY_URL` is a variable and `TEAMCITY_TOKEN` a secret, in the repository or the organization.
- Ensure the stub passes `secrets: inherit`.
- Ensure the build configuration variable of that platform is set, or `build-type-prefix` is given.

### "Failed to trigger TeamCity build"
- Verify the TeamCity URL is correct and reachable from GitHub runners.
- Verify the token has the rights to queue builds.
- Verify the build configuration id exists. Check the `Triggering TeamCity ... build` line in the log for the id that was used.

### The workflow does not start, or cannot find its scripts
- Check that the tag or SHA in the `uses` line exists in this repository.
- Check that `acf-ref` equals that ref.

### "Timed out"
- **Stayed queued**: check that TeamCity agents compatible with the build are running.
- **Stayed running**: speed up the build, or raise `MAX_RUNNING_SECONDS` in the reusable workflow.

### "TeamCity build failed"
- The merge is blocked until the build passes.
- Read the "Fetch Build Logs" step, the analysis comment on the pull request, or the build URL in the log, fix the problem and push again.

## TeamCity build configurations

Per platform there is one configuration for pull requests and one for `main`/`master`:
- Windows ones run on Windows agents (Visual Studio, MSVC), Linux ones on Linux agents (GCC/Clang, make).
- All of them build the same Git repository with CMake or QMake.
- They need no VCS triggers, since GitHub Actions queues them.
- They must accept the branch passed in `branchName`.

## Security notes

- The token is a GitHub **secret**, so it is not shown in logs or the Actions UI.
- Use a dedicated service account with the minimal rights, and rotate or expire the token.
- A repository that needs another server or a narrower token defines its own `TEAMCITY_URL` or `TEAMCITY_TOKEN`. Nothing in the reusable workflow changes.
- The Acf trigger passes the commit under test as `acf-ref`, so a change to the workflow or its scripts is tested in its own pull request and runs with the token. A pull request workflow file already has that power, and pull requests from forks get no secrets. Other repositories pin a released ref and run released scripts.
