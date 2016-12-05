#include <iprm/CSelectableParamsSetComp.h>


// ACF includes
#include <istd/CChangeNotifier.h>

#include <imod/IModel.h>


namespace iprm
{


// static constants
static const istd::IChangeable::ChangeSet s_selectionChangeSet(ISelectionParam::CF_SELECTION_CHANGED);
static const iser::CArchiveTag s_selectedIndexTag("Selected", "Selected index", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_paramsManagerTag("Parameters", "All parameters", iser::CArchiveTag::TT_GROUP);


CSelectableParamsSetComp::CSelectableParamsSetComp()
:	m_updateBridge(this)
{
}


// reimplemented (iprm::IParamsSet)

IParamsSet::Ids CSelectableParamsSetComp::GetParamIds(bool editableOnly) const
{
	Ids retVal;

	if (!editableOnly){
		if (m_selectionParamIdAttrPtr.IsValid()){
			const QByteArray& selectionId = *m_selectionParamIdAttrPtr;
			retVal += selectionId;
		}

		if (m_paramsManagerCompPtr.IsValid() && m_currentSelectionCompPtr.IsValid()){
			int selectedIndex = m_currentSelectionCompPtr->GetSelectedOptionIndex();
			if ((selectedIndex >= 0) && (selectedIndex < m_paramsManagerCompPtr->GetParamsSetsCount())){
				const iprm::IParamsSet* paramsPtr = m_paramsManagerCompPtr->GetParamsSet(selectedIndex);
				if (paramsPtr != NULL){
					retVal += paramsPtr->GetParamIds(false);
				}
			}
		}
	}

	return retVal;
}


const iser::ISerializable* CSelectableParamsSetComp::GetParameter(const QByteArray& id) const
{
	if (m_selectionParamIdAttrPtr.IsValid()){
		const QByteArray& selectionId = *m_selectionParamIdAttrPtr;
		if (id == selectionId){
			return m_currentSelectionCompPtr.GetPtr();
		}
	}

	if (m_paramsManagerCompPtr.IsValid() && m_currentSelectionCompPtr.IsValid()){
		int selectedIndex = m_currentSelectionCompPtr->GetSelectedOptionIndex();
		if ((selectedIndex >= 0) && (selectedIndex < m_paramsManagerCompPtr->GetParamsSetsCount())){
			const iprm::IParamsSet* paramsPtr = m_paramsManagerCompPtr->GetParamsSet(selectedIndex);
			if (paramsPtr != NULL){
				return paramsPtr->GetParameter(id);
			}
		}
	}

	return NULL;
}


iser::ISerializable* CSelectableParamsSetComp::GetEditableParameter(const QByteArray& /*id*/)
{
	return NULL;
}


// reimplemented (iser::ISerializable)

bool CSelectableParamsSetComp::Serialize(iser::IArchive& /*archive*/)
{
	return false;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CSelectableParamsSetComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_currentSelectionModelCompPtr.IsValid()){
		m_currentSelectionModelCompPtr->AttachObserver(&m_updateBridge);
	}

	if (m_paramsManagerModelCompPtr.IsValid()){
		m_paramsManagerModelCompPtr->AttachObserver(&m_updateBridge);
	}
}


void CSelectableParamsSetComp::OnComponentDestroyed()
{
	m_updateBridge.EnsureModelsDetached();

	BaseClass::OnComponentDestroyed();
}


} // namespace iprm


