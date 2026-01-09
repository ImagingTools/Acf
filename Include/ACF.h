#pragma once


/**
\defgroup AcfCore ACF Core Libraries

ACF Core Libraries is a set of implementations of most important concepts and design patterns.

\mainpage ACF - Application Component Framework

\section Introduction Introduction

ACF (Application Component Framework) is a comprehensive C++ based, cross-platform component framework that was especially designed for creation of complex, customizable and highly scalable applications. The framework provides a robust foundation for building industrial-strength software systems with emphasis on modularity, reusability, and maintainability.

\subsection KeyPrinciples Key Principles

ACF follows the <b>component-oriented programming paradigm</b> and provides instruments to build software from reusable, loosely-coupled components. The framework serves well-defined interfaces and concepts to create those components with the following core principles:

- <b>Separation of Concerns</b>: Clear boundaries between business logic, data model, and presentation layer
- <b>Interface-Based Communication</b>: Components interact exclusively through well-defined interfaces
- <b>Dependency Injection</b>: Automatic resolution and injection of component dependencies
- <b>Design Pattern Implementation</b>: Built-in support for common patterns (Factory, Observer, Serialization, etc.)
- <b>Type Safety</b>: Strong typing throughout the framework with compile-time checking where possible
- <b>Cross-Platform Support</b>: Runs on Windows, Linux, and macOS

\subsection TechnicalFoundation Technical Foundation

ACF contains a set of libraries with implementations of most useful design patterns:
- <b>Serialization</b>: Archive-based persistence with multiple backend support (XML, JSON, Binary)
- <b>Model/Observer</b>: Sophisticated change notification and data binding mechanisms
- <b>Factory Pattern</b>: Dynamic component instantiation with registry system
- <b>Dependency Injection</b>: Automatic dependency resolution and lifecycle management
- <b>Document/View Architecture</b>: Complete MDI/SDI application framework
- <b>Parameter Management</b>: Type-safe parameter handling with GUI integration

The UI-part of the framework is based on Qt library provided by Qt Company, providing native look-and-feel across platforms and extensive widget support.

ACF implements a powerful component concept and includes many well-designed components, which you can directly use in your application, significantly reducing development time and improving code quality.

ACF is open source and released under the terms of the <b>GNU Lesser General Public License (LGPL)</b>, allowing use in both open source and commercial applications.

\subsection TheoreticalBackground Theoretical Background

More theoretical considerations about component-based development can be found at:
- Component-Based Software Engineering: http://en.wikipedia.org/wiki/Component-based_software_engineering
- Dependency Injection: http://en.wikipedia.org/wiki/Dependency_injection
- Model-View-Controller: http://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93controller

\section HowItWorks How ACF Works

\subsection ComponentArchitecture Component Architecture

The main idea behind ACF is to see each software product as a composition of components with clearly defined interfaces. Interface-based communication ensures loose coupling and high testability.

<b>Interface is the only way for communication between components</b>, which provides:
- Clear API contracts between components
- Easy mocking for unit tests
- Runtime component replacement (plugin architecture)
- Version compatibility through interface stability

\subsection ComponentImplementation Implementing Components

The typical workflow to implement an ACF component involves:

\subsubsection Step1 1. Define the Interface

A C++ pure virtual interface must be defined, inheriting from istd::IPolymorphic (or a derived interface like iser::ISerializable):

\code{.cpp}
// Define the interface
class IDataProcessor : virtual public istd::IPolymorphic
{
public:
    virtual bool ProcessData(const QString& input, QString& output) = 0;
    virtual void SetProcessingMode(int mode) = 0;
};
\endcode

Note: You typically don't inherit from icomp::IComponent directly - it's an internal framework interface. Use istd::IPolymorphic or iser::ISerializable as the base.

\subsubsection Step2 2. Implement the Component Class

Create a class that implements this interface. There are several approaches:

<b>a) Basic Component Implementation:</b>
\code{.cpp}
// Implement the component
class CDataProcessor : 
    public icomp::CComponentBase,
    virtual public IDataProcessor
{
public:
    typedef icomp::CComponentBase BaseClass;
    
    // Component initialization
    I_BEGIN_COMPONENT(CDataProcessor);
        I_REGISTER_INTERFACE(IDataProcessor);
    I_END_COMPONENT;
    
    CDataProcessor() : m_mode(0) {}
    
    // Implement interface methods
    virtual bool ProcessData(const QString& input, QString& output) override
    {
        output = input.toUpper(); // Example processing
        return true;
    }
    
    virtual void SetProcessingMode(int mode) override
    {
        m_mode = mode;
    }
    
private:
    int m_mode;
};
\endcode

<b>b) Component with References (Dependencies):</b>

Components can declare references to other components for dependency injection:

\code{.cpp}
class CAdvancedProcessor : 
    public icomp::CComponentBase,
    virtual public IDataProcessor
{
public:
    typedef icomp::CComponentBase BaseClass;
    
    I_BEGIN_COMPONENT(CAdvancedProcessor);
        I_REGISTER_INTERFACE(IDataProcessor);
        
        // Declare reference to another component
        I_ASSIGN_INTERFACE_REFERENCE(ILogger, m_loggerRefPtr, "Logger", "Logging component", false);
    I_END_COMPONENT;
    
    virtual bool ProcessData(const QString& input, QString& output) override
    {
        // Access referenced component
        istd::ILogger* logger = GetReferenceValue(m_loggerRefPtr);
        if (logger)
            logger->LogMessage("Processing data...");
        
        output = input.toUpper();
        return true;
    }
    
    virtual void SetProcessingMode(int mode) override {}
    
private:
    // Reference pointer
    I_REF_PTR(istd::ILogger, m_loggerRefPtr);
};
\endcode

<b>c) Component with Attributes:</b>

Attributes allow configuration of component behavior:

\code{.cpp}
class CConfigurableProcessor : 
    public icomp::CComponentBase,
    virtual public IDataProcessor
{
public:
    typedef icomp::CComponentBase BaseClass;
    
    I_BEGIN_COMPONENT(CConfigurableProcessor);
        I_REGISTER_INTERFACE(IDataProcessor);
        
        // Declare attributes with defaults
        I_ASSIGN(m_maxLengthAttrPtr, "MaxLength", "Maximum input length", false, 100);
        I_ASSIGN(m_enableLoggingAttrPtr, "EnableLogging", "Enable logging", false, true);
    I_END_COMPONENT;
    
    virtual bool ProcessData(const QString& input, QString& output) override
    {
        // Access attribute values
        int maxLength = GetAttributeValue(m_maxLengthAttrPtr);
        bool enableLogging = GetAttributeValue(m_enableLoggingAttrPtr);
        
        if (input.length() > maxLength)
            return false;
            
        if (enableLogging)
            qDebug() << "Processing:" << input;
            
        output = input.toUpper();
        return true;
    }
    
    virtual void SetProcessingMode(int mode) override {}
    
protected:
    virtual void OnComponentCreated() override
    {
        BaseClass::OnComponentCreated();
        // Initialize component here - attributes and references are now available
    }
    
private:
    // Attribute pointers
    I_ATTR(int, m_maxLengthAttrPtr);
    I_ATTR(bool, m_enableLoggingAttrPtr);
};
\endcode

\subsubsection Step3 3. Register in a Package

After implementation, components must be registered in a <b>Package</b>. Packages are logical containers that group related components:

<b>Package Types:</b>
- <b>C++ Packages (DLL/Shared Library):</b> For compiled C++ components with special export interface
- <b>Composition Packages (ARX files):</b> For visual component compositions created with ACF Compositor

All packages use the extension <b>.ARP</b> (ACF Registry Package) for consistency.

\code{.cpp}
// Package registration example
ICOMP_REGISTER_COMPONENT(CDataProcessor)
ICOMP_REGISTER_COMPONENT(CAdvancedProcessor)
ICOMP_REGISTER_COMPONENT(CConfigurableProcessor)
\endcode

\subsection ComponentComposition Component Composition

Components can be combined into larger components:

<b>ACF Compositor Tool:</b>
- Graphical editor for component dependencies and attributes
- UML-like visualization of component structure
- Visual connection of component references
- Attribute configuration through property editors
- Export to ARX files (registry format)

\subsection ApplicationExecution Application Execution

There are two deployment strategies:

<b>1. Runtime Execution (Prototyping):</b>
- Component compositions run directly by Framework-Runtime
- Dynamic loading of component packages
- Useful for rapid prototyping and testing
- Hot-swapping of components during development

<b>2. Compiled Execution (Production):</b>
- ACF provides <b>ARX Compiler (ARXC)</b> - a source code generator
- Converts component compositions to native C++ code
- Resulting code is compiled normally
- <b>Advantages:</b>
  - No runtime DLL loading overhead
  - Better performance
  - Reduced deployment complexity
  - Static linking possible
  - Better optimization by compiler

\subsection DevelopmentWorkflow Development Workflow Summary

1. <b>Design</b>: Define interfaces and component contracts
2. <b>Implement</b>: Create component classes with dependencies and attributes
3. <b>Compose</b>: Use ACF Compositor to create component compositions
4. <b>Test</b>: Run with Framework-Runtime for rapid testing
5. <b>Compile</b>: Generate C++ code with ARX Compiler
6. <b>Deploy</b>: Build and distribute optimized executable

\section Features Key Features and Capabilities

ACF provides a comprehensive set of features for enterprise-grade application development:

\subsection ArchitecturalFeatures Architectural Features

<b>1. Component Framework (icomp)</b>
- Component model with lifecycle management
- Interface-based programming with IComponent base
- Component registry and factory system
- Automatic dependency injection and resolution
- Attribute system for component configuration
- Component composition and aggregation
- Plugin architecture with dynamic loading
- Type-safe component casting and querying

<b>2. Graphical Component Editing</b>
- <b>ACF Compositor</b>: Visual UML-like component editor
- Drag-and-drop component composition
- Visual dependency wiring
- Property editing with live preview
- Export to ARX (ACF Registry XML) format
- Validation and error checking

<b>3. Code Generation</b>
- <b>ARX Compiler (ARXC)</b>: Component composition to C++ converter
- Generates efficient, readable C++ code
- Preserves component structure and dependencies
- Enables static linking and optimization
- Reduces runtime overhead
- Facilitates debugging with source-level access

\subsection DataManagementFeatures Data Management Features

<b>4. Type-Safe Parameter Management (iprm)</b>
- Strongly typed parameter interfaces
- Runtime type checking and validation
- Parameter grouping and hierarchies
- Default values and constraints
- Change notification integration
- GUI auto-generation support
- Serialization support for all parameter types

<b>5. Comprehensive Serialization (iser)</b>
- Archive-based abstraction layer
- Multiple format support:
  - Binary archives for performance
  - XML (simple and compact) for interoperability
  - JSON for web integration
  - Memory archives for networking
  - Bitwise archives for compression
- Extensible architecture for custom formats
- Support for complex object graphs
- Version compatibility mechanisms

<b>6. Document Management (idoc, iqtdoc)</b>
- Template-based document framework
- Full undo/redo support with command pattern
- Multiple document interface (MDI) support
- Single document interface (SDI) support
- Document import/export with multiple formats
- Print preview and printing support
- Recent files management
- Auto-save functionality

\subsection UIFeatures User Interface Features

<b>7. Qt Integration (iqtgui, iqtprm)</b>
- Native Qt widget integration
- Automatic UI generation from parameters
- Custom widget library (iwidgets)
- Drag-and-drop support
- Docking and toolbar management
- Splash screens and progress dialogs
- Designer plugin support
- Style sheet support

<b>8. 2D/3D Graphics (i2d, i3d, iview)</b>
- Geometric primitives (points, lines, polygons, curves)
- Transformations (translation, rotation, scaling)
- Calibration support for real-world measurements
- Interactive shapes with selection and manipulation
- Calibrated views with zoom and pan
- 3D transformations and projections
- OpenGL integration support

\subsection DevelopmentFeatures Development Support Features

<b>9. Testing Framework (itest)</b>
- Qt Test-based unit testing
- Component testing utilities
- Mock component support
- Test data management
- Automated test discovery
- Performance benchmarking
- Code coverage integration

<b>10. Logging System (ilog, iloggui)</b>
- Multi-level logging (Debug, Info, Warning, Error)
- Multiple output targets (console, file, GUI)
- Thread-safe logging
- Structured log messages
- Log filtering and search
- GUI log viewer with color coding
- Export functionality

<b>11. File Handling (ifile, ifilegui)</b>
- Generic file loading/saving interface
- Format auto-detection
- File dialogs with preview
- Recent files tracking
- Path management utilities
- Auto-persistence support
- File watchers for change detection

\subsection IntegrationFeatures Integration Features

<b>12. Third-Party Library Wrappers</b>

ACF provides component wrappers for popular libraries:
- <b>Boost</b>: Smart pointers, algorithms, utilities
- <b>MongoDB</b>: NoSQL database integration
- <b>PostgreSQL</b>: Relational database support
- <b>Qwt</b>: Scientific plotting and graphing
- <b>Qwt3D</b>: 3D surface plotting
- <b>OpenCV</b>: Computer vision and image processing
- <b>Phonon</b>: Multimedia playback
- <b>FFmpeg</b>: Video encoding/decoding
- <b>zlib</b>: Compression
- <b>QScintilla</b>: Source code editing

<b>13. Package Management (ipackage)</b>
- Dynamic package loading
- Plugin discovery and enumeration
- Package dependencies management
- Version compatibility checking
- Package metadata and descriptions
- Hot-reloading support (development mode)

\subsection AdvancedFeatures Advanced Features

<b>14. Model/Observer Pattern (imod)</b>
- Sophisticated change notification
- Hierarchical model support
- Selective observation (filter by change type)
- Automatic observer lifecycle management
- Multi-threaded observer support
- Change delegation chains

<b>15. Color Management (icmm)</b>
- Multiple color models (RGB, HSV, CMYK, Lab)
- Color space conversions
- Color interpolation
- Palette management
- Color calibration support

<b>16. Mathematical Utilities (imath)</b>
- Interpolation algorithms (linear, spline, polynomial)
- Complex number operations
- Numerical algorithms
- Statistical functions
- Matrix operations
- Curve fitting

\subsection ImageProcessing Image Processing Features

<b>17. Imaging Components (iimg)</b>
- Bitmap handling and manipulation
- Multiple format support (PNG, JPEG, BMP, TIFF)
- Color space conversions
- Filtering and transformations
- ROI (Region of Interest) support
- Histogram operations
- Image metadata handling
\section LibraryOverview ACF Libraries Reference

ACF is organized into focused, cohesive libraries that can be used independently or together. Each library provides detailed documentation with examples and best practices.

\subsection CoreFoundation Core Foundation Libraries

These libraries form the foundation of the framework:

<b>\ref page_istd "istd - Standard Utilities"</b>
- Foundational utilities and interfaces (change notification, smart pointers, utilities)
- Key interfaces: istd::IChangeable, istd::ICloneable
- Smart pointers: istd::TUniqueInterfacePtr, istd::TSharedInterfacePtr
- RAII helpers: istd::CChangeNotifier, istd::CChangeGroup
- See \ref page_istd for detailed documentation and examples

<b>\ref page_icomp "icomp - Component Framework"</b>
- Comprehensive component-based architecture with dependency injection
- Core interfaces: icomp::IComponent, icomp::IRegistry, icomp::IComponentFactory
- Attribute system for component configuration
- Automatic dependency resolution and lifecycle management
- See \ref page_icomp for complete guide

<b>\ref page_iser "iser - Serialization Framework"</b>
- Archive-based object persistence with multiple format support
- Archive types: Binary, XML, JSON, Memory, File-based
- Interfaces: iser::ISerializable, iser::IArchive
- Version management and type identification
- See \ref page_iser for usage examples and best practices

<b>\ref page_iprm "iprm - Parameter Management"</b>
- Type-safe parameter handling with validation and GUI integration
- Parameter types: Text, ID, Name, Selection, Enableable, Variable
- Containers: iprm::IParamsSet, iprm::IParamsManager
- Options and constraints: iprm::IOptionsList, iprm::IOptionsManager
- See \ref page_iprm for comprehensive examples

\subsection DataMathematics Data and Mathematics

<b>\ref page_imath "imath - Mathematical Utilities"</b>
- Numerical algorithms (interpolation, complex numbers, curve fitting)
- Vector and matrix operations
- Statistical functions and numerical solvers
- See \ref page_imath for details

<b>\ref page_i2d "i2d - 2D Graphics Utilities"</b>
- 2D geometric primitives (points, lines, polygons, circles)
- Affine transformations and geometric calculations
- Calibration support for real-world measurements
- See \ref page_i2d for comprehensive documentation

<b>\ref page_i3d "i3d - 3D Graphics Utilities"</b>
- 3D geometric primitives and transformations
- Projection matrices and 3D-to-2D projection
- See \ref page_i3d for details

<b>\ref page_icmm "icmm - Color Management"</b>
- Multiple color models (RGB, HSV, CMYK, Lab)
- Color space conversions and interpolation
- Palette management and calibration
- See \ref page_icmm for comprehensive guide

\subsection DocumentView Document and View Framework

<b>\ref page_idoc "idoc - Document Framework"</b>
- Template-based document management with multi-level undo/redo
- Document lifecycle: idoc::IDocument, idoc::IDocumentTemplate
- Multi-document management and persistence
- See \ref page_idoc for details

<b>\ref page_iview "iview - View Framework"</b>
- Interactive shapes and calibrated views
- Selection, manipulation, zoom and pan support
- See \ref page_iview for documentation

<b>\ref page_imod "imod - Model-Observer Pattern"</b>
- Observer pattern for change notification
- Model base classes: imod::IModel, imod::IModelObserver
- Automatic observer management and change filtering
- See \ref page_imod for implementation guide

\subsection FileData File and Data Handling

<b>\ref page_ifile "ifile - File Handling"</b>
- Generic file operations with format auto-detection
- Archive implementations for files (Binary, XML, JSON)
- See \ref page_ifile for details

<b>\ref page_ifilegui "ifilegui - File GUI Components"</b>
- Extended file dialogs with preview
- Recent files management and directory browsers
- See \ref page_ifilegui for documentation

<b>\ref page_iimg "iimg - Imaging"</b>
- Image loading, manipulation and display
- Multiple format support and color space operations
- See \ref page_iimg for comprehensive guide

\subsection LoggingTesting Logging and Testing

<b>\ref page_ilog "ilog - Logging"</b>
- Multi-level logging infrastructure (Debug, Info, Warning, Error)
- Thread-safe logging with multiple output targets
- See \ref page_ilog for details

<b>\ref page_iloggui "iloggui - Logging GUI"</b>
- Log viewer widget with color coding
- Filtering and search capabilities
- See \ref page_iloggui for documentation

<b>\ref page_itest "itest - Testing Framework"</b>
- Qt Test-based unit testing support
- Component testing utilities and assertion helpers
- See \ref page_itest for testing guide

\subsection QtIntegration Qt Framework Integration

<b>\ref page_iqt "iqt - Qt Utilities"</b>
- Qt-specific utilities (settings, clipboard, events)
- See \ref page_iqt for details

<b>\ref page_iqt2d "iqt2d - Qt 2D Graphics"</b>
- Qt widgets for 2D geometry parameters
- Interactive geometry editors
- See \ref page_iqt2d for documentation

<b>\ref page_iqtdoc "iqtdoc - Qt Document Framework"</b>
- Qt-specific document management
- MDI/SDI application framework
- See \ref page_iqtdoc for comprehensive guide

<b>\ref page_iqtgui "iqtgui - Qt GUI Utilities"</b>
- Application base classes and main window framework
- Splash screens, progress dialogs, Designer support
- See \ref page_iqtgui for details

<b>\ref page_iqtprm "iqtprm - Qt Parameter Widgets"</b>
- Auto-generated parameter editing widgets
- Property browser-style editors with constraint-aware editing
- See \ref page_iqtprm for documentation

<b>\ref page_iwidgets "iwidgets - Widget Utilities"</b>
- Custom Qt widgets and delegates
- Enhanced standard widgets
- See \ref page_iwidgets for details

\subsection Infrastructure Infrastructure Libraries

<b>\ref page_iattr "iattr - Attributes"</b>
- Metadata and attribute management for components
- See \ref page_iattr for details

<b>\ref page_ibase "ibase - Base Components"</b>
- Application infrastructure and progress management
- See \ref page_ibase for documentation

<b>\ref page_ipackage "ipackage - Package Management"</b>
- Dynamic package and plugin loading
- Dependency resolution and package metadata
- See \ref page_ipackage for comprehensive guide

\section AdditionalResources Additional Resources

\subsection Tutorials Tutorials and Examples

- <b>Component Framework Guide</b>: Comprehensive guide to building components (see Docs/ComponentFramework.md)
- <b>Component Examples</b>: Practical implementation examples (see Docs/ComponentFrameworkExamples.md)
- <b>Document/View Framework</b>: Building document-based applications (see Docs/DocumentViewFrameworkREADME.md)

\subsection ArchitectureGuides Architecture Guides

- \ref DataModel "Data Model": Building observable data models with change notification
- \ref ModelObserver "Model/Observer Concept": Implementing the observer pattern
- \ref Persistence "Persistence Concept": Object serialization and file persistence
- \ref ComponentConcept "Component Concept": Understanding component-based architecture

\subsection ApplicationFrameworks Application Frameworks

- \ref ApplicationFramework "Application Framework": General application structure
- \ref DocumentBasedFramework "Document-Based Framework": MDI/SDI applications
- \ref ParameterManagement "Parameter Management": Parameter-driven applications

\section BuildingACF Building ACF

\subsection Prerequisites Prerequisites

<b>Required Software:</b>
- C++ compiler with C++11 support or later
- Qt Framework (version 5.x or 6.x)
- CMake or qmake build system
- Doxygen (for documentation generation)

<b>Optional Dependencies:</b>
- Boost (for extended functionality)
- OpenCV (for image processing components)
- Qwt/Qwt3D (for plotting components)
- Other third-party libraries as needed

\subsection BuildProcess Build Process

ACF uses a standard CMake or qmake build process:

1. <b>Configure the environment:</b> Set ACFDIR environment variable to the ACF installation directory
2. <b>Generate build files:</b> Use CMake or qmake to generate platform-specific build files
3. <b>Build libraries:</b> Build the core libraries in dependency order
4. <b>Build applications:</b> Build applications and tools
5. <b>Run tests:</b> Execute the test suite to verify the build

<b>Example Build Commands:</b>
\code{.sh}
# Set environment variable
export ACFDIR=/path/to/acf

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build
make -j8

# Run tests
ctest
\endcode

\subsection DeploymentOptions Deployment Options

ACF supports multiple deployment strategies:

<b>1. Development Mode:</b>
- Dynamic component loading from DLL/shared libraries
- Runtime composition with ARX files
- Hot-reloading support for rapid iteration
- Use ACF Compositor for visual editing

<b>2. Production Mode:</b>
- Compile component compositions to C++ using ARX Compiler (ARXC)
- Static linking of components into executable
- Optimized performance with compiler optimizations
- Reduced deployment size and complexity

<b>3. Plugin Mode:</b>
- Package components as ARP files (ACF Registry Package)
- Dynamic discovery and loading of plugins
- Extensible application architecture
- Update plugins without recompiling main application

\section GettingStarted Getting Started

\subsection QuickStart Quick Start Guide

<b>1. Define an Interface</b>
\code{.cpp}
// MyInterface.h
#include <istd/IPolymorphic.h>

class IMyProcessor : virtual public istd::IPolymorphic
{
public:
    virtual bool Process(const QString& input, QString& output) = 0;
};
\endcode

<b>2. Implement a Component</b>
\code{.cpp}
// MyComponent.h
#include <icomp/CComponentBase.h>
#include "MyInterface.h"

class CMyProcessor : 
    public icomp::CComponentBase, 
    virtual public IMyProcessor
{
public:
    typedef icomp::CComponentBase BaseClass;
    
    I_BEGIN_COMPONENT(CMyProcessor);
        I_REGISTER_INTERFACE(IMyProcessor);
    I_END_COMPONENT;
    
    CMyProcessor();
    
    // Implement interface
    virtual bool Process(const QString& input, QString& output) override;
};

// MyComponent.cpp
CMyProcessor::CMyProcessor() 
{
}

bool CMyProcessor::Process(const QString& input, QString& output)
{
    output = input.toUpper();
    return true;
}
\endcode

<b>3. Register and Use</b>

Components are registered in package registration files and accessed through the component registry:

\code{.cpp}
// Package registration (in .cpp file)
ICOMP_REGISTER_COMPONENT(CMyProcessor)

// Usage - accessing via registry
icomp::IRegistry* registry = GetRegistry();
IMyProcessor* processor = registry->QueryInterface<IMyProcessor>("MyProcessor");
if (processor)
{
    QString result;
    processor->Process("hello", result);
    // result = "HELLO"
}
\endcode

\subsection LearnMore Learning Resources

- Start with \ref page_istd for understanding change notification
- Learn \ref page_icomp for component architecture
- Study \ref page_iser for object persistence
- Explore \ref page_iprm for parameter management
- Read the complete Component Framework guide (Docs/ComponentFramework.md)

\section DesignPatterns Design Patterns in ACF

ACF implements many proven design patterns:

<b>Creational Patterns:</b>
- <b>Factory Pattern:</b> icomp::IComponentFactory for dynamic component creation
- <b>Abstract Factory:</b> Component families through package system
- <b>Singleton:</b> Registry and environment management
- <b>Builder:</b> Component composition with ACF Compositor

<b>Structural Patterns:</b>
- <b>Composite:</b> Hierarchical component composition (core concept)
- <b>Proxy:</b> imod::CModelProxy for delegation
- <b>Adapter:</b> Archive adapters for different formats
- <b>Facade:</b> High-level library interfaces

<b>Behavioral Patterns:</b>
- <b>Observer:</b> istd::IChangeable and imod::IModelObserver
- <b>Strategy:</b> Interface-based component implementations
- <b>Command:</b> Undo/redo in document framework
- <b>Template Method:</b> Component lifecycle hooks

\section BestPractices Best Practices and Guidelines

\subsection ComponentDesign Component Design

-# <b>Interface Segregation:</b> Define focused, single-purpose interfaces
-# <b>Dependency Injection:</b> Use references for component dependencies
-# <b>Immutable IDs:</b> Component IDs should not change after creation
-# <b>Loose Coupling:</b> Communicate only through interfaces
-# <b>Composition Over Inheritance:</b> Prefer component composition

\subsection ErrorHandling Error Handling

-# <b>Return Value Checking:</b> Check return values from all operations
-# <b>Change Notification Scoping:</b> Use istd::CChangeNotifier for RAII
-# <b>Archive Error Handling:</b> Verify archive state after operations
-# <b>Validation:</b> Validate parameters and constraints before processing
-# <b>Logging:</b> Use ilog for diagnostic information

\subsection Performance Performance Considerations

-# <b>Lazy Initialization:</b> Components support lazy reference resolution
-# <b>Batch Changes:</b> Group related changes with istd::CChangeGroup
-# <b>Binary Archives:</b> Use binary archives for performance-critical serialization
-# <b>Smart Pointers:</b> Use appropriate smart pointer types to avoid overhead
-# <b>Static Linking:</b> Compile compositions for production deployment

\subsection ThreadSafety Thread Safety

-# Most ACF components are <b>not thread-safe</b> by default
-# Use Qt's signal/slot mechanism for cross-thread communication
-# Use iqtgui::TGuiObserverWrap for GUI updates from worker threads
-# Synchronize access to shared components with mutexes
-# Consider thread-local component instances for parallel processing

\section TroubleshootingFAQ Troubleshooting and FAQ

\subsection CommonIssues Common Issues

<b>Q: Component not found in registry</b>
- Verify component is registered with ICOMP_REGISTER_COMPONENT
- Check package is loaded correctly
- Ensure component ID matches in registry and lookup

<b>Q: Dependency injection not working</b>
- Verify reference ID matches target component ID
- Check component hierarchy and scope
- Ensure referenced component is created before referencing component

<b>Q: Serialization fails</b>
- Implement iser::ISerializable correctly
- Check archive is in correct mode (reading/writing)
- Verify Serialize() is symmetric for read/write
- Add version information for compatibility

<b>Q: GUI not updating on model change</b>
- Verify observer is attached to model
- Check change flags are correct
- Use iqtgui::TGuiObserverWrap for thread safety
- Ensure istd::CChangeNotifier is used in setter methods

<b>Q: Memory leaks detected</b>
- Use smart pointers (istd::TUniqueInterfacePtr, TSharedInterfacePtr)
- Check for circular references
- Verify component lifecycle management
- Use memory profiling tools to identify sources

\subsection DebugTips Debug Tips

-# <b>Enable Logging:</b> Use ilog with Debug level for detailed information
-# <b>Inspect Registry:</b> Use registry traversal to verify component structure
-# <b>Test in Isolation:</b> Unit test components separately before integration
-# <b>Check Archives:</b> Export to XML to inspect serialized data
-# <b>Use Compositor:</b> Visualize component connections graphically

\section Gallery Screenshots and Examples

\image html Compositor1.png "ACF Compositor - Visual Component Editing" width=80%

<b>ACF Compositor</b> provides a graphical interface for:
- Creating and editing component compositions
- Visual dependency wiring between components
- Property and attribute configuration
- UML-like component diagrams
- Export to ARX format for runtime or compilation

\image html Compositor2.png "Multi-Document Application Framework" width=80%

<b>Document Framework</b> example showing:
- Multiple document interface (MDI) with docking windows
- Integrated parameter editing panels
- Log viewer and diagnostic tools
- Menu and toolbar customization
- Full undo/redo support

\image html HttServerFramework.png "HTTP-based Server Framework" width=80%

<b>Server Framework</b> demonstrating:
- Component-based server architecture
- RESTful API implementation with components
- Modular request handling
- Extensible service architecture

\section LicenseContributing License and Contributing

\subsection License License Information

ACF is open source software released under the <b>GNU Lesser General Public License (LGPL)</b>.

<b>Key Points:</b>
- Free to use in both open source and commercial applications
- Modifications to ACF itself must be shared under LGPL
- Applications using ACF can be proprietary
- See LGPL directory for complete license text

\subsection Contributing Contributing to ACF

Contributions are welcome! To contribute:

1. Fork the repository on GitHub
2. Create a feature branch
3. Follow the existing coding conventions
4. Add tests for new functionality
5. Update documentation as needed
6. Submit a pull request

<b>Repository:</b> https://github.com/ImagingTools/Acf

<b>Documentation:</b> https://imagingtools.github.io/Acf/

\subsection Support Support and Contact

- <b>Issues:</b> Report bugs and feature requests on GitHub
- <b>Documentation:</b> Read the comprehensive guides and API reference
- <b>Examples:</b> Study the example code in the repository
- <b>Community:</b> Engage with other developers using ACF

*/


