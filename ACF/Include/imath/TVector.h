#ifndef imath_TVector_included
#define imath_TVector_included


#include <cmath>

#include "iser/IArchive.h"

#include "imath/imath.h"


namespace imath{


/**	Simple implementation of fixed-size vector.
 */
template <int Size, class ElementType = double>
class TVector
{
public:
	/**
		Create an uninitialized point.
	 */
	TVector();
	/**
		Copy constructor.
	 */
	TVector(const TVector<Size, ElementType>& vector);

	/**
		Get element at specified index.
	*/
	const ElementType& GetElement(int index) const;

	/**
		Get reference to element at specified index.
	*/
	ElementType& GetElementRef(int index);

	/**
		Set element at specified index.
	*/
	void SetElement(int index, const ElementType& value);

	/**
		Set all coordinates to zero.
	*/
	void Reset();

	/**
		Translate the point.
	*/
	void Translate(const TVector<Size, ElementType>& vector);

	/**
		Get translated point.
	*/
	TVector<Size, ElementType> GetTranslated(const TVector<Size, ElementType>& vector);

	/**
		/overloaded
	*/
	void GetTranslated(const TVector<Size, ElementType>& vector, TVector<Size, ElementType>& result);

	/**
		Check if this vector is null.
	*/
	bool IsNull(ElementType tolerance = I_BIG_EPSILON) const;

	/**
		Return dot product of two vectors.
	*/
	ElementType GetDotProduct(const TVector<Size, ElementType>& vector) const;

	/**
		Return euclidian length square.
	*/
	ElementType GetLength2() const;
	/**
		Return euclidian length.
	*/
	ElementType GetLength() const;

	/**
		Return distance square between two vectors.
	*/
	ElementType GetDistance2(const TVector<Size, ElementType>& vector) const;

	/**
		Return distance between two vectors.
	*/
	ElementType GetDistance(const TVector<Size, ElementType>& vector) const;

	/**
		Normalize vector to specified length.
		\param	length	new vector length.
		\return	true, if normalization successed.
	*/
	bool Normalize(ElementType length = 1.0);
	/**
		Return normalized vector with the same direction and specified length.
		\param	length	new vector length.
		\return	true, if normalization successed.
	*/
	bool GetNormalized(TVector<Size, ElementType>& result, ElementType length = 1.0) const;

	/**
		Serialize this vector to specified archive.
	*/
	bool Serialize(iser::IArchive& archive);

	bool operator==(const TVector<Size, ElementType>& vector) const;
	bool operator!=(const TVector<Size, ElementType>& vector) const;

	TVector<Size, ElementType> operator-() const;

	TVector<Size, ElementType> operator+(const TVector<Size, ElementType>& vector) const;
	TVector<Size, ElementType> operator-(const TVector<Size, ElementType>& vector) const;
	TVector<Size, ElementType> operator*(ElementType scalar) const;
	TVector<Size, ElementType> operator/(ElementType scalar) const;

	TVector<Size, ElementType>& operator+=(const TVector<Size, ElementType>& vector);
	TVector<Size, ElementType>& operator-=(const TVector<Size, ElementType>& vector);
	TVector<Size, ElementType>& operator*=(ElementType scalar);
	TVector<Size, ElementType>& operator/=(ElementType scalar);

	const ElementType& operator[](int index) const;
	ElementType& operator[](int index);

	// static methods

