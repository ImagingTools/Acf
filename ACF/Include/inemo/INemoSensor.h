#ifndef INemoSensor_h_included
#define INemoSensor_h_included


#include "inemo/inemo.h"


#include "Persist/SerializableInterface.h"

#include "Base/NamedInterface.h"


namespace inemo
{

/**
	\ingroup inemo

	Common interface of a NEMO sensor.
	This interface provides access to the meta data of the sensor.
*/
class INemoSensor: virtual public acf::SerializableInterface, virtual public acf::NamedInterface
{
};


} // namespace inemo


#endif // !INemoSensor_h_included
