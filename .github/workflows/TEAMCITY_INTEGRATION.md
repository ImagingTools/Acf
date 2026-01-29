# GitHub Actions - TeamCity Integration

This GitHub Actions workflow integrates with an external TeamCity build server to trigger builds and report results back to GitHub.

## Overview

The `teamcity-trigger.yml` workflow:
1. Triggers TeamCity builds (Windows and Linux) in parallel using a matrix strategy when pull requests are created/updated or commits are pushed to main/master
2. Waits for both TeamCity builds to complete with separate timeouts for queued and running states
3. Reports the build status back to GitHub (success/failure)

## Key Features

- **Matrix Strategy**: Single job with platform matrix (windows/linux) instead of duplicate jobs for better maintainability
- **JSON API**: Uses JSON format with `jq` for safe payload construction instead of XML
- **Branch Handling**: Proper `branchName` parameter and ref checkout for accurate PR builds
- **Retry Logic**: Automatic retry on network errors with `curl --fail-with-body --retry`
- **Separate Timeouts**: Different limits for queued (30min) vs running (60min) build states
- **Better Debugging**: Enhanced error messages and build URL tracking

## Configuration

### Prerequisites

1. A TeamCity server with a configured build configuration for this project
2. A TeamCity access token with permissions to trigger builds and query build status

### Required GitHub Repository Variables

Configure the following variables in your GitHub repository settings (Settings → Secrets and variables → Actions → Variables tab):

| Variable Name | Description | Example |
|------------|-------------|---------|
| `TEAMCITY_URL` | Your TeamCity server URL (without trailing slash) | `https://teamcity.example.com` |
| `TEAMCITY_TOKEN` | TeamCity access token with build trigger permissions | `eyJ0eXAiOiJKV1Q...` |
| `TEAMCITY_BUILD_TYPE_WINDOWS` | TeamCity build configuration ID for Windows builds | `Acf_Build_Windows` or `ProjectId_BuildConfigId_Windows` |
| `TEAMCITY_BUILD_TYPE_LINUX` | TeamCity build configuration ID for Linux builds | `Acf_Build_Linux` or `ProjectId_BuildConfigId_Linux` |

### How to Get TeamCity Configuration Values

#### 1. TeamCity Server URL
Your TeamCity server base URL, for example: `https://teamcity.example.com`

#### 2. TeamCity Access Token
1. Log into your TeamCity server
2. Go to your profile (click on your username in the top right)
3. Select **Access Tokens** from the menu
4. Click **Create access token**
5. Give it a name (e.g., "GitHub Actions Integration")
6. Set appropriate permissions (at minimum: trigger builds, view build status)
7. Copy the generated token

#### 3. TeamCity Build Configuration IDs
You need to set up two separate build configurations in TeamCity - one for Windows and one for Linux.

For each build configuration:
1. Open your build configuration in TeamCity
2. The build configuration ID is shown in the URL or in the build configuration settings
3. Format is usually: `ProjectId_BuildConfigurationId`
4. Examples:
   - Windows: `Acf_CMakeBuild_Windows` or `ImagingTools_Acf_Build_Windows`
   - Linux: `Acf_CMakeBuild_Linux` or `ImagingTools_Acf_Build_Linux`

## Setting Up GitHub Repository Variables

1. Go to your repository on GitHub
2. Click **Settings** → **Secrets and variables** → **Actions**
3. Click on the **Variables** tab
4. Click **New repository variable**
5. Add each of the four variables listed above:
   - `TEAMCITY_URL`
   - `TEAMCITY_TOKEN`
   - `TEAMCITY_BUILD_TYPE_WINDOWS`
   - `TEAMCITY_BUILD_TYPE_LINUX`

## Workflow Triggers

The workflow triggers on:
- **Pull Requests**: Any pull request event (opened, synchronized, reopened)
- **Push to main/master**: Direct commits to main or master branches

Both Windows and Linux builds are triggered in parallel using a matrix strategy for each event.

## Build Information Passed to TeamCity

The workflow passes the following information to TeamCity:
- `branchName`: The Git branch name (e.g., `main`, `feature-branch`) - used by TeamCity for proper branch association
- `env.GIT_BRANCH`: The Git branch reference for backward compatibility
- `env.GIT_COMMIT`: The Git commit SHA

