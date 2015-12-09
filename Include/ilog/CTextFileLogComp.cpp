#include "ilog/CTextFileLogComp.h"


// ACF includes
#include "istd/IInformationProvider.h"
#include "imod/IModel.h"


namespace ilog
{


// public methods

CTextFileLogComp:: CTextFileLogComp()
:	m_outputFile(),
	m_outputFileStream(&m_outputFile),
	m_filePathObserver(*this),
	m_mutex(QMutex::Recursive)
{
}


// protected methods

// reimplemented (CStreamLogCompBase)

void CTextFileLogComp::WriteText(const QString& text, istd::IInformationProvider::InformationCategory /*category*/)
{
	QMutexLocker lock(&m_mutex);

	if (m_outputFile.isOpen()){
		m_outputFileStream << text;
		m_outputFileStream.flush();
	}
}


// reimplemented (icomp::CComponentBase)

void CTextFileLogComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_fileNameCompPtr.IsValid() && m_fileNameModelCompPtr.IsValid()){
		m_fileNameModelCompPtr->AttachObserver(&m_filePathObserver);
	}
	
	OpenFileStream();
}


void CTextFileLogComp::OnComponentDestroyed()
{
	if (m_fileNameModelCompPtr.IsValid() && m_fileNameModelCompPtr->IsAttached(&m_filePathObserver)){
		m_fileNameModelCompPtr->DetachObserver(&m_filePathObserver);
	}

	CloseFileStream();

	BaseClass::OnComponentDestroyed();
}


void CTextFileLogComp::OpenFileStream()
{
	QMutexLocker lock(&m_mutex);

	if (m_outputFile.isOpen()){
		m_outputFile.flush();

		m_outputFile.close();
	}

	if (m_fileNameCompPtr.IsValid()){
		QIODevice::OpenMode openMode = QIODevice::Text | QIODevice::WriteOnly;

		if (*m_isAppendAttrPtr){
			openMode |= QIODevice::Append;
		}

		if (m_fileNameCompPtr->GetPath().isEmpty()){
			return;
		}

		m_outputFile.setFileName(m_fileNameCompPtr->GetPath());
		m_outputFile.open(openMode);
	}
}


void CTextFileLogComp::CloseFileStream()
{
	QMutexLocker lock(&m_mutex);

	m_outputFile.close();
}


// embedded class FilePathObserver

// public methods

CTextFileLogComp::FilePathObserver::FilePathObserver(CTextFileLogComp& parent)
	:m_parent(parent)
{
}


// protected methods

// reimplemented (imod::CSingleModelObserverBase)
	
void CTextFileLogComp::FilePathObserver::OnUpdate(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	m_parent.OpenFileStream();
}


} // namespace ilog


