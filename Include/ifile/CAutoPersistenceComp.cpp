#include "ifile/CAutoPersistenceComp.h"


namespace ifile
{


// protected methods

// reimplemented (imod::CSingleModelObserverBase)

void CAutoPersistenceComp::OnUpdate(int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if (*m_storeOnChangeAttrPtr){
		if (m_fileLoaderCompPtr.IsValid() && m_objectCompPtr.IsValid()){
			QString filePath;
			if (m_filePathCompPtr.IsValid()){
				filePath = m_filePathCompPtr->GetPath();
			}

			m_fileLoaderCompPtr->SaveToFile(*m_objectCompPtr, filePath);
		}
	}
}


// reimplemented (icomp::CComponentBase)

void CAutoPersistenceComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (*m_restoreOnBeginAttrPtr){
		if (m_fileLoaderCompPtr.IsValid() && m_objectCompPtr.IsValid()){
			QString filePath;
			if (m_filePathCompPtr.IsValid()){
				filePath = m_filePathCompPtr->GetPath();
			}

			m_fileLoaderCompPtr->LoadFromFile(*m_objectCompPtr, filePath);
		}
	}

	if (m_objectModelCompPtr.IsValid())
	{
		m_objectModelCompPtr->AttachObserver(this);
	}
}


void CAutoPersistenceComp::OnComponentDestroyed()
{
	EnsureModelDetached();

	if (*m_storeOnEndAttrPtr){
		if (m_fileLoaderCompPtr.IsValid() && m_objectCompPtr.IsValid()){
			QString filePath;
			if (m_filePathCompPtr.IsValid()){
				filePath = m_filePathCompPtr->GetPath();
			}

			m_fileLoaderCompPtr->SaveToFile(*m_objectCompPtr, filePath);
		}
	}

	BaseClass::OnComponentDestroyed();
}


} // namespace ifile