	/**
		Get number of elements.
	*/
	static int GetElementsCount();


protected:
    ElementType m_elements[Size];
};


// inline constructors

template <int Size, class ElementType>
inline TVector<Size, ElementType>::TVector()
{
}


template <int Size, class ElementType>
inline TVector<Size, ElementType>::TVector(const TVector<Size, ElementType>& vector)
{
	for (int index = 0; index < Size; ++index){
		m_elements[index] = vector.m_elements[index];
	}
}


// inline methods

template <int Size, class ElementType>
inline typename const ElementType& TVector<Size, ElementType>::GetElement(int index) const
{
	return operator[](index);
}


template <int Size, class ElementType>
inline typename ElementType& TVector<Size, ElementType>::GetElementRef(int index)
{
	return operator[](index);
}


template <int Size, class ElementType>
inline void TVector<Size, ElementType>::SetElement(int index, const ElementType& value)
{
	operator[](index) = value;
}


template <int Size, class ElementType>
inline void TVector<Size, ElementType>::Reset()
{
	for (int index = 0; index < Size; ++index){
		m_elements[index] = 0.0;
	}
}


template <int Size, class ElementType>
inline void TVector<Size, ElementType>::Translate(const TVector<Size, ElementType>& vector)
{
	for (int index = 0; index < Size; ++index){
		m_elements[index] += vector.m_elements[index];
	}
}


template <int Size, class ElementType>
TVector<Size, ElementType> TVector<Size, ElementType>::GetTranslated(const TVector<Size, ElementType>& vector)
{
	return *this + vector;
}


template <int Size, class ElementType>
void TVector<Size, ElementType>::GetTranslated(const TVector<Size, ElementType>& vector, TVector<Size, ElementType>& result)
{
	result = *this + vector;
}


template <int Size, class ElementType>
inline bool TVector<Size, ElementType>::IsNull(ElementType tolerance) const
{
	return GetLength2() <= tolerance * tolerance;
}


template <int Size, class ElementType>
inline ElementType TVector<Size, ElementType>::GetDotProduct(const TVector<Size, ElementType>& vector) const
{
	ElementType retVal = 0.0;

	for (int index = 0; index < Size; ++index){
		retVal +=  m_elements[index] * vector.m_elements[index];
	}

	return retVal;
}


template <int Size, class ElementType>
inline ElementType TVector<Size, ElementType>::GetLength2() const
{
	return GetDotProduct(*this);
}


template <int Size, class ElementType>
inline ElementType TVector<Size, ElementType>::GetLength() const
{
	return ::sqrt(GetLength2());
}


template <int Size, class ElementType>
inline ElementType TVector<Size, ElementType>::GetDistance2(const TVector<Size, ElementType>& vector) const
{
	return (*this - vector).GetLength2();
}


template <int Size, class ElementType>
inline ElementType TVector<Size, ElementType>::GetDistance(const TVector<Size, ElementType>& vector) const
{
	return ::sqrt(GetDistance2(vector));
}


// operators

template <int Size, class ElementType>
inline bool TVector<Size, ElementType>::operator==(const TVector<Size, ElementType>& vector) const
{
	for (int index = 0; index < Size; ++index){
		if (m_elements[index] != vector.m_elements[index]){
			return false;
		}
	}
	return true;
}


template <int Size, class ElementType>
inline bool TVector<Size, ElementType>::operator!=(const TVector<Size, ElementType>& vector) const
{
	return !operator==(vector);
}


template <int Size, class ElementType>
inline TVector<Size, ElementType>& TVector<Size, ElementType>::operator+=(const TVector<Size, ElementType>& vector)
{
	for (int index = 0; index < Size; ++index){
		m_elements[index] += vector.m_elements[index];
	}

	return *this;
}


template <int Size, class ElementType>
inline TVector<Size, ElementType>& TVector<Size, ElementType>::operator-=(const TVector<Size, ElementType>& vector)
{
	for (int index = 0; index < Size; ++index){
		m_elements[index] -= vector.m_elements[index];
	}

	return *this;
}


template <int Size, class ElementType>
inline TVector<Size, ElementType>& TVector<Size, ElementType>::operator*=(ElementType scalar)
{
	for (int index = 0; index < Size; ++index){
		m_elements[index] *= scalar;
	}

	return *this;
}


template <int Size, class ElementType>
inline TVector<Size, ElementType>& TVector<Size, ElementType>::operator/=(ElementType scalar)
{
	for (int index = 0; index < Size; ++index){
		m_elements[index] /= scalar;
	}

	return *this;
}


template <int Size, class ElementType>
inline TVector<Size, ElementType> TVector<Size, ElementType>::operator-() const
{
	TVector<Size, ElementType> retVal;

	for (int index = 0; index < Size; ++index){
		retVal.m_elements[index] = -m_elements[index];
	}

	return retVal;
}


template <int Size, class ElementType>
inline TVector<Size, ElementType> TVector<Size, ElementType>::operator+(const TVector<Size, ElementType>& vector) const
{
	TVector<Size, ElementType> retVal;

	for (int index = 0; index < Size; ++index){
		retVal.m_elements[index] = m_elements[index] + vector.m_elements[index];
	}

	return retVal;
}


template <int Size, class ElementType>
inline TVector<Size, ElementType> TVector<Size, ElementType>::operator-(const TVector<Size, ElementType>& vector) const
{
	TVector<Size, ElementType> retVal;

	for (int index = 0; index < Size; ++index){
		retVal.m_elements[index] = m_elements[index] - vector.m_elements[index];
	}

	return retVal;
}


template <int Size, class ElementType>
inline TVector<Size, ElementType> TVector<Size, ElementType>::operator*(ElementType scalar) const
{
	TVector<Size, ElementType> retVal;

	for (int index = 0; index < Size; ++index){
		retVal.m_elements[index] = m_elements[index] * scalar;
	}

	return retVal;
}


template <int Size, class ElementType>
inline TVector<Size, ElementType> TVector<Size, ElementType>::operator/(ElementType scalar) const
{
	TVector<Size, ElementType> retVal;

	for (int index = 0; index < Size; ++index){
		retVal.m_elements[index] = m_elements[index] / scalar;
	}

	return retVal;
}


template <int Size, class ElementType>
const ElementType& TVector<Size, ElementType>::operator[](int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < Size);

	return m_elements[index];
}


template <int Size, class ElementType>
ElementType& TVector<Size, ElementType>::operator[](int index)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < Size);

	return m_elements[index];
}


// static inline methods

template <int Size, class ElementType>
inline int TVector<Size, ElementType>::GetElementsCount()
{
	return Size;
}


// public methods

template <int Size, class ElementType>
bool TVector<Size, ElementType>::Normalize(ElementType length)
{
    ElementType isLength = GetLength();

    ElementType proportion = isLength / length;

	if (::fabs(proportion) < I_BIG_EPSILON){
        for (int index = 0; index < Size; ++index){
            m_elements[index] = m_elements[index] / proportion;
        }

		return true;
    }
	else{
		return false;
	}
}


template <int Size, class ElementType>
bool TVector<Size, ElementType>::GetNormalized(TVector<Size, ElementType>& result, ElementType length) const
{
    ElementType isLength = GetLength();

    ElementType proportion = isLength / length;

	if (::fabs(proportion) < I_BIG_EPSILON){
        ElementType proportion = length / isLength;

        for (int index = 0; index < Size; ++index){
            result.m_elements[index] = m_elements[index] * proportion;
        }

		return true;
    }
	else{
		return false;
	}
}


template <int Size, class ElementType>
bool TVector<Size, ElementType>::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	for (int index = 0; index < Size; ++index){
		retVal = retVal && archive.Process(m_elements[index]);
	}

	return retVal;
}


} // namespace imath


#endif //!imath_TVector_included


