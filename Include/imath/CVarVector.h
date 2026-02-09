// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// STL includes
#include <vector>

// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtCore/QtMath>
#else
#include <QtCore/qmath.h>
#endif
#include <QtCore/QVector>

// ACF includes
#include <imath/TVector.h>


namespace iser
{
	class IArchive;
}


namespace imath
{


/**
	Implementation of variable-size mathematical vector with double precision elements.
	
	\section VarVectorPurpose Purpose
	CVarVector is a dynamic-size vector class that provides mathematical vector operations
	similar to TVector but with runtime-determined size. It's ideal for scenarios where
	the vector dimension is not known at compile time or needs to change during execution.
	The vector stores double precision floating-point values and supports common operations
	like addition, scaling, dot product, normalization, and distance calculations.
	
	\section VarVectorVsFixed CVarVector vs TVector
	- **CVarVector**: Dynamic size determined at runtime, stored on heap, flexible resizing
	- **TVector**: Fixed size at compile time, stored on stack, more efficient for known dimensions
	
	Use CVarVector when:
	- Vector dimension is determined at runtime (e.g., from file or user input)
	- You need to resize vectors dynamically
	- Working with vectors of varying dimensions in the same data structure
	
	Use TVector when:
	- Vector dimension is known at compile time (e.g., 2D/3D graphics)
	- Performance is critical and dimension is fixed
	- Want compile-time size checking
	
	\section VarVectorUsageExamples Usage Examples
	\code
	// Create empty vector and set size later
	imath::CVarVector vec1;
	vec1.SetElementsCount(3);
	vec1.SetElement(0, 1.0);
	vec1.SetElement(1, 2.0);
	vec1.SetElement(2, 3.0);
	
	// Create vector with initial size and value
	imath::CVarVector vec2(5, 0.0);  // 5 elements, all initialized to 0
	
	// Create from TVector
	imath::TVector<3, double> fixedVec = {1.0, 2.0, 3.0};
	imath::CVarVector vec3(fixedVec);
	
	// Vector operations
	imath::CVarVector vec4(3);
	vec4.SetElement(0, 4.0);
	vec4.SetElement(1, 5.0);
	vec4.SetElement(2, 6.0);
	
	// Addition
	vec1.Translate(vec4);  // vec1 += vec4
	
	// Dot product
	double dot = vec1.GetDotProduct(vec4);  // 1*4 + 2*5 + 3*6 = 32
	
	// Length and normalization
	double length = vec1.GetLength();
	if (vec1.Normalize()) {
		// vec1 is now a unit vector
	}
	
	// Dynamic resizing
	vec1.EnsureElementsCount(10, 0.0);  // Grow to at least 10 elements
	\endcode
	
	\section VarVectorOperations Common Operations
	- **Construction**: Default, size-based, copy, from TVector, from iterators
	- **Element Access**: GetElement(), SetElement(), operator[], GetElements()
	- **Sizing**: SetElementsCount(), EnsureElementsCount(), IsEmpty()
	- **Initialization**: Clear(), Reset(), SetAllElements(), SetElementsFrom()
	- **Arithmetic**: Translate(), ScaledCumulate(), operators (+, -, *, /)
	- **Metrics**: GetLength(), GetLength2(), GetDistance(), GetDotProduct()
	- **Normalization**: Normalize(), GetNormalized()
	- **Comparison**: IsNull(), operators (==, !=, <, >, <=, >=)
	- **Min/Max**: GetMinimal(), GetMaximal()
	- **Serialization**: Serialize()
	
	\sa imath::TVector, imath::TFastVector, imath::CVarMatrix
	
	\ingroup Geometry
*/
class CVarVector
{
public:
	typedef std::vector<double> Elements;

	/**
		Create the vector without components.
	 */
	CVarVector();

	/**
		Create vector and initialize number of components.
	*/
	explicit CVarVector(int componentsCount, double value = 0);

	/**
		Creates a vector from a range of values using iterators.
		
		This constructor enables creation from any container or sequence that provides
		iterators (e.g., std::vector, std::list, arrays).
		
		\param beginIter Iterator to the first element
		\param endIter Iterator past the last element
		
		\code
		std::vector<double> values = {1.0, 2.0, 3.0, 4.0};
		imath::CVarVector vec(values.begin(), values.end());
		// vec now contains {1.0, 2.0, 3.0, 4.0}
		\endcode
	*/
	template <typename Iterator>
	CVarVector(Iterator beginIter, Iterator endIter);

