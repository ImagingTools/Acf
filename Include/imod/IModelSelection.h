#ifndef imod_IModelSelection_included
#define imod_IModelSelection_included


#include "istd/IChangeable.h"

#include "imod/IModel.h"


namespace imod
{


/**
	
*/
class IModelSelection: virtual public istd::IChangeable
{
public:
	virtual imod::IModel* GetSelectedModel() const = 0;
	virtual void SetSelectedModel(imod::IModel* modelPtr) = 0;
};


} // namespace imod


#endif // !imod_IModelSelection_included


