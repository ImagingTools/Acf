// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtCore/QtMath>
#else
#include <QtCore/qmath.h>
#endif

// STL includes
#include <initializer_list>

// ACF includes
#include <iser/IArchive.h>
#include <imath/imath.h>


namespace imath
{


/**
	Implementation of fixed-size mathematical vector with specified type of elements.
	
	\section VectorPurpose Purpose
	TVector is a template class that provides a generic implementation of fixed-size
	mathematical vectors. It supports common vector operations like addition, scaling,
	dot product, normalization, and distance calculations. The vector size is determined
	at compile-time, enabling efficient memory layout and operations.

	\section VectorTemplate Template Parameters
	- \b Size: The number of elements in the vector (compile-time constant)
	- \b Element: The type of each vector element (default: double). Common types include
	  float, double, int, or any numeric type.

	\section VectorUsageExamples Usage Examples
	\code
	// Create a 3D vector with double precision
	imath::TVector<3, double> vec3d;
	vec3d.SetElement(0, 1.0);
	vec3d.SetElement(1, 2.0);
	vec3d.SetElement(2, 3.0);

	// Create a 2D vector with float precision using initializer list
	imath::TVector<2, float> vec2f = {3.0f, 4.0f};

	// Calculate vector length
	double length = vec3d.GetLength();  // sqrt(1^2 + 2^2 + 3^2) = sqrt(14)

	// Normalize vector to unit length
	imath::TVector<3, double> unitVec = vec3d;
	if (unitVec.Normalize()) {
		// Now unitVec has length 1.0
	}

	// Dot product of two vectors
	imath::TVector<3, double> other = {1.0, 0.0, 0.0};
	double dotProduct = vec3d.GetDotProduct(other);  // 1.0 * 1.0 = 1.0

	// Calculate distance between two points
	imath::TVector<2, float> point1 = {0.0f, 0.0f};
	imath::TVector<2, float> point2 = {3.0f, 4.0f};
	float distance = point1.GetDistance(point2);  // 5.0

	// Vector addition
	imath::TVector<2, float> result = point1;
	result.Translate(point2);  // result is now {3.0f, 4.0f}

	// Check if vector is null (all zeros)
	imath::TVector<3, double> zero;
	zero.Clear();
	if (zero.IsNull()) {
		// Vector is all zeros
	}
	\endcode

	\section VectorOperations Common Operations
	- Element access: GetElement(), SetElement(), GetElements()
	- Initialization: Clear(), Reset(), SetAllElements()
	- Arithmetic: Translate(), ScaledCumulate()
	- Metrics: GetLength(), GetLength2(), GetDistance(), GetDotProduct()
	- Normalization: Normalize(), GetNormalized()
	- Comparison: IsNull()
	- Operators: +, -, *, /, ==, != (if implemented)

	\sa imath::TFastVector, imath::CVarVector, i3d::CVector3d

	\ingroup Geometry
*/
template <int Size, class Element = double>
class TVector
{
public:
	typedef Element ElementType;
	typedef Element Elements[Size];

	/**
		Creates an uninitialized vector.
		
		This constructor creates a vector with the specified size but does not initialize
		the element values. The element values will contain undefined data until explicitly
		set using SetElement(), SetAllElements(), or other initialization methods.
		
		\note For better performance, use this constructor when you plan to immediately
		      initialize all elements anyway.
		\note If you need a zero-initialized vector, call Clear() or SetAllElements(0) after construction.
		
		\sa Clear(), SetAllElements()
	 */
	TVector();
	
	/**
		Creates a copy of another vector.
		
		\param vector The source vector to copy from. All elements will be copied to
		             this new vector.
	 */
	TVector(const TVector<Size, Element>& vector);

	/**
		Creates a vector from an initializer list.
		
		Allows convenient initialization using brace syntax. If the initializer list
		contains fewer elements than Size, remaining elements are default-initialized
		(typically to zero for numeric types). If the list contains more elements than
		Size, only the first Size elements are used.
		
		\param values Initializer list of element values
		
		\code
		// Create a 3D vector
		TVector<3, double> v = {1.0, 2.0, 3.0};
		
		// Partial initialization (remaining elements are 0)
		TVector<5, int> v2 = {1, 2};  // v2 = {1, 2, 0, 0, 0}
		\endcode
	*/
	TVector(std::initializer_list<Element> values);

