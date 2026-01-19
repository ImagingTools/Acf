#!/usr/bin/env python3
"""
Update GitHub release 2.0.0 with reduced release notes.
Requires GITHUB_TOKEN environment variable to be set.
"""

import os
import sys
import json
import urllib.request
import urllib.error

RELEASE_TAG = "2.0.0"
REPO_OWNER = "ImagingTools"
REPO_NAME = "Acf"

REDUCED_RELEASE_BODY = """Initial Release from Git

## What's Changed
* Comstics2 by @kirilllepski in https://github.com/ImagingTools/Acf/pull/2
* Extend i3d library with 3D geometric primitives and algorithms by @Copilot in https://github.com/ImagingTools/Acf/pull/6
* Changes from SVN repository by @kirilllepski in https://github.com/ImagingTools/Acf/pull/12
* Implement interactive 2D graph plot shape for iview library by @Copilot in https://github.com/ImagingTools/Acf/pull/14
* Improve variable naming for readability across core components by @Copilot in https://github.com/ImagingTools/Acf/pull/15
* Fix thread-safety issues in icomp library for concurrent access by @Copilot in https://github.com/ImagingTools/Acf/pull/16
* Enhance Doxygen documentation with detailed descriptions and comprehensive usage examples by @Copilot in https://github.com/ImagingTools/Acf/pull/19
* Replace #ifndef include guards with #pragma once by @Copilot in https://github.com/ImagingTools/Acf/pull/22
* Add missing override keywords to virtual method overrides by @Copilot in https://github.com/ImagingTools/Acf/pull/23
* Document all 26 ACF libraries in README and Doxygen by @Copilot in https://github.com/ImagingTools/Acf/pull/25
* Add comprehensive component framework documentation by @Copilot in https://github.com/ImagingTools/Acf/pull/27
* Changes from SVN repository by @kirilllepski in https://github.com/ImagingTools/Acf/pull/28
* Enhance ACF.h main page documentation with comprehensive technical reference by @Copilot in https://github.com/ImagingTools/Acf/pull/29
* Fix icmm library: uninitialized members, pointer safety, and serialization bugs by @Copilot in https://github.com/ImagingTools/Acf/pull/33
* Implement GetSupportedOperations, CopyFrom, and CloneMe for icmm IChangeable classes by @Copilot in https://github.com/ImagingTools/Acf/pull/34
* Add template overload for SerializeRange to fix TRange<double> linker error by @Copilot in https://github.com/ImagingTools/Acf/pull/35
* Linux build fixed by @kucherenkord-cyber in https://github.com/ImagingTools/Acf/pull/36
* Create LICENSE by @kirilllepski in https://github.com/ImagingTools/Acf/pull/39
* Add SPDX 2.3 format SBOM and update ACF to multi-license support by @Copilot in https://github.com/ImagingTools/Acf/pull/45

## New Contributors
* @kirilllepski made their first contribution in https://github.com/ImagingTools/Acf/pull/2
* @Copilot made their first contribution in https://github.com/ImagingTools/Acf/pull/7
* @kucherenkord-cyber made their first contribution in https://github.com/ImagingTools/Acf/pull/36

**Full Changelog**: https://github.com/ImagingTools/Acf/commits/2.0.0"""


def get_release_id(token):
    """Get the release ID for the specified tag."""
    url = f"https://api.github.com/repos/{REPO_OWNER}/{REPO_NAME}/releases/tags/{RELEASE_TAG}"
    
    req = urllib.request.Request(url)
    req.add_header("Authorization", f"Bearer {token}")
    req.add_header("Accept", "application/vnd.github+json")
    req.add_header("X-GitHub-Api-Version", "2022-11-28")
    
    try:
        with urllib.request.urlopen(req) as response:
            data = json.loads(response.read().decode())
            return data["id"]
    except urllib.error.HTTPError as e:
        print(f"Error fetching release: {e}", file=sys.stderr)
        print(f"Response: {e.read().decode()}", file=sys.stderr)
        sys.exit(1)


def update_release(token, release_id):
    """Update the release with reduced notes."""
    url = f"https://api.github.com/repos/{REPO_OWNER}/{REPO_NAME}/releases/{release_id}"
    
    data = json.dumps({"body": REDUCED_RELEASE_BODY}).encode()
    
    req = urllib.request.Request(url, data=data, method="PATCH")
    req.add_header("Authorization", f"Bearer {token}")
    req.add_header("Accept", "application/vnd.github+json")
    req.add_header("X-GitHub-Api-Version", "2022-11-28")
    req.add_header("Content-Type", "application/json")
    
    try:
        with urllib.request.urlopen(req) as response:
            result = json.loads(response.read().decode())
            return result
    except urllib.error.HTTPError as e:
        print(f"Error updating release: {e}", file=sys.stderr)
        print(f"Response: {e.read().decode()}", file=sys.stderr)
        sys.exit(1)


def main():
    token = os.environ.get("GITHUB_TOKEN")
    if not token:
        print("Error: GITHUB_TOKEN environment variable is not set", file=sys.stderr)
        print("Please set it with: export GITHUB_TOKEN=your_token", file=sys.stderr)
        sys.exit(1)
    
    print(f"Updating release {RELEASE_TAG} for {REPO_OWNER}/{REPO_NAME}...")
    
    # Count entries
    entry_count = REDUCED_RELEASE_BODY.count("\n*")
    print(f"New release body has {entry_count} bullet point entries.")
    
    # Get release ID
    print("Fetching release ID...")
    release_id = get_release_id(token)
    print(f"Release ID: {release_id}")
    
    # Update release
    print("Updating release notes...")
    result = update_release(token, release_id)
    
    print("\n✓ Release has been updated successfully!")
    print(f"  View at: {result['html_url']}")


if __name__ == "__main__":
    main()
