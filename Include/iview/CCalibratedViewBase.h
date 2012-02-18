#ifndef iview_CCalibratedViewBase_included
#define iview_CCalibratedViewBase_included


#include "iview/ICalibration.h"
#include "iview/IVisualCalibrationInfo.h"
#include "iview/CNoneCalibration.h"
#include "iview/CViewBase.h"


namespace iview
{


/**
	Abstract base class for all calibrated view.
	Calibrated view implements iview::ICalibrated interface,
	and can automatically show calibration grid if supported.
*/
class CCalibratedViewBase: 
			public iview::CViewBase,
			virtual public IVisualCalibrationInfo
{
public:
	typedef iview::CViewBase BaseClass;

	enum LayerType2
	{
		LT_CALIBRATION = 0x100
	};

	CCalibratedViewBase();

	/**
		Set calibration object for this view.
	*/
	void SetCalibrationPtr(const ICalibration* calibrationPtr);

	/**
		Set grid visibility state.
	*/
	virtual void SetGridVisible(bool state = true);

	/**
		Connect visualisation shape for calibration object.
	*/
	virtual void ConnectCalibrationShape(iview::IShape* shapePtr);

	/**
		Set minimal grid size.
		It determines minimal distance between grid lines.
	*/
	void SetMinGridDistance(double gridDistance);

	/**
		Set if grid should be shown in milimeter.
	*/
	void SetGridInMm(bool state = true);

	const iview::CSingleLayer& GetCalibrationLayer() const;

	// reimplemented (iview::CViewBase)
	virtual void UpdateAllShapes(int changeFlag);
	virtual void InsertDefaultLayers();

	// reimplemented (iview::IShapeView)
	virtual int InsertLayer(iview::ILayer* layerPtr, int index = -1, int layerType = LT_NONE);
	virtual void RemoveLayer(int index);

	// reimplemented (iview::IVisualCalibrationInfo)
	virtual bool IsGridVisible() const;
	virtual double GetMinGridDistance() const;
	virtual bool IsGridInMm() const;

	// reimplemented (iview::ICalibrated)
	virtual const ICalibration& GetCalibration() const;

	using BaseClass::InvalidateBackground;

private:
	const ICalibration* m_calibrationPtr;
	bool m_isGridVisible;
	bool m_isGridInMm;
	double m_minGridDistance;

	int m_calibrationLayerIndex;

	// default layers
	iview::CSingleLayer m_calibrationLayer;
};


// inline methods

inline 	void CCalibratedViewBase::SetCalibrationPtr(const ICalibration* calibrationPtr)
{
	if (calibrationPtr != m_calibrationPtr){
		if (calibrationPtr != NULL){
			m_calibrationPtr = calibrationPtr;
		}
		else{
			m_calibrationPtr = &CNoneCalibration::GetInstance();
		}

		InvalidateBackground();
	}

	UpdateAllShapes(iview::IShape::CF_CALIB);
}


inline void CCalibratedViewBase::SetGridVisible(bool state)
{
	if (m_isGridVisible != state){
		m_isGridVisible = state;
		InvalidateBackground();
	}
}


inline void CCalibratedViewBase::SetMinGridDistance(double gridDistance)
{
	if (m_minGridDistance != gridDistance){
		m_minGridDistance = gridDistance;
		if (m_isGridVisible){
			SetBackgroundBufferValid(false);
		}
	}
}


inline void CCalibratedViewBase::SetGridInMm(bool state)
{
	if (m_isGridInMm != state){
		m_isGridInMm = state;
		SetBackgroundBufferValid(false);
	}
}


inline const iview::CSingleLayer& CCalibratedViewBase::GetCalibrationLayer() const
{
	return m_calibrationLayer;
}


inline bool CCalibratedViewBase::IsGridVisible() const
{
	return m_isGridVisible;
}


inline double CCalibratedViewBase::GetMinGridDistance() const
{
	return m_minGridDistance;
}


inline bool CCalibratedViewBase::IsGridInMm() const
{
	return m_isGridInMm;
}


// reimplemented (iview::ICalibrated)

inline const ICalibration& CCalibratedViewBase::GetCalibration() const
{
	return *m_calibrationPtr;
}



} // namespace iview



#endif // !iview_CCalibratedViewBase_included




