# TeamCity Configuration

This directory contains the TeamCity CI/CD pipeline configuration for the ACF project using Kotlin DSL.

## Overview

The TeamCity configuration defines four build configurations:

1. **CMake Build (Linux)** - Builds and tests the project using CMake on Linux
2. **CMake Build (Windows)** - Builds and tests the project using CMake on Windows
3. **QMake Build (Linux)** - Builds the project using QMake on Linux
4. **QMake Build (Windows)** - Builds the project using QMake on Windows

## Setup Instructions

### 1. Enable Versioned Settings in TeamCity

1. Open your TeamCity server
2. Go to your project settings
3. Navigate to **Versioned Settings**
4. Select **Synchronization enabled**
5. Choose **Use settings from VCS**
6. Set VCS root to your repository
7. Set the format to **Kotlin**
8. Set the settings path to `.teamcity`
9. Click **Apply**

### 2. Configure TeamCity Agents

Ensure your TeamCity agents have the following installed:

#### Linux Agents:
- Qt 6.x (qt6-base-dev, qt6-tools-dev)
- CMake 3.x
- Ninja build system
- GCC or Clang compiler
- Git

#### Windows Agents:
- Qt 6.x
- CMake 3.x
- Ninja build system (will be installed via Chocolatey)
- Visual Studio (for MSVC compiler)
- Git

### 3. Build Configuration Details

Each build configuration:
- Triggers on any VCS changes (branch filter: `+:*`)
- Sets required environment variables (`ACFDIR`, `ACFCONFIGDIR`)
- Generates version files using Git scripts
- Builds the project
- Runs tests (CMake configurations only)

### 4. Required Environment Variables

The builds set the following environment variables:
- `ACFDIR` - Points to the checkout directory
- `ACFCONFIGDIR` - Points to the Config subdirectory

### 5. Customization

To customize the build:
- Edit `.teamcity/settings.kts`
- Modify build steps, triggers, or requirements as needed
- Commit changes to VCS
- TeamCity will automatically pick up the changes

## Build Triggers

All builds are configured to trigger on:
- Any branch changes (using branch filter `+:*`)
- Manual trigger is also available

## Requirements

- TeamCity 2024.03 or later (as specified in the version)
- Kotlin DSL support enabled on the TeamCity server

## Support

For issues with the TeamCity configuration, please contact the development team or refer to the [TeamCity documentation](https://www.jetbrains.com/help/teamcity/kotlin-dsl.html).