/**
	\defgroup DataModel Data model
	Basic interfaces and implementations for abstract definition of the data model.

	\section BasicMotivation Motivation
	A fundamental problem in the implementation of complex software applications is ensuring a clean separation between the data model, business logic (controller) and data presentation (GUI).
	Such a separation allows a high degree of reusability of source code. The following aspects have to be represented by interfaces:
	- Data changes. Transaction concept.
	- Notification of data changes.
	- Forwarding of data changes.
	- Subscribing/Unsubscribing for the entire data object or for concrete data change notifications.
	- General concept of the data visualization and editing.
	- Undo/Redo mechanism to roll back the data changes.

	\section DataModelOverview Overview
	The most important interface for a general data model definition is istd::IChangeable.
	This is a common interface for describing of objects which change their state during the run time of the application.
	The interface provides methods for managing data change transaction (\c istd::IChangeable::BeginChanges and \c istd::IChangeable::EndChanges), methods for coping, cloning, reseting and comparison of objects. The realization of change notification mechanism is also based on this interface.
	Following example demonstrates implementation of a simple data object:
	\code
	class CPerson: virtual public istd::IChangeable
	{
	public:
		enum ChangeFlags
		{
			CF_NAME_CHANGED = 0x74b520	// Some random, unique number
		}

		CPerson(const QString& firstName = QString(), const QString& lastName = QString());
		CPerson(const CPerson& person);

		QString GetFirstName() const;
		void SetFirstName(const QString& firstName);
		QString GetLastName() const;
		void SetLastName(const QString& name);

	private:
		QString m_firstName;
		QString m_lastName;
	};

	QString CPerson::GetFirstName() const
	{
		return m_firstName;
	}

	QString CPerson::SetFirstName(const QString& firstName)
	{
		if (m_firstName != firstName){
			static ChangeSet changeSet(CF_NAME_CHANGED, "Change first name");
			BeginChanges(changeSet);

			m_firstName = firstName;

			EndChanges(changeSet);
		}
	}

	QString CPerson::GetLastName() const
	{
		return m_lastName;
	}

	QString CPerson::SetLastName(const QString& lastName)
	{
		if (m_lastName != lastName){
			static ChangeSet changeSet(CF_NAME_CHANGED, "Change last name");
			BeginChanges(changeSet);

			m_lastName = lastName;

			EndChanges(changeSet);
		}
	}
	\endcode

	To ensure that the transaction block is always consistent, you could also use a helper class - istd::CChangeNotifier:

	\code
	QString CPerson::SetFirstName(const QString& firstName)
	{
		if (m_firstName != firstName){
			static ChangeSet changeSet(CF_NAME_CHANGED, "Change first name");
			istd::CChangeNotifier notifier(this, &changeSet);
			Q_UNUSED(notifier);

			m_firstName = firstName;
		}
	}
	\endcode

	istd::CChangeNotifier calls BeginChanges in its constructor and EndChanges in the destructor.

	\subsection WhyTransaction Why do we need begin-end notification of the changes of data?
	Often you want to be informed about the upcoming changes. A simple example you want to save the existing data before it is overwritten with the changed data.
	This is responsibility of the istd::IChangeable::BeginChanges method. 
	The end change notification you will need, if you want to know when the data changes are complete, for example to update a GUI.
	
	\section DelegatingOfChanges Delegating of changes
	An important aspect in the management of data change notifications is the delegating of changes from a part of data to another.
	Let us consider the following situation - the data object of class CPerson could "live" in any container class (eg. in a database).
	In this case we want the container implentation will also notice about the changes of the CPerson instance.
	What we have to do is to extend our CPerson implementation as following:

	\code
	class CPerson: public istd::TChangeDelegator<istd::IChangeable>
	\endcode

	Then our container class can be defined as:
	\code
	class CPersonDatabase: virtual public istd::IChangeable
	{
	public:
		int GetPersonsCount() const
		{
			return m_persons.count();
		}

		const CPerson& GetPerson(int personIndex) const
		{
			return m_persons[personIndex];
		}

		CPerson& AddNewPerson(const QString& firstName, const QString& lastName);

	protected:
		// reimplemented (istd::IChangeable)
		virtual void OnEndChanges(const ChangeSet& changeSet)

	private:
		QVector<CPerson> m_persons;
	};

	CPerson& CPersonDatabase::AddNewPerson(const QString& firstName, const QString& lastName)
	{
		CPerson person(firstName, lastName);

		m_persons.push_back(person);

		// Register the container as change notification target:
		m_persons.back().SetSlavePtr(this);

		return m_persons.back();
	}
	\endcode

	Now we can catch the changes of person instances in the implementation of the method OnEndChanges():

	\code
	// reimplemented (istd::IChangeable)

	void CPersonDatabase::OnEndChanges(const ChangeSet& changeSet)
	{
		// Use CF_DELEGATED masking to filter out the delegated changes:
		if (changeSet.Contains(CF_DELEGATED)){
			// We will end up here, every time when CPerson::SetFirstName or CPerson::SetLastName were called:
			qDebug("Some person data have been changed");
		}
	}
	\endcode
	
	In this section we have considered situations in which a decision that a data model would delegate its changes has been made in the design phase.
	For situations where this is not the case, we must rely on other mechanisms. These are described in the \ref ModelObserver Section.

	\ingroup AcfCore
*/

