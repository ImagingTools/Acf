// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <istd/TOptDelPtr.h>

// ACF-Solutions includes
#include <iattr/INumericAttributeMetaInfo.h>
#include <iattr/CAttributeMetaInfo.h>


namespace iattr
{


/**
	Represents constraints of properties allowing values from enumerated set.
*/
class CNumericAttributeMetaInfo:
			public CAttributeMetaInfo,
			virtual public INumericAttributeMetaInfo
{
public:
	typedef CAttributeMetaInfo BaseClass;

	explicit CNumericAttributeMetaInfo(const QByteArray& typeId = "");

	/**
		Set minimal value of attribute.
		\param	releaseFlag	if true, the value object will be managed by this object, it means it will be deleted from memory during destruction.
	*/
	bool SetMinAttributeValue(const iser::IObject* valuePtr, bool releaseFlag);
	/**
		Set maximal value of attribute.
		\param	releaseFlag	if true, the value object will be managed by this object, it means it will be deleted from memory during destruction.
	*/
	bool SetMaxAttributeValue(const iser::IObject* valuePtr, bool releaseFlag);

	// reimplemented (iattr::IEnumAttributeMetaInfo)
	virtual bool GetMinimalValue(iser::IObject& result) const override;
	virtual bool GetMaximalValue(iser::IObject& result) const override;

private:
	istd::TOptDelPtr<const iser::IObject> m_minAttributePtr;
	istd::TOptDelPtr<const iser::IObject> m_maxAttributePtr;
};


} // namespace iattr




