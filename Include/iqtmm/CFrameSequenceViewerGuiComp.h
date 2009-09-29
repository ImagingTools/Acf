#ifndef iqtmm_CFrameSequenceViewerGuiComp_included
#define iqtmm_CFrameSequenceViewerGuiComp_included


// Qt includes
#include <QTimer>


// ACF inlcudes
#include "iser/IFileLoader.h"

#include "imod/TModelWrap.h"

#include "iprm/IFileNameParam.h"

#include "imm/IVideoController.h"

#include "iqt/CBitmap.h"
#include "iqt/CFileList.h"

#include "iqtgui/TDesignerGuiCompBase.h"

#include "generated/ui_CFrameSequenceViewerGuiComp.h"


namespace iqtmm
{


/**
	Component to play some image (frame) sequence from a directory.
*/
class CFrameSequenceViewerGuiComp:
			public iqtgui::TDesignerGuiCompBase<Ui::CFrameSequenceViewerGuiComp>,
			virtual public imm::IVideoController
{
	Q_OBJECT
public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CFrameSequenceViewerGuiComp> BaseClass;

	I_BEGIN_COMPONENT(CFrameSequenceViewerGuiComp);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_REGISTER_INTERFACE(imm::IMediaController);
		I_REGISTER_INTERFACE(imm::IVideoInfo);
		I_REGISTER_INTERFACE(imm::IVideoController);
		I_ASSIGN(m_framesPerSecondAttrPtr, "FramesPerSecond", "Default number of frames per second if this info is unavailable from video", true, 25.0);
		I_ASSIGN(m_frameLoaderCompPtr, "FrameLoader", "Loader for the single frame", true, "FrameLoader");
		I_ASSIGN(m_imageObserverCompPtr, "FrameView", "View for a single frame", true, "FrameView");
		I_ASSIGN(m_imageViewCompPtr, "FrameView", "View for a single frame", true, "FrameView");
		I_ASSIGN(m_directoryParamsObserverCompPtr, "SequenceDirectorySelector", "Observer for sequence directory", false, "SequenceDirectorySelector");
		I_ASSIGN(m_directoryParamsGuiCompPtr, "SequenceDirectorySelector", "Sequence directory selection GUI", false, "SequenceDirectorySelector");
		I_ASSIGN(m_directoryParamsCompPtr, "SequenceDirectory", "Sequence Directory", true, "SequenceDirectory");
	I_END_COMPONENT();

	CFrameSequenceViewerGuiComp();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

	// reimplemented (imm::IMediaController)
	virtual istd::CString GetOpenedMediumUrl() const;
	virtual bool OpenMediumUrl(const istd::CString& url, bool autoPlay = true);
	virtual void CloseMedium();
	virtual bool IsPlaying() const;
	virtual bool SetPlaying(bool state = true);
	virtual double GetMediumLength() const;
	virtual double GetCurrentPosition() const;
	virtual bool SetCurrentPosition(double position);

	// reimplemented (imm::IVideoInfo)
	virtual int GetFramesCount() const;
	virtual double GetFrameTimeDiff() const;
	virtual istd::CIndex2d GetFrameSize() const;
	virtual double GetPixelAspectRatio() const;

	// reimplemented (imm::IVideoController)
	virtual int GetCurrentFrame() const;
	virtual bool SetCurrentFrame(int frameIndex);
	virtual bool GrabFrame(iimg::IBitmap& result, int frameIndex = -1) const;

private Q_SLOTS:
	void OnTimerTick();

private:
	int m_currentFrameIndex;

	istd::CString m_mediumUrl;

	QTimer m_playTimer;

	imod::TModelWrap<iqt::CBitmap> m_currentFrame;

	istd::CString m_currentDirectory;
	iqt::CFileList m_fileList;

	I_ATTR(istd::CString, m_defaultSequenceDirectoryAttrPtr);
	I_ATTR(double, m_framesPerSecondAttrPtr);
	I_REF(iser::IFileLoader, m_frameLoaderCompPtr);
	I_REF(imod::IObserver, m_imageObserverCompPtr);
	I_REF(iqtgui::IGuiObject, m_imageViewCompPtr);
	I_REF(imod::IObserver, m_directoryParamsObserverCompPtr);
	I_REF(iqtgui::IGuiObject, m_directoryParamsGuiCompPtr);
	I_REF(iprm::IFileNameParam, m_directoryParamsCompPtr);
};


} // namespace iqtmm


#endif // !iqtmm_CFrameSequenceViewerGuiComp_included