	/**
		Gets the element at the specified index (read-only).
		
		\param i Zero-based index of the element to retrieve (0 <= i < Size)
		
		\return Const reference to the element at index i
		
		\note No bounds checking is performed in release builds. Accessing out-of-bounds
		      indices results in undefined behavior.
		
		\sa GetElementRef(), SetElement()
	*/
	const Element& GetElement(int i) const;

	/**
		Gets a reference to the element at the specified index (read-write).
		
		Returns a non-const reference allowing direct modification of the element.
		
		\param i Zero-based index of the element (0 <= i < Size)
		
		\return Reference to the element at index i
		
		\code
		TVector<3, double> v;
		v.GetElementRef(0) = 1.0;  // Direct assignment
		v.GetElementRef(1) += 2.0;  // Direct modification
		\endcode
		
		\sa GetElement(), SetElement()
	*/
	Element& GetElementRef(int i);

	/**
		Sets the element at the specified index.
		
		\param i Zero-based index of the element to set (0 <= i < Size)
		\param value The new value for the element
		
		\sa GetElement(), GetElementRef()
	*/
	void SetElement(int i, const Element& value);

	/**
		Sets all elements to the same value.
		
		\param value The value to assign to all vector elements
		
		\code
		TVector<5, int> v;
		v.SetAllElements(7);  // All elements are now 7
		\endcode
		
		\sa Clear(), Reset()
	*/
	void SetAllElements(const Element& value);

	/**
		Sets all coordinates to zero.
		
		Equivalent to SetAllElements(0). This method is provided for compatibility
		with other vector implementations.
		
		\sa Clear(), SetAllElements()
	*/
	void Reset();

	/**
		Sets all coordinates to zero.
		
		Equivalent to SetAllElements(0) and Reset().
		
		\sa Reset(), SetAllElements()
	*/
	void Clear();

	/**
		Get read-only access to internal element container.
	*/
	const typename TVector<Size, Element>::Elements& GetElements() const;
	/**
		Get access to internal element container.
	*/
	typename TVector<Size, Element>::Elements& GetElementsRef();

	/**
		Translates (adds) another vector to this vector.
		
		Performs element-wise addition: this[i] += vector[i] for all i.
		
		\param vector The vector to add to this vector
		
		\code
		TVector<2, double> v1 = {1.0, 2.0};
		TVector<2, double> v2 = {3.0, 4.0};
		v1.Translate(v2);  // v1 is now {4.0, 6.0}
		\endcode
		
		\sa GetTranslated(), ScaledCumulate()
	*/
	void Translate(const TVector<Size, Element>& vector);

	/**
		Returns a new vector that is the translation of this vector.
		
		\param vector The vector to add
		\return A new vector containing the sum of this vector and the given vector
		
		\sa Translate(), GetTranslated(const TVector<Size, Element>&, TVector<Size, Element>&)
	*/
	TVector<Size, Element> GetTranslated(const TVector<Size, Element>& vector);

	/**
		Computes the translated vector and stores it in the result parameter.
		
		\param vector The vector to add to this vector
		\param result Output parameter that receives the translated vector
		
		\sa Translate(), GetTranslated(const TVector<Size, Element>&)
	*/
	void GetTranslated(const TVector<Size, Element>& vector, TVector<Size, Element>& result);

	/**
		Adds a scaled vector to this vector.
		
		Performs the operation: this[i] += vector[i] * scale for all i.
		This is equivalent to Translate(vector * scale) but may be more efficiently implemented.
		
		\param vector The vector to add (scaled)
		\param scale The scaling factor to apply to the vector before addition
		
		\code
		TVector<2, double> v1 = {1.0, 2.0};
		TVector<2, double> v2 = {3.0, 4.0};
		v1.ScaledCumulate(v2, 2.0);  // v1 is now {7.0, 10.0} = {1+3*2, 2+4*2}
		\endcode
		
		\sa Translate()
	*/
	void ScaledCumulate(const TVector<Size, Element>& vector, Element scale);

	/**
		Checks if this vector is null (all elements approximately zero).
		
		\param tolerance The maximum absolute value for each element to be considered zero.
		                Default is I_BIG_EPSILON (typically 1e-6 for double precision).
		
		\return true if all elements have absolute value less than or equal to tolerance
		
		\code
		TVector<3, double> v = {0.0, 0.0000001, 0.0};
		bool isNull = v.IsNull();  // true with default tolerance
		bool isExactNull = v.IsNull(0.0);  // false, middle element is not exactly zero
		\endcode
		
		\sa Clear(), Reset()
	*/
	bool IsNull(Element tolerance = I_BIG_EPSILON) const;

