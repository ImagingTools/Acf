// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// STL includes
#include <cstring>
#include <initializer_list>

// ACF includes
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>
#include <imath/TVector.h>


namespace imath
{


/**
	Optimized implementation of a variable-size vector with compile-time maximum capacity.
	
	\section FastVectorPurpose Purpose
	TFastVector is a hybrid between fixed-size and dynamic vectors. It allocates space
	for MaxSize elements on the stack (or inline within the containing object), but
	allows the actual number of active elements to vary at runtime. This provides the
	performance benefits of stack allocation while maintaining some runtime flexibility.

	\section FastVectorVsOthers TFastVector vs Other Vector Types
	- **vs TVector**: TVector has a fixed size known at compile time. TFastVector has
	  a runtime-variable size up to a compile-time maximum.
	- **vs CVarVector**: CVarVector is fully dynamic with heap allocation. TFastVector
	  uses stack/inline storage for better performance.
	- **vs QVector/std::vector**: TFastVector avoids heap allocation up to MaxSize
	  elements, making it faster for small vectors in performance-critical code.

	\section FastVectorUseCases When to Use TFastVector
	- Small vectors (typically < 20 elements) where size varies at runtime
	- Performance-critical code where heap allocation overhead matters
	- Temporary vectors in tight loops
	- Return values from functions (avoids heap allocation)
	- Embedded or real-time systems with limited dynamic memory

	\section FastVectorExample Usage Examples
	\code
	// Example 1: Basic usage
	imath::TFastVector<10, double> vec;  // Can hold up to 10 doubles
	
	// Start with 3 elements, initialized to 0.0
	vec.SetElementsCount(3, 0.0);
	
	// Set values
	vec.SetElement(0, 1.5);
	vec.SetElement(1, 2.5);
	vec.SetElement(2, 3.5);
	
	// Access values
	double sum = 0.0;
	for (int i = 0; i < vec.GetElementsCount(); ++i) {
		sum += vec.GetElement(i);
	}

	// Example 2: Dynamic growth
	imath::TFastVector<100, int> numbers;
	numbers.SetElementsCount(0);  // Start empty
	
	for (int i = 0; i < 50; ++i) {
		int currentSize = numbers.GetElementsCount();
		if (numbers.SetElementsCount(currentSize + 1)) {
			numbers.SetElement(currentSize, i * i);
		}
	}

	// Example 3: Initializer list
	imath::TFastVector<5, double> coords = {1.0, 2.0, 3.0};
	// coords has 3 elements

	// Example 4: Conversion from TVector
	imath::TVector<3, double> fixedVec;
	fixedVec.SetElement(0, 1.0);
	fixedVec.SetElement(1, 2.0);
	fixedVec.SetElement(2, 3.0);
	
	imath::TFastVector<10, double> fastVec(fixedVec);  // Convert to TFastVector

	// Example 5: Ensure minimum size
	imath::TFastVector<20, float> buffer;
	buffer.SetElementsCount(5);
	
	// Ensure at least 10 elements (grows if needed, stays same if >= 10)
	buffer.EnsureElementsCount(10);
	// Now has 10 elements

	// Example 6: Vector operations
	imath::TFastVector<10, double> v1(3, 1.0);  // [1, 1, 1]
	imath::TFastVector<10, double> v2(3, 2.0);  // [2, 2, 2]
	
	v1.Translate(v2);  // v1 is now [3, 3, 3]
	
	double length = v1.GetLength();  // sqrt(3^2 + 3^2 + 3^2) = sqrt(27)
	
	v1.Normalize();  // Make unit length
	\endcode

	\section FastVectorPerformance Performance Characteristics
	- **No heap allocation** up to MaxSize elements (stack/inline storage)
	- **Cache-friendly** - all data stored contiguously
	- **Fast copy** - simple memory copy for assignment
	- **Zero overhead** for size queries (just returns m_elementsCount)
	- **Bounds checking** in SetElementsCount() prevents overflow

	\section FastVectorLimitations Limitations
	- Cannot grow beyond MaxSize (compile-time maximum)
	- Wastes memory if typical size << MaxSize
	- Large MaxSize values increase stack usage / object size
	- SetElementsCount() returns false if requested size > MaxSize

	\sa imath::TVector, imath::CVarVector, imath::TMathVectorWrap

	\ingroup Geometry
*/
template <int MaxSize, class Element = double>
class TFastVector
{
public:
	typedef Element ElementType;

