#include "icmpstr/CRegistryPreviewComp.h"


// Qt includes
#include <QTemporaryFile>
#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QMetaType>

#include "iser/CXmlFileWriteArchive.h"


namespace icmpstr
{


// public methods

// reimplemented (icomp::IComponent)

void CRegistryPreviewComp::OnComponentCreated()
{
	qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");

	BaseClass::OnComponentCreated();

	connect(	&m_process, 
				SIGNAL(stateChanged(QProcess::ProcessState)), 
				this, 
				SLOT(OnFinished(QProcess::ProcessState)), 
				Qt::QueuedConnection);
}


void CRegistryPreviewComp::OnComponentDestroyed()
{
	if (IsRunning()){
		m_process.kill();

		if (m_process.waitForFinished(5000)){
			QFile::remove(m_tempFileName);
		}
	}

	BaseClass::OnComponentDestroyed();
}


// reimplemented (icmpstr::IRegistryPreview)

bool CRegistryPreviewComp::StartRegistry(const icomp::IRegistry& registry)
{
	if (IsRunning()){
		return false;
	}

	m_tempFileName.clear();

	QDir tempDir = QDir::temp();
	if (!tempDir.exists()){
		return false;
	}

	m_tempFileName = tempDir.absoluteFilePath("registry_preview.arx");

	iser::CXmlFileWriteArchive archive(m_tempFileName.toStdString());

	if (!(const_cast<icomp::IRegistry&>(registry)).Serialize(archive)){
		return false;
	}

	static QString acfExeFile = "Acf";

	QDir applicationDir(QCoreApplication::applicationDirPath());
	QString acfApplicationPath = applicationDir.absoluteFilePath(acfExeFile);

	m_process.setWorkingDirectory(applicationDir.path());
	m_process.start(acfApplicationPath, QStringList() << m_tempFileName);

	return m_process.waitForStarted();
}


bool CRegistryPreviewComp::IsRunning() const
{
	if (m_process.state() == QProcess::Running){
		return true;
	}

	return false;
}


void CRegistryPreviewComp::AbortRegistry()
{
	m_process.terminate();
	if (!m_process.waitForFinished(5000)){
		m_process.kill();
	}
}


// protected slots

void CRegistryPreviewComp::OnStateChanged(QProcess::ProcessState state)
{
	if (state == QProcess::NotRunning){
		QFile::remove(m_tempFileName);
	}
}


} // namespace icmpstr


