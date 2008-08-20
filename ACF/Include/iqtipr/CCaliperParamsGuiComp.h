#ifndef iqtipr_CCaliperParamsGuiComp_included
#define iqtipr_CCaliperParamsGuiComp_included


#include "iipr/ICaliperParams.h"

#include "iqt/TDesignerGuiObserverCompBase.h"

#include "iqtipr/Generated/ui_CCaliperParamsGuiComp.h"


namespace iqtipr
{


class CCaliperParamsGuiComp: public iqt::TDesignerGuiObserverCompBase<
			Ui::CCaliperParamsGuiComp,
			iipr::ICaliperParams>
{
	Q_OBJECT

public:
	typedef iqt::TDesignerGuiObserverCompBase<
				Ui::CCaliperParamsGuiComp,
				iipr::ICaliperParams> BaseClass;

	I_BEGIN_COMPONENT(CCaliperParamsGuiComp);
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor();

protected slots:
	void on_ThresholdSB_valueChanged(int value);
	void on_EdgePolarityCB_currentIndexChanged(int index);
	void on_DirectionCB_currentIndexChanged(int index);

private:
	I_ATTR(istd::CString, m_unitNameAttrPtr);
	I_ATTR(double, m_lineZValueAttrPtr);
};


} // namespace iqtipr


#endif // !iqtipr_CCaliperParamsGuiComp_included