	enum{
		MAX_ELEMENTS_COUNT = MaxSize
	};

	/**
		Create an uninitialized point.
	 */
	TFastVector();

	/**
		Create vector and initialize number of components.
	*/
	explicit TFastVector(int componentsCount, const Element& value = Element());

	/**
		Copy constructor.
	 */
	TFastVector(const TFastVector<MaxSize, Element>& vector);

	/**
		Initializer list constructor.
	*/
	TFastVector(std::initializer_list<Element> values);

	template <int Size>
	TFastVector(const imath::TVector<Size, Element>& vector)
	:	m_elementsCount(Size)
	{
		Q_ASSERT(Size <= MaxSize);

		for (int i = 0; i < Size; ++i){
			m_elements[i] = vector[i];
		}
	}

	/**
		Get number of elements.
	*/
	int GetElementsCount() const;

	/**
		Set number of elements.
		\param	count	number of elements.
		\return			true, if the number of set is not greater than template parameter MaxSize.
	*/
	bool SetElementsCount(int count, const Element& value = Element());

	/**
		Ensure, that number of elements vector cannot be smaller that some value.
		If number of elements was bigger or equal to specified value, it does nothing.
		In other case, number of elements will be set.
		\param	count	number of elements.
		\return			true, if the number of set is not greater than template parameter MaxSize.
	*/
	bool EnsureElementsCount(int count, const Element& value = Element());

	/**
		Get element at specified i.
	*/
	const Element& GetElement(int i) const;

	/**
		Get reference to element at specified i.
	*/
	Element& GetElementRef(int i);

	/**
		Set element at specified i.
	*/
	void SetElement(int i, const Element& value);

	/**
		Set all coordinates to zero.
	*/
	void Clear();

	/**
		Set elemenents from other vector without resizing.
		\param	vector			source of element values will be copied.
		\param	expansionValue	if actual vector has more elements than \c vector, rest will be replaced with this value.
	*/
	void SetElementsFrom(const TFastVector& vector, const Element& expansionValue = Element());

	/**
		Translate the point.
	*/
	void Translate(const TFastVector<MaxSize, Element>& vector);

	/**
		Get translated point.
	*/
	TFastVector<MaxSize, Element> GetTranslated(const TFastVector<MaxSize, Element>& vector);

	/**
		/overloaded
	*/
	void GetTranslated(const TFastVector<MaxSize, Element>& vector, TFastVector<MaxSize, Element>& result);

	/**
		Add second vector scaled by specified factor.
		It is equal of Translate(vector * scale) but can be faster implemented.
	*/
	void ScaledCumulate(const TFastVector<MaxSize, Element>& vector, Element scale);

	/**
		Check if this vector is null.
	*/
	bool IsNull(Element tolerance = I_BIG_EPSILON) const;

	/**
		Return dot product of two vectors.
	*/
	Element GetDotProduct(const TFastVector<MaxSize, Element>& vector) const;

	/**
		Return euclidian length square.
	*/
	Element GetLength2() const;
	/**
		Return euclidian length.
	*/
	Element GetLength() const;

	/**
		Return distance square between two vectors.
	*/
	Element GetDistance2(const TFastVector<MaxSize, Element>& vector) const;

	/**
		Return distance between two vectors.
	*/
	Element GetDistance(const TFastVector<MaxSize, Element>& vector) const;

	/**
		Get simple sum of all elements.
	*/
	Element GetElementsSum() const;

	/**
		Normalize vector to specified length.
		\param	length	new vector length.
		\return	true, if normalization succeeded.
	*/
	bool Normalize(Element length = 1.0);
	/**
		Return normalized vector with the same direction and specified length.
		\param	length	new vector length.
		\return	true, if normalization succeeded.
	*/
	bool GetNormalized(TFastVector<MaxSize, Element>& result, Element length = 1.0) const;

	/**
		Get vector with minimal elements values.
	*/
	void GetMinimal(const TFastVector<MaxSize, Element>& vector, TFastVector<MaxSize, Element>& result) const;
	/**
		Get vector with maximal elements values.
	*/
	void GetMaximal(const TFastVector<MaxSize, Element>& vector, TFastVector<MaxSize, Element>& result) const;

	/**
		Serialize this vector to specified archive.
	*/
	bool Serialize(iser::IArchive& archive);