	/**
		Copy constructor.
	 */
	CVarVector(const CVarVector& vector);

	/**
		Creates a variable-size vector from a fixed-size TVector.
		
		Allows conversion from compile-time sized vectors to runtime-sized vectors.
		Useful when interfacing between subsystems using different vector types.
		
		\param vector The fixed-size TVector to convert from
		
		\code
		imath::TVector<3, double> fixedVec = {1.0, 2.0, 3.0};
		imath::CVarVector varVec(fixedVec);
		// varVec.GetElementsCount() == 3
		\endcode
	*/
	template <int Size>
	CVarVector(const TVector<Size, double>& vector);

	/**
		Get \c true, if the element ist is empty.
	*/
	bool IsEmpty() const;

	/**
		Get number of elements.
	*/
	int GetElementsCount() const;

	/**
		Set number of elements.
		\return	always true, this value is provided for template implementations.
	*/
	bool SetElementsCount(int count, double value = 0);

	/**
		Get element at specified index.
	*/
	double GetElement(int index) const;

	/**
		Get reference to element at specified index.
	*/
	double& GetElementRef(int index);

	/**
		Set element at specified index.
	*/
	void SetElement(int index, double value);

	/**
		Set some value to all elements.
	*/
	void SetAllElements(double value);

	/**
		Set size to zero.
	*/
	void Reset();

	/**
		Set number of elements and set all elements to specified value.
	*/
	void Reset(int elementsCount, double value);

	/**
		Set all coordinates to zero.
	*/
	void Clear();

	/**
		Set elements from other vector without resizing.
		\param	vector			source of element values will be copied.
		\param	expansionValue	if actual vector has more elements than \c vector, rest will be replaced with this value.
	*/
	void SetElementsFrom(const CVarVector& vector, double expansionValue = 0);

	/**
		Get read-only access to internal element container.
	*/
	const Elements& GetElements() const;

	/**
		Get access to internal element container.
	*/
	Elements& GetElementsRef();

	/**
		Ensure, that number of elements is at least the specified value.
		It resize the vector if the new size is bigger than the current one.
		\return	always true, this value is provided for template implementations.
	*/
	bool EnsureElementsCount(int count, double value = 0);

	/**
		Translate the point.
	*/
	void Translate(const CVarVector& vector);

	/**
		Get translated point.
	*/
	CVarVector GetTranslated(const CVarVector& vector);

	/**
		/overloaded
	*/
	void GetTranslated(const CVarVector& vector, CVarVector& result);

	/**
		Add second vector scaled by specified factor.
		It is equal of Translate(vector * scale) but can be faster implemented.
	*/
	void ScaledCumulate(const CVarVector& vector, double scale);

	/**
		Check if this vector is null.
	*/
	bool IsNull(double tolerance = I_BIG_EPSILON) const;

	/**
		Return dot product of two vectors.
	*/
	double GetDotProduct(const CVarVector& vector) const;

	/**
		Return euclidean length square.
	*/
	double GetLength2() const;

	/**
		Return euclidian length.
	*/
	double GetLength() const;

	/**
		Return distance square between two vectors.
	*/
	double GetDistance2(const CVarVector& vector) const;

	/**
		Return distance between two vectors.
	*/
	double GetDistance(const CVarVector& vector) const;

	/**
		Get simple sum of all elements.
	*/
	double GetElementsSum() const;

	/**
		Normalize vector to specified length.
		\param	length	new vector length.
		\return	true, if normalization succeeded.
	*/
	bool Normalize(double length = 1.0);

	/**
		Return normalized vector with the same direction and specified length.
		\param	length	new vector length.
		\return	true, if normalization succeeded.
	*/
	bool GetNormalized(CVarVector& result, double length = 1.0) const;

	/**
		Get vector with minimal elements values.
	*/
	void GetMinimal(const CVarVector& vector, CVarVector& result) const;

	/**
		Get vector with maximal elements values.
	*/
	void GetMaximal(const CVarVector& vector, CVarVector& result) const;

	/**
		Serialize this vector to specified archive.
	*/
	bool Serialize(iser::IArchive& archive);

	bool operator==(const CVarVector& vector) const;
	bool operator!=(const CVarVector& vector) const;
	bool operator<(const CVarVector& vector) const;
	bool operator>(const CVarVector& vector) const;
	bool operator<=(const CVarVector& vector) const;
	bool operator>=(const CVarVector& vector) const;

	CVarVector operator-() const;

