#ifndef iproc_CProcessingElementBase_included
#define iproc_CProcessingElementBase_included


#include "iproc/COperatorBase.h"

#include "iser/ISerializable.h"



namespace iproc
{		


/**
	Basic implementation of an serializable processing element.
*/
class CProcessingElementBase: virtual public iproc::COperatorBase, virtual public iser::ISerializable
{
public:
	// reimplemented (iproc::IOperator)
	virtual bool Execute();

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	virtual bool DoProcessing() = 0;
};


} // namespace iproc


#endif // !iproc_CProcessingElementBase_included

