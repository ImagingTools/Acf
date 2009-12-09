#ifndef imm_IVideoController_included
#define imm_IVideoController_included


#include "iimg/IBitmap.h"

#include "imm/IMediaController.h"
#include "imm/IVideoInfo.h"


namespace imm
{


class IVideoController: virtual public IMediaController, virtual public IVideoInfo
{
public:
	enum SupportedFeatures
	{
		SF_GRAB_ANY = 0x100
	};

	/**
		Get index of current frame.
	*/
	virtual int GetCurrentFrame() const = 0;

	/**
		Set index of current frame.
		This works similiar to imm::IMediaController::SetCurrentPosition, but the parameter is frame index.
	*/
	virtual bool SetCurrentFrame(int frameIndex) = 0;
};


} // namespace imm


#endif // !imm_IVideoController_included