/**
	\defgroup ModelObserver Model/Observer concept
	The main use-case of this concept is to get information on the observer side about changes of the data.
	It can be used e.g. to realize GUI update if related data model changes.

	\section ModelObserverExternalLinks External Links
	http://en.wikipedia.org/wiki/Observer_pattern

	http://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93controller

	\sa DataModel

	\ingroup AcfCore
*/

/**
	\defgroup Persistence Persistence concept
	Common interfaces and implementations for file based persistence and general object serialization.
	
	\section ObjectSerialization Archive-based object serialization
	Objects that provide serialization must implement the iser::ISerializable interface. The most important method of this interface is Serialize().
	Serialize method becomes as input an so called archive (iser::IArchive). An archive provides an abstract low level read/write access to a data medium.
	By example an archive can represent a file, a memory block, a database or a network resource, but the concrete kind of the archive is completely hidden from data object's point of view.
	Thus, we create a complete separation between the data model and the medium on which it is to be made persistent. Following archive types are provided by ACF core libraries:
	- Read from a memory block - iser::CMemoryReadArchive
	- Write to a memory block - iser::CMemoryWriteArchive
	- Read from a binary file - ifile::CFileReadArchive
	- Write to a binary file - ifile::CFileWriteArchive
	- Read from a fast parsed XML document given as a string - iser::CXmlStringReadArchive
	- Write to a fast parsed XML-string - iser::CXmlStringWriteArchive
	- Read from a fast parsed XML file - ifile::CSimpleXmlFileReadArchive
	- Write to a fast parsed XML file - ifile::CSimpleXmlFileWriteArchive
	- Read from a memory block bitwise - iser::CBitMemoryReadArchive
	- Write to a memory block bitwise - iser::CBitMemoryWriteArchive
	- Write to a TCP/IP socket - iqt::CNetworkWriteArchive
	- Read from a compact XML document given as a string - iser::CCompactXmlMemReadArchive
	- Write to a compact XML-string - iser::CCompactXmlMemWriteArchive
	- Read from a compact XML file - ifile::CCompactXmlFileReadArchive
	- Write to a compact XML file - ifile::CCompactXmlFileWriteArchive
	- Read from a JSON file - ifile:CJsonFileReadArchive
	- Write to a JSON file - ifile::CJsonFileWriteArchive
	- Read from a JSON representation in memory - ifile:CJsonMemReadArchive
	- Write to a JSON representation in memory - ifile::CJsonMemWriteArchive

	We demonstrate the implementation of object serialization on example of CPerson class introduced in the \ref DataModel section. 
	First of all we have to change the CPerson class to be inherited from iser::ISerializable. Because iser::ISerializable is already derived from istd::IChangeable, we have nothing else to change in this aspect.
	Also we rewrite:

	\code
	class CPerson: virtual public iser::ISerializable
	{
	public:
		
		// reimplemented (iser::ISerializable)
		virtual bool Serialize(iser::IArchive& archive) override;

	private:
		QString m_firstName;
		QString m_lastName;
	};

	// reimplemented (iser::ISerializable)

	bool CPerson::Serialize(iser::IArchive& archive)
	{
		bool retVal = true;

		// Use data model change notification only if the object will be reading from archive:
		istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);

		// Serialize object using tags:

		// Serialize first name of the person:
		static iser::CArchiveTag firstNameTag("FirstName", "First name of the person");
		retVal = retVal && archive.BeginTag(firstNameTag); // Begin tag
		retVal = retVal && archive.Process(m_firstName); // Process first name by archive	
		retVal = retVal && archive.EndTag(firstNameTag); // Close tag

		// Serialize last name of the person:
		static iser::CArchiveTag lastNameTag("LastName", "Last name of the person");
		retVal = retVal && archive.BeginTag(lastNameTag);
		retVal = retVal && archive.Process(m_lastName);
		retVal = retVal && archive.EndTag(lastNameTag);

		return retVal;
	}

	\endcode

	\section FilePersistence File-based persistence

	Serialization is a low-level persistence mechanism, in which the structure of the archived data on the medium depends on Serialize method implementation inside of the data object.
	But often you will offer persistence functionality, that is not depending on concrete object implementation.
	A such situation is the supporting of known data formats (for example reading or writing of PNG images, OpenOffice documents and so on).
	For such persistence implementations ACF provides ifile::IFilePersistence interface.

	\sa DataModel

	\ingroup AcfCore
*/

