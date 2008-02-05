#ifndef iser_CBinaryArchiveBase_h_included
#define iser_CBinaryArchiveBase_h_included


#include "iser/IArchive.h"


namespace iser
{


class CBinaryArchiveBase: public IArchive
{
public:
	// reimplemented (iser::IArchive)
	virtual bool Process(char& value);
	virtual bool Process(bool& value);
	virtual bool Process(int& value);
	virtual bool Process(double& value);
	virtual bool Process(short& value);
	virtual bool Process(float& value);
};


} // namespace iser


#endif // iser_CBinaryArchiveBase_h_included


