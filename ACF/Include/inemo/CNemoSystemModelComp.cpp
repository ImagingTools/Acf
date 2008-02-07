#include "inemo/CNemoSystemModelComp.h"


#include "inemo/INemoSensors.h"


namespace inemo
{


CNemoSystemModelComp::CNemoSystemModelComp()
	:BaseClass(),
	m_sensorsModelIfPtr(this, "SensorModel")
{
	registerInterface<acf::ModelInterface>(this);
}


// reimplemented (inemo::INemoSystemModel)

inemo::INemoSensors* CNemoSystemModelComp::GetNemoSensorsModel() const
{
	return m_sensorsModelIfPtr.getInterface();
}


// reimplemented (acf::ComponentInterface)

bool CNemoSystemModelComp::onInitialize(acf::ComponentManagerInterface* managerPtr)
{
	if (BaseClass::onInitialize(managerPtr)){
		if (m_sensorsModelIfPtr.isValid()){
			acf::ModelInterface* modelPtr = dynamic_cast<acf::ModelInterface*>(m_sensorsModelIfPtr.getInterface());
			if (modelPtr != NULL){
				addChildModel(modelPtr);
			}
		}

		return true;
	}

	return false;
}


} // namespace inemo

