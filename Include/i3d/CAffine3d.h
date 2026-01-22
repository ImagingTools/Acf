// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// ACF includes
#include <i3d/CVector3d.h>
#include <i3d/CMatrix3d.h>
#include <i3d/CQuaternion3d.h>


namespace i3d
{


/**
	Affine transformation in 3D space.
	Combines rotation/scale (matrix) and translation (vector).
	Represents transformations of the form: v' = M * v + t
*/
class CAffine3d
{
public:
	/**
		Default constructor.
		Creates identity transformation.
	*/
	CAffine3d();
	
	/**
		Construct from matrix and translation.
		\param matrix		Rotation/scale matrix.
		\param translation	Translation vector.
	*/
	CAffine3d(const CMatrix3d& matrix, const CVector3d& translation);
	
	/**
		Create identity transformation.
	*/
	static CAffine3d GetIdentity();
	
	/**
		Create translation transformation.
	*/
	static CAffine3d CreateTranslation(const CVector3d& translation);
	
	/**
		Create rotation transformation from quaternion.
	*/
	static CAffine3d CreateRotation(const CQuaternion3d& rotation);
	
	/**
		Create rotation transformation from axis and angle.
		\param axis		Rotation axis.
		\param angle	Rotation angle in radians.
	*/
	static CAffine3d CreateRotation(const CVector3d& axis, double angle);
	
	/**
		Create uniform scale transformation.
	*/
	static CAffine3d CreateScale(double scale);
	
	/**
		Create non-uniform scale transformation.
	*/
	static CAffine3d CreateScale(double sx, double sy, double sz);
	
	/**
		Create scale transformation from vector.
	*/
	static CAffine3d CreateScale(const CVector3d& scale);
	
	/**
		Get matrix part (rotation/scale).
	*/
	const CMatrix3d& GetMatrix() const;
	
	/**
		Set matrix part.
	*/
	void SetMatrix(const CMatrix3d& matrix);
	
	/**
		Get translation part.
	*/
	const CVector3d& GetTranslation() const;
	
	/**
		Set translation part.
	*/
	void SetTranslation(const CVector3d& translation);
	
	/**
		Reset to identity transformation.
	*/
	void Reset();
	
	/**
		Transform a point.
	*/
	CVector3d Transform(const CVector3d& point) const;
	
	/**
		Transform a direction vector (ignores translation).
	*/
	CVector3d TransformDirection(const CVector3d& direction) const;
	
	/**
		Get inverse transformation.
	*/
	CAffine3d GetInverse() const;
	
	/**
		Get inverse transformation.
		\return false if matrix is not invertible.
	*/
	bool GetInverse(CAffine3d& result) const;
	
	/**
		Compose this transformation with another (this = this * other).
	*/
	void Compose(const CAffine3d& other);
	
	/**
		Compose transformations (returns this * other).
	*/
	CAffine3d GetComposed(const CAffine3d& other) const;
	
	/**
		Serialize this transformation to specified archive.
	*/
	bool Serialize(iser::IArchive& archive);
	
	CAffine3d operator*(const CAffine3d& other) const;
	CVector3d operator*(const CVector3d& point) const;
	
	bool operator==(const CAffine3d& other) const;
	bool operator!=(const CAffine3d& other) const;

private:
	CMatrix3d m_matrix;			///< Rotation/scale matrix
	CVector3d m_translation;	///< Translation vector
};


// inline methods

inline CAffine3d::CAffine3d()
:	m_matrix(CMatrix3d::GetIdentity())
,	m_translation(0.0, 0.0, 0.0)
{
}


inline CAffine3d::CAffine3d(const CMatrix3d& matrix, const CVector3d& translation)
:	m_matrix(matrix)
,	m_translation(translation)
{
}


inline CAffine3d CAffine3d::GetIdentity()
{
	return CAffine3d();
}


inline CAffine3d CAffine3d::CreateTranslation(const CVector3d& translation)
{
	return CAffine3d(CMatrix3d::GetIdentity(), translation);
}


inline CAffine3d CAffine3d::CreateRotation(const CQuaternion3d& rotation)
{
	return CAffine3d(rotation.ToMatrix(), CVector3d(0.0, 0.0, 0.0));
}


inline CAffine3d CAffine3d::CreateRotation(const CVector3d& axis, double angle)
{
	return CreateRotation(CQuaternion3d::FromAxisAngle(axis, angle));
}


inline CAffine3d CAffine3d::CreateScale(double scale)
{
	return CreateScale(scale, scale, scale);
}


inline CAffine3d CAffine3d::CreateScale(double sx, double sy, double sz)
{
	return CAffine3d(CMatrix3d(sx, 0.0, 0.0, 0.0, sy, 0.0, 0.0, 0.0, sz), CVector3d(0.0, 0.0, 0.0));
}


inline CAffine3d CAffine3d::CreateScale(const CVector3d& scale)
{
	return CreateScale(scale.GetX(), scale.GetY(), scale.GetZ());
}


inline const CMatrix3d& CAffine3d::GetMatrix() const
{
	return m_matrix;
}


inline void CAffine3d::SetMatrix(const CMatrix3d& matrix)
{
	m_matrix = matrix;
}


inline const CVector3d& CAffine3d::GetTranslation() const
{
	return m_translation;
}


inline void CAffine3d::SetTranslation(const CVector3d& translation)
{
	m_translation = translation;
}


inline void CAffine3d::Reset()
{
	m_matrix = CMatrix3d::GetIdentity();
	m_translation = CVector3d(0.0, 0.0, 0.0);
}


inline CVector3d CAffine3d::Transform(const CVector3d& point) const
{
	return m_matrix.GetMultiplied(point) + m_translation;
}


inline CVector3d CAffine3d::TransformDirection(const CVector3d& direction) const
{
	return m_matrix.GetMultiplied(direction);
}


inline CAffine3d CAffine3d::GetComposed(const CAffine3d& other) const
{
	// (M1, t1) * (M2, t2) = (M1 * M2, M1 * t2 + t1)
	return CAffine3d(
		m_matrix.GetMultiplied(other.m_matrix),
		m_matrix.GetMultiplied(other.m_translation) + m_translation
	);
}


inline void CAffine3d::Compose(const CAffine3d& other)
{
	*this = GetComposed(other);
}


inline CAffine3d CAffine3d::operator*(const CAffine3d& other) const
{
	return GetComposed(other);
}


inline CVector3d CAffine3d::operator*(const CVector3d& point) const
{
	return Transform(point);
}


inline bool CAffine3d::operator==(const CAffine3d& other) const
{
	return m_matrix == other.m_matrix && m_translation == other.m_translation;
}


inline bool CAffine3d::operator!=(const CAffine3d& other) const
{
	return !(*this == other);
}


} // namespace i3d


