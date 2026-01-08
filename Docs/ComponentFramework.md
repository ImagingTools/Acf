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
- Implement one or more interfaces
- Have a unique ID within their parent
- Can have attributes (configuration parameters)
- Can reference other components
- Can be serialized and deserialized
- Support dependency injection

### Interfaces

Components communicate through **interfaces** (abstract base classes). This enables:
- Loose coupling between components
- Multiple implementations of the same interface
- Runtime polymorphism
- Easy testing with mock implementations

### Registry

The **registry** is a container that holds component configurations. It defines:
- What components exist
- How components are connected (references)
- Component attribute values
- Component addresses (package and component IDs)

Registries are typically loaded from `.acc` (ACF Component Configuration) XML files.

### Context

A **component context** (`IComponentContext`) provides:
- Access to the component's registry information
- Component ID and address
- Parent component reference
- Attribute and reference metadata

### Environment

A **component environment** manages:
- Component creation and lifecycle
- Registry lookup
- Interface resolution
- Shared component instances

---

## Component Architecture

### Core Interfaces

#### IComponent

The base interface for all components:

```cpp
class IComponent: virtual public istd::IPolymorphic
{
public:
    // Get parent component
    virtual const IComponent* GetParentComponent(bool ownerOnly = false) const = 0;
    
    // Get access to an interface
    virtual void* GetInterface(const istd::CClassInfo& interfaceType, 
                               const QByteArray& subId = "") = 0;
    
    // Get component context
    virtual IComponentContextSharedPtr GetComponentContext() const = 0;
    
    // Set component context (for internal use)
    virtual void SetComponentContext(
        const IComponentContextSharedPtr& contextPtr,
        const IComponent* parentPtr,
        bool isParentOwner) = 0;
};
```

#### ICompositeComponent

Interface for components that contain subcomponents:

```cpp
class ICompositeComponent: virtual public IComponent
{
public:
    // Get subcomponent by ID
    virtual IComponentSharedPtr GetSubcomponent(const QByteArray& componentId) const = 0;
    
    // Get subcomponent context
    virtual IComponentContextSharedPtr GetSubcomponentContext(
        const QByteArray& componentId) const = 0;
    
    // Create subcomponent instance
    virtual IComponentUniquePtr CreateSubcomponent(const QByteArray& componentId) const = 0;
    
    // Notification when subcomponent is deleted
    virtual void OnSubcomponentDeleted(const IComponent* subcomponentPtr) = 0;
};
```

#### IRegistry

Interface for component registry:

```cpp
class IRegistry: virtual public iser::ISerializable
{
public:
    // Get list of all component IDs
    virtual Ids GetElementIds() const = 0;
    
    // Get component info
    virtual const ElementInfo* GetElementInfo(const QByteArray& elementId) const = 0;
    
    // Insert component info
    virtual ElementInfo* InsertElementInfo(
        const QByteArray& elementId,
        const CComponentAddress& address,
        bool ensureElementCreated = true) = 0;
    
    // Map of exported interfaces
    virtual const ExportedInterfacesMap& GetExportedInterfacesMap() const = 0;
};
```

### Base Implementation Classes

#### CComponentBase

The standard base class for component implementations:

```cpp
class CComponentBase: virtual public IComponent
{
protected:
    // Check if component is active
    bool IsComponentActive() const;
    
    // Lifecycle hooks
    virtual void OnComponentCreated();
    virtual void OnComponentDestroyed();
    
    // Abstract: provide static info
    virtual const IRealComponentStaticInfo& GetComponentStaticInfo() const = 0;
};
```

---

## Component Lifecycle

Components go through a well-defined lifecycle:

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
    // I_ASSIGN(member, attrId, description, isOptional, defaultValue)
    I_ASSIGN(m_enabledAttr, "Enabled", "Enable this feature", false, true);
    I_ASSIGN(m_countAttr, "Count", "Number of items", true, 10);
    I_ASSIGN(m_scaleAttr, "Scale", "Scaling factor", true, 1.0);
    I_ASSIGN(m_nameAttr, "Name", "Component name", false, QByteArray("default"));
