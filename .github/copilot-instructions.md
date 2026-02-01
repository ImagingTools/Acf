# Copilot Instructions for ACF (Application Component Framework)

## Project Overview

ACF is a C++/Qt-based component framework for building extensible applications. It provides:
- Component-based architecture with dependency injection
- Type-safe parameter management
- Document/View framework
- Serialization support
- 2D/3D graphics utilities
- Comprehensive testing infrastructure

## Build System

The project supports two build systems:
- **CMake**: Modern build system (primary)
- **QMake**: Legacy Qt build system (still supported)

Build configurations are located in:
- `Build/CMake/` - CMake configuration
- `Build/QMake/` - QMake configuration
- `Build/VC*_64/` - Scripts for generating native Visual Studio projects from CMake or QMake projects

## Code Organization

```
Include/          # Source code for static libraries/modules (headers and implementations)
├── icomp/       # Component framework
├── iprm/        # Parameter management
├── iser/        # Serialization
├── idoc/        # Document framework
├── iqtgui/      # Qt GUI utilities
└── ...          # Other libraries

Impl/            # Component packages (binaries - DLLs/shared libraries) and applications
├── BasePck/     # Component packages
├── FilePck/
├── AcfExe/      # Applications
└── ...

Tests/           # Unit tests (Qt Test framework)
Config/          # Component configuration files
Docs/            # Documentation
```

## Coding Conventions

### C++ Style
- Use modern C++ practices (C++11/14/17 features)
- Prefer RAII and smart pointers for resource management
- Follow Qt naming conventions for Qt-related code
- Use consistent indentation (existing code style)
- Add Doxygen comments for public APIs

### Naming Conventions
- Classes: PascalCase with prefixes
  - `I*` for interfaces (e.g., `IComponent`)
  - `C*` for concrete classes (e.g., `CComponentBase`)
  - `T*` for templates (e.g., `TRestorableGuiWrap`)
- Methods: PascalCase (Qt style)
- Variables: camelCase
- Private members: `m_` prefix

### Header Guards
- Use `#pragma once` or traditional include guards
- Follow existing patterns in the module

## Component Framework

Key concepts when working with components:
- Components inherit from `CComponentBase`
- Component structure is defined using `I_BEGIN_COMPONENT` / `I_END_COMPONENT` macros
- Components are exported from packages using `I_EXPORT_COMPONENT`
- Interfaces are registered with `I_REGISTER_INTERFACE`
- Attributes are declared with `I_ATTR` and assigned with `I_ASSIGN`
- Component references use `I_REF` macro
- Component factories use `I_FACT` or `I_MULTIFACT` macros

### Common Component Pattern
```cpp
// Component header (.h file)
class CMyComponent : public icomp::CComponentBase
{
public:
    typedef icomp::CComponentBase BaseClass;
    
    // Define component structure
    I_BEGIN_COMPONENT(CMyComponent);
        I_REGISTER_INTERFACE(IMyInterface);
        I_ASSIGN(m_enabled, "Enabled", "Enable feature", true, true);
    I_END_COMPONENT;
    
    CMyComponent();
    
protected:
    virtual void OnComponentCreated() override;
    
private:
    I_ATTR(bool, m_enabled);
};

// Package registration (in package .cpp file)
#include <icomp/export.h>

namespace MyPck
{
    typedef CMyComponent MyComponent;
    
    I_EXPORT_PACKAGE(
        "MyCompany/MyPck",
        "My package description",
        IM_PROJECT("MyProject"));
    
    I_EXPORT_COMPONENT(
        MyComponent,
        "My component description",
        IM_TAG("MyTag"));
}
```

## Security Considerations

ACF follows practices aligned with the EU Cyber Resilience Act (CRA) requirements. See [CRA_COMPLIANCE.md](CRA_COMPLIANCE.md) for detailed compliance information.

### Secure Coding Practices
- **Input Validation**: Always validate external inputs
- **Memory Safety**: Use smart pointers, avoid raw pointer arithmetic
- **No Hardcoded Secrets**: Never commit credentials or sensitive data
- **Safe Type Conversions**: Be careful with type casts
- **Error Handling**: Handle errors gracefully without exposing sensitive info

### Dependency Management
- Document new dependencies in `sbom.json` (Software Bill of Materials)
- Check dependencies for known vulnerabilities
- Use well-maintained libraries with active security support
- See `SBOM.md` for SBOM guidelines

### Files to Update for Dependencies
- `sbom.json` - Main SBOM file
- `sbom.spdx.json` - SPDX format SBOM
- `SBOM.md` - Documentation