	bool operator==(const TFastVector<MaxSize, Element>& vector) const;
	bool operator!=(const TFastVector<MaxSize, Element>& vector) const;
	bool operator<(const TFastVector<MaxSize, Element>& vector) const;
	bool operator>(const TFastVector<MaxSize, Element>& vector) const;
	bool operator<=(const TFastVector<MaxSize, Element>& vector) const;
	bool operator>=(const TFastVector<MaxSize, Element>& vector) const;

	TFastVector<MaxSize, Element> operator-() const;

	TFastVector<MaxSize, Element> operator+(const TFastVector<MaxSize, Element>& vector) const;
	TFastVector<MaxSize, Element> operator-(const TFastVector<MaxSize, Element>& vector) const;
	TFastVector<MaxSize, Element> operator*(Element scalar) const;
	TFastVector<MaxSize, Element> operator/(Element scalar) const;

	TFastVector<MaxSize, Element>& operator=(const TFastVector<MaxSize, Element>& vector);

	TFastVector<MaxSize, Element>& operator+=(const TFastVector<MaxSize, Element>& vector);
	TFastVector<MaxSize, Element>& operator-=(const TFastVector<MaxSize, Element>& vector);
	TFastVector<MaxSize, Element>& operator*=(Element scalar);
	TFastVector<MaxSize, Element>& operator/=(Element scalar);

