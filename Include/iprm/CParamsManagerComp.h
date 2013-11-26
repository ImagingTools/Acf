#ifndef iprm_CParamsManagerComp_included
#define iprm_CParamsManagerComp_included


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include "istd/TDelPtr.h"
#include "imod/TModelWrap.h"
#include "iprm/INameParam.h"
#include "iprm/CParamsManagerCompBase.h"


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
		I_ASSIGN(m_elementIndexParamId, "ElementIndexParamId", "ID of index of returned parameter set in manager list", false, "Index");
		I_ASSIGN(m_elementNameParamId, "ElementNameParamId", "ID of the name of returned parameter set in manager list", false, "Name");
		I_ASSIGN(m_paramSetsFactPtr, "ParamsSetFactory", "Factory of variable parameter set", false, "ParamsSet");
	I_END_COMPONENT;

	bool SetSetsCount(int count);

	// reimplemented (iprm::IParamsManager)
	virtual const IOptionsList* GetParamsTypeConstraints() const;
	virtual int InsertParamsSet(int typeIndex = -1, int index = -1);
	virtual bool RemoveParamsSet(int index);
	virtual bool SwapParamsSet(int index1, int index2);
	virtual IParamsSet* GetParamsSet(int index) const;
	virtual QString GetParamsSetName(int index) const;
	virtual bool SetParamsSetName(int index, const QString& name);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (iprm::IOptionsManager)
	virtual int GetOptionOperationFlags(int index = -1) const;
	virtual bool SetOptionEnabled(int index, bool isEnabled = true);
	virtual bool RemoveOption(int index);
	virtual bool InsertOption(
				const QString& optionName,
				const QByteArray& optionId,
				const QString& optionDescription = QString(),
				int index = -1);
	virtual bool SwapOptions(int index1, int index2);
	virtual bool SetOptionName(int optionIndex, const QString& optionName);
	virtual bool SetOptionDescription(int optionIndex, const QString& optionDescription);

	// reimplemented (iprm::IOptionsList)
	virtual int GetOptionsFlags() const;
	virtual int GetOptionsCount() const;
	virtual QString GetOptionName(int index) const;
	virtual QString GetOptionDescription(int index) const;
	virtual QByteArray GetOptionId(int index) const;
	virtual bool IsOptionEnabled(int index) const;

protected:
	QString GetNewSetName() const;

	// reimplemented (CParamsManagerCompBase)
	virtual bool IsParameterCreationSupported() const;
	virtual int GetCreatedParamsSetsCount() const;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_ATTR(QByteArray, m_elementIndexParamId);
	I_ATTR(QByteArray, m_elementNameParamId);
	I_FACT(IParamsSet, m_paramSetsFactPtr);

	class ParamSet:
				public CMultiModelBridgeBase,
				virtual public IParamsSet,
				virtual public ISelectionParam,
				virtual public INameParam
	{
	public:
		ParamSet();

		// reimplemented (iprm::IParamsSet)
		virtual Ids GetParamIds(bool editableOnly = false) const;
		virtual const iser::ISerializable* GetParameter(const QByteArray& id) const;
		virtual iser::ISerializable* GetEditableParameter(const QByteArray& id);

		// reimplemented (iprm::ISelectionParam)
		virtual const IOptionsList* GetSelectionConstraints() const;
		virtual int GetSelectedOptionIndex() const;
		virtual bool SetSelectedOptionIndex(int index);
		virtual ISelectionParam* GetSubselection(int index) const;

		// reimplemented (iser::INameParam)
		virtual const QString& GetName() const;
		virtual void SetName(const QString& name);
		virtual bool IsNameFixed() const;

		// reimplemented (iser::ISerializable)
		virtual bool Serialize(iser::IArchive& archive);

		istd::TDelPtr<IParamsSet> paramSetPtr;
		QString name;
		bool isEnabled;
		const CParamsManagerComp* parentPtr;
	};
	
	int FindParamSetIndex(const QString& name) const;

	typedef istd::TDelPtr<ParamSet> ParamSetPtr;

	typedef QList<ParamSetPtr> ParamSets;

	ParamSets m_paramSets;
};


} // namespace iprm


#endif // !iprm_CParamsManagerComp_included


