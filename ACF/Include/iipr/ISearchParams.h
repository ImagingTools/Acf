#ifndef iipr_IISearchParams_included
#define iipr_IISearchParams_included


#include "q2d/CSearchArea.h"

#include "iser/ISerializable.h"

#include "qclb/IIsomorphCalib.h"


namespace iipr
{


class ISearchModel;


/**	
	This interface is common for all classes,
	which implements a set of parameters for a model search.
*/
class ISearchParams: virtual public iser::ISerializable
{
public:
	virtual void SetCalibrationPtr(const qclb::IIsomorphCalib* calibrationPtr) = 0;

	/*
		Gets the search area. 
	*/
	virtual q2d::CSearchArea GetSearchArea(const qclb::IIsomorphCalib* calibrationPtr = NULL) const = 0;
		
	/*
		Resets all parameters to their default values.
	*/
	virtual void ResetParams() = 0;

	/**	
		Get search model.
	*/
	virtual const ISearchModel& GetModel() const = 0;

	/**	
		Creates an internal search model from the image \c modelImage.
	*/
	virtual bool CreateModel(const iimg::IBitmap& inputImage) = 0;

	/**	
		Returns a \c true if the search model exists.
	*/
	virtual bool IsModelCreated() const = 0;

	/**	
		Returns a \c true if the search model is preprocessed.
	*/
	virtual bool IsModelPreprocessed() const = 0;

	/** 
		Returns the model image. 
	*/
	virtual const iimg::IBitmap& GetModelImage() const = 0;

	virtual double GetMinScore() const = 0;
	virtual void SetMinScore(double minScore) = 0;
	virtual double GetMinAngle() const = 0;
	virtual void SetMinAngle(double minAngle) = 0;
	virtual double GetMaxAngle() const = 0;
	virtual void SetMaxAngle(double maxAngle) = 0;
	virtual double GetMinScale() const = 0;
	virtual void SetMinScale(double minScale) = 0;
	virtual double GetMaxScale() const = 0;
	virtual void SetMaxScale(double maxScale) = 0;
	virtual int GetMatchesCount() const = 0;
	virtual void SetMatchesCount(int matchesCount) = 0;
};


} // namespace iipr


#endif // !iipr_IISearchParams_included