I_END_COMPONENT;
```

Parameters:
- **member**: The attribute member variable
- **attrId**: Attribute ID used in configuration files
- **description**: Human-readable description
- **isOptional**: `true` if attribute can be omitted (nullable)
- **defaultValue**: Default value if not specified in configuration

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
class MyComponent: public icomp::CComponentBase
{
public:
    typedef icomp::CComponentBase BaseClass;
    
    I_BEGIN_COMPONENT(MyComponent);
        I_REGISTER_INTERFACE(IMyInterface);
        
        // Single reference
        I_ASSIGN(m_loggerRef, "Logger", "Logging component", false);
        
        // Optional reference
        I_ASSIGN(m_helpProviderRef, "HelpProvider", "Help provider", true);
    I_END_COMPONENT;
    
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
    I_FACT(m_documentFactory);
    I_FACT(m_viewFactory);
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
    I_ASSIGN_MULTI_0(m_pluginFactories, "PluginFactories", 
                     "Plugin factory list", true);
I_END_COMPONENT;

private:
    I_MULTIFACT(m_pluginFactories);
```

Usage:

```cpp
void MyComponent::LoadPlugins()
{
    if (m_pluginFactories.IsValid()) {
        int count = m_pluginFactories.GetCount();
        for (int i = 0; i < count; ++i) {
            IComponentUniquePtr plugin = m_pluginFactories.CreateInstance(i);
            // Use plugin
        }
    }
}
```

---

## Component Registry

The registry stores component configurations and enables dependency injection.

### Registry Structure

```cpp
// Registry contains elements (components)
IRegistry::Ids ids = registry->GetElementIds();

// Each element has:
struct ElementInfo {
    IRegistryElement* elementPtr;       // Registry element
    CComponentAddress address;          // Package + Component ID
};
```

### Component Address

Identifies component type:

```cpp
class CComponentAddress
{
public:
    QByteArray GetPackageId() const;    // e.g., "LoggingPck"
    QByteArray GetComponentId() const;  // e.g., "ConsoleLogger"
};
```

### Accessing Registry

From within a component:

```cpp
void MyComponent::OnComponentCreated()
{
    BaseClass::OnComponentCreated();
    
    // Get component context
    IComponentContextSharedPtr ctx = GetComponentContext();
    if (!ctx) return;
    
    // Get registry
    IRegistry* registry = ctx->GetRegistry();
    if (!registry) return;
    
    // Enumerate all components in registry
    IRegistry::Ids ids = registry->GetElementIds();
    for (const QByteArray& id : ids) {
        const IRegistry::ElementInfo* info = registry->GetElementInfo(id);
        // Process component info
    }
}
```

### Registry Serialization

Registries can be loaded from and saved to files:

```cpp
// Load registry from file
iser::CXmlFileReadArchive readArchive("config.acc");
IRegistryUniquePtr registry(new CRegistry());
registry->Serialize(readArchive);

// Save registry to file
iser::CXmlFileWriteArchive writeArchive("config.acc");
registry->Serialize(writeArchive);
```

---

## Composite Components

Composite components contain and manage subcomponents.

### Defining Composite Components

```cpp
#include <icomp/CCompositeComponent.h>

class MyContainer: public icomp::CCompositeComponent
{
public:
    typedef icomp::CCompositeComponent BaseClass;
    
    I_BEGIN_COMPONENT(MyContainer);
        I_REGISTER_INTERFACE(IContainer);
    I_END_COMPONENT;
    
    MyContainer();
    
protected:
    virtual void OnComponentCreated() override;
};
```

### Accessing Subcomponents

```cpp
void MyContainer::OnComponentCreated()
{
    BaseClass::OnComponentCreated();
    
    // Get subcomponent by ID
    IComponentSharedPtr subcomp = GetSubcomponent("Processor1");
    if (subcomp) {
        // Use subcomponent
    }
    
    // Create new instance of subcomponent
    IComponentUniquePtr instance = CreateSubcomponent("Processor2");
    
    // Get subcomponent context without creating instance
    IComponentContextSharedPtr ctx = GetSubcomponentContext("Processor3");
}
```

### Registry Configuration

