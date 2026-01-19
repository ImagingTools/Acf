#!/bin/bash
# Script to update the GitHub release 2.0.0 with reduced release notes
# This script requires GitHub CLI (gh) to be authenticated

set -e

RELEASE_TAG="2.0.0"
REPO="ImagingTools/Acf"

# The reduced release body (max 25 entries)
read -r -d '' RELEASE_BODY << 'EOF' || true
Initial Release from Git

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

**Full Changelog**: https://github.com/ImagingTools/Acf/commits/2.0.0
EOF

echo "Updating GitHub release ${RELEASE_TAG}..."
echo "Repository: ${REPO}"
echo ""
echo "New release body has $(echo "$RELEASE_BODY" | grep -c '^*') bullet point entries."
echo ""

# Update the release using GitHub CLI
gh release edit "$RELEASE_TAG" \
  --repo "$REPO" \
  --notes "$RELEASE_BODY"

echo ""
echo "✓ Release ${RELEASE_TAG} has been updated successfully!"
echo "  View at: https://github.com/${REPO}/releases/tag/${RELEASE_TAG}"
