#pragma once


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include <istd/TDelPtr.h>
#include <imod/TModelWrap.h>
#include <iprm/CParamsManagerCompBase.h>


namespace iprm
{


/**
	Implementation of parameter manager.
*/
class CParamsManagerComp: public iprm::CParamsManagerCompBase
{
public:
	typedef CParamsManagerCompBase BaseClass;

	I_BEGIN_COMPONENT(CParamsManagerComp);
		I_REGISTER_INTERFACE(ISelectionParam);
		I_REGISTER_INTERFACE(IParamsManager);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(IOptionsManager);
		I_REGISTER_INTERFACE(IOptionsList);
		I_ASSIGN(m_paramSetsFactPtr, "ParamsSetFactory", "Factory of variable parameter set", false, "ParamsSet");
	I_END_COMPONENT;

	bool SetSetsCount(int count);

	// reimplemented (iprm::IParamsManager)
	virtual const IOptionsList* GetParamsTypeConstraints() const override;

	// reimplemented (iprm::IOptionsManager)
	virtual int GetOptionOperationFlags(int index = -1) const override;
	virtual bool SetOptionEnabled(int index, bool isEnabled = true) override;
	virtual bool RemoveOption(int index) override;
	virtual bool InsertOption(
				const QString& optionName,
				const QByteArray& optionId,
				const QString& optionDescription = QString(),
				int index = -1);
	virtual bool SwapOptions(int index1, int index2) override;
	virtual bool SetOptionName(int optionIndex, const QString& optionName) override;
	virtual bool SetOptionDescription(int optionIndex, const QString& optionDescription) override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const override;
	virtual bool CopyFrom(const istd::IChangeable& object, istd::IChangeable::CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual bool IsEqual(const IChangeable& object) const override;


protected:
	// reimplemented (CParamsManagerCompBase)
	virtual bool IsParameterCreationSupported() const override;
	virtual int GetCreatedParamsSetsCount() const override;
	virtual IParamsSetUniquePtr CreateParamsSetInstance(int typeIndex = -1) const override;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed() override;

private:
	I_FACT(IParamsSet, m_paramSetsFactPtr);
};


} // namespace iprm




