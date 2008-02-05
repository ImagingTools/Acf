#include "iser/CBinaryArchiveBase.h"


#include "istd/CString.h"


namespace iser
{


bool CBinaryArchiveBase::Process(char& value)
{
	return ProcessData(&value, sizeof(char));
}


bool CBinaryArchiveBase::Process(bool& value)
{
	return ProcessData(&value, sizeof(bool));
}


bool CBinaryArchiveBase::Process(int& value)
{
	return ProcessData(&value, sizeof(int));
}


bool CBinaryArchiveBase::Process(double& value)
{
	return ProcessData(&value, sizeof(double));
}


bool CBinaryArchiveBase::Process(short& value)
{
	return ProcessData(&value, sizeof(short));
}


bool CBinaryArchiveBase::Process(float& value)
{
	return ProcessData(&value, sizeof(float));
}


} // namespace iser