## Testing

- Unit tests use Qt Test framework
- Tests are located in `Tests/` directory
- Each test has its own subdirectory with CMake/QMake builds
- Run tests through the build system
- Follow existing test patterns for consistency

### Test Structure
```
Tests/MyTest/
├── CMake/CMakeLists.txt
├── QMake/MyTest.pro
├── MyTest.cpp
├── MyTest.h
└── Data/  (if needed)
```

## Common Tasks

### Adding a New Component
1. Create header in `Include/<library>/`
2. Create implementation in `Impl/<package>/`
3. Use `I_BEGIN_COMPONENT` / `I_END_COMPONENT` to define component structure
4. Export from package using `I_EXPORT_COMPONENT`
5. Add to Config file if needed
6. Update documentation

### Adding a New Library
1. Create directory in `Include/<library>/` with source and header files
2. Create package directory in `Impl/<package>/` for component packages
3. Add CMake configuration in `Build/CMake/`
4. Add QMake configuration in `Build/QMake/`
5. Update relevant package files
6. Add tests in `Tests/`
7. Update documentation

### Fixing Build Issues
- Check component registration and exports
- Verify PackageId matches in Config files
- Ensure all required libraries are linked
- Check for missing includes
- Auto-fix system may help with common issues

## Documentation

- API documentation is generated with Doxygen
- Published at: https://imagingtools.github.io/Acf/
- Key docs are in `Docs/`:
  - `ComponentFramework.md` - Component system guide
  - `ComponentFrameworkExamples.md` - Practical examples
  - `DocumentViewFrameworkREADME.md` - Document/View framework

### Doxygen Comments
```cpp
/**
	\brief Brief description of the class/method
	
	Detailed description goes here.
	
	\param paramName Description of parameter
	\return Description of return value
*/
```

## Continuous Integration

- TeamCity CI builds for Windows and Linux
- Automated security scanning
- Auto-fix system for common build failures
- Pull requests must pass all CI checks

## Important Files

- `CONTRIBUTING.md` - Contribution guidelines
- `SECURITY.md` - Security policy and vulnerability reporting
- `CRA_COMPLIANCE.md` - EU CRA compliance information
- `SBOM.md` - Software Bill of Materials guide
- `LICENSE` - LGPL-2.1 license

## Best Practices

1. **Minimal Changes**: Make the smallest possible changes to achieve the goal
2. **Follow Patterns**: Match existing code style and patterns
3. **Test Changes**: Test your changes before committing
4. **Document Updates**: Keep documentation in sync with code
5. **Security First**: Consider security implications of all changes
6. **Check Dependencies**: Verify compatibility and security of any new dependencies
7. **Update SBOM**: Keep the Software Bill of Materials current
8. **Git Best Practices**: Write clear commit messages, use descriptive branch names

## When Working on Issues

1. Understand the full context before making changes
2. Explore the codebase to understand affected areas
3. Check existing tests and add new ones if needed
4. Build and test your changes
5. Update documentation if needed
6. Ensure no security vulnerabilities are introduced
7. Update SBOM if dependencies change
8. Get code review before finalizing

## Qt Integration

- ACF heavily uses Qt framework
- Qt-specific libraries: `iqt*` prefix
- Follow Qt naming conventions for Qt-related code
- Use Qt's signal/slot mechanism where appropriate
- Qt Test framework for unit tests

## Helpful Commands

The project uses CMake and QMake build systems. Build configurations are located in:
- `Build/CMake/` - CMake configuration
- `Build/QMake/` - QMake configuration  
- `Tests/<TestName>/CMake/` - Individual test CMake files

Refer to project-specific build documentation for detailed build instructions. The build process may vary depending on your environment and configuration.

General workflow:
```bash
# Set ACFDIR environment variable (required)
# Point this to your ACF repository root directory
export ACFDIR=/path/to/your/Acf

# Build main project with CMake
cd Build/CMake
cmake [options] <source-dir>
make

# Build main project with QMake
cd Build/QMake
qmake
make

# Build individual tests
cd Tests/<TestName>/CMake
cmake [options] .
make
./<TestName>
```

## Avoid

- Don't add unnecessary dependencies
- Don't break existing APIs without good reason
- Don't ignore security considerations
- Don't skip testing
- Don't commit build artifacts or temporary files
- Don't remove or modify working code unnecessarily
- Don't ignore CI failures

## Questions or Issues?

- Check documentation: https://imagingtools.github.io/Acf/
- Review CONTRIBUTING.md
- Look at existing code for patterns
- Ask maintainers for clarification
