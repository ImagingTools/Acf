// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <imod/CModelUpdateBridge.h>
#include <icomp/CComponentBase.h>
#include <iprm/IVariableParam.h>


namespace iprm
{


class CVariableParamComp:
			public icomp::CComponentBase,
			virtual public IVariableParam
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CVariableParamComp);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(IVariableParam);
		I_ASSIGN_MULTI_0(m_factoriesFact, "Factories", "List of factories for each type", true);
		I_ASSIGN_MULTI_0(m_factoryIdsAttrPtr, "TypeIds", "List of factory ID for each factory object", true);
	I_END_COMPONENT;

	CVariableParamComp();

	// reimplemented (iprm::IVariableParam)
	virtual TypeIds GetKnownTypeIds() const override;
	virtual QByteArray GetParameterTypeId() const override;
	virtual iser::ISerializable* GetParameter() const override;
	virtual bool AssignTypeId(const QByteArray& typeId) override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;
	virtual quint32 GetMinimalVersion(int versionId) const override;

private:
	I_MULTIFACT(iser::ISerializable, m_factoriesFact);
	I_MULTIATTR(QString, m_factoryIdsAttrPtr);

	QByteArray m_paramTypeId;
	iser::ISerializableUniquePtr m_paramPtr;

	imod::CModelUpdateBridge m_updateBridge;
};


} // namespace iprm




