#ifndef iqt2d_CSceneBinderComp_included
#define iqt2d_CSceneBinderComp_included


#include "icomp/CComponentBase.h"

#include "iqt2d/ISceneProvider.h"
#include "iqt2d/ISceneExtender.h"


namespace iqt2d
{


/**
	Binder between scene and shapes creator provider.
*/
class CSceneBinderComp: public icomp::CComponentBase
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CSceneBinderComp);
		I_ASSIGN(m_sceneProviderCompPtr, "SceneProvider", "A graphical scene provider", true, "SceneProvider");
		I_ASSIGN(m_extenderCompPtr, "SceneExtender", "Shapes provider for the graphical scene", true, "SceneExtender");
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_REF(ISceneProvider, m_sceneProviderCompPtr);
	I_REF(ISceneExtender, m_extenderCompPtr);
};


} // namespace iqt2d


#endif // !iqt2d_CSceneBinderComp_included


