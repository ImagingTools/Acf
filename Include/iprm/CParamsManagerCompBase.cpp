#include "iprm/CParamsManagerCompBase.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "imod/IModel.h"


namespace iprm
{


CParamsManagerCompBase::CParamsManagerCompBase()
	:m_selectedIndex(-1)
{
}


// reimplemented (iprm::IParamsManager)

int CParamsManagerCompBase::GetIndexOperationFlags(int index) const
{
	Q_ASSERT(index < GetParamsSetsCount());

	int retVal = 0;

	if (IsParameterCreationSupported()){
		if ((index < 0) || (index >= m_fixedParamSetsCompPtr.GetCount())){
			retVal |= MF_SUPPORT_INSERT | MF_SUPPORT_SWAP | MF_SUPPORT_RENAME;
			
			if (*m_allowDisabledAttrPtr){
				retVal |= MF_DISABLE_ALLOWED;
			}

			if (*m_supportEnablingAttrPtr){
				retVal |= OOF_SUPPORT_ENABLING;
			}

			if (GetCreatedParamsSetsCount() > 0){
				retVal |= MF_SUPPORT_DELETE;
			}
		}
	}
	else{
		retVal |= MF_COUNT_FIXED;
	}

	if ((index >= 0) && (index < CParamsManagerCompBase::GetParamsSetsCount())){
		retVal |= MF_SUPPORT_EDIT;

		if (index < m_fixedParamSetsCompPtr.GetCount() && !*m_allowEditFixedAttrPtr){
			retVal &= ~MF_SUPPORT_EDIT;		
		}
	}

	return retVal;
}


int CParamsManagerCompBase::GetParamsSetsCount() const
{
	return m_fixedParamSetsCompPtr.GetCount() + GetCreatedParamsSetsCount();
}


// reimplemented (iprm::ISelectionParam)

const IOptionsList* CParamsManagerCompBase::GetSelectionConstraints() const
{
	return this;
}


int CParamsManagerCompBase::GetSelectedOptionIndex() const
{
	return m_selectedIndex;
}


bool CParamsManagerCompBase::SetSelectedOptionIndex(int index)
{
	if (index < GetOptionsCount()){
		if (index != m_selectedIndex){
			istd::CChangeNotifier notifier(this, CF_SELECTION_CHANGED | CF_OPTIONS_CHANGED | CF_MODEL);

			m_selectedIndex = index;
		}

		return true;
	}

	return false;
}


ISelectionParam* CParamsManagerCompBase::GetSubselection(int /*index*/) const
{
	return NULL;
}


} // namespace iprm