	CVarVector operator+(const CVarVector& vector) const;
	CVarVector operator-(const CVarVector& vector) const;
	CVarVector operator*(double scalar) const;
	CVarVector operator/(double scalar) const;

	CVarVector& operator+=(const CVarVector& vector);
	CVarVector& operator-=(const CVarVector& vector);
	CVarVector& operator*=(double scalar);
	CVarVector& operator/=(double scalar);

	CVarVector& operator=(const CVarVector& vector);

	double operator[](int i) const;
	double& operator[](int i);

private:
	Elements m_elements;
};


// inline constructors

inline CVarVector::CVarVector()
{
}


inline CVarVector::CVarVector(int componentsCount, double value)
:	m_elements(componentsCount, value)
{
}


inline CVarVector::CVarVector(const CVarVector& vector)
:	m_elements(vector.m_elements)
{
}


template <typename Iterator>
inline CVarVector::CVarVector(Iterator beginIter, Iterator endIter)
:	m_elements(beginIter, endIter)
{
}


// inline methods

inline bool CVarVector::IsEmpty() const
{
	return m_elements.empty();
}


inline int CVarVector::GetElementsCount() const
{
	return int(m_elements.size());
}


inline bool CVarVector::SetElementsCount(int count, double value)
{
	Q_ASSERT(count >= 0);

	int oldCount = int(m_elements.size());

	m_elements.resize(count);

	for (int i = oldCount; i < count; ++i){
		m_elements[i] = value;
	}

	return true;
}


inline double CVarVector::GetElement(int i) const
{
	return operator[](i);
}


inline double& CVarVector::GetElementRef(int i)
{
	return operator[](i);
}


inline void CVarVector::SetElement(int i, double value)
{
	operator[](i) = value;
}


inline void CVarVector::SetAllElements(double value)
{
	int elementsCount = GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		m_elements[i] = value;
	}
}


inline void CVarVector::Reset()
{
	m_elements.clear();
}


inline void CVarVector::Clear()
{
	int elementsCount = GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		m_elements[i] = 0;
	}
}


inline void CVarVector::Reset(int elementsCount, double value)
{
	m_elements.resize(elementsCount);

	for (int i = 0; i < elementsCount; ++i){
		m_elements[i] = value;
	}
}


inline const CVarVector::Elements& CVarVector::GetElements() const
{
	return m_elements;
}


inline CVarVector::Elements& CVarVector::GetElementsRef()
{
	return m_elements;
}


inline void CVarVector::Translate(const CVarVector& vector)
{
	Q_ASSERT(GetElementsCount() == vector.GetElementsCount());

	int elementsCount = GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		m_elements[i] += vector.m_elements[i];
	}
}


inline CVarVector CVarVector::GetTranslated(const CVarVector& vector)
{
	return *this + vector;
}


inline void CVarVector::GetTranslated(const CVarVector& vector, CVarVector& result)
{
	result = *this + vector;
}


inline void CVarVector::ScaledCumulate(const CVarVector& vector, double scale)
{
	Q_ASSERT(GetElementsCount() == vector.GetElementsCount());

	int elementsCount = GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		m_elements[i] += vector.m_elements[i] * scale;
	}
}


inline bool CVarVector::IsNull(double tolerance) const
{
	return GetLength2() <= tolerance * tolerance;
}


inline double CVarVector::GetDotProduct(const CVarVector& vector) const
{
	Q_ASSERT(GetElementsCount() == vector.GetElementsCount());

	double retVal = 0.0;

	int elementsCount = GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		retVal += m_elements[i] * vector.m_elements[i];
	}

	return retVal;
}


inline double CVarVector::GetLength2() const
{
	return GetDotProduct(*this);
}


inline double CVarVector::GetLength() const
{
	return qSqrt(GetLength2());
}


inline double CVarVector::GetDistance2(const CVarVector& vector) const
{
	return (*this - vector).GetLength2();
}


inline double CVarVector::GetDistance(const CVarVector& vector) const
{
	return qSqrt(GetDistance2(vector));
}


// operators

inline bool CVarVector::operator==(const CVarVector& vector) const
{
	if (m_elements.size() != vector.m_elements.size()){
		return false;
	}

	bool retVal = true;
	for (		Elements::const_iterator iter1 = m_elements.begin(), iter2 = vector.m_elements.begin();
				iter1 != m_elements.end();
				++iter1,++iter2){
		retVal = retVal && qFuzzyCompare(*iter1, *iter2);
	}

	return retVal;
}


inline bool CVarVector::operator!=(const CVarVector& vector) const
{
	return !operator==(vector);
}


