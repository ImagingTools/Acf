#ifndef inemo_CSensorSpecification_included
#define inemo_CSensorSpecification_included


#include "Model/Model.h"


namespace inemo
{


/**
	\ingroup inemo

	Implementation of sensor specification data record.
*/
class CSensorSpecification: public acf::Model
{
public:
	typedef acf::Model BaseClass;

	CSensorSpecification();
};


} // namespace inemo


#endif // !inemo_CSensorSpecification_included