For pull requests, the workflow checks out the actual PR head SHA (not the merge commit) to ensure TeamCity builds exactly what was pushed to the PR branch.

## Timeout

The workflow has intelligent timeout handling:
- **Queued State**: Maximum 30 minutes (1800 seconds) - if the build stays queued longer, the workflow fails
- **Running State**: Maximum 60 minutes (3600 seconds) - if the build runs longer, the workflow fails
- **Overall**: 90 minutes timeout at the GitHub Actions job level

These separate timeouts help identify whether builds are stuck in queue or taking too long to execute.

You can adjust these by modifying the `MAX_QUEUED_SECONDS` and `MAX_RUNNING_SECONDS` variables in the workflow file.

## Troubleshooting

### "TeamCity configuration not found in repository variables"
- Ensure all four required variables (TEAMCITY_URL, TEAMCITY_TOKEN, TEAMCITY_BUILD_TYPE_WINDOWS, TEAMCITY_BUILD_TYPE_LINUX) are configured in your repository settings under the Variables tab

### "Failed to trigger TeamCity build"
- Verify the TeamCity URL is correct and accessible
- Verify the access token has sufficient permissions
- Check that the build configuration ID exists and is correct
- Ensure the TeamCity server accepts REST API requests from GitHub Actions runners

### "Timeout waiting for TeamCity build to complete"
- **"Timed out: build stayed queued for Xs"**: The build waited in queue for more than 30 minutes
  - Check TeamCity agent availability
  - Ensure agents compatible with your build are running
- **"Timed out: build stayed running for Xs"**: The build ran for more than 60 minutes
  - Optimize your TeamCity build to complete faster
  - Or increase the `MAX_RUNNING_SECONDS` value in the workflow file
- **Job-level timeout (90min)**: Increase the `timeout-minutes` in the workflow file

### "TeamCity build failed"
- The build failed in TeamCity
- Click on the "Build URL" link in the GitHub Actions log to view detailed build logs in TeamCity
- Fix the build issues in TeamCity and retry

## Example TeamCity Build Configurations

You should have two separate TeamCity build configurations:

### Windows Build Configuration
- Configured to build on Windows agents
- Should include Windows-specific build steps (Visual Studio, MSVC compiler, etc.)
- VCS root pointing to the Git repository
- Optionally use QMake or CMake with Windows-specific settings

### Linux Build Configuration
- Configured to build on Linux agents
- Should include Linux-specific build steps (GCC/Clang compiler, make, etc.)
- VCS root pointing to the Git repository
- Optionally use QMake or CMake with Linux-specific settings

Both configurations:
1. Must be configured to build from the same Git repository
2. Should have the necessary build steps (CMake, QMake, etc.)
3. Can rely solely on GitHub Actions to trigger builds (no VCS triggers needed)
4. Should be configured to recognize branch names passed via the `branchName` parameter

## Security Notes

- Store your TeamCity token securely in GitHub repository variables
- Consider using GitHub Secrets instead of Variables if you need additional security for the token (the workflow supports both)
- Use a dedicated service account or token with minimal required permissions
- Consider using TeamCity's token expiration features
- Regularly rotate access tokens

## Technical Implementation Details

### JSON API Usage
The workflow uses TeamCity's JSON API instead of XML for better maintainability:
- Payloads are constructed using `jq` for safe JSON generation
- Avoids shell escaping issues common with XML
- Better error messages from TeamCity

### Error Handling
- Uses `curl --fail-with-body` to get response bodies even on HTTP errors
- Automatic retry with `--retry 5 --retry-delay 2 --retry-all-errors` for status checks
- Strict error handling with `set -euo pipefail` in all bash steps

### Matrix Strategy
Instead of duplicate jobs, the workflow uses a single job with a matrix:
```yaml
strategy:
  fail-fast: false
  matrix:
    platform: [windows, linux]
```
This reduces code duplication and makes the workflow easier to maintain.

## Support

For issues with:
- **GitHub Actions workflow**: Check the Actions tab in GitHub for detailed logs
- **TeamCity build**: Check the TeamCity web interface for build logs and configuration (URL shown in workflow logs)
- **Integration**: Ensure variables are correctly configured and TeamCity REST API is accessible
