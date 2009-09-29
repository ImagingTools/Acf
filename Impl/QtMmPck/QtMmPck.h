#ifndef QtMmPck_included
#define QtMmPck_included


#include "icomp/TModelCompWrap.h"

#include "iqtmm/CPhononVideoViewGuiComp.h"
#include "iqtmm/CMediaLoaderComp.h"
#include "iqtmm/CFrameSequenceViewerGuiComp.h"
#include "iqtmm/CPlaybackControllerGuiComp.h"


/**
	Qt based multimedia package.
*/
namespace QtMmPck
{


typedef iqtmm::CPhononVideoViewGuiComp PhononVideoView;
typedef iqtmm::CMediaLoaderComp MediaLoader;
typedef icomp::TModelCompWrap<iqtmm::CFrameSequenceViewerGuiComp> FrameSequenceViewer;
typedef iqtmm::CPlaybackControllerGuiComp PlaybackController;


} // namespace QtMmPck


#endif // !QtMmPck_included


