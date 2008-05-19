#include "CRegistryPreviewComp.h"

#include <QTemporaryFile>
#include <QApplication>


#include "iser/CXmlFileWriteArchive.h"


CRegistryPreviewComp::CRegistryPreviewComp()
{
	QFont font = qApp->font();
	font.setPointSize(12);
	m_threadBox.setFont(font);
	m_threadBox.setText(QApplication::tr("Application is starting..."));
	m_threadBox.setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowStaysOnTopHint);

	connect(this, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(OnFinished(int, QProcess::ExitStatus)));
}

	
// reimplemented (IRegistryPreview)

bool CRegistryPreviewComp::StartRegistry(const icomp::IRegistry& registry)
{
	if (IsRunning()){
		return false;
	}

	m_tempFileName.clear();

//	m_threadBox.move(this->rect().center());
	m_threadBox.show();

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
		m_threadBox.hide();

		return true;
	}

	m_threadBox.hide();
	
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

	if (!waitForFinished(10)){
		kill();
	}
}


// protected slots

void CRegistryPreviewComp::OnFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	QFile::remove(m_tempFileName);
}