inline bool CVarVector::operator<(const CVarVector& vector) const
{
	int count = int(m_elements.size());
	int vectorCount = int(vector.m_elements.size());
	int commonSize = qMin(count, vectorCount);
	for (int i = 0; i < commonSize; ++i){
		double element = m_elements[i];
		double vectorElement = vector.m_elements[i];
		if (!qFuzzyCompare(element, vectorElement)){
			return m_elements[i] < vectorElement;
		}
	}

	return count < vectorCount;
}


inline bool CVarVector::operator>(const CVarVector& vector) const
{
	int count = int(m_elements.size());
	int vectorCount = int(vector.m_elements.size());
	int commonSize = qMin(count, vectorCount);
	for (int i = 0; i < commonSize; ++i){
		double element = m_elements[i];
		double vectorElement = vector.m_elements[i];
		if (!qFuzzyCompare(element, vectorElement)){
			return element > vectorElement;
		}
	}

	return count > vectorCount;
}


inline bool CVarVector::operator<=(const CVarVector& vector) const
{
	int count = int(m_elements.size());
	int vectorCount = int(vector.m_elements.size());
	int commonSize = qMin(count, vectorCount);
	for (int i = 0; i < commonSize; ++i){
		double element = m_elements[i];
		double vectorElement = vector.m_elements[i];
		if (!qFuzzyCompare(element, vectorElement)){
			return element < vectorElement;
		}
	}

	return count <= vectorCount;
}


inline bool CVarVector::operator>=(const CVarVector& vector) const
{
	int count = int(m_elements.size());
	int vectorCount = int(vector.m_elements.size());
	int commonSize = qMin(count, vectorCount);
	for (int i = 0; i < commonSize; ++i){
		double element = m_elements[i];
		double vectorElement = vector.m_elements[i];
		if (!qFuzzyCompare(element, vectorElement)){
			return element > vectorElement;
		}
	}

	return count >= vectorCount;
}


inline CVarVector& CVarVector::operator+=(const CVarVector& vector)
{
	Q_ASSERT(GetElementsCount() == vector.GetElementsCount());

	int elementsCount = GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		m_elements[i] += vector.m_elements[i];
	}

	return *this;
}


inline CVarVector& CVarVector::operator-=(const CVarVector& vector)
{
	Q_ASSERT(GetElementsCount() == vector.GetElementsCount());

	int elementsCount = GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		m_elements[i] -= vector.m_elements[i];
	}

	return *this;
}


inline CVarVector& CVarVector::operator*=(double scalar)
{
	int elementsCount = GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		m_elements[i] *= scalar;
	}

	return *this;
}


inline CVarVector& CVarVector::operator/=(double scalar)
{
	int elementsCount = GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		m_elements[i] /= scalar;
	}

	return *this;
}


inline CVarVector& CVarVector::operator=(const CVarVector& vector)
{
	SetElementsCount(vector.GetElementsCount(), 0);
	SetElementsFrom(vector);

	return *this;
}


inline CVarVector CVarVector::operator-() const
{
	int elementsCount = GetElementsCount();

	CVarVector retVal(elementsCount);

	for (int i = 0; i < elementsCount; ++i){
		retVal.m_elements[i] = -m_elements[i];
	}

	return retVal;
}


inline CVarVector CVarVector::operator+(const CVarVector& vector) const
{
	CVarVector retVal(*this);

	retVal += vector;

	return retVal;
}


inline CVarVector CVarVector::operator-(const CVarVector& vector) const
{
	CVarVector retVal(*this);

	retVal -= vector;

	return retVal;
}


inline CVarVector CVarVector::operator*(double scalar) const
{
	CVarVector retVal(*this);

	retVal *= scalar;

	return retVal;
}


inline CVarVector CVarVector::operator/(double scalar) const
{
	CVarVector retVal(*this);

	retVal /= scalar;

	return retVal;
}


inline double CVarVector::operator[](int i) const
{
	Q_ASSERT(i >= 0);
	Q_ASSERT(i < GetElementsCount());

	return m_elements[i];
}


inline double& CVarVector::operator[](int i)
{
	Q_ASSERT(i >= 0);
	Q_ASSERT(i < GetElementsCount());

	return m_elements[i];
}


// template methods

template <int Size>
CVarVector::CVarVector(const TVector<Size, double>& vector)
{
	m_elements.resize(Size);

	for (int i = 0; i < Size; ++i){
		m_elements[i] = vector[i];
	}
}


} // namespace imath