/**
	\defgroup Main Main concepts and implementations
	If you want to learn how to use ACF, you should begin with elements defined in this module.

	\ingroup AcfCore
*/

/**
	\defgroup SystemDefines Operation system services
	Access to operating system related functionality.

	\ingroup AcfCore
*/

/**
	\defgroup ComponentConcept Component concept
	This concept allows to divide application into smaller modules.
	At the implementation level you don't need to care about the application topology.
	The topology will be described separately using \em .acc files and can be started using
	runtime environment over ACF tool or compiled to C++ code using ARXC compiler.
	Additionally you can use component simulation to use components as "normal" classes.

	\ingroup AcfCore
*/

/**
	\defgroup ApplicationFramework Application Framework

	Set of components and basic implementations for general definition of a software application.
*/

/*
	\defgroup Logging Diagnostic Management
	Interfaces and implementations related to informations logging.

	\ingroup ApplicationFramework
*/

/**
	\defgroup BasicUi Basic UI
	Basic implementation for UI-based components.

	\ingroup ApplicationFramework
*/

/**
	\defgroup UiEditors UI-based Editors
	Basic implementation for UI-based data editors.

	\sa DataModel

	\ingroup ApplicationFramework
*/

/**
	\defgroup ParameterManagement Parameter Management
	Implementation of parameter management concept.

	\ingroup ApplicationFramework
*/

/**
	\defgroup DocumentBasedFramework Document-based Framework
	Framework for document-based applications (MDI, SDI)

	\ingroup ApplicationFramework
*/

/**
	\defgroup NumericalBase Numerical Basics & Algorithms
	Basic implementations of mathematical and algorithmical primitives.
*/


/**
	\defgroup ImageProcessing Image Processing
	Image-based data definition and algorithms.
*/

/**
	\defgroup Geometry Geometry
	Basic implementations of geometrical primitives in 2D- and 3D-space.

	\ingroup NumericalBase
*/

/**
	\defgroup Color Color
	Data primitives and algorithms related to color definition and management.
*/




