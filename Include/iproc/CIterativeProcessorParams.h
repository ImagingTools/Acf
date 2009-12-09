#ifndef iproc_CIterativeProcessorParams_included
#define iproc_CIterativeProcessorParams_included


#include "iser/ISerializable.h"


namespace iproc
{


/**
	Basic implementation for the local operators with a rectangular kernel
*/
class CIterativeProcessorParams: virtual public iser::ISerializable
{
public:
	CIterativeProcessorParams();

	virtual int GetIterationsCount() const;
	virtual void SetIterationsCount(int iterationsCount);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	int m_iterationsCount;
};


} // namespace iproc


#endif // !iproc_CIterativeProcessorParams_included

