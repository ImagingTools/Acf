# ACF Component Framework Examples

This document provides additional practical examples for working with the ACF Component Framework. For comprehensive documentation, see [ComponentFramework.md](ComponentFramework.md).

## Table of Contents

1. [Data Processing Pipeline with Multiple Processors](#data-processing-pipeline-with-multiple-processors)
2. [Service Locator Pattern](#service-locator-pattern)
3. [Observer Pattern with Components](#observer-pattern-with-components)
4. [Configuration System](#configuration-system)
5. [Multi-Threaded Processing Pipeline](#multi-threaded-processing-pipeline)
6. [Component Testing Patterns](#component-testing-patterns)

---

## Data Processing Pipeline with Multiple Processors

This example shows how to build a flexible data processing system using component composition. The system manager coordinates multiple processor components without treating them as "plugins" - they're simply components in a composition.

### Processor Interface

```cpp
// IDataProcessor.h
class IDataProcessor
{
public:
    virtual ~IDataProcessor() = default;
    
    virtual QString GetProcessorName() const = 0;
    virtual QString GetProcessorVersion() const = 0;
    virtual bool Initialize() = 0;
    virtual void Shutdown() = 0;
    virtual bool Process(const QVariant& data) = 0;
};
```

### Processing Coordinator Component

```cpp
// CProcessingCoordinator.h
#include <icomp/CComponentBase.h>
#include "IDataProcessor.h"

class CProcessingCoordinator: public icomp::CComponentBase
{
public:
    typedef icomp::CComponentBase BaseClass;
    
    I_BEGIN_COMPONENT(CProcessingCoordinator);
        I_REGISTER_INTERFACE(IProcessingCoordinator);
        
        // Multiple processor factories
        I_ASSIGN_MULTI_0(m_processorFactories, "Processors", 
                        "List of data processor factories", true);
        
        // Logger reference
        I_ASSIGN(m_loggerRef, "Logger", "Logger component", true);
    I_END_COMPONENT;
    
    CProcessingCoordinator();
    virtual ~CProcessingCoordinator();
    
    // Processor management
    bool InitializeProcessors();
    void ShutdownProcessors();
    bool ProcessData(const QVariant& data);
    
    int GetProcessorCount() const;
    IDataProcessor* GetProcessor(int index) const;
    IDataProcessor* FindProcessor(const QString& name) const;
    
protected:
    virtual void OnComponentCreated() override;
    virtual void OnComponentDestroyed() override;
    
private:
    struct ProcessorInfo {
        IComponentSharedPtr componentPtr;
        IDataProcessor* processorPtr;
        QString name;
        bool initialized;
    };
    
    I_MULTIFACT(IDataProcessor, m_processorFactories);
    I_REF(ILogger, m_loggerRef);
    
    std::vector<ProcessorInfo> m_processors;
};

// CProcessingCoordinator.cpp
CProcessingCoordinator::CProcessingCoordinator()
{
}

CProcessingCoordinator::~CProcessingCoordinator()
{
    ShutdownProcessors();
}

void CProcessingCoordinator::OnComponentCreated()
{
    BaseClass::OnComponentCreated();
    
    // Auto-initialize processors
    InitializeProcessors();
}

void CProcessingCoordinator::OnComponentDestroyed()
{
    ShutdownProcessors();
    BaseClass::OnComponentDestroyed();
}

bool CProcessingCoordinator::InitializeProcessors()
{
    if (!m_processorFactories.IsValid()) {
        if (m_loggerRef.IsValid()) {
            m_loggerRef->LogMessage("No processors configured");
        }
        return true;
    }
    
    int count = m_processorFactories.GetCount();
    
    if (m_loggerRef.IsValid()) {
        m_loggerRef->LogMessage(QString("Initializing %1 processor(s)...").arg(count));
    }
    
    for (int i = 0; i < count; ++i) {
        // Create processor instance
        IComponentUniquePtr componentPtr = m_processorFactories.CreateInstance(i);
        if (!componentPtr) {
            if (m_loggerRef.IsValid()) {
                m_loggerRef->LogError(QString("Failed to create processor %1").arg(i));
            }
            continue;
        }
        
        // Query for IDataProcessor interface
        IDataProcessor* processorPtr = dynamic_cast<IDataProcessor*>(componentPtr.get());
        if (!processorPtr) {
            if (m_loggerRef.IsValid()) {
                m_loggerRef->LogError(
                    QString("Component %1 doesn't implement IDataProcessor").arg(i));
            }
            continue;
        }
        
        // Store processor info
        ProcessorInfo info;
        info.componentPtr = IComponentSharedPtr(std::move(componentPtr));
        info.processorPtr = processorPtr;
        info.name = processorPtr->GetProcessorName();
        info.initialized = false;
        
        // Initialize processor
        if (processorPtr->Initialize()) {
            info.initialized = true;
            m_processors.push_back(info);
            
            if (m_loggerRef.IsValid()) {
                m_loggerRef->LogMessage(
                    QString("Initialized processor: %1 v%2")
                        .arg(info.name)
                        .arg(processorPtr->GetProcessorVersion()));
            }
        } else {
            if (m_loggerRef.IsValid()) {
                m_loggerRef->LogError(
                    QString("Failed to initialize processor: %1").arg(info.name));
            }
        }
    }
    
    return !m_processors.empty();
}

void CProcessingCoordinator::ShutdownProcessors()
{
    if (m_loggerRef.IsValid()) {
        m_loggerRef->LogMessage("Shutting down processors...");
    }
    
    // Shutdown in reverse order
    for (auto it = m_processors.rbegin(); it != m_processors.rend(); ++it) {
        if (it->initialized && it->processorPtr) {
            it->processorPtr->Shutdown();
            
            if (m_loggerRef.IsValid()) {
                m_loggerRef->LogMessage(
                    QString("Shutdown processor: %1").arg(it->name));
            }
        }
    }
    
    m_processors.clear();
}

bool CProcessingCoordinator::ProcessData(const QVariant& data)
{
    bool allSuccess = true;
    
    for (const auto& info : m_processors) {
        if (info.initialized && info.processorPtr) {
            try {
                if (!info.processorPtr->Process(data)) {
                    allSuccess = false;
                    if (m_loggerRef.IsValid()) {
                        m_loggerRef->LogError(
                            QString("Processor %1 failed").arg(info.name));
                    }
                }
            }
            catch (const std::exception& e) {
                allSuccess = false;
                if (m_loggerRef.IsValid()) {
                    m_loggerRef->LogError(
                        QString("Processor %1 threw exception: %2")
                            .arg(info.name)
                            .arg(e.what()));
                }
            }
        }
    }
    
    return allSuccess;
}

int CProcessingCoordinator::GetProcessorCount() const
{
    return static_cast<int>(m_processors.size());
}

IDataProcessor* CProcessingCoordinator::GetProcessor(int index) const
{
    if (index >= 0 && index < static_cast<int>(m_processors.size())) {
        return m_processors[index].processorPtr;
    }
    return nullptr;
}

IDataProcessor* CProcessingCoordinator::FindProcessor(const QString& name) const
{
    for (const auto& info : m_processors) {
        if (info.name == name) {
            return info.processorPtr;
        }
    }
    return nullptr;
}
```

### Example Processor Implementation

```cpp
// CImageFilterProcessor.h
#include <icomp/CComponentBase.h>
#include "IDataProcessor.h"

class CImageFilterProcessor: 
    public icomp::CComponentBase,
    public IDataProcessor
{
public:
    typedef icomp::CComponentBase BaseClass;
    
    I_BEGIN_COMPONENT(CImageFilterProcessor);
        I_REGISTER_INTERFACE(IDataProcessor);
        
        I_ASSIGN(m_nameAttr, "ProcessorName", "Processor name", 
                 true, QByteArray("ImageFilter"));
        I_ASSIGN(m_versionAttr, "ProcessorVersion", "Processor version",
                 true, QByteArray("1.0.0"));
        I_ASSIGN(m_enabledAttr, "Enabled", "Enable processor", 
                 true, true);
    I_END_COMPONENT;
    
    // IDataProcessor implementation
    virtual QString GetProcessorName() const override;
    virtual QString GetProcessorVersion() const override;
    virtual bool Initialize() override;
    virtual void Shutdown() override;
    virtual bool Process(const QVariant& data) override;
    
private:
    I_ATTR(QByteArray, m_nameAttr);
    I_ATTR(QByteArray, m_versionAttr);
    I_ATTR(bool, m_enabledAttr);
};

// CImageFilterProcessor.cpp
QString CImageFilterProcessor::GetProcessorName() const
{
    return QString::fromUtf8(*m_nameAttr);
}

QString CImageFilterProcessor::GetProcessorVersion() const
{
    return QString::fromUtf8(*m_versionAttr);
}

bool CImageFilterProcessor::Initialize()
{
    qDebug() << "Initializing" << GetProcessorName();
    return true;
}

void CImageFilterProcessor::Shutdown()
{
    qDebug() << "Shutting down" << GetProcessorName();
}

bool CImageFilterProcessor::Process(const QVariant& data)
{
    if (*m_enabledAttr) {
        qDebug() << "Processing data with" << GetProcessorName();
        // Perform image filtering...
        return true;
    }
    return false;
}
```

### Configuration

```xml
<Element Id="ProcessingSystem" PackageId="ProcessingPck" ComponentId="ProcessingCoordinator">
    <Data IsEnabled="true">
        <AttributeInfoMap>
            <AttributeInfo Id="Processors" Type="MultiFactory">
                <Data IsEnabled="true">
                    <Values>
                        <Value>ImageFilterProcessor</Value>
                        <Value>DataValidatorProcessor</Value>
                        <Value>ResultAggregatorProcessor</Value>
                    </Values>
                </Data>
            </AttributeInfo>
            <AttributeInfo Id="Logger" Type="Reference">
                <Data IsEnabled="true" Value="SystemLogger"/>
            </AttributeInfo>
        </AttributeInfoMap>
    </Data>
</Element>
```

**Key Point**: This demonstrates component composition where all processors are equal components coordinated by another component. There's no "host application with plugins" - it's a composition of components where each has a specific role.

---

## Service Locator Pattern

Implement a service locator for global service access:

```cpp
// CServiceLocator.h
#include <icomp/CCompositeComponent.h>
#include <QMap>

class CServiceLocator: public icomp::CCompositeComponent
{
public:
    typedef icomp::CCompositeComponent BaseClass;
    
    I_BEGIN_COMPONENT(CServiceLocator);
        I_REGISTER_INTERFACE(IServiceLocator);
    I_END_COMPONENT;
    
    // Service registration
    template<typename Interface>
    bool RegisterService(const QByteArray& serviceId);
    
    // Service lookup
    template<typename Interface>
    Interface* GetService(const QByteArray& serviceId);
    
    template<typename Interface>
    Interface* GetRequiredService(const QByteArray& serviceId);
    
protected:
    virtual void OnComponentCreated() override;
    
private:
    struct ServiceInfo {
        IComponentSharedPtr componentPtr;
        void* interfacePtr;
        QByteArray interfaceType;
    };
    
    QMap<QByteArray, ServiceInfo> m_services;
    QMutex m_servicesMutex;
};

// Template implementations
template<typename Interface>
bool CServiceLocator::RegisterService(const QByteArray& serviceId)
{
    QMutexLocker lock(&m_servicesMutex);
    
    // Get subcomponent
    IComponentSharedPtr comp = GetSubcomponent(serviceId);
    if (!comp) {
        return false;
    }
    
    // Query interface
    Interface* iface = dynamic_cast<Interface*>(comp.get());
    if (!iface) {
        return false;
    }
    
    // Store service
    ServiceInfo info;
    info.componentPtr = comp;
    info.interfacePtr = iface;
    info.interfaceType = typeid(Interface).name();
    
    m_services[serviceId] = info;
    return true;
}

template<typename Interface>
Interface* CServiceLocator::GetService(const QByteArray& serviceId)
{
    QMutexLocker lock(&m_servicesMutex);
    
    auto it = m_services.find(serviceId);
    if (it == m_services.end()) {
        // Try to register on demand
        lock.unlock();
        if (!RegisterService<Interface>(serviceId)) {
            return nullptr;
        }
        lock.relock();
        it = m_services.find(serviceId);
    }
    
    if (it != m_services.end()) {
        return static_cast<Interface*>(it->interfacePtr);
    }
    
    return nullptr;
}

template<typename Interface>
Interface* CServiceLocator::GetRequiredService(const QByteArray& serviceId)
{
    Interface* service = GetService<Interface>(serviceId);
    if (!service) {
        throw std::runtime_error(
            QString("Required service not found: %1")
                .arg(QString::fromUtf8(serviceId))
                .toStdString());
    }
    return service;
}

// Usage
void MyComponent::OnComponentCreated()
{
    BaseClass::OnComponentCreated();
    
    // Get service locator
    IServiceLocator* locator = GetServiceLocator();
    
    // Get services
    ILogger* logger = locator->GetService<ILogger>("Logger");
    IConfig* config = locator->GetRequiredService<IConfig>("Configuration");
}
```

---

## Observer Pattern with Components

Implement the observer pattern using component references:

```cpp
// IObserver.h
class IObserver
{
public:
    virtual void OnNotification(const QString& event, const QVariant& data) = 0;
};

// CObservableComponent.h
class CObservableComponent: public icomp::CComponentBase
{
public:
    typedef icomp::CComponentBase BaseClass;
    
    I_BEGIN_COMPONENT(CObservableComponent);
        I_REGISTER_INTERFACE(IObservable);
        
        // Multiple observer references
        I_ASSIGN_MULTI_0(m_observersRef, "Observers", 
                        "List of observers", true);
    I_END_COMPONENT;
    
protected:
    void NotifyObservers(const QString& event, const QVariant& data);
    
    virtual void OnComponentCreated() override;
    
private:
    I_MULTIREF(IObserver, m_observersRef);
};

// CObservableComponent.cpp
void CObservableComponent::OnComponentCreated()
{
    BaseClass::OnComponentCreated();
    
    // Notify observers that component is ready
    NotifyObservers("ComponentCreated", QVariant());
}

void CObservableComponent::NotifyObservers(const QString& event, 
                                          const QVariant& data)
{
    if (!m_observersRef.IsValid()) {
        return;
    }
    
    int count = m_observersRef.GetCount();
    for (int i = 0; i < count; ++i) {
        IObserver* observer = m_observersRef.GetPtr(i);
        if (observer) {
            try {
                observer->OnNotification(event, data);
            }
            catch (const std::exception& e) {
                qWarning() << "Observer notification failed:" << e.what();
            }
        }
    }
}

// CDataModel.h - Observable data model
class CDataModel: public CObservableComponent
{
public:
    typedef CObservableComponent BaseClass;
    
    I_BEGIN_COMPONENT(CDataModel);
        I_REGISTER_INTERFACE(IDataModel);
    I_END_COMPONENT;
    
    void SetData(const QString& key, const QVariant& value);
    QVariant GetData(const QString& key) const;
    
private:
    QMap<QString, QVariant> m_data;
    mutable QMutex m_dataMutex;
};

void CDataModel::SetData(const QString& key, const QVariant& value)
{
    {
        QMutexLocker lock(&m_dataMutex);
        m_data[key] = value;
    }
    
    // Notify observers
    QVariantMap eventData;
    eventData["key"] = key;
    eventData["value"] = value;
    NotifyObservers("DataChanged", eventData);
}

// CDataObserver.h - Observer implementation
class CDataObserver: 
    public icomp::CComponentBase,
    public IObserver
{
public:
    typedef icomp::CComponentBase BaseClass;
    
    I_BEGIN_COMPONENT(CDataObserver);
        I_REGISTER_INTERFACE(IObserver);
        
        I_ASSIGN(m_nameAttr, "Name", "Observer name", 
                 false, QByteArray("Observer"));
    I_END_COMPONENT;
    
    // IObserver implementation
    virtual void OnNotification(const QString& event, 
                               const QVariant& data) override;
    
private:
    I_ATTR(QByteArray, m_nameAttr);
};

void CDataObserver::OnNotification(const QString& event, 
                                   const QVariant& data)
{
    QString name = QString::fromUtf8(*m_nameAttr);
    qDebug() << name << "received event:" << event;
    
    if (event == "DataChanged") {
        QVariantMap eventData = data.toMap();
        QString key = eventData["key"].toString();
        QVariant value = eventData["value"];
        qDebug() << "  Data changed:" << key << "=" << value;
    }
}
```

Configuration:

```xml
<Element Id="DataModel" PackageId="DataPck" ComponentId="DataModel">
    <Data IsEnabled="true">
        <AttributeInfoMap>
            <AttributeInfo Id="Observers" Type="MultiReference">
                <Data IsEnabled="true">
                    <Values>
                        <Value>Observer1</Value>
                        <Value>Observer2</Value>
                        <Value>Observer3</Value>
                    </Values>
                </Data>
            </AttributeInfo>
        </AttributeInfoMap>
    </Data>
</Element>

<Element Id="Observer1" PackageId="DataPck" ComponentId="DataObserver">
    <Data IsEnabled="true">
        <AttributeInfoMap>
            <AttributeInfo Id="Name" Type="String" Value="LoggingObserver"/>
        </AttributeInfoMap>
    </Data>
</Element>
```

---

## Configuration System

Build a hierarchical configuration system:

```cpp
// IConfiguration.h
class IConfiguration
{
public:
    virtual QVariant GetValue(const QString& key, 
                             const QVariant& defaultValue = QVariant()) const = 0;
    virtual void SetValue(const QString& key, const QVariant& value) = 0;
    virtual bool HasValue(const QString& key) const = 0;
    virtual QStringList GetKeys() const = 0;
};

// CConfigurationComponent.h
#include <icomp/CCompositeComponent.h>

class CConfigurationComponent: 
    public icomp::CCompositeComponent,
    public IConfiguration
{
public:
    typedef icomp::CCompositeComponent BaseClass;
    
    I_BEGIN_COMPONENT(CConfigurationComponent);
        I_REGISTER_INTERFACE(IConfiguration);
        
        // Optional parent configuration
        I_ASSIGN(m_parentConfigRef, "ParentConfig",
                 "Parent configuration for inheritance", true);
        
        // Persistence
        I_ASSIGN(m_persistenceRef, "Persistence",
                 "Configuration persistence handler", true);
        
        I_ASSIGN(m_autoSaveAttr, "AutoSave", 
                "Auto-save on changes", true, false);
    I_END_COMPONENT;
    
    // IConfiguration implementation
    virtual QVariant GetValue(const QString& key, 
                             const QVariant& defaultValue = QVariant()) const override;
    virtual void SetValue(const QString& key, const QVariant& value) override;
    virtual bool HasValue(const QString& key) const override;
    virtual QStringList GetKeys() const override;
    
    // Additional methods
    void Load();
    void Save();
    void Clear();
    
    // Typed getters
    template<typename T>
    T GetValueAs(const QString& key, const T& defaultValue) const
    {
        QVariant value = GetValue(key);
        if (value.isValid() && value.canConvert<T>()) {
            return value.value<T>();
        }
        return defaultValue;
    }
    
protected:
    virtual void OnComponentCreated() override;
    virtual void OnComponentDestroyed() override;
    
private:
    I_REF(IConfiguration, m_parentConfigRef);
    I_REF(IPersistence, m_persistenceRef);
    I_ATTR(bool, m_autoSaveAttr);
    
    QMap<QString, QVariant> m_values;
    mutable QMutex m_valuesMutex;
    bool m_modified;
};

// Implementation
void CConfigurationComponent::OnComponentCreated()
{
    BaseClass::OnComponentCreated();
    
    m_modified = false;
    
    // Load configuration
    Load();
}

void CConfigurationComponent::OnComponentDestroyed()
{
    // Auto-save if enabled
    if (*m_autoSaveAttr && m_modified) {
        Save();
    }
    
    BaseClass::OnComponentDestroyed();
}

QVariant CConfigurationComponent::GetValue(const QString& key, 
                                          const QVariant& defaultValue) const
{
    QMutexLocker lock(&m_valuesMutex);
    
    // Check local values first
    auto it = m_values.find(key);
    if (it != m_values.end()) {
        return it.value();
    }
    
    // Check parent configuration
    if (m_parentConfigRef.IsValid()) {
        lock.unlock(); // Release lock before calling parent
        return m_parentConfigRef->GetValue(key, defaultValue);
    }
    
    return defaultValue;
}

void CConfigurationComponent::SetValue(const QString& key, const QVariant& value)
{
    {
        QMutexLocker lock(&m_valuesMutex);
        m_values[key] = value;
        m_modified = true;
    }
    
    // Auto-save if enabled
    if (*m_autoSaveAttr) {
        Save();
    }
}

bool CConfigurationComponent::HasValue(const QString& key) const
{
    QMutexLocker lock(&m_valuesMutex);
    
    if (m_values.contains(key)) {
        return true;
    }
    
    if (m_parentConfigRef.IsValid()) {
        lock.unlock();
        return m_parentConfigRef->HasValue(key);
    }
    
    return false;
}

QStringList CConfigurationComponent::GetKeys() const
{
    QMutexLocker lock(&m_valuesMutex);
    
    QStringList keys = m_values.keys();
    
    if (m_parentConfigRef.IsValid()) {
        lock.unlock();
        QStringList parentKeys = m_parentConfigRef->GetKeys();
        for (const QString& key : parentKeys) {
            if (!keys.contains(key)) {
                keys.append(key);
            }
        }
    }
    
    return keys;
}

void CConfigurationComponent::Load()
{
    if (m_persistenceRef.IsValid()) {
        QMutexLocker lock(&m_valuesMutex);
        m_values = m_persistenceRef->LoadConfiguration();
        m_modified = false;
    }
}

void CConfigurationComponent::Save()
{
    if (m_persistenceRef.IsValid()) {
        QMutexLocker lock(&m_valuesMutex);
        m_persistenceRef->SaveConfiguration(m_values);
        m_modified = false;
    }
}

void CConfigurationComponent::Clear()
{
    QMutexLocker lock(&m_valuesMutex);
    m_values.clear();
    m_modified = true;
}

// Usage example
void MyApplication::OnComponentCreated()
{
    BaseClass::OnComponentCreated();
    
    // Get configuration
    IConfiguration* config = GetConfiguration();
    
    // Read settings
    QString serverUrl = config->GetValueAs<QString>("server/url", "http://localhost");
    int port = config->GetValueAs<int>("server/port", 8080);
    bool useSSL = config->GetValueAs<bool>("server/ssl", false);
    
    // Update settings
    config->SetValue("last_opened", QDateTime::currentDateTime());
    config->SetValue("window/width", 800);
    config->SetValue("window/height", 600);
}
```

---

## Multi-Threaded Processing Pipeline

Implement a pipeline processing system with components:

```cpp
// IProcessor.h
class IProcessor
{
public:
    virtual bool Process(const QVariant& input, QVariant& output) = 0;
    virtual QString GetProcessorName() const = 0;
};

// CProcessingPipeline.h
class CProcessingPipeline: public icomp::CComponentBase
{
public:
    typedef icomp::CComponentBase BaseClass;
    
    I_BEGIN_COMPONENT(CProcessingPipeline);
        I_REGISTER_INTERFACE(IPipeline);
        
        // Ordered list of processors
        I_ASSIGN_MULTI_0(m_processorsRef, "Processors", 
                        "Processing stages", false);
        
        I_ASSIGN(m_loggerRef, "Logger", "Logger", true);
        
        I_ASSIGN(m_parallelAttr, "Parallel", 
                "Use parallel processing", true, false);
        I_ASSIGN(m_threadCountAttr, "ThreadCount",
                "Number of threads", true, 4);
    I_END_COMPONENT;
    
    // Process data through pipeline
    bool ProcessData(const QVariant& input, QVariant& output);
    
    // Batch processing
    QList<QVariant> ProcessBatch(const QList<QVariant>& inputs);
    
protected:
    virtual void OnComponentCreated() override;
    virtual void OnComponentDestroyed() override;
    
private:
    bool ProcessSequential(const QVariant& input, QVariant& output);
    QVariant ProcessSingle(const QVariant& input);
    
    I_MULTIREF(IProcessor, m_processorsRef);
    I_REF(ILogger, m_loggerRef);
    I_ATTR(bool, m_parallelAttr);
    I_ATTR(int, m_threadCountAttr);
    
    QThreadPool* m_threadPool;
};

// CProcessingPipeline.cpp
void CProcessingPipeline::OnComponentCreated()
{
    BaseClass::OnComponentCreated();
    
    // Create thread pool for parallel processing
    if (*m_parallelAttr) {
        m_threadPool = new QThreadPool(this);
        m_threadPool->setMaxThreadCount(*m_threadCountAttr);
        
        if (m_loggerRef.IsValid()) {
            m_loggerRef->LogMessage(
                QString("Pipeline using %1 threads").arg(*m_threadCountAttr));
        }
    } else {
        m_threadPool = nullptr;
    }
}

void CProcessingPipeline::OnComponentDestroyed()
{
    if (m_threadPool) {
        m_threadPool->waitForDone();
        delete m_threadPool;
        m_threadPool = nullptr;
    }
    
    BaseClass::OnComponentDestroyed();
}

bool CProcessingPipeline::ProcessData(const QVariant& input, QVariant& output)
{
    if (!m_processorsRef.IsValid()) {
        if (m_loggerRef.IsValid()) {
            m_loggerRef->LogError("No processors configured");
        }
        return false;
    }
    
    return ProcessSequential(input, output);
}

bool CProcessingPipeline::ProcessSequential(const QVariant& input, 
                                           QVariant& output)
{
    QVariant current = input;
    int count = m_processorsRef.GetCount();
    
    for (int i = 0; i < count; ++i) {
        IProcessor* processor = m_processorsRef.GetPtr(i);
        if (!processor) {
            if (m_loggerRef.IsValid()) {
                m_loggerRef->LogError(
                    QString("Processor %1 is invalid").arg(i));
            }
            return false;
        }
        
        QVariant stageOutput;
        bool success = processor->Process(current, stageOutput);
        
        if (!success) {
            if (m_loggerRef.IsValid()) {
                m_loggerRef->LogError(
                    QString("Processing failed at stage %1 (%2)")
                        .arg(i)
                        .arg(processor->GetProcessorName()));
            }
            return false;
        }
        
        current = stageOutput;
    }
    
    output = current;
    return true;
}

QList<QVariant> CProcessingPipeline::ProcessBatch(
    const QList<QVariant>& inputs)
{
    QList<QVariant> results;
    
    if (*m_parallelAttr && m_threadPool) {
        // Parallel processing
        QMutex resultsMutex;
        QAtomicInt completedCount(0);
        int totalCount = inputs.size();
        
        for (const QVariant& input : inputs) {
            // Create task
            auto task = [this, input, &results, &resultsMutex, 
                        &completedCount, totalCount]() {
                QVariant output = ProcessSingle(input);
                
                {
                    QMutexLocker lock(&resultsMutex);
                    results.append(output);
                }
                
                completedCount.fetchAndAddRelaxed(1);
                
                if (m_loggerRef.IsValid()) {
                    int completed = completedCount.loadRelaxed();
                    if (completed % 10 == 0 || completed == totalCount) {
                        m_loggerRef->LogMessage(
                            QString("Progress: %1/%2").arg(completed).arg(totalCount));
                    }
                }
            };
            
            // Submit to thread pool
            QtConcurrent::run(m_threadPool, task);
        }
        
        // Wait for completion
        m_threadPool->waitForDone();
    } else {
        // Sequential processing
        for (const QVariant& input : inputs) {
            results.append(ProcessSingle(input));
        }
    }
    
    return results;
}

QVariant CProcessingPipeline::ProcessSingle(const QVariant& input)
{
    QVariant output;
    ProcessData(input, output);
    return output;
}
```

---

## Component Testing Patterns

Patterns for unit testing components:

```cpp
// ComponentTest.h
#include <itest/CTestBase.h>
#include <icomp/CComponentBase.h>

class CComponentTest: public itest::CTestBase
{
    Q_OBJECT
    
private slots:
    void initTestCase();
    void cleanupTestCase();
    
    void testComponentCreation();
    void testAttributeAccess();
    void testReferenceResolution();
    void testFactoryCreation();
    void testLifecycle();
    
private:
    // Helper methods
    IComponentUniquePtr CreateTestComponent(const QString& configXml);
    IRegistryUniquePtr CreateTestRegistry(const QString& xml);
};

// ComponentTest.cpp
void CComponentTest::testComponentCreation()
{
    // Create minimal test registry
    QString xml = R"(
        <?xml version="1.0"?>
        <Acf>
            <ElementsList>
                <Element Id="TestComp" PackageId="TestPck" ComponentId="TestComponent">
                    <Data IsEnabled="true">
                        <AttributeInfoMap>
                            <AttributeInfo Id="Name" Type="String">
                                <Data IsEnabled="true" Value="TestInstance"/>
                            </AttributeInfo>
                        </AttributeInfoMap>
                    </Data>
                </Element>
            </ElementsList>
        </Acf>
    )";
    
    IComponentUniquePtr comp = CreateTestComponent(xml);
    QVERIFY(comp != nullptr);
}

void CComponentTest::testAttributeAccess()
{
    QString xml = R"(
        <Element Id="TestComp">
            <AttributeInfoMap>
                <AttributeInfo Id="Count" Type="Integer">
                    <Data IsEnabled="true" Value="42"/>
                </AttributeInfo>
                <AttributeInfo Id="Enabled" Type="Boolean">
                    <Data IsEnabled="true" Value="true"/>
                </AttributeInfo>
            </AttributeInfoMap>
        </Element>
    )";
    
    IComponentUniquePtr comp = CreateTestComponent(xml);
    QVERIFY(comp != nullptr);
    
    // Get typed interface
    ITestComponent* testComp = dynamic_cast<ITestComponent*>(comp.get());
    QVERIFY(testComp != nullptr);
    
    // Verify attributes
    QCOMPARE(testComp->GetCount(), 42);
    QCOMPARE(testComp->IsEnabled(), true);
}

void CComponentTest::testLifecycle()
{
    // Create component
    IComponentUniquePtr comp = CreateTestComponent("<Element Id='Test'/>");
    QVERIFY(comp != nullptr);
    
    ITestComponent* testComp = dynamic_cast<ITestComponent*>(comp.get());
    QVERIFY(testComp != nullptr);
    
    // Check creation was called
    QVERIFY(testComp->WasCreated());
    QVERIFY(!testComp->WasDestroyed());
    
    // Destroy component
    comp.reset();
    
    // Note: Can't check WasDestroyed() as component is deleted
}

// Mock component for testing
class CMockLogger: 
    public icomp::CComponentBase,
    public ILogger
{
public:
    typedef icomp::CComponentBase BaseClass;
    
    I_BEGIN_COMPONENT(CMockLogger);
        I_REGISTER_INTERFACE(ILogger);
    I_END_COMPONENT;
    
    virtual void LogMessage(const QString& message) override
    {
        m_messages.append(message);
    }
    
    virtual void LogError(const QString& error) override
    {
        m_errors.append(error);
    }
    
    QStringList GetMessages() const { return m_messages; }
    QStringList GetErrors() const { return m_errors; }
    void Clear() { m_messages.clear(); m_errors.clear(); }
    
private:
    QStringList m_messages;
    QStringList m_errors;
};

// Test with mock
void CComponentTest::testWithMock()
{
    // Create mock logger
    CMockLogger mockLogger;
    
    // Create component under test
    CMyComponent component;
    
    // Inject mock
    component.SetLogger(&mockLogger);
    
    // Exercise component
    component.DoWork();
    
    // Verify logging
    QStringList messages = mockLogger.GetMessages();
    QCOMPARE(messages.size(), 1);
    QCOMPARE(messages[0], QString("Work completed"));
}
```

---

## Summary

These examples demonstrate:

1. **Data Processing Pipeline**: Component composition where multiple processor components are coordinated by another component
2. **Service Locator**: Centralized service access and dependency management
3. **Observer Pattern**: Event-driven communication between components
4. **Configuration System**: Hierarchical configuration with persistence
5. **Processing Pipeline**: Multi-threaded data processing architecture
6. **Testing Patterns**: Unit testing strategies for components

For more information, see the main [ComponentFramework.md](ComponentFramework.md) documentation.
