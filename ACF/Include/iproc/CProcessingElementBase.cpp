#include "iproc/CProcessingElementBase.h"


namespace iproc
{		


// reimplemented (iproc::IOperator)

bool CProcessingElementBase::Execute()
{
	int processingState = GetProcessingState();

	if (processingState == iproc::IOperator::StateProcessing || IsAborted()){
		return false;
	}

	SetProcessingState(iproc::IOperator::StateProcessing);

	bool result = DoProcessing();

	if (!IsAborted()){
		SetProcessingState(iproc::IOperator::StateFinished);
	}

	return result && !IsAborted();
}


// reimplemented (iser::ISerializable)

bool CProcessingElementBase::Serialize(iser::IArchive& archive)
{
	bool retVal = SerializeName(archive);

	retVal = retVal && SerializeEnabled(archive);

	return retVal;
}


} // namespace iproc

