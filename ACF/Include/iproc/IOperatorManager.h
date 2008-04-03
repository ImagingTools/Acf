#ifndef iproc_IOperatorManager_included
#define iproc_IOperatorManager_included


#include "istd/IPolymorphic.h"
#include "istd/CString.h"

#include "iproc/IOperator.h"


namespace iproc
{


class IOperatorManager: public istd::IPolymorphic
{
public:
	virtual int GetOperatorCount() const = 0;
	virtual void ExecuteOperator(int index) = 0;
	virtual iproc::IOperator* GetOperator(int index) const = 0;
	virtual istd::CStringList GetSupportedDocuments(int operatorIndex) const = 0;
};


} // namespace acf


#endif // !iproc_IOperatorManager_included