```xml
<Element Id="Container" PackageId="MyPck" ComponentId="MyContainer">
    <Data IsEnabled="true">
        <ElementsList>
            <!-- Subcomponent 1 -->
            <Element Id="Processor1" PackageId="ProcessingPck" ComponentId="DataProcessor">
                <Data IsEnabled="true"/>
            </Element>
            
            <!-- Subcomponent 2 -->
            <Element Id="Processor2" PackageId="ProcessingPck" ComponentId="DataProcessor">
                <Data IsEnabled="true"/>
            </Element>
        </ElementsList>
    </Data>
</Element>
```

### Subcomponent Lifecycle

Parent components are notified when subcomponents are deleted:

```cpp
void MyContainer::OnSubcomponentDeleted(const IComponent* subcomponentPtr)
{
    // Clean up references to deleted subcomponent
    // Release resources
}
```

---

## Static Information and Metadata

Static info provides compile-time reflection for components.

### Component Static Info

```cpp
class IRealComponentStaticInfo
{
public:
    // Component identification
    virtual QByteArray GetComponentId() const = 0;
    virtual QByteArray GetPackageId() const = 0;
    
    // Description
    virtual QString GetDescription() const = 0;
    virtual QString GetKeywords() const = 0;
    
    // Interfaces
    virtual bool IsInterfaceSupported(const istd::CClassInfo& interfaceType) const = 0;
    
    // Attributes
    virtual const IAttributeStaticInfo* GetAttributeStaticInfo(
        const QByteArray& attributeId) const = 0;
        
    // Factory
    virtual IComponent* CreateInstance() const = 0;
};
```

### Accessing Static Info

```cpp
void MyComponent::OnComponentCreated()
{
    BaseClass::OnComponentCreated();
    
    // Get static info for this component
    const IRealComponentStaticInfo& staticInfo = GetComponentStaticInfo();
    
    // Get metadata
    QByteArray componentId = staticInfo.GetComponentId();
    QString description = staticInfo.GetDescription();
    
    // Check interface support
    bool supportsLogger = staticInfo.IsInterfaceSupported(
        istd::CClassInfo::Get<ilog::ILogger>());
}
```

### Attribute Static Info

```cpp
class IAttributeStaticInfo
{
public:
    virtual QByteArray GetAttributeId() const = 0;
    virtual QByteArray GetAttributeType() const = 0;
    virtual QString GetDescription() const = 0;
    virtual int GetFlags() const = 0;
};
```

---

## Component Registration

Components must be registered to be available for instantiation.

### Package Definition

```cpp
// MyPackage.h
#include <icomp/CPackageStaticInfo.h>

namespace mypkg
{
    // Declare package
    extern icomp::CPackageStaticInfo& GetMyPackageStaticInfo();
}
```

### Component Registration

```cpp
// MyPackage.cpp
#include "MyPackage.h"
#include "MyComponent.h"
#include "AnotherComponent.h"

namespace mypkg
{

// Define package
icomp::CPackageStaticInfo& GetMyPackageStaticInfo()
{
    static icomp::CPackageStaticInfo packageInfo(
        "MyPackage",              // Package ID
        "My Package",             // Package name
        "Custom components");     // Description
    return packageInfo;
}

// Register components
static icomp::TComponentRegistrator<MyComponent> s_myComponentReg(
    "MyComponent",                          // Component ID
    GetMyPackageStaticInfo(),              // Package
    "My custom component",                 // Description
    "Custom Processing");                  // Keywords

static icomp::TComponentRegistrator<AnotherComponent> s_anotherComponentReg(
    "AnotherComponent",
    GetMyPackageStaticInfo(),
    "Another component",
    "Custom Helper");

} // namespace mypkg
```

### Package Initialization

Register package with the system:

```cpp
#include <ipackage/IPackagesManager.h>

void InitializePackages()
{
    IPackagesManager* manager = GetPackagesManager();
    
    // Register package
    manager->RegisterPackage(&mypkg::GetMyPackageStaticInfo());
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
    I_FACT(m_documentFactory);
    I_MULTIFACT(m_viewFactories);
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

## Advanced Topics

### Subelement Registration

For components with identifiable subelements:

```cpp
class CFormComponent: public icomp::CComponentBase
{
public:
    I_BEGIN_COMPONENT(CFormComponent);
        // Register subelements
        I_REGISTER_SUBELEMENT(NameField);
        I_REGISTER_SUBELEMENT(EmailField);
        I_REGISTER_SUBELEMENT(SubmitButton);
        
