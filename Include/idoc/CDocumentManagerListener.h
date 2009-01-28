#ifndef idoc_CDocumentManagerListener_included
#define idoc_CDocumentManagerListener_included


#include "imod/IModelSelection.h"
#include "imod/TSingleModelObserverBase.h"


#include "idoc/IDocumentManager.h"


namespace idoc
{		


class CDocumentManagerListener: 
			public imod::TSingleModelObserverBase<idoc::IDocumentManager>,
			virtual public imod::IModelSelection
{
public:
	typedef imod::TSingleModelObserverBase<idoc::IDocumentManager> BaseClass;

	CDocumentManagerListener();

	// reimplemented (imod::IModelSelection)
	virtual imod::IModel* GetSelectedModel() const;
	virtual void SetSelectedModel(imod::IModel* modelPtr);

	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

private:
	imod::IModel* m_currentDocumentModelPtr;
};


} // namespace idoc


#endif // !idoc_CDocumentManagerListenerComp_included

