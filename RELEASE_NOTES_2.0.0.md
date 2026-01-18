# Release Notes for ACF 2.0.0 (Reduced)

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

---

**Note**: This release log has been reduced from 41 entries to 19 key entries (within the max 25 limit) by excluding unit test and infrastructure changes per project requirements.

## Removed Entries (22 total)

### Unit Tests (10 entries removed)
- Add unit tests for IsCmyk() function in icmm library (#7)
- Add unit tests for iprm::IParamsManager interface (#4)
- Expand unit test coverage for iprm::CIdParam (#5)
- Add comprehensive unit tests for iser serialization library (#8)
- Add comprehensive unit tests for i2d library (#9)
- Add unit tests for core ACF utility classes organized by library (#10)
- Add unit tests for istd::CRange, CIntRange, CRanges, and CIntRanges (#11)
- Eliminate duplicated test main.cpp files with shared macro (#13)
- Add missing unit tests for the iprm library (#17)

### Infrastructure Changes (10 entries removed)
- Add GitHub Actions workflow for Doxygen documentation deployment (#24)
- Add CI workflows for CMake and QMake builds on Linux and Windows with Qt 6.8.3 (#30)
- Integrate GitHub Actions with external TeamCity build server (#37)
- Add Linux build configuration to TeamCity CI pipeline (#38)
- Add EU Cyber Resilience Act (CRA) compliance infrastructure (#40)
- ci(deps): bump github/codeql-action from 3 to 4 (#41)
- ci(deps): bump aquasecurity/trivy-action from 0.24.0 to 0.33.1 (#42)
- ci(deps): bump actions/upload-pages-artifact from 3 to 4 (#43)
- ci(deps): bump actions/checkout from 4 to 6 (#44)

### Documentation Entries Consolidated (3 entries removed)
- Add Doxygen documentation for undocumented i2d library classes (#18)
- Add comprehensive Doxygen documentation to idoc and iqtdoc libraries (#20)
- Add comprehensive Doxygen documentation for iprm library (#21)
- Add comprehensive Doxygen documentation for ilog and iloggui libraries (#26)

These entries were consolidated into the remaining comprehensive documentation entries (#19, #25, #27, #29).
