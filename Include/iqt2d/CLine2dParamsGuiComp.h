#ifndef iqt2d_CLine2dParamsGuiComp_included
#define iqt2d_CLine2dParamsGuiComp_included


// ACF includes
#include "i2d/CLine2d.h"

#include "iview/CLineShape.h"

#include "iqt2d/TShapeParamsGuiCompBase.h"
#include "GeneratedFiles/iqt2d/ui_CLine2dParamsGuiComp.h"


namespace iqt2d
{


class CLine2dParamsGuiComp:
			public iqt2d::TShapeParamsGuiCompBase<
						Ui::CLine2dParamsGuiComp,
						iview::CLineShape,
						i2d::CLine2d>
{
	Q_OBJECT

public:
	typedef iqt2d::TShapeParamsGuiCompBase<
				Ui::CLine2dParamsGuiComp,
				iview::CLineShape,
				i2d::CLine2d> BaseClass;

	I_BEGIN_COMPONENT(CLine2dParamsGuiComp);
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();
	virtual void OnGuiRetranslate();

protected Q_SLOTS:
	void OnParamsChanged(double value);
};


} // namespace iqt2d


#endif // !iqt2d_CLine2dParamsGuiComp_included


