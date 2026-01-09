# ACF Component Framework Examples

This document provides additional practical examples for working with the ACF Component Framework. For comprehensive documentation, see [ComponentFramework.md](ComponentFramework.md).

## Table of Contents

1. [Data Processing Pipeline with Multiple Processors](#data-processing-pipeline-with-multiple-processors)
2. [Multi-Threaded Processing Pipeline](#multi-threaded-processing-pipeline)
3. [Component Testing Patterns](#component-testing-patterns)

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
2. **Processing Pipeline**: Multi-threaded data processing architecture
3. **Testing Patterns**: Unit testing strategies for components

For more information, see the main [ComponentFramework.md](ComponentFramework.md) documentation.