	/**
		Calculates the dot product with another vector.
		
		The dot product is defined as: sum(this[i] * vector[i]) for all i.
		It measures the similarity of direction between two vectors.
		
		\param vector The other vector for dot product calculation
		\return The scalar dot product value
		
		\code
		TVector<3, double> v1 = {1.0, 0.0, 0.0};
		TVector<3, double> v2 = {0.0, 1.0, 0.0};
		double dot = v1.GetDotProduct(v2);  // 0.0 (perpendicular vectors)
		
		TVector<3, double> v3 = {2.0, 0.0, 0.0};
		double dot2 = v1.GetDotProduct(v3);  // 2.0 (parallel vectors)
		\endcode
		
		\sa GetLength(), Normalize()
	*/
	Element GetDotProduct(const TVector<Size, Element>& vector) const;

	/**
		Calculates the squared Euclidean length of the vector.
		
		Returns sum(element[i]^2) for all i. This is more efficient than GetLength()
		when only relative lengths need to be compared, as it avoids the square root operation.
		
		\return The squared length of the vector
		
		\sa GetLength(), GetDistance2()
	*/
	Element GetLength2() const;
	
	/**
		Calculates the Euclidean length (magnitude) of the vector.
		
		Returns sqrt(sum(element[i]^2)) for all i.
		
		\return The length of the vector
		
		\code
		TVector<2, double> v = {3.0, 4.0};
		double length = v.GetLength();  // 5.0
		\endcode
		
		\sa GetLength2(), Normalize()
	*/
	Element GetLength() const;

	/**
		Calculates the squared distance to another vector.
		
		More efficient than GetDistance() when only relative distances need to be
		compared, as it avoids the square root operation.
		
		\param vector The other vector (point)
		\return The squared Euclidean distance
		
		\sa GetDistance(), GetLength2()
	*/
	Element GetDistance2(const TVector<Size, Element>& vector) const;

	/**
		Calculates the Euclidean distance to another vector.
		
		\param vector The other vector (point)
		\return The Euclidean distance between this vector and the given vector
		
		\code
		TVector<2, double> p1 = {0.0, 0.0};
		TVector<2, double> p2 = {3.0, 4.0};
		double dist = p1.GetDistance(p2);  // 5.0
		\endcode
		
		\sa GetDistance2(), GetLength()
	*/
	Element GetDistance(const TVector<Size, Element>& vector) const;

	/**
		Calculates the sum of all vector elements.
		
		\return Sum of all element values
		
		\code
		TVector<4, int> v = {1, 2, 3, 4};
		int sum = v.GetElementsSum();  // 10
		\endcode
	*/
	Element GetElementsSum() const;

	/**
		Normalizes the vector to a specified length.
		
		Scales the vector so that its length becomes the specified value.
		If the current length is zero (or very close to zero), the operation fails.
		
		\param length The desired length after normalization (default: 1.0 for unit vector)
		
		\return true if normalization succeeded, false if the vector length was zero
		
		\code
		TVector<2, double> v = {3.0, 4.0};  // length = 5.0
		if (v.Normalize()) {
			// v is now {0.6, 0.8} with length 1.0
		}
		
		TVector<3, double> v2 = {1.0, 2.0, 3.0};
		if (v2.Normalize(10.0)) {
			// v2 now has length 10.0 but same direction
		}
		\endcode
		
		\sa GetNormalized(), GetLength()
	*/
	bool Normalize(Element length = 1.0);
	
	/**
		Returns a normalized copy of this vector with specified length.
		
		Creates a new vector with the same direction as this vector but with the
		specified length. The original vector is not modified.
		
		\param result Output parameter that receives the normalized vector
		\param length The desired length for the normalized vector (default: 1.0)
		
		\return true if normalization succeeded, false if this vector's length was zero
		
		\code
		TVector<2, double> v = {3.0, 4.0};
		TVector<2, double> unit;
		if (v.GetNormalized(unit)) {
			// unit is {0.6, 0.8}, v is unchanged
		}
		\endcode
		
		\sa Normalize()
	*/
	bool GetNormalized(TVector<Size, Element>& result, Element length = 1.0) const;

	/**
		Get vector with minimal elements values.
	*/
	void GetMinimal(const TVector<Size, Element>& vector, TVector<Size, Element>& result) const;
	/**
		Get vector with maximal elements values.
	*/
	void GetMaximal(const TVector<Size, Element>& vector, TVector<Size, Element>& result) const;

