#include "iview/CShapeBase.h"


namespace iview
{


CShapeBase::CShapeBase()
{
	m_modelPtr = NULL;
}


CShapeBase::CShapeBase(const CShapeBase& I_IF_DEBUG(shape))
{
	I_ASSERT(shape.m_modelPtr == NULL);		// Copy constructor not allowed for shapes connected to model 
	m_modelPtr = NULL;
}


CShapeBase::~CShapeBase()
{
	DisconnectDisplay();

	if (m_modelPtr != NULL){
		m_modelPtr->DetachObserver(this);
	}
}


// reimplemented (imod::IObserver)

bool CShapeBase::OnAttached(imod::IModel* modelPtr)
{
	bool retVal = BaseClass::OnAttached(modelPtr);

	Invalidate(CS_CONSOLE);

	return retVal;
}


bool CShapeBase::OnDetached(imod::IModel* modelPtr)
{
	bool retVal = BaseClass::OnDetached(modelPtr);

	Invalidate(CS_CONSOLE);

	return retVal;
}


} // namespace iview


