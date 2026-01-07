#ifndef i3d_CVector3d_included
#define i3d_CVector3d_included


// Qt includes
#include <QtCore/QtGlobal>

// ACF includes
#include <imath/TVector.h>
#include <i2d/CVector2d.h>
#include <i3d/i3d.h>


namespace i3d
{


/**
	Represents a position or mathematical vector in 3D space with double precision.
	
	\section Vector3dPurpose Purpose
	CVector3d is a specialized 3-dimensional vector class built on top of TVector<3>.
	It provides convenient accessors for X, Y, and Z coordinates and implements
	3D-specific operations like cross product. This class is commonly used for
	representing points, directions, velocities, and normals in 3D space.

	\section Vector3dUsage Usage Examples
	\code
	// Creating vectors
	i3d::CVector3d origin;                    // Uninitialized
	i3d::CVector3d point(1.0, 2.0, 3.0);     // Initialize with coordinates
	i3d::CVector3d up(0.0, 0.0, 1.0);        // Unit vector pointing up
	
	// Accessing coordinates
	double x = point.GetX();   // Get X coordinate (1.0)
	point.SetY(5.0);           // Set Y coordinate
	
	// Vector operations
	double length = point.GetLength();                // Calculate length
	i3d::CVector3d normalized = point;
	normalized.Normalize();                           // Make unit length
	
	// 3D-specific: Cross product
	i3d::CVector3d forward(1.0, 0.0, 0.0);
	i3d::CVector3d right(0.0, 1.0, 0.0);
	i3d::CVector3d up_result = forward.GetCrossProduct(right);  // (0, 0, 1)
	
	// Vector arithmetic
	i3d::CVector3d sum = point;
	sum.Translate(up);                               // Add vectors
	
	// Distance calculations
	double dist = point.GetDistance(origin);
	
	// Converting from 2D
	i2d::CVector2d point2d(10.0, 20.0);
	i3d::CVector3d point3d(point2d, 5.0);           // (10, 20, 5)
	\endcode

	\section Vector3dOperations Common Operations
	- **Coordinate Access**: GetX(), GetY(), GetZ(), SetX(), SetY(), SetZ()
	- **3D Operations**: GetCrossProduct() for perpendicular vectors
	- **Inherited Operations**: All TVector<3> operations (dot product, normalization,
	  distance, translation, etc.)
	
	\note This class uses double precision for high accuracy calculations.
	\note Elements are not initialized by default constructor for performance reasons.
	
	\sa imath::TVector, i2d::CVector2d, i3d::CTransform3d

	\ingroup Geometry
*/
class CVector3d: public imath::TVector<3>
{
public:
	typedef imath::TVector<3> BaseClass;

	/**
		Default constructor creating an uninitialized vector.
		
		For performance reasons, the X, Y, and Z coordinates are not initialized
		and contain undefined values. Initialize them explicitly before use.
		
		\code
		CVector3d v;
		v.SetX(0.0);
		v.SetY(0.0);
		v.SetZ(0.0);
		// or use Clear()
		\endcode
		
		\note Elements will not be initialized.
		\sa Clear(), CVector3d(double, double, double)
	 */
	CVector3d();
	
	/**
		Constructs a vector with specified X, Y, and Z coordinates.
		
		\param x The X coordinate
		\param y The Y coordinate  
		\param z The Z coordinate
		
		\code
		CVector3d point(1.0, 2.0, 3.0);
		CVector3d unitX(1.0, 0.0, 0.0);  // Unit vector along X axis
		\endcode
	 */
	CVector3d(double x, double y, double z);
	
	/**
		Copy constructor from a 3D TVector.
		
		\param vector The source 3D vector to copy from
	 */
	CVector3d(const imath::TVector<3>& vector);

	/**
		Constructs a 3D vector from a 2D vector, with optional Z coordinate.
		
		This is useful for converting 2D coordinates to 3D space, such as
		when projecting a 2D screen position into 3D world space.
		
		\param vector The 2D vector providing X and Y coordinates
		\param z The Z coordinate (default: 0.0)
		
		\code
		i2d::CVector2d screenPos(100.0, 200.0);
		i3d::CVector3d worldPos(screenPos, 0.0);  // (100, 200, 0)
		i3d::CVector3d elevated(screenPos, 10.0); // (100, 200, 10)
		\endcode
	*/
	CVector3d(const i2d::CVector2d& vector, double z = 0);

	/**
		Gets the X coordinate of the vector.
		
		\return The X coordinate value
		
		\sa SetX(), GetY(), GetZ()
	*/
	double GetX() const;
	
	/**
		Sets the X coordinate of the vector.
		
		\param value The new X coordinate value
		
		\sa GetX(), SetY(), SetZ()
	*/
	void SetX(double value);
	
	/**
		Gets the Y coordinate of the vector.
		
		\return The Y coordinate value
		
		\sa SetY(), GetX(), GetZ()
	*/
	double GetY() const;
	
	/**
		Sets the Y coordinate of the vector.
		
		\param value The new Y coordinate value
		
		\sa GetY(), SetX(), SetZ()
	*/
	void SetY(double value);
	
	/**
		Gets the Z coordinate of the vector.
		
		\return The Z coordinate value
		
		\sa SetZ(), GetX(), GetY()
	*/
	double GetZ() const;
	