	/**
		Serialize this vector to specified archive.
	*/
	bool Serialize(iser::IArchive& archive);

	bool operator==(const TVector<Size, Element>& vector) const;
	bool operator!=(const TVector<Size, Element>& vector) const;
	bool operator<(const TVector<Size, Element>& vector) const;
	bool operator>(const TVector<Size, Element>& vector) const;
	bool operator<=(const TVector<Size, Element>& vector) const;
    bool operator>=(const TVector<Size, Element>& vector) const;

    TVector<Size, Element>& operator=(const TVector<Size, Element>& vector) = default;

    TVector<Size, Element> operator-() const;

	TVector<Size, Element> operator+(const TVector<Size, Element>& vector) const;
	TVector<Size, Element> operator-(const TVector<Size, Element>& vector) const;
	TVector<Size, Element> operator*(Element scalar) const;
	TVector<Size, Element> operator/(Element scalar) const;

	TVector<Size, Element>& operator+=(const TVector<Size, Element>& vector);
	TVector<Size, Element>& operator-=(const TVector<Size, Element>& vector);
	TVector<Size, Element>& operator*=(Element scalar);
	TVector<Size, Element>& operator/=(Element scalar);

	const Element& operator[](int i) const;
	Element& operator[](int i);

	// static methods

	/**
		Get number of elements.
	*/
	static int GetElementsCount();

	/**
		Set number of elements.
		This method is provided for template implementations.
		\param	count	number of elements.
		\return			true, if the number of set elements equals template parameter.
	*/
	static bool SetElementsCount(int count);

	/**
		Get vector with all coordinates set to 0.
	*/
	static const TVector<Size, Element>& GetZero();

protected:
	Elements m_elements;

private:
	explicit TVector(Element vector);