	const Element& operator[](int i) const;
	Element& operator[](int i);

protected:
	Element m_elements[MaxSize];
	int m_elementsCount;
};


// inline methods

template <int MaxSize, class Element>
inline TFastVector<MaxSize, Element>::TFastVector()
:	m_elementsCount(0)
{
}


template <int MaxSize, class Element>
inline TFastVector<MaxSize, Element>::TFastVector(int componentsCount, const Element& value)
:	m_elementsCount(qMin(MaxSize, componentsCount))
{
	for (int i = 0; i < m_elementsCount; ++i){
		m_elements[i] = value;
	}
}


template <int MaxSize, class Element>
inline TFastVector<MaxSize, Element>::TFastVector(const TFastVector<MaxSize, Element>& vector)
:	m_elementsCount(vector.m_elementsCount)
{
	Q_ASSERT(m_elementsCount <= MaxSize);

	std::memcpy(m_elements, vector.m_elements, sizeof(Element) * m_elementsCount);
}


template <int MaxSize, class Element>
inline TFastVector<MaxSize, Element>::TFastVector(std::initializer_list<Element> values)
:	m_elementsCount(qMin(MaxSize, static_cast<int>(values.size())))
{
	Q_ASSERT(values.size() <= MaxSize);

	for (int i = 0; i < m_elementsCount; ++i){
		m_elements[i] = *(values.begin() + i);
	}
}


template <int MaxSize, class Element>
inline int TFastVector<MaxSize, Element>::GetElementsCount() const
{
	return m_elementsCount;
}


template <int MaxSize, class Element>
inline bool TFastVector<MaxSize, Element>::SetElementsCount(int count, const Element& value)
{
	if (count <= MaxSize){
		for (int i = m_elementsCount; i < count; ++i){
			m_elements[i] = value;
		}

		m_elementsCount = count;

		return true;
	}
	else{
		return false;
	}
}


template <int MaxSize, class Element>
inline bool TFastVector<MaxSize, Element>::EnsureElementsCount(int count, const Element& value)
{
	if (count <= MaxSize){
		if (m_elementsCount < count){
			for (int i = m_elementsCount; i < count; ++i){
				m_elements[i] = value;
			}

			m_elementsCount = count;
		}

		return true;
	}
	else{
		return false;
	}
}


template <int MaxSize, class Element>
inline const Element& TFastVector<MaxSize, Element>::GetElement(int i) const
{
	Q_ASSERT(i >= 0);
	Q_ASSERT(i < m_elementsCount);

	return m_elements[i];
}


template <int MaxSize, class Element>
inline Element& TFastVector<MaxSize, Element>::GetElementRef(int i)
{
	Q_ASSERT(i >= 0);
	Q_ASSERT(i < m_elementsCount);

	return m_elements[i];
}


template <int MaxSize, class Element>
inline void TFastVector<MaxSize, Element>::SetElement(int i, const Element& value)
{
	Q_ASSERT(i >= 0);
	Q_ASSERT(i < m_elementsCount);

	m_elements[i] = value;
}


template <int MaxSize, class Element>
inline void TFastVector<MaxSize, Element>::Clear()
{
	for (int i = 0; i < m_elementsCount; ++i){
		m_elements[i] = 0.0;
	}
}


template <int MaxSize, class Element>
inline void TFastVector<MaxSize, Element>::Translate(const TFastVector<MaxSize, Element>& vector)
{
	int commonSize = qMin(m_elementsCount, vector.m_elementsCount);
	for (int i = 0; i < commonSize; ++i){
		m_elements[i] += vector.m_elements[i];
	}
}


template <int MaxSize, class Element>
inline void TFastVector<MaxSize, Element>::ScaledCumulate(const TFastVector<MaxSize, Element>& vector, Element scale)
{
	if (m_elementsCount < vector.m_elementsCount){
		int i = 0;
		for (; i < m_elementsCount; ++i){
			m_elements[i] += vector.m_elements[i] * scale;
		}

		for (; i < vector.m_elementsCount; ++i){
			m_elements[i] = vector.m_elements[i] * scale;
		}

		m_elementsCount = vector.m_elementsCount;
	}
	else{
		for (int i = 0; i < vector.m_elementsCount; ++i){
			m_elements[i] += vector.m_elements[i] * scale;
		}
	}
}


template <int MaxSize, class Element>
void TFastVector<MaxSize, Element>::SetElementsFrom(const TFastVector& vector, const Element& expansionValue)
{
	int commonSize = qMin(m_elementsCount, vector.m_elementsCount);

	for (int i = 0; i < commonSize; ++i){
		SetElement(i, vector[i]);
	}

	for (int i = commonSize; i < m_elementsCount; ++i){
		SetElement(i, expansionValue);
	}
}


template <int MaxSize, class Element>
inline TFastVector<MaxSize, Element> TFastVector<MaxSize, Element>::GetTranslated(const TFastVector<MaxSize, Element>& vector)
{
	return *this + vector;
}


template <int MaxSize, class Element>
void TFastVector<MaxSize, Element>::GetTranslated(const TFastVector<MaxSize, Element>& vector, TFastVector<MaxSize, Element>& result)
{

	result = *this;
	result.Translate(vector);
}


template <int MaxSize, class Element>
inline bool TFastVector<MaxSize, Element>::IsNull(Element tolerance) const
{
	return GetLength2() <= tolerance * tolerance;
}


template <int MaxSize, class Element>
inline Element TFastVector<MaxSize, Element>::GetDotProduct(const TFastVector<MaxSize, Element>& vector) const
{
	Element retVal = 0.0;

	int commonSize = qMin(m_elementsCount, vector.m_elementsCount);
	for (int i = 0; i < commonSize; ++i){
		retVal +=  m_elements[i] * vector.m_elements[i];
	}

	return retVal;
}


template <int MaxSize, class Element>
inline Element TFastVector<MaxSize, Element>::GetLength2() const
{
	return GetDotProduct(*this);
}


template <int MaxSize, class Element>
inline Element TFastVector<MaxSize, Element>::GetLength() const
{
	return qSqrt(GetLength2());
}


template <int MaxSize, class Element>
inline Element TFastVector<MaxSize, Element>::GetDistance2(const TFastVector<MaxSize, Element>& vector) const
{
	return (*this - vector).GetLength2();
}


template <int MaxSize, class Element>
inline Element TFastVector<MaxSize, Element>::GetDistance(const TFastVector<MaxSize, Element>& vector) const
{
	return qSqrt(GetDistance2(vector));
}


// operators

template <int MaxSize, class Element>
inline bool TFastVector<MaxSize, Element>::operator==(const TFastVector<MaxSize, Element>& vector) const
{
	if (m_elementsCount != vector.m_elementsCount){
		return false;
	}

	for (int i = 0; i < m_elementsCount; ++i){
		if (m_elements[i] != vector.m_elements[i]){
			return false;
		}
	}

	return true;
}


template <int MaxSize, class Element>
inline bool TFastVector<MaxSize, Element>::operator!=(const TFastVector<MaxSize, Element>& vector) const
{
	return !operator==(vector);
}


template <int MaxSize, class Element>
bool TFastVector<MaxSize, Element>::operator<(const TFastVector<MaxSize, Element>& vector) const
{
	int commonSize = qMin(m_elementsCount, vector.m_elementsCount);
	for (int i = 0; i < commonSize; ++i){
		if (m_elements[i] > vector.m_elements[i]){
			return false;
		}
		else if (m_elements[i] < vector.m_elements[i]){
			return true;
		}
	}

	return m_elementsCount < vector.m_elementsCount;
}


template <int MaxSize, class Element>
bool TFastVector<MaxSize, Element>::operator>(const TFastVector<MaxSize, Element>& vector) const
{
	int commonSize = qMin(m_elementsCount, vector.m_elementsCount);
	for (int i = 0; i < commonSize; ++i){
		if (m_elements[i] > vector.m_elements[i]){
			return true;
		}
		else if (m_elements[i] < vector.m_elements[i]){
			return false;
		}
	}

	return m_elementsCount > vector.m_elementsCount;
}


template <int MaxSize, class Element>
bool TFastVector<MaxSize, Element>::operator<=(const TFastVector<MaxSize, Element>& vector) const
{
	int commonSize = qMin(m_elementsCount, vector.m_elementsCount);
	for (int i = 0; i < commonSize; ++i){
		if (m_elements[i] > vector.m_elements[i]){
			return false;
		}
		else if (m_elements[i] < vector.m_elements[i]){
			return true;
		}
	}

	return m_elementsCount <= vector.m_elementsCount;
}


template <int MaxSize, class Element>
bool TFastVector<MaxSize, Element>::operator>=(const TFastVector<MaxSize, Element>& vector) const
{
	int commonSize = qMin(m_elementsCount, vector.m_elementsCount);
	for (int i = 0; i < commonSize; ++i){
		if (m_elements[i] > vector.m_elements[i]){
			return true;
		}
		else if (m_elements[i] < vector.m_elements[i]){
			return false;
		}
	}

	return m_elementsCount >= vector.m_elementsCount;
}


template <int MaxSize, class Element>
inline TFastVector<MaxSize, Element>& TFastVector<MaxSize, Element>::operator=(const TFastVector<MaxSize, Element>& vector)
{
	m_elementsCount = vector.m_elementsCount;
	Q_ASSERT(m_elementsCount < MaxSize);

	for (int i = 0; i < m_elementsCount; ++i){
		m_elements[i] = vector.m_elements[i];
	}

	return *this;
}


template <int MaxSize, class Element>
inline TFastVector<MaxSize, Element>& TFastVector<MaxSize, Element>::operator+=(const TFastVector<MaxSize, Element>& vector)
{
	if (m_elementsCount < vector.m_elementsCount){
		int i = 0;
		for (; i < m_elementsCount; ++i){
			m_elements[i] += vector.m_elements[i];
		}

		for (; i < vector.m_elementsCount; ++i){
			m_elements[i] = vector.m_elements[i];
		}

		m_elementsCount = vector.m_elementsCount;
	}
	else{
		for (int i = 0; i < vector.m_elementsCount; ++i){
			m_elements[i] += vector.m_elements[i];
		}
	}

	return *this;
}


template <int MaxSize, class Element>
inline TFastVector<MaxSize, Element>& TFastVector<MaxSize, Element>::operator-=(const TFastVector<MaxSize, Element>& vector)
{
	if (m_elementsCount < vector.m_elementsCount){
		int i = 0;
		for (; i < m_elementsCount; ++i){
			m_elements[i] -= vector.m_elements[i];
		}

		for (; i < vector.m_elementsCount; ++i){
			m_elements[i] = -vector.m_elements[i];
		}

		m_elementsCount = vector.m_elementsCount;
	}
	else{
		for (int i = 0; i < vector.m_elementsCount; ++i){
			m_elements[i] -= vector.m_elements[i];
		}
	}

	return *this;
}


template <int MaxSize, class Element>
inline TFastVector<MaxSize, Element>& TFastVector<MaxSize, Element>::operator*=(Element scalar)
{
	for (int i = 0; i < m_elementsCount; ++i){
		m_elements[i] *= scalar;
	}

	return *this;
}


template <int MaxSize, class Element>
inline TFastVector<MaxSize, Element>& TFastVector<MaxSize, Element>::operator/=(Element scalar)
{
	for (int i = 0; i < m_elementsCount; ++i){
		m_elements[i] /= scalar;
	}

	return *this;
}


template <int MaxSize, class Element>
inline TFastVector<MaxSize, Element> TFastVector<MaxSize, Element>::operator-() const
{
	TFastVector<MaxSize, Element> retVal(*this);

	for (int i = 0; i < m_elementsCount; ++i){
		retVal.m_elements[i] = -m_elements[i];
	}

	return retVal;
}


template <int MaxSize, class Element>
inline TFastVector<MaxSize, Element> TFastVector<MaxSize, Element>::operator+(const TFastVector<MaxSize, Element>& vector) const
{
	TFastVector<MaxSize, Element> retVal(*this);

	retVal += vector;

	return retVal;
}


template <int MaxSize, class Element>
inline TFastVector<MaxSize, Element> TFastVector<MaxSize, Element>::operator-(const TFastVector<MaxSize, Element>& vector) const
{
	TFastVector<MaxSize, Element> retVal(*this);

	retVal -= vector;

	return retVal;
}


template <int MaxSize, class Element>
inline TFastVector<MaxSize, Element> TFastVector<MaxSize, Element>::operator*(Element scalar) const
{
	TFastVector<MaxSize, Element> retVal(*this);

	retVal *= scalar;

	return retVal;
}


template <int MaxSize, class Element>
inline TFastVector<MaxSize, Element> TFastVector<MaxSize, Element>::operator/(Element scalar) const
{
	TFastVector<MaxSize, Element> retVal(*this);

	retVal /= scalar;

	return retVal;
}


template <int MaxSize, class Element>
const Element& TFastVector<MaxSize, Element>::operator[](int i) const
{
	Q_ASSERT(i >= 0);
	Q_ASSERT(i < m_elementsCount);

	return m_elements[i];
}


template <int MaxSize, class Element>
Element& TFastVector<MaxSize, Element>::operator[](int i)
{
	Q_ASSERT(i >= 0);
	Q_ASSERT(i < MaxSize);

	return m_elements[i];
}


// public methods

template <int MaxSize, class Element>
Element TFastVector<MaxSize, Element>::GetElementsSum() const
{
	Element retVal = 0;

	for (int i = 0; i < m_elementsCount; ++i){
		retVal += m_elements[i];
	}

	return retVal;
}


template <int MaxSize, class Element>
bool TFastVector<MaxSize, Element>::Normalize(Element length)
{
	Element isLength = GetLength();

	Element proportion = isLength / length;

	if (qAbs(proportion) > I_BIG_EPSILON){
		for (int i = 0; i < m_elementsCount; ++i){
			m_elements[i] = m_elements[i] / proportion;
		}

		return true;
	}
	else{
		return false;
	}
}


template <int MaxSize, class Element>
bool TFastVector<MaxSize, Element>::GetNormalized(TFastVector<MaxSize, Element>& result, Element length) const
{
	result = *this;

	return result.Normalize(length);
}


template <int MaxSize, class Element>
void TFastVector<MaxSize, Element>::GetMinimal(const TFastVector<MaxSize, Element>& vector, TFastVector<MaxSize, Element>& result) const
{
	int elementsCount = qMin(GetElementsCount(), vector.GetElementsCount());

	result.SetElementsCount(elementsCount);

	for (int i = 0; i < elementsCount; ++i){
		result.SetElement(i, qMin(GetElement(i), vector.GetElement(i)));
	}
}


template <int MaxSize, class Element>
void TFastVector<MaxSize, Element>::GetMaximal(const TFastVector<MaxSize, Element>& vector, TFastVector<MaxSize, Element>& result) const
{
	int elementsCount = qMin(GetElementsCount(), vector.GetElementsCount());

	result.SetElementsCount(elementsCount);

	for (int i = 0; i < elementsCount; ++i){
		result.SetElement(i, qMax(GetElement(i), vector.GetElement(i)));
	}
}


template <int MaxSize, class Element>
bool TFastVector<MaxSize, Element>::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag elementsTag("Elements", "List of vector element", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag elementTag("Element", "Single vector element", iser::CArchiveTag::TT_LEAF, &elementsTag);

	retVal = retVal && archive.BeginMultiTag(elementsTag, elementTag, m_elementsCount);

	if (!retVal || (m_elementsCount > MaxSize)){
		if (!archive.IsStoring()){
			m_elementsCount = 0;
		}

		return false;
	}

	for (int i = 0; i < m_elementsCount; ++i){
		retVal = retVal && archive.BeginTag(elementTag);
		retVal = retVal && archive.Process(m_elements[i]);
		retVal = retVal && archive.EndTag(elementTag);
	}

	retVal = retVal && archive.EndTag(elementsTag);

	return retVal;
}


// related global functions

template <int MaxSize, class Element>
inline uint qHash(const TFastVector<MaxSize, Element>& v, uint seed = 0)
{
	uint retVal = seed;

	int elementsCount = v.GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		double element = v[i];

		retVal = (retVal << 1) ^ *((uint*)&element) + 1;
	}

	return retVal;
}


} // namespace imath


