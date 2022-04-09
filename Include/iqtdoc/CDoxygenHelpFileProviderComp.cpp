/********************************************************************************
**
**	Copyright (C) 2007-2017 Witold Gantzke & Kirill Lepskiy
**
**	This file is part of the ACF Toolkit.
**
**	This file may be used under the terms of the GNU Lesser
**	General Public License version 2.1 as published by the Free Software
**	Foundation and appearing in the file LicenseLGPL.txt included in the
**	packaging of this file.  Please review the following information to
**	ensure the GNU Lesser General Public License version 2.1 requirements
**	will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
**	If you are unsure which license is appropriate for your use, please
**	contact us at info@imagingtools.de.
**
** 	See http://www.ilena.org or write info@imagingtools.de for further
** 	information about the ACF.
**
********************************************************************************/


#include <iqtdoc/CDoxygenHelpFileProviderComp.h>


// Qt includes
#include <QtCore/QFileInfo>


// ACF includes
#include <istd/CClassInfo.h>

#include <istd/CSystem.h>


namespace iqtdoc
{


// reimplemented (idoc::IHelpFileProvider)

double CDoxygenHelpFileProviderComp::GetHelpQuality(const QString& contextText, const istd::IPolymorphic* contextObjectPtr) const
{
	QFileInfo fileInfo(CalcFilePath(CalcClassInfo(contextText, contextObjectPtr)));

	if (fileInfo.exists()){
		return 1.0;
	}
	else if (m_slaveProvider.IsValid()){
		return m_slaveProvider->GetHelpQuality(contextText, contextObjectPtr);
	}
	else{
		return 0.0;
	}
}


QString CDoxygenHelpFileProviderComp::GetHelpFilePath(const QString& contextText, const istd::IPolymorphic* contextObjectPtr) const
{
	QFileInfo fileInfo(CalcFilePath(CalcClassInfo(contextText, contextObjectPtr)));

	if (fileInfo.exists()){
		return fileInfo.absoluteFilePath();
	}
	else if (m_slaveProvider.IsValid()){
		return m_slaveProvider->GetHelpFilePath(contextText, contextObjectPtr);
	}
	else{
		return "";
	}
}


// protected methods

istd::CClassInfo CDoxygenHelpFileProviderComp::CalcClassInfo(const QString& contextText, const istd::IPolymorphic* contextObjectPtr) const
{
	if (contextObjectPtr != NULL){
		const istd::CClassInfo* classInfoPtr = dynamic_cast<const istd::CClassInfo*>(contextObjectPtr);
		if (classInfoPtr != NULL){
			return *classInfoPtr;
		}
		else{
			return istd::CClassInfo(*contextObjectPtr);
		}
	}
	else{
		return istd::CClassInfo(contextText.toLocal8Bit());
	}
}


QString CDoxygenHelpFileProviderComp::CalcFilePath(const istd::CClassInfo& classInfo) const
{
	QString retVal = *m_doxygenDirectoryAttrPtr + "/class";

	QByteArray className = classInfo.GetName();
	for (		QByteArray::const_iterator iter = className.begin();
				iter != className.end();
				++iter){
		char c = *iter;
		if (c == ':'){
			retVal += "_1";
		}
		else if ((c >= 'A') && (c <= 'Z')){
			retVal += "_";
			retVal.push_back(QChar(c - 'A' + 'a'));
		}
		else{
			retVal.push_back(c);
		}
	}

	retVal += ".html";

	return istd::CSystem::GetEnrolledPath(retVal);
}


} // namespace iqtdoc