	static TVector<Size, Element> s_zero;
};


// inline constructors

template <int Size, class Element>
inline TVector<Size, Element>::TVector()
{
}


template <int Size, class Element>
inline TVector<Size, Element>::TVector(const TVector<Size, Element>& vector)
{
	for (int i = 0; i < Size; ++i){
		m_elements[i] = vector.m_elements[i];
	}
}


template <int Size, class Element>
inline TVector<Size, Element>::TVector(std::initializer_list<Element> values)
{
	int count = static_cast<int>(values.size());
	Q_ASSERT(count <= Size);

	for (int i = 0; i < count; ++i){
		m_elements[i] = *(values.begin() + i);
	}

	for (int i = count; i < Size; ++i){
		m_elements[i] = 0;
	}
}


// inline methods

template <int Size, class Element>
inline const Element& TVector<Size, Element>::GetElement(int i) const
{
	return operator[](i);
}


template <int Size, class Element>
inline Element& TVector<Size, Element>::GetElementRef(int i)
{
	return operator[](i);
}


template <int Size, class Element>
inline void TVector<Size, Element>::SetElement(int i, const Element& value)
{
	operator[](i) = value;
}


template <int Size, class Element>
inline void TVector<Size, Element>::SetAllElements(const Element& value)
{
	for (int i = 0; i < Size; ++i){
		m_elements[i] = value;
	}
}


template <int Size, class Element>
inline void TVector<Size, Element>::Reset()
{
	for (int i = 0; i < Size; ++i){
		m_elements[i] = 0;
	}
}


template <int Size, class Element>
inline void TVector<Size, Element>::Clear()
{
	for (int i = 0; i < Size; ++i){
		m_elements[i] = 0;
	}
}


template <int Size, class Element>
inline const typename TVector<Size, Element>::Elements& TVector<Size, Element>::GetElements() const
{
	return m_elements;
}


template <int Size, class Element>
inline typename TVector<Size, Element>::Elements& TVector<Size, Element>::GetElementsRef()
{
	return m_elements;
}


template <int Size, class Element>
inline void TVector<Size, Element>::Translate(const TVector<Size, Element>& vector)
{
	for (int i = 0; i < Size; ++i){
		m_elements[i] += vector.m_elements[i];
	}
}


template <int Size, class Element>
TVector<Size, Element> TVector<Size, Element>::GetTranslated(const TVector<Size, Element>& vector)
{
	return *this + vector;
}


template <int Size, class Element>
void TVector<Size, Element>::GetTranslated(const TVector<Size, Element>& vector, TVector<Size, Element>& result)
{
	result = *this + vector;
}


template <int Size, class Element>
inline void TVector<Size, Element>::ScaledCumulate(const TVector<Size, Element>& vector, Element scale)
{
	for (int i = 0; i < Size; ++i){
		m_elements[i] += vector.m_elements[i] * scale;
	}
}


template <int Size, class Element>
inline bool TVector<Size, Element>::IsNull(Element tolerance) const
{
	return GetLength2() <= tolerance * tolerance;
}


template <int Size, class Element>
inline Element TVector<Size, Element>::GetDotProduct(const TVector<Size, Element>& vector) const
{
	Element retVal = 0.0;

	for (int i = 0; i < Size; ++i){
		retVal +=  m_elements[i] * vector.m_elements[i];
	}

	return retVal;
}


template <int Size, class Element>
inline Element TVector<Size, Element>::GetLength2() const
{
	return GetDotProduct(*this);
}


template <int Size, class Element>
inline Element TVector<Size, Element>::GetLength() const
{
	return qSqrt(GetLength2());
}


template <int Size, class Element>
inline Element TVector<Size, Element>::GetDistance2(const TVector<Size, Element>& vector) const
{
	return (*this - vector).GetLength2();
}


template <int Size, class Element>
inline Element TVector<Size, Element>::GetDistance(const TVector<Size, Element>& vector) const
{
	return qSqrt(GetDistance2(vector));
}


// operators

template <int Size, class Element>
inline bool TVector<Size, Element>::operator==(const TVector<Size, Element>& vector) const
{
	for (int i = 0; i < Size; ++i){
		if (m_elements[i] != vector.m_elements[i]){
			return false;
		}
	}
	return true;
}


template <int Size, class Element>
inline bool TVector<Size, Element>::operator!=(const TVector<Size, Element>& vector) const
{
	return !operator==(vector);
}


template <int Size, class Element>
bool TVector<Size, Element>::operator<(const TVector<Size, Element>& vector) const
{
	for (int i = 0; i < Size; ++i){
		if (m_elements[i] > vector.m_elements[i]){
			return false;
		}
		else if (m_elements[i] < vector.m_elements[i]){
			return true;
		}
	}

	return false;
}


template <int Size, class Element>
bool TVector<Size, Element>::operator>(const TVector<Size, Element>& vector) const
{
	for (int i = 0; i < Size; ++i){
		if (m_elements[i] > vector.m_elements[i]){
			return true;
		}
		else if (m_elements[i] < vector.m_elements[i]){
			return false;
		}
	}

	return false;
}


template <int Size, class Element>
bool TVector<Size, Element>::operator<=(const TVector<Size, Element>& vector) const
{
	for (int i = 0; i < Size; ++i){
		if (m_elements[i] > vector.m_elements[i]){
			return false;
		}
		else if (m_elements[i] < vector.m_elements[i]){
			return true;
		}
	}

	return true;
}


template <int Size, class Element>
bool TVector<Size, Element>::operator>=(const TVector<Size, Element>& vector) const
{
	for (int i = 0; i < Size; ++i){
		if (m_elements[i] > vector.m_elements[i]){
			return true;
		}
		else if (m_elements[i] < vector.m_elements[i]){
			return false;
		}
	}

	return true;
}


template <int Size, class Element>
inline TVector<Size, Element>& TVector<Size, Element>::operator+=(const TVector<Size, Element>& vector)
{
	for (int i = 0; i < Size; ++i){
		m_elements[i] += vector.m_elements[i];
	}

	return *this;
}


template <int Size, class Element>
inline TVector<Size, Element>& TVector<Size, Element>::operator-=(const TVector<Size, Element>& vector)
{
	for (int i = 0; i < Size; ++i){
		m_elements[i] -= vector.m_elements[i];
	}

	return *this;
}


template <int Size, class Element>
inline TVector<Size, Element>& TVector<Size, Element>::operator*=(Element scalar)
{
	for (int i = 0; i < Size; ++i){
		m_elements[i] *= scalar;
	}

	return *this;
}


template <int Size, class Element>
inline TVector<Size, Element>& TVector<Size, Element>::operator/=(Element scalar)
{
	for (int i = 0; i < Size; ++i){
		m_elements[i] /= scalar;
	}

	return *this;
}


template <int Size, class Element>
inline TVector<Size, Element> TVector<Size, Element>::operator-() const
{
	TVector<Size, Element> retVal;

	for (int i = 0; i < Size; ++i){
		retVal.m_elements[i] = -m_elements[i];
	}

	return retVal;
}


template <int Size, class Element>
inline TVector<Size, Element> TVector<Size, Element>::operator+(const TVector<Size, Element>& vector) const
{
	TVector<Size, Element> retVal;

	for (int i = 0; i < Size; ++i){
		retVal.m_elements[i] = m_elements[i] + vector.m_elements[i];
	}

	return retVal;
}


template <int Size, class Element>
inline TVector<Size, Element> TVector<Size, Element>::operator-(const TVector<Size, Element>& vector) const
{
	TVector<Size, Element> retVal;

	for (int i = 0; i < Size; ++i){
		retVal.m_elements[i] = m_elements[i] - vector.m_elements[i];
	}

	return retVal;
}


template <int Size, class Element>
inline TVector<Size, Element> TVector<Size, Element>::operator*(Element scalar) const
{
	TVector<Size, Element> retVal;

	for (int i = 0; i < Size; ++i){
		retVal.m_elements[i] = m_elements[i] * scalar;
	}

	return retVal;
}


template <int Size, class Element>
inline TVector<Size, Element> TVector<Size, Element>::operator/(Element scalar) const
{
	TVector<Size, Element> retVal;

	for (int i = 0; i < Size; ++i){
		retVal.m_elements[i] = m_elements[i] / scalar;
	}

	return retVal;
}


template <int Size, class Element>
const Element& TVector<Size, Element>::operator[](int i) const
{
	Q_ASSERT(i >= 0);
	Q_ASSERT(i < Size);

	return m_elements[i];
}


template <int Size, class Element>
Element& TVector<Size, Element>::operator[](int i)
{
	Q_ASSERT(i >= 0);
	Q_ASSERT(i < Size);

	return m_elements[i];
}


// static inline methods

template <int Size, class Element>
inline int TVector<Size, Element>::GetElementsCount()
{
	return Size;
}


template <int Size, class Element>
inline bool TVector<Size, Element>::SetElementsCount(int count)
{
	return count == Size;
}


template <int Size, class Element>
inline const TVector<Size, Element>& TVector<Size, Element>::GetZero()
{
	return s_zero;
}


// public methods

template <int Size, class Element>
Element TVector<Size, Element>::GetElementsSum() const
{
	Element retVal = 0;

	for (int i = 0; i < Size; ++i){
		retVal += m_elements[i];
	}

	return retVal;
}


template <int Size, class Element>
bool TVector<Size, Element>::Normalize(Element length)
{
	Element isLength = GetLength();

	Element proportion = isLength / length;

	if (qAbs(proportion) > I_BIG_EPSILON){
		for (int i = 0; i < Size; ++i){
			m_elements[i] = m_elements[i] / proportion;
		}

		return true;
	}
	else{
		return false;
	}
}


template <int Size, class Element>
bool TVector<Size, Element>::GetNormalized(TVector<Size, Element>& result, Element length) const
{
	Element isLength = GetLength();

	Element proportion = isLength / length;

	if (qAbs(proportion) > I_BIG_EPSILON){
		for (int i = 0; i < Size; ++i){
			result.m_elements[i] = m_elements[i] / proportion;
		}

		return true;
	}
	else{
		return false;
	}
}


template <int Size, class Element>
void TVector<Size, Element>::GetMinimal(const TVector<Size, Element>& vector, TVector<Size, Element>& result) const
{
	for (int i = 0; i < Size; ++i){
		result.SetElement(i, qMin(GetElement(i), vector.GetElement(i)));
	}
}


template <int Size, class Element>
void TVector<Size, Element>::GetMaximal(const TVector<Size, Element>& vector, TVector<Size, Element>& result) const
{
	for (int i = 0; i < Size; ++i){
		result.SetElement(i, qMax(GetElement(i), vector.GetElement(i)));
	}
}


template <int Size, class Element>
bool TVector<Size, Element>::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	for (int i = 0; i < Size; ++i){
		retVal = retVal && archive.Process(m_elements[i]);
	}

	return retVal;
}


// private methods

template <int Size, class Element>
TVector<Size, Element>::TVector(Element element)
{
	for (int i = 0; i < Size; ++i){
		m_elements[i] = element;
	}
}


template <int Size, class Element>
TVector<Size, Element> TVector<Size, Element>::s_zero(Element(0));


} // namespace imath


