#include "iqtproc/CAcquisitonProcessorGuiComp.h"


// Qt includes
#include <QMessageBox>

#include "istd/TDelPtr.h"


namespace iqtproc
{


CAcquisitonProcessorGuiComp::CAcquisitonProcessorGuiComp()
{
	m_timer.setInterval(40);
	QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(OnTimerReady()));
}


// reimplemented (icomp::IComponent)

void CAcquisitonProcessorGuiComp::OnComponentCreated()
{
	I_ASSERT(m_liveIntervalAttrPtr.IsValid());

	m_timer.setInterval(int(*m_liveIntervalAttrPtr * 1000));
}


// protected slots

void CAcquisitonProcessorGuiComp::on_DataAcquisitionButton_clicked()
{
	LiveDataAcquisitionButton->setChecked(false);

	DoDataAcquisition();
}


void CAcquisitonProcessorGuiComp::on_LiveDataAcquisitionButton_toggled(bool checked)
{
	if (checked){
		m_timer.start();
	}
	else{
		m_timer.stop();
	}
}


void CAcquisitonProcessorGuiComp::on_SaveDataButton_clicked()
{
	if (m_processingDataLoaderCompPtr.IsValid() && m_processingDataCompPtr.IsValid()){
		if (m_processingDataLoaderCompPtr->SaveToFile(*m_processingDataCompPtr, "") == iser::IFileLoader::StateFailed){
			QMessageBox::information(
						NULL,
						QObject::tr("Error"),
						QObject::tr("Cannot save image"));
		}
	}
}


void CAcquisitonProcessorGuiComp::on_LoadParamsButton_clicked()
{
	if (m_paramsLoaderCompPtr.IsValid() && m_paramsSetCompPtr.IsValid()){
		if (m_paramsLoaderCompPtr->LoadFromFile(*m_paramsSetCompPtr, "") == iser::IFileLoader::StateFailed){
			QMessageBox::information(
						NULL,
						QObject::tr("Error"),
						QObject::tr("Cannot load parameters"));
		}
	}
}


void CAcquisitonProcessorGuiComp::on_SaveParamsButton_clicked()
{
	if (m_paramsLoaderCompPtr.IsValid() && m_paramsSetCompPtr.IsValid()){
		if (m_paramsLoaderCompPtr->SaveToFile(*m_paramsSetCompPtr, "") == iser::IFileLoader::StateFailed){
			QMessageBox::information(
						NULL,
						QObject::tr("Error"),
						QObject::tr("Cannot save parameters"));
		}
	}
}


void CAcquisitonProcessorGuiComp::OnTimerReady()
{
	DoDataAcquisition();
}


// protected methods

bool CAcquisitonProcessorGuiComp::DoDataAcquisition()
{
	if (m_processingDataAcquisitionCompPtr.IsValid() && m_processingDataCompPtr.IsValid()){
		int taskId = m_processingDataAcquisitionCompPtr->BeginTask(m_paramsSetCompPtr.GetPtr(), NULL, m_processingDataCompPtr.GetPtr());
		if (taskId >= 0){
			return m_processingDataAcquisitionCompPtr->WaitTaskFinished(-1, 1) != iproc::IProcessor::TS_INVALID;
		}
	}

	return false;
}


// reimplemented (iqtgui::CGuiComponentBase)

void CAcquisitonProcessorGuiComp::OnGuiCreated()
{
	bool hasProcessingData = m_processingDataCompPtr.IsValid();
	bool hasProcessor = m_processingDataAcquisitionCompPtr.IsValid();

	DataAcquisitionButton->setVisible(hasProcessingData && hasProcessor);
	LiveDataAcquisitionButton->setVisible(hasProcessingData && hasProcessor);
	SaveDataButton->setVisible(hasProcessingData && m_processingDataLoaderCompPtr.IsValid());

	bool areParamsEditable = false;
	if (m_paramsSetModelCompPtr.IsValid() && m_paramsSetGuiCompPtr.IsValid() && m_paramsSetObserverCompPtr.IsValid()){
		m_paramsSetModelCompPtr->AttachObserver(m_paramsSetObserverCompPtr.GetPtr());
		m_paramsSetGuiCompPtr->CreateGui(ParamsFrame);

		areParamsEditable = true;
	}

	ParamsGB->setVisible(
				m_paramsSetCompPtr.IsValid() &&
				(areParamsEditable || m_paramsLoaderCompPtr.IsValid()));
	LoadParamsButton->setVisible(m_paramsLoaderCompPtr.IsValid());
	SaveParamsButton->setVisible(m_paramsLoaderCompPtr.IsValid());
	ParamsFrame->setVisible(m_paramsSetCompPtr.IsValid() && areParamsEditable);
}


void CAcquisitonProcessorGuiComp::OnGuiDestroyed()
{
	m_timer.stop();

	if (		m_paramsSetModelCompPtr.IsValid() &&
				m_paramsSetObserverCompPtr.IsValid() &&
				m_paramsSetModelCompPtr->IsAttached(m_paramsSetObserverCompPtr.GetPtr())){
		m_paramsSetModelCompPtr->DetachObserver(m_paramsSetObserverCompPtr.GetPtr());
	}

	if (m_paramsSetGuiCompPtr.IsValid() && m_paramsSetGuiCompPtr->IsGuiCreated()){
		m_paramsSetGuiCompPtr->DestroyGui();
	}
}


} // namespace iqtproc


