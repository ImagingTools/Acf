#ifndef iqtproc_CAcquisitonProcessorGuiComp_included
#define iqtproc_CAcquisitonProcessorGuiComp_included


// Qt includes
#include <QTimer>


// ACF includes
#include "iser/IFileLoader.h"

#include "imod/IModel.h"
#include "imod/IObserver.h"

#include "iprm/IParamsSet.h"

#include "iqtgui/IGuiObject.h"
#include "iqtgui/TDesignerGuiCompBase.h"

#include "isys/ITimer.h"

#include "iproc/IProcessor.h"

#include "iqtproc/Generated/ui_CAcquisitonProcessorGuiComp.h"


namespace iqtproc
{


class CAcquisitonProcessorGuiComp: public iqtgui::TDesignerGuiCompBase<Ui::CAcquisitonProcessorGuiComp, QWidget>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CAcquisitonProcessorGuiComp, QWidget> BaseClass;

	I_BEGIN_COMPONENT(CAcquisitonProcessorGuiComp)
		I_ASSIGN(m_processingDataCompPtr, "ProcessingData", "Processing data that will be shown", true, "ProcessingData");
		I_ASSIGN(m_processingDataModelCompPtr, "ProcessingData", "Processing data that will be shown", true, "ProcessingData");
		I_ASSIGN(m_processingDataAcquisitionCompPtr, "ProcessingDataAcquisition", "Processing data acquision object", false, "ProcessingDataAcquisition");
		I_ASSIGN(m_processingDataLoaderCompPtr, "ProcessingDataLoader", "Saves processing data to file", false, "ProcessingDataLoader");
		I_ASSIGN(m_paramsLoaderCompPtr, "ParamsLoader", "Loads and saves parameters from and to file", false, "ParamsLoader");
		I_ASSIGN(m_paramsSetCompPtr, "ParamsSet", "Parameters set", false, "ParamsSet");
		I_ASSIGN(m_paramsSetModelCompPtr, "ParamsSet", "Parameters set", false, "ParamsSet");
		I_ASSIGN(m_paramsSetGuiCompPtr, "ParamsSetGui", "Shows parameter set", false, "ParamsSetGui");
		I_ASSIGN(m_paramsSetObserverCompPtr, "ParamsSetGui", "Shows parameter set", false, "ParamsSetGui");
		I_ASSIGN(m_liveIntervalAttrPtr, "LiveInterval", "Interval (in seconds) of acquisition in continuous mode", true, 0.04);
	I_END_COMPONENT

	CAcquisitonProcessorGuiComp();

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

protected slots:
	void on_DataAcquisitionButton_clicked();
	void on_LiveDataAcquisitionButton_toggled(bool checked);
	void on_SaveDataButton_clicked();
	void on_LoadParamsButton_clicked();
	void on_SaveParamsButton_clicked();
	void OnTimerReady();

protected:
	bool DoDataAcquisition();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
	I_REF(istd::IChangeable, m_processingDataCompPtr);
	I_REF(imod::IModel, m_processingDataModelCompPtr);
	I_REF(iproc::IProcessor, m_processingDataAcquisitionCompPtr);

	I_REF(iser::IFileLoader, m_processingDataLoaderCompPtr);
	I_REF(iser::IFileLoader, m_paramsLoaderCompPtr);

	I_REF(iprm::IParamsSet, m_paramsSetCompPtr);
	I_REF(imod::IModel, m_paramsSetModelCompPtr);
	I_REF(iqtgui::IGuiObject, m_paramsSetGuiCompPtr);
	I_REF(imod::IObserver, m_paramsSetObserverCompPtr);

	I_ATTR(double, m_liveIntervalAttrPtr);

	QTimer m_timer;
};


} // namespace iqtproc


#endif // !iqtproc_CAcquisitonProcessorGuiComp_included