	/**
		Sets the Z coordinate of the vector.
		
		\param value The new Z coordinate value
		
		\sa GetZ(), SetX(), SetY()
	*/
	void SetZ(double value);

	using BaseClass::GetTranslated;
	/**
		Returns a new vector translated by the given offset.
		
		\param vector The translation offset vector
		\return A new CVector3d that is this vector plus the offset
		
		\sa Translate()
	 */
	CVector3d GetTranslated(const imath::TVector<3>& vector);

	/**
		Calculates the cross product of this vector with another vector.
		
		The cross product of two vectors produces a third vector that is
		perpendicular to both input vectors. The magnitude of the result
		is equal to the area of the parallelogram formed by the two vectors.
		
		\param vector The other vector for the cross product calculation
		
		\return A new vector perpendicular to both this vector and the input vector
		
		\note The cross product is not commutative: A × B ≠ B × A (in fact, A × B = -(B × A))
		\note The result follows the right-hand rule for determining direction
		
		\code
		// Calculate perpendicular vectors (orthogonal basis)
		CVector3d forward(1.0, 0.0, 0.0);  // X axis
		CVector3d right(0.0, 1.0, 0.0);    // Y axis
		CVector3d up = forward.GetCrossProduct(right);  // (0, 0, 1) - Z axis
		
		// Calculate surface normal from two edge vectors
		CVector3d edge1 = p2 - p1;
		CVector3d edge2 = p3 - p1;
		CVector3d normal = edge1.GetCrossProduct(edge2);
		normal.Normalize();  // Make it a unit normal
		\endcode
		
		\sa GetDotProduct()
	 */
	CVector3d GetCrossProduct(const imath::TVector<3>& vector) const;

	using BaseClass::GetNormalized;
	/**
		Return normalized vector with the same direction and specified length.
	 */
	CVector3d GetNormalized(double length = 1.0) const;

	/**
		Get XY part of this 3D vector.
	*/
	i2d::CVector2d GetPlaneCasted() const;

	/**
		Serialize this vector to specified archive.
	*/
	bool Serialize(iser::IArchive& archive);

	CVector3d operator-() const;

	CVector3d operator+(const imath::TVector<3>& vector) const;
	CVector3d operator-(const imath::TVector<3>& vector) const;
	CVector3d operator*(double scalar) const;
	CVector3d operator/(double scalar) const;

	CVector3d& operator+=(const imath::TVector<3>& vector);
	CVector3d& operator-=(const imath::TVector<3>& vector);
	CVector3d& operator*=(double scalar);
	CVector3d& operator/=(double scalar);
};


// inline methods

inline CVector3d::CVector3d()
{
}


inline CVector3d::CVector3d(double x, double y, double z)
{
	operator[](0) = x;
	operator[](1) = y;
	operator[](2) = z;
}


inline CVector3d::CVector3d(const imath::TVector<3>& vector)
:	BaseClass(vector)
{
}


inline CVector3d::CVector3d(const i2d::CVector2d& vector, double z)
{
	operator[](0) = vector.GetX();
	operator[](1) = vector.GetY();
	operator[](2) = z;
}


inline double CVector3d::GetX() const
{
	return operator[](0);
}


inline void CVector3d::SetX(double value)
{
	operator[](0) = value;
}


inline double CVector3d::GetY() const
{
	return operator[](1);
}


inline void CVector3d::SetY(double value)
{
	operator[](1) = value;
}


inline double CVector3d::GetZ() const
{
	return operator[](2);
}


inline void CVector3d::SetZ(double value)
{
	operator[](2) = value;
}


inline CVector3d CVector3d::GetTranslated(const imath::TVector<3>& vector)
{
	return *this + vector;
}


inline i2d::CVector2d CVector3d::GetPlaneCasted() const
{
	return i2d::CVector2d(GetX(), GetY());
}


inline CVector3d CVector3d::operator-() const
{
	return CVector3d(-GetX(), -GetY(), -GetZ());
}



inline CVector3d CVector3d::operator+(const imath::TVector<3>& vector) const
{
	return CVector3d(GetX() + vector[0], GetY() + vector[1], GetZ() + vector[2]);
}



inline CVector3d CVector3d::operator-(const imath::TVector<3>& vector) const
{
	return CVector3d(GetX() - vector[0], GetY() - vector[1], GetZ() - vector[2]);
}



inline CVector3d CVector3d::operator*(double scalar) const
{
	return CVector3d(GetX() * scalar, GetY() * scalar, GetZ() * scalar);
}



inline CVector3d CVector3d::operator/(double scalar) const
{
	if (qAbs(scalar) < I_BIG_EPSILON){
		scalar = (scalar > 0)? I_BIG_EPSILON: -I_BIG_EPSILON;
	}

	return CVector3d(GetX() / scalar, GetY() / scalar, GetZ() / scalar);
}



inline CVector3d& CVector3d::operator+=(const imath::TVector<3>& vector)
{
	BaseClass::operator+=(vector);

	return *this;
}



inline CVector3d& CVector3d::operator-=(const imath::TVector<3>& vector)
{
	BaseClass::operator-=(vector);

	return *this;
}


inline CVector3d& CVector3d::operator*=(double scalar)
{
	BaseClass::operator*=(scalar);

	return *this;
}


inline CVector3d& CVector3d::operator/=(double scalar)
{
	BaseClass::operator/=(scalar);

	return *this;
}


} // namespace i3d


#endif // !i3d_CVector3d_included

