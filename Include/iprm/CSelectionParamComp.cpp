#include "iprm/CSelectionParamComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iprm
{


// reimplemented (iprm::ISelectionParam)

ISelectionParam* CSelectionParamComp::GetActiveSubselection() const
{
	return m_subselectionCompPtr.GetPtr();
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CSelectionParamComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	SetSelectionConstraints(m_constraintsCompPtr.GetPtr());
	m_selectedOptionIndex = *m_defaultIndexAttrPtr;
}


} // namespace iprm


