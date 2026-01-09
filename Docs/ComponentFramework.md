# ACF Component Framework Documentation

## Table of Contents

1. [Introduction](#introduction)
2. [Core Concepts](#core-concepts)
3. [Component Architecture](#component-architecture)
4. [Component Lifecycle](#component-lifecycle)
5. [Defining Components](#defining-components)
6. [Attributes and Parameters](#attributes-and-parameters)
7. [References and Dependencies](#references-and-dependencies)
8. [Factory Attributes](#factory-attributes)
9. [Component Registry](#component-registry)
10. [Composite Components](#composite-components)
11. [Static Information and Metadata](#static-information-and-metadata)
12. [Component Registration](#component-registration)
13. [Complete Examples](#complete-examples)
14. [Best Practices](#best-practices)
15. [Advanced Topics](#advanced-topics)

---

## Introduction

The **icomp** library provides a comprehensive component-based architecture for building modular, extensible applications. It implements a sophisticated dependency injection system, component registration, and a flexible attribute-based configuration mechanism.

### The Key Concept: Components All The Way Down

The fundamental and unique feature of ACF is that **a composition of components is itself a component**. This means:

- The **entire application** is a composition of components
- A complex application with thousands of components can be seen from the outside as a **single component**
- This single "application component" can be **reused** as a component in another application
- There is **no distinction** between a host application and plugins - everything is a component
- Components can be **nested** to any depth, creating hierarchical structures

This compositional approach enables:
- **Extreme Modularity**: Build large systems from small, testable pieces
- **Reusability**: Entire subsystems can be dropped into other projects
- **Testability**: Test complex components in isolation
- **Flexibility**: Reconfigure systems by changing component composition

### Example: Application as a Component

A complete text editor application might be structured as:

```
TextEditorApp (Component)
├── DocumentManager (Component)
│   ├── TextDocument (Component)
│   ├── FileLoader (Component)
│   └── UndoManager (Component)
├── MainWindow (Component)
│   ├── MenuBar (Component)
│   ├── ToolBar (Component)
│   └── TextView (Component)
└── SettingsManager (Component)
    └── XmlPersistence (Component)
```

This entire `TextEditorApp` component can be:
- Run as a standalone application
- Embedded in another application as a document editor
- Tested as a single unit
- Configured via a single `.acc` file

### Key Features

- **Interface-Based Architecture**: Components implement interfaces for loose coupling
- **Dependency Injection**: Automatic resolution and injection of component dependencies
- **Hierarchical Registry**: Component lookup and navigation through a registry system
- **Attribute System**: Type-safe, extensible attribute-based metadata and configuration
- **Factory Pattern**: Dynamic component instantiation with factories
- **Serialization**: Built-in support for component persistence via the iser library
- **Static Metadata**: Compile-time reflection for component types
- **Thread-Safe**: Thread-safe reference resolution and lazy initialization

### Dependencies

The icomp library depends on:
- **istd**: Standard utilities and interfaces (change notification, smart pointers)
- **iattr**: Attribute system (typed attributes, multi-attributes)
- **iser**: Serialization framework (archive-based object persistence)
- **Qt Core**: QString, QByteArray, QMap, QSet, and other core classes

---

## Core Concepts

### Components

A **component** is the fundamental building block in ACF. Components:
- Implement one or more interfaces for their functionality
- Have a unique ID within their parent/context
- Can have attributes (configuration parameters)
- Can reference other components (dependency injection)
- Can be composed into larger components (composite pattern)
- Support serialization when implementing `iser::ISerializable`

### Interfaces

Components communicate through **interfaces** (abstract base classes). This enables:
- Loose coupling between components
- Multiple implementations of the same interface
- Runtime polymorphism
- Easy testing with mock implementations

### Component Composition

The **composite pattern** is central to ACF:
- A composite component contains other components as subcomponents
- The composite component itself is also a component
- Subcomponents can be accessed by ID
- The whole hierarchy can be configured via `.acc` XML files
- Any composition can be treated as a single reusable unit

### Configuration Files (.acc)

Component configurations are stored in XML files:
- Define what components exist and how they're connected
- Specify attribute values
- Define references between components
- Enable runtime reconfiguration without code changes

---

## Component Architecture

### Core Interfaces for Component Users

#### IComponent

The base interface for all components (from the framework, not typically used directly):

```cpp
class IComponent: virtual public istd::IPolymorphic
{
public:
    // Get parent component
    virtual const IComponent* GetParentComponent(bool ownerOnly = false) const = 0;
    
    // Get component context (framework usage)
    virtual IComponentContextSharedPtr GetComponentContext() const = 0;
};
```

**Note**: Most component developers don't interact with `IComponent` directly - they inherit from `CComponentBase` instead.

#### Component Base Classes

**CComponentBase** - The standard base class for implementing components:

```cpp
class CComponentBase: virtual public IComponent
{
protected:
    // Check if component is active
    bool IsComponentActive() const;
    
    // Lifecycle hooks (override these in your component)
    virtual void OnComponentCreated();
    virtual void OnComponentDestroyed();
};
```

**CCompositeComponent** - Base class for components that contain subcomponents:

```cpp
class CCompositeComponent: public CComponentBase
{
protected:
    // Access subcomponents by ID
    IComponentSharedPtr GetSubcomponent(const QByteArray& componentId) const;
    
    // Create new instance of subcomponent
    IComponentUniquePtr CreateSubcomponent(const QByteArray& componentId) const;
};
```

### Typical Component Structure

Most components follow this pattern:

```cpp
// 1. Define your interface
class IDataProcessor
{
public:
    virtual ~IDataProcessor() = default;
    virtual void ProcessData(const Data& input) = 0;
};

// 2. Implement your component
class CMyProcessor: 
    public icomp::CComponentBase,
    public IDataProcessor
{
public:
    typedef icomp::CComponentBase BaseClass;
    
    // 3. Define component structure with macros
    I_BEGIN_COMPONENT(CMyProcessor);
        I_REGISTER_INTERFACE(IDataProcessor);
        I_ASSIGN(m_bufferSize, "BufferSize", "Processing buffer size", true, 1024);
    I_END_COMPONENT;
    
    // 4. Implement your interface
    virtual void ProcessData(const Data& input) override;
    
protected:
    // 5. Initialize in OnComponentCreated
    virtual void OnComponentCreated() override;
    
private:
    I_ATTR(int, m_bufferSize);
};
```

---

## Component Lifecycle

Components go through a well-defined lifecycle with symmetric creation/destruction pairs:

### Lifecycle Diagram

```
Component Lifecycle States and Transitions:

Constructor()
    |
    | [Object created in memory]
    v
SetComponentContext(context, parent, isOwner)
    |
    | [Component configured with registry data]
    v
OnComponentCreated()  <--- INITIALIZATION PHASE
    |
    | [Component is active]
    | [Safe to use attributes, references, factories]
    | [Component performs its work]
    |
OnComponentDestroyed()  <--- CLEANUP PHASE
    |
    | [Component resources released]
    v
~Destructor()
    |
    v
[Object destroyed]


Model/Observer Lifecycle (for components using imod interfaces):

AttachObserver(observer)
    |
    v
observer->OnModelAttached(model, changeMask)  <--- OBSERVER INITIALIZATION
    |
    | [Observer receives updates]
    | BeforeUpdate() / AfterUpdate()
    |
DetachObserver(observer)
    |
    v
observer->OnModelDetached(model)  <--- OBSERVER CLEANUP


GUI Component Lifecycle (for iqtgui components):

OnComponentCreated()
    |
    v
OnGuiCreated()  <--- GUI WIDGET CREATION
    |
    | [Optional] OnGuiModelAttached()  <--- GUI-MODEL BINDING
    |     |
    |     | [GUI displays and updates model]
    |     |
    |     v
    | OnGuiModelDetached()  <--- GUI-MODEL UNBINDING
    |
    v
OnGuiDestroyed()  <--- GUI WIDGET CLEANUP
    |
    v
OnComponentDestroyed()
```

### Symmetric Pairs

The lifecycle has symmetric pairs that must be balanced:

| Creation Phase | Destruction Phase | Purpose |
|----------------|-------------------|---------|
| `Constructor()` | `~Destructor()` | C++ object lifecycle |
| `SetComponentContext(...)` | `SetComponentContext(null, ...)` | Framework configuration |
| `OnComponentCreated()` | `OnComponentDestroyed()` | Component initialization/cleanup |
| `OnModelAttached(...)` | `OnModelDetached(...)` | Observer pattern binding |
| `OnGuiCreated()` | `OnGuiDestroyed()` | GUI widget creation/destruction |
| `OnGuiModelAttached()` | `OnGuiModelDetached()` | GUI-to-model binding |

### 1. Construction

```cpp
MyComponent::MyComponent()
{
    // Basic C++ construction
    // DO NOT access attributes or references here!
}
```

### 2. Context Assignment

The framework calls `SetComponentContext()` with the component's configuration.

### 3. Component Creation

```cpp
void MyComponent::OnComponentCreated()
{
    BaseClass::OnComponentCreated();
    
    // Initialize attributes
    // Connect to other components
    // Register signal handlers
    // NOW it's safe to access attributes and references
}
```

### 4. Active Phase

Component is fully functional and can process requests.

### 5. Component Destruction

```cpp
void MyComponent::OnComponentDestroyed()
{
    // Disconnect signals
    // Release resources
    // Clean up references
    
    BaseClass::OnComponentDestroyed();
}
```

### 6. Destructor

```cpp
MyComponent::~MyComponent()
{
    // Final cleanup
    // Component is no longer valid
}
```

### Important Rules

- **Never access attributes/references in constructor** - they're not initialized yet
- **Always use `OnComponentCreated()`** for initialization that depends on configuration
- **Always call base class methods** in lifecycle hooks
- **Clean up in `OnComponentDestroyed()`** before resources are released

---

## Defining Components

### Basic Component Definition

Use macros to define component structure:

```cpp
#include <icomp/CComponentBase.h>

class MyComponent: public icomp::CComponentBase
{
public:
    typedef icomp::CComponentBase BaseClass;
    
    // Component definition section
    I_BEGIN_COMPONENT(MyComponent);
        // Register interfaces this component implements
        I_REGISTER_INTERFACE(IMyInterface);
        
        // Declare attributes (see next section)
        I_ASSIGN(m_nameAttr, "Name", "Component name", false, QByteArray("default"));
        
    I_END_COMPONENT;
    
    // Constructor
    MyComponent();
    
protected:
    // Lifecycle hooks
    virtual void OnComponentCreated() override;
    virtual void OnComponentDestroyed() override;
    
private:
    // Attribute members
    I_ATTR(QByteArray, m_nameAttr);
};
```

### Component Definition Macros

#### I_BEGIN_COMPONENT / I_END_COMPONENT

Delimits the component definition section. Automatically creates:
- Static info structure
- `InitStaticInfo()` static method
- `GetComponentStaticInfo()` virtual method

```cpp
I_BEGIN_COMPONENT(MyComponent);
    // Component declarations go here
I_END_COMPONENT;
```

#### I_BEGIN_BASE_COMPONENT

For abstract base components that cannot be instantiated:

```cpp
I_BEGIN_BASE_COMPONENT(MyAbstractComponent);
    I_REGISTER_INTERFACE(IMyInterface);
I_END_COMPONENT;
```

#### I_REGISTER_INTERFACE

Declares that the component implements an interface:

```cpp
I_REGISTER_INTERFACE(ILogger);
I_REGISTER_INTERFACE(ISerializable);
```

This allows the component to be queried for these interfaces via `GetInterface()`.

---

## Attributes and Parameters

Attributes are type-safe configuration parameters attached to components.

### Declaring Attributes

#### Simple Attributes

```cpp
// Boolean attribute
I_ATTR(bool, m_enabledAttr);

// Integer attribute
I_ATTR(int, m_countAttr);

// Double attribute
I_ATTR(double, m_scaleAttr);

// String attribute (non-translatable)
I_ATTR(QByteArray, m_idAttr);
I_ATTR(QString, m_pathAttr);
```

#### Translatable Text Attributes

```cpp
// Translatable string attribute
I_TEXTATTR(m_titleAttr);
```

#### Multiple Value Attributes

```cpp
// Multiple integers
I_MULTIATTR(int, m_valuesAttr);

// Multiple strings
I_MULTIATTR(QString, m_namesAttr);
```

### Initializing Attributes

Use `I_ASSIGN` macro in the component definition:

```cpp
I_BEGIN_COMPONENT(MyComponent);
    // I_ASSIGN(member, attrId, description, isObligatory, defaultValue)
    
    // Obligatory attribute - must be configured, framework ensures it exists
    I_ASSIGN(m_enabledAttr, "Enabled", "Enable this feature", true, true);
    
    // Optional attributes - can be omitted, may return invalid
    I_ASSIGN(m_countAttr, "Count", "Number of items", false, 10);
    I_ASSIGN(m_scaleAttr, "Scale", "Scaling factor", false, 1.0);
    
    // Obligatory attribute with default
    I_ASSIGN(m_nameAttr, "Name", "Component name", true, QByteArray("default"));
I_END_COMPONENT;
```

Parameters:
- **member**: The attribute member variable
- **attrId**: Attribute ID used in configuration files
- **description**: Human-readable description
- **isObligatory**: 
  - `true` = Obligatory attribute - framework ensures the attribute is created. `IsValid()` will always return `true`. Use for required configuration.
  - `false` = Optional/nullable attribute - may not be configured. `IsValid()` returns `false` if not set. Check validity before use.
- **defaultValue**: Default value used when not explicitly configured

### Accessing Attribute Values

After `OnComponentCreated()`:

```cpp
void MyComponent::OnComponentCreated()
{
    BaseClass::OnComponentCreated();
    
    // Get value using dereference operator
    bool enabled = *m_enabledAttr;
    int count = *m_countAttr;
    double scale = *m_scaleAttr;
    QByteArray name = *m_nameAttr;
    
    // Check if attribute is valid (assigned)
    if (m_nameAttr.IsValid()) {
        // Use the attribute
    }
    
    // Access via pointer
    const QByteArray& nameRef = *m_nameAttr;
}
```

### Attribute Configuration

In `.acc` XML files:

```xml
<Element Id="MyComp" PackageId="MyPackage" ComponentId="MyComponent">
    <Data IsEnabled="true">
        <AttributeInfoMap>
            <AttributeInfo Id="Enabled" Type="Boolean">
                <Data IsEnabled="true" Value="true"/>
            </AttributeInfo>
            <AttributeInfo Id="Count" Type="Integer">
                <Data IsEnabled="true" Value="42"/>
            </AttributeInfo>
            <AttributeInfo Id="Name" Type="String">
                <Data IsEnabled="true" Value="MyInstance"/>
            </AttributeInfo>
        </AttributeInfoMap>
    </Data>
</Element>
```

---

## References and Dependencies

References allow components to depend on and access other components.

### Declaring References

```cpp
// IMyInterface.h - Interface definition
class IMyInterface
{
public:
    virtual ~IMyInterface() = default;
    virtual void DoWork() = 0;
};

// MyComponent.h
class MyComponent: 
    public icomp::CComponentBase,
    public IMyInterface  // Inherit from the interface
{
public:
    typedef icomp::CComponentBase BaseClass;
    
    I_BEGIN_COMPONENT(MyComponent);
        I_REGISTER_INTERFACE(IMyInterface);  // Register the interface we inherit
        
        // Single reference
        I_ASSIGN(m_loggerRef, "Logger", "Logging component", true);
        
        // Optional reference
        I_ASSIGN(m_helpProviderRef, "HelpProvider", "Help provider", false);
    I_END_COMPONENT;
    
    // IMyInterface implementation
    virtual void DoWork() override;
    
private:
    // Reference to ILogger interface
    I_REF(ilog::ILogger, m_loggerRef);
    
    // Optional reference
    I_REF(ihelp::IHelpProvider, m_helpProviderRef);
};
```

### Using References

```cpp
void MyComponent::DoWork()
{
    // Check if reference is valid
    if (m_loggerRef.IsValid()) {
        // Use the arrow operator to access interface
        m_loggerRef->LogMessage("Work started");
    }
    
    // Ensure reference is initialized (same as IsValid)
    if (m_loggerRef.EnsureInitialized()) {
        m_loggerRef->LogMessage("Work completed");
    }
    
    // Direct pointer access
    ilog::ILogger* logger = m_loggerRef.GetPtr();
    if (logger) {
        logger->LogMessage("Direct access");
    }
}
```

### Multiple References

For arrays of references:

```cpp
I_BEGIN_COMPONENT(MyComponent);
    // Multiple references
    I_ASSIGN_MULTI_0(m_processorsRef, "Processors", "Data processors", true);
I_END_COMPONENT;

private:
    I_MULTIREF(IDataProcessor, m_processorsRef);
```

Access:

```cpp
void MyComponent::ProcessData()
{
    if (m_processorsRef.IsValid()) {
        int count = m_processorsRef.GetCount();
        for (int i = 0; i < count; ++i) {
            IDataProcessor* processor = m_processorsRef.GetPtr(i);
            if (processor) {
                processor->Process();
            }
        }
    }
}
```

### Reference Configuration

In `.acc` files:

```xml
<AttributeInfo Id="Logger" Type="Reference">
    <Data IsEnabled="true" Value="ApplicationLogger"/>
</AttributeInfo>
<AttributeInfo Id="Processors" Type="MultiReference">
    <Data IsEnabled="true">
        <Values>
            <Value>Processor1</Value>
            <Value>Processor2</Value>
            <Value>Processor3</Value>
        </Values>
    </Data>
</AttributeInfo>
```

### Thread Safety

References use thread-safe lazy initialization:
- First access resolves the reference
- Result is cached for future accesses
- Uses atomic operations and mutex for thread safety
- Safe to call from multiple threads

---

## Factory Attributes

Factory attributes specify components to be instantiated dynamically.

### Declaring Factory Attributes

```cpp
class DocumentTemplate: public icomp::CComponentBase
{
public:
    typedef icomp::CComponentBase BaseClass;
    
    I_BEGIN_COMPONENT(DocumentTemplate);
        // Factory for creating documents
        I_ASSIGN(m_documentFactory, "DocumentFactory", 
                 "Factory for document instances", false);
        
        // Optional factory
        I_ASSIGN(m_viewFactory, "ViewFactory", 
                 "Factory for view instances", true);
    I_END_COMPONENT;
    
private:
    I_FACT(IDocument, m_documentFactory);
    I_FACT(IView, m_viewFactory);
};
```

### Using Factories

```cpp
IComponentUniquePtr DocumentTemplate::CreateDocument()
{
    // Check if factory is valid
    if (m_documentFactory.IsValid()) {
        // Create instance from factory
        return m_documentFactory.CreateInstance();
    }
    return nullptr;
}

IComponentSharedPtr DocumentTemplate::CreateView()
{
    if (m_viewFactory.IsValid()) {
        // Can also get shared pointer
        IComponentUniquePtr viewPtr = m_viewFactory.CreateInstance();
        return IComponentSharedPtr(std::move(viewPtr));
    }
    return nullptr;
}
```

### Factory Configuration

```xml
<AttributeInfo Id="DocumentFactory" Type="Factory">
    <Data IsEnabled="true" Value="TextDocument"/>
</AttributeInfo>
<AttributeInfo Id="ViewFactory" Type="Factory">
    <Data IsEnabled="true" Value="TextEditorView"/>
</AttributeInfo>
```

### Multiple Factories

```cpp
I_BEGIN_COMPONENT(MyComponent);
    I_ASSIGN_MULTI_0(m_processorFactories, "ProcessorFactories", 
                     "Data processor factory list", true);
I_END_COMPONENT;

private:
    I_MULTIFACT(IDataProcessor, m_processorFactories);
```

Usage:

```cpp
void MyComponent::InitializeProcessors()
{
    if (m_processorFactories.IsValid()) {
        int count = m_processorFactories.GetCount();
        for (int i = 0; i < count; ++i) {
            IComponentUniquePtr processor = m_processorFactories.CreateInstance(i);
            // Use processor
        }
    }
}
```

---

## Component Configuration

### Configuration Files (.acc)

Component systems are configured using XML files with `.acc` extension (ACF Component Configuration):

```xml
<?xml version="1.0" encoding="UTF-8"?>
<Acf>
    <AcfHeader>
        <VersionInfos>
            <Version Id="0" Number="4358" Description="ACF"/>
        </VersionInfos>
    </AcfHeader>
    <Description>My Application Configuration</Description>
    <ElementsList>
        <!-- Define your components here -->
        <Element Id="MyComponent" PackageId="MyPackage" ComponentId="MyComponentType">
            <Data IsEnabled="true">
                <AttributeInfoMap>
                    <AttributeInfo Id="Name" Type="String">
                        <Data IsEnabled="true" Value="MyInstance"/>
                    </AttributeInfo>
                </AttributeInfoMap>
            </Data>
        </Element>
    </ElementsList>
</Acf>
```

### Loading Configuration

The framework automatically loads and applies configuration from .acc files. Component developers typically don't need to interact with the registry directly - the framework handles dependency injection based on the configuration.

---

## Composite Components

Composite components are NOT defined in C++ code. Instead, they are created using the **Compositor** visual editing tool and saved as `.acc` files in the `Partitura` directory.

### Understanding Component Packages

**Physical Components (C++)**:
- C++ classes that implement components
- Registered in a **Component Package** (a DLL/shared library)
- Example packages: `BasePck`, `FilePck`, `QtGuiPck`
- Components are statically registered within the package

**Composite Components (Configuration)**:
- XML-based configurations (`.acc` files)
- Describe connections between physical components or other composite components
- Live in the `Partitura` folder and `*Voce` subfolders
- `*Voce` subfolders represent composite component packages (analogous to physical C++ packages)

### Creating Composite Components

Composite components are created and edited using the **Compositor** application:

1. Open Compositor tool
2. Create component diagram visually
3. Connect components and configure attributes
4. Save as `.acc` file to `Partitura` directory

### Example Composite Component Structure

```
Partitura/
├── AcfVoce.arp/              # Composite component package
│   ├── XmlFilePersistence.acc    # Composite component
│   ├── BinaryFilePersistence.acc # Composite component
│   └── ...
└── AcfGuiVoce.arp/           # Another composite package
    ├── MultiDocApp.acc
    └── ...
```

### Accessing Subcomponents in C++

From within a C++ component, you can access subcomponents defined in the registry:

```cpp
void MyComponent::OnComponentCreated()
{
    BaseClass::OnComponentCreated();
    
    // Get component context to access registry
    IComponentContextSharedPtr ctx = GetComponentContext();
    
    // Access would be through the framework's registry system
    // This is typically handled automatically by references (I_REF)
}
```

**Note**: Direct manipulation of composite components is rare in C++ code. The framework handles composition automatically based on `.acc` configuration files.

---

## Component Registration

Physical components (C++ classes) must be registered in a package to be available for instantiation. A package is a DLL/shared library that contains component implementations.

### Package Structure

Looking at `BasePck` (Base Package) as an example:

**BasePck.h** - Package header with component type definitions:
```cpp
#pragma once

#include <icomp/TModelCompWrap.h>
#include <ilog/CLogComp.h>
#include <ibase/CInstantiatorComp.h>
// ... other includes

namespace BasePck
{
    // Type definitions for components in this package
    typedef ilog::CConsoleLogComp ConsoleLog;
    typedef icomp::TModelCompWrap<ilog::CLogComp> Log;
    typedef ibase::CInstantiatorComp Instantiator;
    // ... more component typedefs
}
```

**BasePck.cpp** - Package implementation with component exports:
```cpp
#include "BasePck.h"
#include <icomp/export.h>

namespace BasePck
{

// Export the package itself
I_EXPORT_PACKAGE(
    "Acf/Base",
    "Base system-independent general package",
    IM_PROJECT("ACF") IM_TAG("Basic") IM_COMPANY("ImagingTools"));

// Export individual components
I_EXPORT_COMPONENT(
    ConsoleLog,
    "Simple log on windows console",
    "Windows Message Consumer" IM_CATEGORY(I_SERVICE) IM_TAG("Log"));

I_EXPORT_COMPONENT(
    Log,
    "Data model of the logging component",
    IM_TAG("Log"));

I_EXPORT_COMPONENT(
    Instantiator,
    "Access specified components forcing to create its instances",
    "Instance Create Free Components" IM_CATEGORY(I_SERVICE));

// ... more component exports
}
```

### Key Registration Macros

- **I_EXPORT_PACKAGE**: Registers the package with metadata
  - Package path (e.g., "Acf/Base")
  - Description
  - Project info, tags, company, author

- **I_EXPORT_COMPONENT**: Registers a component in the package
  - Component type name
  - Description
  - Keywords and tags for searchability

### Creating Your Own Package

To create a new component package:

1. Create package header file (e.g., `MyPck.h`)
2. Define component typedefs in package namespace
3. Create package implementation file (e.g., `MyPck.cpp`)
4. Use `I_EXPORT_PACKAGE` to register the package
5. Use `I_EXPORT_COMPONENT` for each component
6. Build as a DLL/shared library

Example:

```cpp
// MyPck.h
#pragma once
#include <icomp/CComponentBase.h>
#include "CMyComponent.h"

namespace MyPck
{
    typedef CMyComponent MyComponent;
}

// MyPck.cpp
#include "MyPck.h"
#include <icomp/export.h>

namespace MyPck
{
    I_EXPORT_PACKAGE(
        "MyCompany/MyPck",
        "My custom component package",
        IM_PROJECT("MyProject") IM_COMPANY("MyCompany"));
    
    I_EXPORT_COMPONENT(
        MyComponent,
        "My custom component for data processing",
        IM_TAG("Processing") IM_TAG("Custom"));
}
```

---

## Complete Examples

### Example 1: Simple Logger Component

```cpp
// ILogger.h - Interface
class ILogger
{
public:
    virtual void LogMessage(const QString& message) = 0;
    virtual void LogError(const QString& error) = 0;
};

// CSimpleLogger.h - Component Header
#include <icomp/CComponentBase.h>
#include "ILogger.h"

class CSimpleLogger: 
    public icomp::CComponentBase,
    public ILogger
{
public:
    typedef icomp::CComponentBase BaseClass;
    
    I_BEGIN_COMPONENT(CSimpleLogger);
        I_REGISTER_INTERFACE(ILogger);
        I_ASSIGN(m_enabledAttr, "Enabled", "Enable logging", true, true);
        I_ASSIGN(m_prefixAttr, "Prefix", "Message prefix", true, QByteArray("[LOG]"));
    I_END_COMPONENT;
    
    CSimpleLogger();
    virtual ~CSimpleLogger();
    
    // ILogger implementation
    virtual void LogMessage(const QString& message) override;
    virtual void LogError(const QString& error) override;
    
protected:
    virtual void OnComponentCreated() override;
    virtual void OnComponentDestroyed() override;
    
private:
    I_ATTR(bool, m_enabledAttr);
    I_ATTR(QByteArray, m_prefixAttr);
};

// CSimpleLogger.cpp - Implementation
CSimpleLogger::CSimpleLogger()
{
    // Constructor - DO NOT access attributes here
}

CSimpleLogger::~CSimpleLogger()
{
}

void CSimpleLogger::OnComponentCreated()
{
    BaseClass::OnComponentCreated();
    
    // NOW attributes are available
    if (*m_enabledAttr) {
        LogMessage("Logger initialized");
    }
}

void CSimpleLogger::OnComponentDestroyed()
{
    if (*m_enabledAttr) {
        LogMessage("Logger shutting down");
    }
    
    BaseClass::OnComponentDestroyed();
}

void CSimpleLogger::LogMessage(const QString& message)
{
    if (*m_enabledAttr) {
        QString prefix = QString::fromUtf8(*m_prefixAttr);
        qDebug() << prefix << message;
    }
}

void CSimpleLogger::LogError(const QString& error)
{
    if (*m_enabledAttr) {
        QString prefix = QString::fromUtf8(*m_prefixAttr);
        qWarning() << prefix << "ERROR:" << error;
    }
}
```

Configuration (.acc file):

```xml
<Element Id="AppLogger" PackageId="LoggingPck" ComponentId="SimpleLogger">
    <Data IsEnabled="true">
        <AttributeInfoMap>
            <AttributeInfo Id="Enabled" Type="Boolean">
                <Data IsEnabled="true" Value="true"/>
            </AttributeInfo>
            <AttributeInfo Id="Prefix" Type="String">
                <Data IsEnabled="true" Value="[MyApp]"/>
            </AttributeInfo>
        </AttributeInfoMap>
    </Data>
</Element>
```

### Example 2: Component with References

```cpp
// CDataProcessor.h
#include <icomp/CComponentBase.h>
#include "ILogger.h"
#include "IDataSource.h"

class CDataProcessor: public icomp::CComponentBase
{
public:
    typedef icomp::CComponentBase BaseClass;
    
    I_BEGIN_COMPONENT(CDataProcessor);
        I_REGISTER_INTERFACE(IDataProcessor);
        
        // Required reference to logger
        I_ASSIGN(m_loggerRef, "Logger", "Logging component", false);
        
        // Required reference to data source
        I_ASSIGN(m_dataSourceRef, "DataSource", "Data source", false);
        
        // Optional reference to backup processor
        I_ASSIGN(m_backupRef, "Backup", "Backup processor", true);
        
        // Attributes
        I_ASSIGN(m_enabledAttr, "Enabled", "Enable processing", true, true);
        I_ASSIGN(m_bufferSizeAttr, "BufferSize", "Buffer size", true, 1024);
    I_END_COMPONENT;
    
    void ProcessData();
    
protected:
    virtual void OnComponentCreated() override;
    
private:
    // References
    I_REF(ILogger, m_loggerRef);
    I_REF(IDataSource, m_dataSourceRef);
    I_REF(IDataProcessor, m_backupRef);
    
    // Attributes
    I_ATTR(bool, m_enabledAttr);
    I_ATTR(int, m_bufferSizeAttr);
};

// CDataProcessor.cpp
void CDataProcessor::OnComponentCreated()
{
    BaseClass::OnComponentCreated();
    
    // Log initialization
    if (m_loggerRef.IsValid()) {
        m_loggerRef->LogMessage("Data processor initialized");
        
        QString msg = QString("Buffer size: %1").arg(*m_bufferSizeAttr);
        m_loggerRef->LogMessage(msg);
    }
}

void CDataProcessor::ProcessData()
{
    if (!*m_enabledAttr) {
        return;
    }
    
    // Verify required references
    if (!m_loggerRef.IsValid() || !m_dataSourceRef.IsValid()) {
        return;
    }
    
    m_loggerRef->LogMessage("Starting data processing");
    
    try {
        // Get data from source
        QByteArray data = m_dataSourceRef->GetData();
        
        // Process data
        // ... processing logic ...
        
        m_loggerRef->LogMessage("Processing completed");
    }
    catch (const std::exception& e) {
        m_loggerRef->LogError(QString("Processing failed: %1").arg(e.what()));
        
        // Try backup processor if available
        if (m_backupRef.IsValid()) {
            m_loggerRef->LogMessage("Attempting backup processing");
            m_backupRef->ProcessData();
        }
    }
}
```

Configuration:

```xml
<Element Id="MainProcessor" PackageId="ProcessingPck" ComponentId="DataProcessor">
    <Data IsEnabled="true">
        <AttributeInfoMap>
            <AttributeInfo Id="Logger" Type="Reference">
                <Data IsEnabled="true" Value="AppLogger"/>
            </AttributeInfo>
            <AttributeInfo Id="DataSource" Type="Reference">
                <Data IsEnabled="true" Value="FileDataSource"/>
            </AttributeInfo>
            <AttributeInfo Id="Backup" Type="Reference">
                <Data IsEnabled="true" Value="BackupProcessor"/>
            </AttributeInfo>
            <AttributeInfo Id="Enabled" Type="Boolean">
                <Data IsEnabled="true" Value="true"/>
            </AttributeInfo>
            <AttributeInfo Id="BufferSize" Type="Integer">
                <Data IsEnabled="true" Value="4096"/>
            </AttributeInfo>
        </AttributeInfoMap>
    </Data>
</Element>
```

### Example 3: Component with Factory

```cpp
// CDocumentManager.h
#include <icomp/CComponentBase.h>

class CDocumentManager: public icomp::CComponentBase
{
public:
    typedef icomp::CComponentBase BaseClass;
    
    I_BEGIN_COMPONENT(CDocumentManager);
        I_REGISTER_INTERFACE(IDocumentManager);
        
        // Factory for creating documents
        I_ASSIGN(m_documentFactory, "DocumentFactory", "Document factory", false);
        
        // Multiple view factories
        I_ASSIGN_MULTI_0(m_viewFactories, "ViewFactories", "View factories", true);
        
        I_ASSIGN(m_loggerRef, "Logger", "Logger", true);
    I_END_COMPONENT;
    
    IDocument* CreateDocument();
    IView* CreateView(int index = 0);
    
private:
    I_FACT(IDocument, m_documentFactory);
    I_MULTIFACT(IView, m_viewFactories);
    I_REF(ILogger, m_loggerRef);
    
    std::vector<IComponentSharedPtr> m_documents;
};

// CDocumentManager.cpp
IDocument* CDocumentManager::CreateDocument()
{
    if (!m_documentFactory.IsValid()) {
        if (m_loggerRef.IsValid()) {
            m_loggerRef->LogError("Document factory not configured");
        }
        return nullptr;
    }
    
    // Create new document instance
    IComponentUniquePtr componentPtr = m_documentFactory.CreateInstance();
    if (!componentPtr) {
        return nullptr;
    }
    
    // Query for IDocument interface
    IDocument* docPtr = dynamic_cast<IDocument*>(componentPtr.get());
    if (!docPtr) {
        return nullptr;
    }
    
    // Store document (transfer ownership to shared_ptr)
    m_documents.push_back(IComponentSharedPtr(std::move(componentPtr)));
    
    if (m_loggerRef.IsValid()) {
        m_loggerRef->LogMessage("Document created");
    }
    
    return docPtr;
}

IView* CDocumentManager::CreateView(int index)
{
    if (!m_viewFactories.IsValid()) {
        return nullptr;
    }
    
    int count = m_viewFactories.GetCount();
    if (index < 0 || index >= count) {
        return nullptr;
    }
    
    // Create view from specific factory
    IComponentUniquePtr componentPtr = m_viewFactories.CreateInstance(index);
    if (!componentPtr) {
        return nullptr;
    }
    
    return dynamic_cast<IView*>(componentPtr.release());
}
```

Configuration:

```xml
<Element Id="DocManager" PackageId="DocumentPck" ComponentId="DocumentManager">
    <Data IsEnabled="true">
        <AttributeInfoMap>
            <AttributeInfo Id="DocumentFactory" Type="Factory">
                <Data IsEnabled="true" Value="TextDocument"/>
            </AttributeInfo>
            <AttributeInfo Id="ViewFactories" Type="MultiFactory">
                <Data IsEnabled="true">
                    <Values>
                        <Value>TextView</Value>
                        <Value>HexView</Value>
                        <Value>PreviewView</Value>
                    </Values>
                </Data>
            </AttributeInfo>
            <AttributeInfo Id="Logger" Type="Reference">
                <Data IsEnabled="true" Value="AppLogger"/>
            </AttributeInfo>
        </AttributeInfoMap>
    </Data>
</Element>
```

### Example 4: Composite Component

```cpp
// CApplication.h
#include <icomp/CCompositeComponent.h>

class CApplication: public icomp::CCompositeComponent
{
public:
    typedef icomp::CCompositeComponent BaseClass;
    
    I_BEGIN_COMPONENT(CApplication);
        I_REGISTER_INTERFACE(IApplication);
    I_END_COMPONENT;
    
    CApplication();
    
    void Run();
    void Shutdown();
    
protected:
    virtual void OnComponentCreated() override;
    virtual void OnComponentDestroyed() override;
    
private:
    ILogger* m_loggerPtr;
    IDocumentManager* m_docManagerPtr;
};

// CApplication.cpp
CApplication::CApplication()
    : m_loggerPtr(nullptr)
    , m_docManagerPtr(nullptr)
{
}

void CApplication::OnComponentCreated()
{
    BaseClass::OnComponentCreated();
    
    // Get subcomponents
    IComponentSharedPtr loggerComp = GetSubcomponent("Logger");
    if (loggerComp) {
        m_loggerPtr = dynamic_cast<ILogger*>(loggerComp.get());
    }
    
    IComponentSharedPtr docMgrComp = GetSubcomponent("DocumentManager");
    if (docMgrComp) {
        m_docManagerPtr = dynamic_cast<IDocumentManager*>(docMgrComp.get());
    }
    
    if (m_loggerPtr) {
        m_loggerPtr->LogMessage("Application initialized");
    }
}

void CApplication::OnComponentDestroyed()
{
    if (m_loggerPtr) {
        m_loggerPtr->LogMessage("Application shutting down");
    }
    
    m_loggerPtr = nullptr;
    m_docManagerPtr = nullptr;
    
    BaseClass::OnComponentDestroyed();
}

void CApplication::Run()
{
    if (m_loggerPtr) {
        m_loggerPtr->LogMessage("Application started");
    }
    
    // Application main loop
    // ...
}

void CApplication::Shutdown()
{
    if (m_loggerPtr) {
        m_loggerPtr->LogMessage("Application shutdown requested");
    }
    
    // Cleanup
}
```

Configuration:

```xml
<Element Id="App" PackageId="ApplicationPck" ComponentId="Application">
    <Data IsEnabled="true">
        <ElementsList>
            <!-- Logger subcomponent -->
            <Element Id="Logger" PackageId="LoggingPck" ComponentId="SimpleLogger">
                <Data IsEnabled="true">
                    <AttributeInfoMap>
                        <AttributeInfo Id="Enabled" Type="Boolean">
                            <Data IsEnabled="true" Value="true"/>
                        </AttributeInfo>
                    </AttributeInfoMap>
                </Data>
            </Element>
            
            <!-- Document Manager subcomponent -->
            <Element Id="DocumentManager" PackageId="DocumentPck" ComponentId="DocumentManager">
                <Data IsEnabled="true">
                    <AttributeInfoMap>
                        <AttributeInfo Id="Logger" Type="Reference">
                            <Data IsEnabled="true" Value="Logger"/>
                        </AttributeInfo>
                        <AttributeInfo Id="DocumentFactory" Type="Factory">
                            <Data IsEnabled="true" Value="TextDocument"/>
                        </AttributeInfo>
                    </AttributeInfoMap>
                </Data>
            </Element>
        </ElementsList>
    </Data>
</Element>
```

---

## Best Practices

### 1. Component Design

- **Single Responsibility**: Each component should have one clear purpose
- **Interface Segregation**: Define focused interfaces, not monolithic ones
- **Dependency Inversion**: Depend on interfaces, not concrete classes
- **Minimal Public API**: Expose only what's necessary through interfaces

### 2. Lifecycle Management

```cpp
// CORRECT: Initialize in OnComponentCreated
void MyComponent::OnComponentCreated()
{
    BaseClass::OnComponentCreated();
    
    // Access attributes
    m_value = *m_valueAttr;
    
    // Connect signals
    if (m_loggerRef.IsValid()) {
        // Setup logger
    }
}

// WRONG: Don't access attributes in constructor
MyComponent::MyComponent()
{
    // This will CRASH - attributes not initialized yet!
    // m_value = *m_valueAttr;  // DON'T DO THIS
}
```

### 3. Reference Validation

```cpp
// CORRECT: Always check references
void MyComponent::DoWork()
{
    if (m_processorRef.IsValid()) {
        m_processorRef->Process();
    } else {
        // Handle missing reference gracefully
    }
}

// WRONG: Assuming reference is valid
void MyComponent::DoWork()
{
    // May crash if reference not configured!
    m_processorRef->Process();
}
```

### 4. Attribute Defaults

```cpp
// CORRECT: Provide sensible defaults
I_BEGIN_COMPONENT(MyComponent);
    I_ASSIGN(m_timeoutAttr, "Timeout", "Timeout in seconds", true, 30);
    I_ASSIGN(m_retriesAttr, "Retries", "Retry count", true, 3);
I_END_COMPONENT;

// WRONG: No defaults for optional attributes
I_BEGIN_COMPONENT(MyComponent);
    // If not specified in config, will be uninitialized!
    I_ASSIGN(m_timeoutAttr, "Timeout", "Timeout", true, 0);
I_END_COMPONENT;
```

### 5. Error Handling

```cpp
void MyComponent::ProcessData()
{
    // Validate preconditions
    if (!m_dataSourceRef.IsValid()) {
        if (m_loggerRef.IsValid()) {
            m_loggerRef->LogError("Data source not available");
        }
        return;
    }
    
    try {
        // Do work
        m_dataSourceRef->GetData();
    }
    catch (const std::exception& e) {
        if (m_loggerRef.IsValid()) {
            m_loggerRef->LogError(
                QString("Processing failed: %1").arg(e.what()));
        }
    }
}
```

### 6. Resource Cleanup

```cpp
void MyComponent::OnComponentDestroyed()
{
    // Disconnect signals
    disconnect(this);
    
    // Release resources
    m_cachedData.clear();
    
    // Clear references to prevent dangling pointers
    m_helperPtr = nullptr;
    
    // Call base class
    BaseClass::OnComponentDestroyed();
}
```

### 7. Thread Safety

```cpp
// References are thread-safe for initialization
void MyComponent::WorkerThread()
{
    // Safe to call from any thread
    if (m_processorRef.IsValid()) {
        m_processorRef->Process();
    }
}

// But component methods may not be thread-safe
class MyComponent: public icomp::CComponentBase
{
private:
    QMutex m_mutex;
    QList<int> m_data;
    
public:
    void AddData(int value)
    {
        QMutexLocker lock(&m_mutex);
        m_data.append(value);
    }
};
```

### 8. Component Naming

```cpp
// GOOD: Descriptive names
"DocumentManager"
"TextEditorView"
"FileSystemDataSource"
"ApplicationLogger"

// BAD: Vague names
"Manager"
"Handler"
"Thing"
"Comp1"
```

### 9. Configuration Organization

```xml
<!-- GOOD: Organized with clear hierarchy -->
<Element Id="Application">
    <ElementsList>
        <!-- Core Services -->
        <Element Id="Logger"/>
        <Element Id="Settings"/>
        
        <!-- Document Management -->
        <Element Id="DocumentManager">
            <AttributeInfoMap>
                <AttributeInfo Id="Logger" Type="Reference" Value="Logger"/>
            </AttributeInfoMap>
        </Element>
    </ElementsList>
</Element>

<!-- BAD: Flat, disorganized -->
<Element Id="Thing1"/>
<Element Id="Thing2"/>
<Element Id="Thing3"/>
```

### 10. Documentation

```cpp
/**
    Document template for text files.
    
    This component creates and manages text documents and their views.
    It provides factories for both documents and views, and handles
    file persistence through a file loader component.
    
    \par Configuration
    - DocumentFactory: Factory for creating document instances
    - ViewFactory: Factory for creating view instances
    - FileLoader: Component for loading/saving files
    
    \par Example
    \code
    CTextDocumentTemplate* template = ...;
    IDocument* doc = template->CreateDocument();
    IView* view = template->CreateView(doc);
    \endcode
*/
class CTextDocumentTemplate: public icomp::CComponentBase
{
    // ...
};
```

---

## Summary

The ACF Component Framework provides:

1. **Modular Architecture**: Build applications from reusable components
2. **Dependency Injection**: Automatic resolution of component dependencies
3. **Configuration**: External configuration via XML files
4. **Type Safety**: Compile-time checking of interfaces and attributes
5. **Flexibility**: Runtime component creation and wiring
6. **Persistence**: Built-in serialization support
7. **Thread Safety**: Thread-safe reference resolution
8. **Introspection**: Static metadata for reflection

### Key Takeaways

- Components implement interfaces for loose coupling
- Use `OnComponentCreated()` for initialization, never the constructor
- Always validate references before use
- Provide sensible defaults for optional attributes
- Clean up resources in `OnComponentDestroyed()`
- Follow the lifecycle rules strictly
- Document component purpose and configuration
- Organize components into packages
- Use registries for component configuration
- Leverage dependency injection for flexibility

### Further Reading

- **icomp API Documentation**: Generated Doxygen documentation
- **Document/View Framework**: `Docs/DocumentViewFrameworkREADME.md`
- **Serialization**: iser library documentation
- **Attributes**: iattr library documentation
- **Examples**: `Docs/Tutorial/` and `Partitura/` directories

---

## Appendix: Macro Reference

### Component Definition Macros

| Macro | Purpose |
|-------|---------|
| `I_BEGIN_COMPONENT(Type)` | Start component definition (instantiable) |
| `I_BEGIN_BASE_COMPONENT(Type)` | Start component definition (abstract) |
| `I_END_COMPONENT` | End component definition |
| `I_REGISTER_INTERFACE(Interface)` | Register supported interface |
| `I_REGISTER_SUBELEMENT(Name)` | Register subelement |

### Attribute Declaration Macros

| Macro | Purpose |
|-------|---------|
| `I_ATTR(Type, member)` | Declare simple attribute |
| `I_TEXTATTR(member)` | Declare translatable text attribute |
| `I_MULTIATTR(Type, member)` | Declare multi-value attribute |
| `I_USER_ATTR(AttrType, member)` | Declare custom attribute type |

### Reference Declaration Macros

| Macro | Purpose |
|-------|---------|
| `I_REF(Interface, member)` | Declare component reference |
| `I_MULTIREF(Interface, member)` | Declare multi-reference |

### Factory Declaration Macros

| Macro | Purpose |
|-------|---------|
| `I_FACT(InterfaceType, member)` | Declare factory attribute |
| `I_MULTIFACT(InterfaceType, member)` | Declare multi-factory attribute |

### Initialization Macros

| Macro | Purpose |
|-------|---------|
| `I_ASSIGN(member, id, desc, opt, def)` | Initialize attribute |
| `I_ASSIGN(member, id, desc, opt)` | Initialize reference (no default needed) |
| `I_ASSIGN(member, id, desc, opt)` | Initialize factory (no default needed) |
| `I_ASSIGN_MULTI_0(member, id, desc, opt)` | Initialize multi-reference/factory (no defaults) |
| `I_ASSIGN_MULTI_1(member, id, desc, opt, def1)` | Initialize multi-attribute with 1 default |

---

*This documentation covers the core functionality of the ACF Component Framework. For the latest API details, refer to the Doxygen-generated documentation.*
