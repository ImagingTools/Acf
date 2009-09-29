#include "iqtmm/CFrameSequenceViewerGuiComp.h"


// Qt includes
#include <QCoreApplication>
#include <QEventLoop>
#include <QUrl>


// ACF includes
#include "istd/TChangeNotifier.h"


namespace iqtmm
{


// public methods

CFrameSequenceViewerGuiComp::CFrameSequenceViewerGuiComp()
:	m_currentFrameIndex(0)
{
}


// reimplemented (iqtgui::CGuiComponentBase)

void CFrameSequenceViewerGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (m_imageObserverCompPtr.IsValid() && m_imageViewCompPtr.IsValid()){
		if (m_imageViewCompPtr->CreateGui(FrameView)){
			m_currentFrame.AttachObserver(m_imageObserverCompPtr.GetPtr());
		}
	}

	if (m_directoryParamsCompPtr.IsValid()){
		if (m_directoryParamsObserverCompPtr.IsValid() && m_directoryParamsGuiCompPtr.IsValid()){
			if (m_directoryParamsGuiCompPtr->CreateGui(DirectorySelectorFrame)){
				imod::IModel* directoryModelPtr = dynamic_cast<imod::IModel*>(m_directoryParamsCompPtr.GetPtr());
				if (directoryModelPtr != NULL){
					directoryModelPtr->AttachObserver(m_directoryParamsObserverCompPtr.GetPtr());
				}
			}
		}

		OpenMediumUrl(m_directoryParamsCompPtr->GetPath(), false);

		connect(&m_playTimer, SIGNAL(timeout()), this, SLOT(OnTimerTick()));
	}
}


void CFrameSequenceViewerGuiComp::OnGuiDestroyed()
{
	CloseMedium();

	if (m_directoryParamsCompPtr.IsValid()){
		if (m_directoryParamsObserverCompPtr.IsValid() && m_directoryParamsGuiCompPtr.IsValid()){
			m_directoryParamsGuiCompPtr->DestroyGui();
			imod::IModel* directoryModelPtr = dynamic_cast<imod::IModel*>(m_directoryParamsCompPtr.GetPtr());
			if (directoryModelPtr != NULL){
				directoryModelPtr->DetachObserver(m_directoryParamsObserverCompPtr.GetPtr());
			}
		}
	}

	BaseClass::OnGuiDestroyed();
}


// reimplemented (imm::IMediaController)

istd::CString CFrameSequenceViewerGuiComp::GetOpenedMediumUrl() const
{
	return m_mediumUrl;
}


bool CFrameSequenceViewerGuiComp::OpenMediumUrl(const istd::CString& url, bool autoPlay)
{
	istd::CChangeNotifier notifier(this, CF_STATUS);

	m_mediumUrl = url;

	istd::CStringList fileExtensions;
	
	m_frameLoaderCompPtr->GetFileExtensions(fileExtensions);

	QStringList fileFilter;
	for (int fileExtensionIndex = 0; fileExtensionIndex < int(fileExtensions.size()); fileExtensionIndex++){
		fileFilter.push_back(QString("*.") + iqt::GetQString(fileExtensions[fileExtensionIndex]));
	}
	
	m_fileList.Create(iqt::GetQString(m_mediumUrl), false, fileFilter);

	SetPlaying(autoPlay);

	return true;
}


void CFrameSequenceViewerGuiComp::CloseMedium()
{
	istd::CChangeNotifier notifier(this, CF_STATUS);

	SetPlaying(false);
}


bool CFrameSequenceViewerGuiComp::IsPlaying() const
{
	return m_playTimer.isActive();
}


bool CFrameSequenceViewerGuiComp::SetPlaying(bool state)
{
	istd::CChangeNotifier notifier(this, CF_STATUS);

	if (state){
		int timerIntervall = 40;
		
		if (m_framesPerSecondAttrPtr.IsValid()){
			timerIntervall = 1000 / *m_framesPerSecondAttrPtr;
		}

		m_playTimer.start(timerIntervall);
	}
	else{
		m_playTimer.stop();
	}

	return true;
}


double CFrameSequenceViewerGuiComp::GetMediumLength() const
{
	return 0;
}


double CFrameSequenceViewerGuiComp::GetCurrentPosition() const
{
	return m_currentFrameIndex;
}


bool CFrameSequenceViewerGuiComp::SetCurrentPosition(double position)
{
	if (fabs(position - GetCurrentPosition()) < I_BIG_EPSILON){
		return true;
	}

	istd::CChangeNotifier notifier(this, CF_POSITION);

	m_currentFrameIndex = position;

	return true;
}


// reimplemented (imm::IVideoInfo)

int CFrameSequenceViewerGuiComp::GetFramesCount() const
{
	return m_fileList.count();
}


double CFrameSequenceViewerGuiComp::GetFrameTimeDiff() const
{
	return 1.0 / *m_framesPerSecondAttrPtr;
}


istd::CIndex2d CFrameSequenceViewerGuiComp::GetFrameSize() const
{
	return istd::CIndex2d::GetInvalid();
}


double CFrameSequenceViewerGuiComp::GetPixelAspectRatio() const
{
	return 1;
}


// reimplemented (imm::IVideoController)

int CFrameSequenceViewerGuiComp::GetCurrentFrame() const
{
	return int(GetCurrentPosition() * *m_framesPerSecondAttrPtr + 0.5);
}


bool CFrameSequenceViewerGuiComp::SetCurrentFrame(int frameIndex)
{
	return SetCurrentPosition(frameIndex / *m_framesPerSecondAttrPtr);
}


bool CFrameSequenceViewerGuiComp::GrabFrame(iimg::IBitmap& /*result*/, int /*frameIndex*/) const
{
	return false;
}


// private slots:

void CFrameSequenceViewerGuiComp::OnTimerTick()
{
	if (m_frameLoaderCompPtr.IsValid() && GetFramesCount() > 0){
		int position = m_currentFrameIndex++ % m_fileList.count();

		istd::CChangeNotifier changePtr(&m_currentFrame);

		QString currentFile = m_fileList[position];

		m_frameLoaderCompPtr->LoadFromFile(m_currentFrame, iqt::GetCString(currentFile));
	}
}


} // namespace iqtmm


