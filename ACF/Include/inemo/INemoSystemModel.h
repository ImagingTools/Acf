#ifndef inemo_INemoSystemModel_included
#define inemo_INemoSystemModel_included


#include "inemo/inemo.h"

#include "Base/NamedInterface.h"


namespace inemo
{


class INemoSensors;

/**
	\ingroup inemo

	INemoSystemModel is a high level interface to access the data in the NEMO database.	
	The system model reflects all changes in the nemo database.
	All NEMO view components are direct or indirect observers of this model.
*/
class INemoSystemModel: virtual public acf::PolymorphicInterface
{
public:
	virtual inemo::INemoSensors* GetNemoSensorsModel() const = 0;
};


} // namespace inemo


#endif // !inemo_INemoSystemModel_included
