#include "CRegistryPreviewComp.h"

#include <QTemporaryFile>
#include <QApplication>


#include "iser/CXmlFileWriteArchive.h"

	

// public methods

// reimplemented (icomp::IComponent)

void CRegistryPreviewComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	QFont font = qApp->font();
	font.setPointSize(12);
	m_startLabel.setFont(font);
	m_startLabel.setText(QApplication::tr("Application is starting..."));
	m_startLabel.setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowStaysOnTopHint);

	connect(	this, 
				SIGNAL(finished(int, QProcess::ExitStatus)), 
				this, 
				SLOT(OnFinished(int, QProcess::ExitStatus)), 
				Qt::QueuedConnection);
}


void CRegistryPreviewComp::OnComponentDestroyed()
{
	if (IsRunning()){
		kill();

		if (waitForFinished()){
			QFile::remove(m_tempFileName);
		}
	}

	BaseClass::OnComponentDestroyed();
}


// reimplemented (IRegistryPreview)

bool CRegistryPreviewComp::StartRegistry(const icomp::IRegistry& registry)
{
	if (IsRunning()){
		return false;
	}

	m_tempFileName.clear();

//	m_startLabel.move(this->rect().center());
	m_startLabel.show();

	QTemporaryFile tempFile("registry_preview.arx");
	if (tempFile.open()){
		m_tempFileName = tempFile.fileName();

		iser::CXmlFileWriteArchive archive(m_tempFileName.toStdString());
		if (!(const_cast<icomp::IRegistry&>(registry)).Serialize(archive)){
			return false;
		}
	}
	else{
		return false;
	}

	QProcess::start("./Acf.exe", QStringList() << m_tempFileName);
	if (waitForStarted(10000)){
		m_startLabel.hide();

		return true;
	}

	m_startLabel.hide();
	
	return false;
}


bool CRegistryPreviewComp::IsRunning() const
{
	if (QProcess::state() == QProcess::Running){
		return true;
	}

	return false;
}


void CRegistryPreviewComp::AbortRegistry()
{
	QProcess::terminate();
	if (!waitForFinished()){
		kill();
	}
}


// protected slots

void CRegistryPreviewComp::OnFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	QFile::remove(m_tempFileName);
}

