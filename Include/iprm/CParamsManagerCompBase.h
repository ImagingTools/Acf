#ifndef iprm_CParamsManagerCompBase_included
#define iprm_CParamsManagerCompBase_included


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include "istd/TSmartPtr.h"
#include "imod/CMultiModelBridgeBase.h"
#include "icomp/CComponentBase.h"
#include "iprm/IParamsSet.h"
#include "iprm/ISelectionParam.h"
#include "iprm/IOptionsManager.h"
#include "iprm/IParamsManager.h"


namespace iprm
{


/**
	Implementation of parameter manager.
*/
class CParamsManagerCompBase:
			public icomp::CComponentBase,
			virtual public IParamsManager,
			virtual public IOptionsManager,
			protected imod::CMultiModelBridgeBase
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_BASE_COMPONENT(CParamsManagerCompBase);
		I_REGISTER_INTERFACE(ISelectionParam);
		I_REGISTER_INTERFACE(IParamsManager);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(IOptionsManager);
		I_REGISTER_INTERFACE(IOptionsList);
		I_ASSIGN_MULTI_0(m_fixedParamSetsCompPtr, "FixedParamSets", "List of references to fixed parameter set", false);
		I_ASSIGN_MULTI_0(m_fixedSetNamesAttrPtr, "FixedSetNames", "List of fixed parameter names", false);
		I_ASSIGN(m_defaultSetNameAttrPtr, "DefaultSetName", "Default name of parameter set. Use %1 to insert automatic enumeration", true, "<noname>");
		I_ASSIGN(m_serializeSelectionAttrPtr, "SerializeSelection", "If enabled, the current parameter set selection will be serialized", true, true);
		I_ASSIGN(m_allowDisabledAttrPtr, "AllowDisabled", "Control if disabled parameters are supported", true, false);
		I_ASSIGN(m_supportEnablingAttrPtr, "SupportEnabling", "Control if enabling or disabling of parameters is allowed (works only if disabled parameters are supported)", true, true);
		I_ASSIGN(m_allowEditFixedAttrPtr, "AllowEditFixed", "If enabled, the editing of fixed parameters is enabled", true, true);
	I_END_COMPONENT;

	CParamsManagerCompBase();

	// reimplemented (iprm::IParamsManager)
	virtual int GetIndexOperationFlags(int index = -1) const;
	virtual int GetParamsSetsCount() const;

	// reimplemented (iprm::ISelectionParam)
	virtual const IOptionsList* GetSelectionConstraints() const;
	virtual int GetSelectedOptionIndex() const;
	virtual bool SetSelectedOptionIndex(int index);
	virtual ISelectionParam* GetSubselection(int index) const;

protected:
	// abstract methods

	/**
		Return \c true if creation of parameter sets is supported by the implementation.
	*/
	virtual bool IsParameterCreationSupported() const = 0;

	/**
		Get the number of parameter sets created at the run time.
	*/
	virtual int GetCreatedParamsSetsCount() const = 0;

protected:
	int m_selectedIndex;

	I_MULTIREF(IParamsSet, m_fixedParamSetsCompPtr);
	I_MULTIATTR(QString, m_fixedSetNamesAttrPtr);
	I_ATTR(QString, m_defaultSetNameAttrPtr);
	I_ATTR(bool, m_serializeSelectionAttrPtr);
	I_ATTR(bool, m_allowDisabledAttrPtr);
	I_ATTR(bool, m_supportEnablingAttrPtr);
	I_ATTR(bool, m_allowEditFixedAttrPtr);
};


} // namespace iprm


#endif // !iprm_CParamsManagerCompBase_included


