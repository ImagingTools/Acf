#ifndef iipr_CLineProjectionProcessor_included
#define iipr_CLineProjectionProcessor_included


#include "i2d/CLine2d.h"

#include "iproc/TSyncProcessorWrap.h"

#include "iipr/ILineProjectionProcessor.h"
#include "iipr/IProjectionConstraints.h"


namespace iipr
{


class CLineProjectionProcessor:
	virtual public iproc::TSyncProcessorWrap<ILineProjectionProcessor>,
			virtual public IProjectionConstraints
{
public:
	/**
		Do projection along specified line with variable projection size.
	*/
	bool DoAutosizeProjection(
				const iimg::IBitmap& bitmap,
				const i2d::CLine2d& projectionLine,
				CProjectionData& results) const;

	// reimplemented (iipr::ILineProjectionProcessor)
	virtual bool DoProjection(
				const iimg::IBitmap& bitmap,
				const i2d::CLine2d& projectionLine,
				const IProjectionParams* paramsPtr,
				CProjectionData& results);

	// reimplemented (iipr::IProjectionConstraints)
	virtual istd::CRange GetLineWidthRange() const;
	virtual int GetMinProjectionSize() const;
	virtual int GetMaxProjectionSize() const;
	virtual bool IsAutoProjectionSizeSupported() const;
};


} // namespace iipr


#endif // !iipr_CLineProjectionProcessor_included


