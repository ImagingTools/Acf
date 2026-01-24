// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <ifile/IFilePersistence.h>
#include <ifile/IFilePersistenceInfo.h>
#include <iqtgui/CProcessStartCommandComp.h>


namespace ifilegui
{


class CExternalOpenDocumentCommandCompBase: public iqtgui::CProcessStartCommandComp
{
public:
	typedef iqtgui::CProcessStartCommandComp BaseClass;

	I_BEGIN_BASE_COMPONENT(CExternalOpenDocumentCommandCompBase);
		I_ASSIGN(m_documentPersistenceCompPtr, "DocumentPersistence", "File persistence for the actual document type", true, "DocumentPersistence"); 
		I_ASSIGN_TO(m_documentPersistenceInfoCompPtr, m_documentPersistenceCompPtr, false);
		I_ASSIGN(m_documentFileCompPtr, "DocumentFilePath", "Path used for saving the current document", true, "DocumentFilePath");
	I_END_COMPONENT;

protected:
	// abstract methods
	virtual const istd::IChangeable* GetDocumentPtr() const = 0;

	// reimplemented (iqtgui::CProcessStartCommandComp)
	virtual bool StartProcess(const QStringList& arguments) override;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;

protected:	
	I_REF(ifile::IFilePersistence, m_documentPersistenceCompPtr);
	I_REF(ifile::IFilePersistenceInfo, m_documentPersistenceInfoCompPtr);
	I_REF(ifile::IFileNameParam, m_documentFileCompPtr);
};


}  // namespace ifilegui