        // Register interfaces for subelements
        I_REGISTER_SUBELEMENT_INTERFACE(NameField, ITextField, GetNameField);
        I_REGISTER_SUBELEMENT_INTERFACE(EmailField, ITextField, GetEmailField);
        I_REGISTER_SUBELEMENT_INTERFACE(SubmitButton, IButton, GetSubmitButton);
    I_END_COMPONENT;
    
private:
    static ITextField* GetNameField(CFormComponent& form) {
        return &form.m_nameField;
    }
    
    CTextField m_nameField;
    CTextField m_emailField;
    CButton m_submitButton;
};
```

### Custom Attributes

```cpp
// Define custom attribute type
class CColorAttribute: public iattr::TAttribute<QColor>
{
public:
    CColorAttribute() : TAttribute(QColor(Qt::white)) {}
    explicit CColorAttribute(const QColor& color) : TAttribute(color) {}
    
    // Override serialization if needed
    virtual void Serialize(iser::IArchive& archive) override;
    
    static QByteArray GetTypeName() { return "Color"; }
};

// Use in component
class MyComponent: public icomp::CComponentBase
{
    I_BEGIN_COMPONENT(MyComponent);
        I_ASSIGN(m_colorAttr, "Color", "Background color", true, QColor(Qt::white));
    I_END_COMPONENT;
    
private:
    I_USER_ATTR(CColorAttribute, m_colorAttr);
};
```

### Environment Managers

For advanced component creation control:

```cpp
class IComponentEnvironmentManager
{
public:
    virtual IComponentSharedPtr GetComponent(
        const QByteArray& componentId) = 0;
        
    virtual IComponentUniquePtr CreateComponent(
        const CComponentAddress& address) = 0;
};
```

### Package Dependencies

```cpp
namespace mypkg
{

icomp::CPackageStaticInfo& GetMyPackageStaticInfo()
{
    static icomp::CPackageStaticInfo packageInfo(
        "MyPackage",
        "My Package",
        "Custom components");
    
    // Register dependencies
    static bool initialized = false;
    if (!initialized) {
        packageInfo.RegisterDependency("LoggingPck");
        packageInfo.RegisterDependency("DocumentPck");
        initialized = true;
    }
    
    return packageInfo;
}

} // namespace mypkg
```

### Component Wrappers

For lifecycle control:

```cpp
// TComponentWrap ensures proper OnComponentCreated/OnComponentDestroyed calls
template <class Component>
class TComponentWrap: public Component
{
public:
    virtual void SetComponentContext(
        const IComponentContextSharedPtr& contextPtr,
        const IComponent* parentPtr,
        bool isParentOwner) override;
};

// Usage
typedef icomp::TComponentWrap<MyComponent> MyComponentWrap;
```

### Meta-Information

```cpp
void MyComponent::DumpMetaInfo()
{
    const IRealComponentStaticInfo& staticInfo = GetComponentStaticInfo();
    
    qDebug() << "Component:" << staticInfo.GetComponentId();
    qDebug() << "Package:" << staticInfo.GetPackageId();
    qDebug() << "Description:" << staticInfo.GetDescription();
    qDebug() << "Keywords:" << staticInfo.GetKeywords();
    
    // List supported interfaces
    if (staticInfo.IsInterfaceSupported(
            istd::CClassInfo::Get<ILogger>())) {
        qDebug() << "Supports ILogger";
    }
}
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
| `I_FACT(member)` | Declare factory attribute |
| `I_MULTIFACT(member)` | Declare multi-factory attribute |

### Initialization Macros

| Macro | Purpose |
|-------|---------|
| `I_ASSIGN(member, id, desc, opt, def)` | Initialize attribute |
| `I_ASSIGN(member, id, desc, opt)` | Initialize reference (no default needed) |
| `I_ASSIGN(member, id, desc, opt)` | Initialize factory (no default needed) |
| `I_ASSIGN_MULTI_0(member, id, opt)` | Initialize multi-reference/factory (no defaults) |
| `I_ASSIGN_MULTI_1(member, id, opt, def1)` | Initialize multi-attribute with 1 default |

---

*This documentation covers the core functionality of the ACF Component Framework. For the latest API details, refer to the Doxygen-generated documentation.*
