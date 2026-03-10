// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <istd/TIHierarchical.h>
#include <icomp/CComponentBase.h>
#include <iprm/CParamsSet.h>
#include <iprm/IParamsInfoProvider.h>


namespace iprm
{


/**
	Implementation of interface IParamsSet as component.
	This implementation allows to register list of objects as editable parameters and list of slave parameter sets.
*/
class CComposedParamsSetComp:
			public icomp::CComponentBase,
			public CParamsSet,
			virtual public istd::IHierarchical,
			virtual public IParamsInfoProvider
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef CParamsSet BaseClass2;

	I_BEGIN_COMPONENT(CComposedParamsSetComp);
		I_REGISTER_INTERFACE(istd::IHierarchical);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(IParamsSet);
		I_REGISTER_INTERFACE(IParamsInfoProvider);
		I_ASSIGN_MULTI_0(m_slaveParamsCompPtr, "SlaveSets", "List of slave parameter sets", false);
		I_ASSIGN_MULTI_0(m_parametersCompPtr, "Parameters", "Parameters", true);
		I_ASSIGN_MULTI_0(m_parametersIdAttrPtr, "ParametersId", "ID of each parameter in 'Parameters'", true);
		I_ASSIGN_MULTI_0(m_parameterNameAttrPtr, "ParametersName", "Name of each parameter in 'Parameters'", true);
		I_ASSIGN_MULTI_0(m_parameterDescriptionAttrPtr, "ParametersDescription", "Description of each parameter in 'Parameters'", true);
		I_ASSIGN(m_typeIdAttrPtr, "TypeId", "ID of this parameter set", true, "Default");
	I_END_COMPONENT;

	// reimplemented (iprm::IParamsSet)
	virtual Ids GetParamIds(bool editableOnly = false) const override;
	virtual const iser::ISerializable* GetParameter(const QByteArray& id) const override;
	virtual const IParamsInfoProvider* GetParamsInfoProvider() const override;

	// reimplemented (iprm::IParamsInfoProvider)
	virtual std::unique_ptr<ParamInfo> GetParamInfo(const QByteArray& paramId) const override;

	// reimplemented (istd::IHierarchical)
	virtual int GetHierarchicalFlags() const override;
	virtual int GetChildsCount() const override;
	virtual istd::IPolymorphic* GetChild(int index) const override;
	virtual istd::IPolymorphic* GetParent() const override;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const override;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual bool IsEqual(const IChangeable& object) const override;

protected:
	iprm::IParamsSet* GetSlaveParamsSet(int index) const;
	int GetSlaveParamsSetCount() const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;

private:
	I_MULTIREF(IParamsSet, m_slaveParamsCompPtr);
	I_MULTIREF(iser::ISerializable, m_parametersCompPtr);
	I_MULTIATTR(QByteArray, m_parametersIdAttrPtr);
	I_MULTIATTR(QString, m_parameterNameAttrPtr);
	I_MULTIATTR(QString, m_parameterDescriptionAttrPtr);
	I_ATTR(QByteArray, m_typeIdAttrPtr);
};


} // namespace iprm


