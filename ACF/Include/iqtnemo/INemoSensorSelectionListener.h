#ifndef iqtnemo_INemoSensorSelectionListener_included
#define iqtnemo_INemoSensorSelectionListener_included


#include "iqtnemo/iqtnemo.h"

#include "Model/ModelInterface.h"

#include "Base/NamedInterface.h"


namespace iqtnemo
{


class INemoSensor;


/**
	\ingroup inemo
*/
class INemoSensorSelectionListener: virtual public acf::PolymorphicInterface
{
public:
	virtual void OnSensorSelected(inemo::INemoSensor* selectedSensorPtr) = 0;
};


} // namespace inemo


#endif // !iqtnemo_INemoSensorSelectionListener_included
