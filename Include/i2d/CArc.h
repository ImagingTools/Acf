// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// Acf includes
#include <i2d/CCircle.h>


namespace i2d
{


/**
	Represents a circular arc defined by a center point, radius, and start/end angles.
	An arc is a portion of a circle's circumference between two angles.
*/
class CArc: public CCircle
{
public:
	typedef CCircle BaseClass;

	/**
		Construct an empty arc object.
	*/
	CArc();
	/**
		Construct an arc with specified parameters.
		\param	startAngle	Starting angle in radians.
		\param	endAngle	Ending angle in radians.
		\param	radius		Radius of the arc.
		\param	center		Center point of the arc.
	*/
	CArc(double startAngle, double endAngle, const double radius, const CVector2d& center);

	/**
		Get the starting angle of the arc.
		\return	Starting angle in radians.
	*/
	double GetStartAngle() const;
	/**
		Set the starting angle of the arc.
		\param	angle	Starting angle in radians.
	*/
	void SetStartAngle(double angle);

	/**
		Get the ending angle of the arc.
		\return	Ending angle in radians.
	*/
	double GetEndAngle() const;
	/**
		Set the ending angle of the arc.
		\param	angle	Ending angle in radians.
	*/
	void SetEndAngle(double angle);

	/**
		Compare this arc with another arc for equality.
		\param	arc	Arc to compare with.
		\return	true if arcs are equal, false otherwise.
	*/
	bool operator==(const CArc& arc) const;
	/**
		Compare this arc with another arc for inequality.
		\param	arc	Arc to compare with.
		\return	true if arcs are not equal, false otherwise.
	*/
	bool operator!=(const CArc& arc) const;

	/**
	Get position of point using normed 'alpha' value.
	Value 0 will be mapped to point 1 and value 1 will be mapped to point 2.
	*/
	CVector2d GetPositionFromAlpha(double alpha) const;

	// reimplemented (i2d::IObject2d)
	virtual CRectangle GetBoundingBox() const override;
	virtual bool Transform(
				const ITransformation2d& transformation,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) override;
	virtual bool InvTransform(
				const ITransformation2d& transformation,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) override;
	virtual bool GetTransformed(
				const ITransformation2d& transformation,
				IObject2d& result,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const override;
	virtual bool GetInvTransformed(
				const ITransformation2d& transformation,
				IObject2d& result,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const override;

	// reimplemented istd::IChangeable
	virtual int GetSupportedOperations() const override;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual istd::TUniqueInterfacePtr<istd::IChangeable> CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

protected:
	double m_startAngle;
	double m_endAngle;
};


// public inline methods

inline double CArc::GetStartAngle() const
{
	return m_startAngle;
}


inline double CArc::GetEndAngle() const
{
	return m_endAngle;
}


} // namespace i2d




