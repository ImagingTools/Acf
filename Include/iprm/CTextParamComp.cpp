// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iprm/CTextParamComp.h>


namespace iprm
{


// public methods

// reimplemented (iprm::INameParam)

void CTextParamComp::SetText(const QString& name)
{
	if (!*m_isReadonlyAttrPtr){
		BaseClass2::SetText(name);
	}
}


bool CTextParamComp::IsReadOnly() const
{
	return *m_isReadonlyAttrPtr;
}


// reimplemented (iser::ISerializable)

bool CTextParamComp::Serialize(iser::IArchive& archive)
{
	if (!*m_serializeParamsAttrPtr) {
		return true;
	}

	return BaseClass2::Serialize(archive);
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CTextParamComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_defaultTextAttrPtr.IsValid()){
		BaseClass2::SetText(*m_defaultTextAttrPtr);
	}
}


} // namespace iprm


