#ifndef inemo_CNemoSystemModelComp_included
#define inemo_CNemoSystemModelComp_included


#include "inemo/inemo.h"


#include "Model/HierarchicalModelTemplate.h"
#include "Model/MultiModelObserver.h"

#include "Comp/Component.h"
#include "Comp/ComponentDependency.h"
#include "Comp/Attribute.h"

#include "inemo/INemoSystemModel.h"
#include "inemo/INemoSensor.h"


namespace inemo
{


/**
	\ingroup inemo

	Standard implementation of NEMO data model.
*/
class CNemoSystemModelComp: public acf::Component,
							public acf::HierarchicalModelTemplate<inemo::INemoSystemModel>
{
public:
	typedef acf::Component BaseClass;

	CNemoSystemModelComp();

	// reimplemented (inemo::INemoSystemModel)
	virtual inemo::INemoSensors* GetNemoSensorsModel() const;

	// reimplemented (acf::ComponentInterface)
	virtual bool onInitialize(acf::ComponentManagerInterface* managerPtr);

private:
	acf::ComponentDependency<inemo::INemoSensors> m_sensorsModelIfPtr;
};


} // namespace inemo


#endif // !inemo_CNemoSystemModelComp_included