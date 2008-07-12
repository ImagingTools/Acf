#ifndef iqtcam_CExposureParamsGuiComp_included
#define iqtcam_CExposureParamsGuiComp_included


#include "icam/IExposureParams.h"

#include "iqt/TDesignerGuiObserverCompBase.h"

#include "iqtcam/Generated/ui_CExposureParamsGuiComp.h"


namespace iqtcam
{


class CExposureParamsGuiComp: public iqt::TDesignerGuiObserverCompBase<
			Ui::CExposureParamsGuiComp,
			icam::IExposureParams>
{
	Q_OBJECT

public:
	typedef iqt::TDesignerGuiObserverCompBase<
				Ui::CExposureParamsGuiComp,
				icam::IExposureParams> BaseClass;

	I_BEGIN_COMPONENT(CExposureParamsGuiComp)
	I_END_COMPONENT

	// reimplemented (iqt::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor();

protected slots:
	void OnParamsChanged(double value);
};


} // namespace iqtcam


#endif // !iqtcam_CExposureParamsGuiComp_included


