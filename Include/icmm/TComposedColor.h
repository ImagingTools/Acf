#pragma once


// STL includes
#include <cstring>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <iser/ISerializable.h>
#include <iser/CArchiveTag.h>
#include <imath/IDoubleManip.h>
#include <imath/TVector.h>
#include <imath/CVarVector.h>
#include <icmm/icmm.h>


namespace icmm
{


/**
	Generic fixed-size color implementation template.
	
	\section ComposedColorPurpose Purpose
	TComposedColor is a template base class for fixed-size color representations.
	It combines the mathematical vector operations of TVector with color-specific
	functionality like normalization, rounding, and serialization. This class serves
	as the foundation for CRgb, CHsv, CCmyk, CLab, and other fixed-size color classes.

	\section ComposedColorDesign Design
	TComposedColor<Size> inherits from TVector<Size> and adds:
	- **Normalization**: Clamping components to [0, 1] range
	- **Comparison**: Fuzzy equality with tolerance
	- **Rounding**: Precision control for component values
	- **Serialization**: Save/load via IArchive interface
	- **Operators**: Color-specific arithmetic operations

	\section ComposedColorUsage Usage Examples
	\code
	// Example 1: TComposedColor as base for custom colors
	// (CRgb, CHsv, etc. inherit from TComposedColor)
	class CMyCustomColor : public icmm::TComposedColor<5>
	{
	public:
		CMyCustomColor(double c1 = 0, double c2 = 0, double c3 = 0,
		              double c4 = 0, double c5 = 0)
		{
			SetElement(0, c1);
			SetElement(1, c2);
			SetElement(2, c3);
			SetElement(3, c4);
			SetElement(4, c5);
		}
	};

	// Example 2: Normalization (clamping to [0,1])
	icmm::CRgb rgb(1.5, -0.2, 0.8);  // Out of range values
	
	if (!rgb.IsNormalized()) {
		rgb.Normalize();  // Now (1.0, 0.0, 0.8)
	}

	// Example 3: Fuzzy comparison
	icmm::CRgb color1(0.5, 0.5, 0.5);
	icmm::CRgb color2(0.501, 0.499, 0.502);
	
	bool similar = color1.IsSimilar(color2, 0.01);  // true
	bool exact = (color1 == color2);                // false (if == is implemented)

	// Example 4: Color arithmetic (inherited from TComposedColor)
	icmm::CRgb a(0.8, 0.3, 0.2);
	icmm::CRgb b(0.2, 0.5, 0.7);
	
	// Component-wise operations
	icmm::CRgb sum = a + b;        // {1.0, 0.8, 0.9}
	icmm::CRgb diff = a - b;       // {0.6, -0.2, -0.5}
	icmm::CRgb product = a * b;    // {0.16, 0.15, 0.14}
	icmm::CRgb scaled = a * 0.5;   // {0.4, 0.15, 0.1}

	// Example 5: Serialization
	void SaveColor(const icmm::TComposedColor<3>& color, 
	              const QString& filename)
	{
		ifile::CFileWriteArchive archive(filename);
		if (archive.IsOpen()) {
			icmm::TComposedColor<3> copy = color;
			copy.Serialize(archive);
		}
	}

	void LoadColor(icmm::TComposedColor<3>& color,
	              const QString& filename)
	{
		ifile::CFileReadArchive archive(filename);
		if (archive.IsOpen()) {
			color.Serialize(archive);
		}
	}

	// Example 6: Getting normalized copy
	icmm::CRgb hdrColor(2.0, 1.5, 0.8);  // HDR values
	
	icmm::CRgb sdrColor;
	hdrColor.GetNormalized(sdrColor);  // sdrColor = (1.0, 1.0, 0.8)
	// hdrColor unchanged

	// Example 7: Conversion from CVarVector
	imath::CVarVector varVec(3);
	varVec.SetElement(0, 0.5);
	varVec.SetElement(1, 0.6);
	varVec.SetElement(2, 0.7);
	
	icmm::TComposedColor<3> color(varVec);  // Convert to fixed-size
	\endcode

	\section ComposedColorInheritance Inheritance Hierarchy
	TComposedColor serves as the base for:
	- **CRgb**: TComposedColor<3> - Red, Green, Blue
	- **CHsv**: TComposedColor<3> - Hue, Saturation, Value
	- **CCmyk**: TComposedColor<4> - Cyan, Magenta, Yellow, Black
	- **CLab**: TComposedColor<3> - Lightness, a*, b*
	- **Custom colors**: Any fixed-size color representation

	\section ComposedColorBestPractices Best Practices
	- Use TComposedColor as a base when creating custom fixed-size colors
	- Call Normalize() before operations requiring [0,1] range
	- Use IsSimilar() for fuzzy comparisons (handles floating-point errors)
	- Prefer fixed-size colors over CVarColor when size is known at compile time
	- Leverage inherited TVector operations for mathematical operations

	\note Template parameter Size determines the number of color components at
	      compile time, enabling optimizations not possible with CVarColor.

	\sa icmm::CVarColor, icmm::CRgb, icmm::CHsv, icmm::CCmyk, icmm::CLab,
	    imath::TVector, iser::ISerializable

	\ingroup Color
*/
template <int Size>
class TComposedColor: public imath::TVector<Size>, virtual public iser::ISerializable
{
public:
	typedef imath::TVector<Size> BaseClass;
	typedef typename BaseClass::Elements Elements;

	TComposedColor();
	TComposedColor(const BaseClass& value);
	explicit TComposedColor(const imath::CVarVector& vector);
	TComposedColor(const TComposedColor<Size>& color) = default;

	/**
		Get color after components value rounding with specified precision.
 		\param	manipulator	manipulator object used to round single components.
		\param	result		result object.
	*/
	void GetRounded(const imath::IDoubleManip& manipulator, TComposedColor<Size>& result);

	/**
		Check if two values are equal after rounding.
		\param	color		second color to comparision.
		\param	manipulator	value manipulator used to round single components.
	*/
	bool IsRoundedEqual(const TComposedColor<Size>& color, const imath::IDoubleManip& manipulator) const;

	/**
		Allows to compare two colors with tolerance.
	*/
	bool IsSimilar(const TComposedColor<Size>& color, double tolerance = I_BIG_EPSILON) const;

	/**
		Check if this color value is normalized.
		Normalized values have all components in the range [0, 1].
	*/
	bool IsNormalized() const;

	/**
		Make this color to be normalized.
		Normalized values have all components in the range [0, 1].
	*/
	void Normalize();

	/**
		Get normalized color.
		Normalized values have all components in the range [0, 1].
	*/
	void GetNormalized(TComposedColor<Size>& result) const;

	TComposedColor<Size> operator+(const TComposedColor<Size>& color) const;
	TComposedColor<Size> operator-(const TComposedColor<Size>& color) const;
	TComposedColor<Size> operator*(const TComposedColor<Size>& color) const;
	TComposedColor<Size> operator/(const TComposedColor<Size>& color) const;

	TComposedColor<Size> operator*(double value) const;
	TComposedColor<Size> operator/(double value) const;

	const TComposedColor<Size>& operator=(const TComposedColor<Size>& color);
	const TComposedColor<Size>& operator=(double colorValue);

	const TComposedColor<Size>& operator+=(const TComposedColor<Size>& color);
	const TComposedColor<Size>& operator-=(const TComposedColor<Size>& color);
	const TComposedColor<Size>& operator*=(const TComposedColor<Size>& color);
	const TComposedColor<Size>& operator/=(const TComposedColor<Size>& color);

	const TComposedColor<Size>& operator*=(double value);
	const TComposedColor<Size>& operator/=(double value);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;
};


// inline methods

template <int Size>
inline TComposedColor<Size>::TComposedColor()
	:BaseClass()
{
}


template <int Size>
inline TComposedColor<Size>::TComposedColor(const BaseClass& value)
:	BaseClass(value)
{
}


template <int Size>
inline TComposedColor<Size>::TComposedColor(const imath::CVarVector& vector)
{
	int commonSize = qMin(vector.GetElementsCount(), Size);
	for (int copyIndex = 0; copyIndex < commonSize; ++copyIndex){
		BaseClass::SetElement(copyIndex, vector[copyIndex]);
	}

	for (int resetIndex = commonSize; resetIndex < Size; ++resetIndex){
		BaseClass::SetElement(resetIndex, 0.0);
	}
}


template <int Size>
inline bool TComposedColor<Size>::IsSimilar(const TComposedColor<Size>& color, double tolerance) const
{
	return GetDistance(color) <= tolerance;
}


template <int Size>
inline bool TComposedColor<Size>::IsNormalized() const
{
	for (int i = 0; i < Size; ++i){
		double component = BaseClass::GetElement(i);

		if ((component < 0) || (component > 1)){
			return false;
		}
	}

	return true;
}


template <int Size>
inline TComposedColor<Size> TComposedColor<Size>::operator+(const TComposedColor<Size>& color) const
{
	TComposedColor<Size> retVal;

	for (int i = 0; i < Size; ++i){
		retVal[i] = BaseClass::GetElement(i) + color[i];
	}

	return retVal;
}


template <int Size>
inline TComposedColor<Size> TComposedColor<Size>::operator-(const TComposedColor<Size>& color) const
{
	TComposedColor<Size> retVal;

	for (int i = 0; i < Size; ++i){
		retVal[i] = BaseClass::GetElement(i) - color[i];
	}

	return retVal;
}


template <int Size>
inline TComposedColor<Size> TComposedColor<Size>::operator*(const TComposedColor<Size>& color) const
{
	TComposedColor<Size> retVal;

	for (int i = 0; i < Size; ++i){
		retVal[i] = BaseClass::GetElement(i) * color[i];
	}

	return retVal;
}


template <int Size>
inline TComposedColor<Size> TComposedColor<Size>::operator/(const TComposedColor<Size>& color) const
{
	TComposedColor<Size> retVal;

	for (int i = 0; i < Size; ++i){
		retVal[i] = BaseClass::GetElement(i) / color[i];
	}

	return retVal;
}


template <int Size>
inline TComposedColor<Size> TComposedColor<Size>::operator*(double value) const
{
	TComposedColor<Size> retVal;

	for (int i = 0; i < Size; ++i){
		retVal[i] = BaseClass::GetElement(i) * value;
	}

	return retVal;
}


template <int Size>
inline TComposedColor<Size> TComposedColor<Size>::operator/(double value) const
{
	TComposedColor<Size> retVal;

	for (int i = 0; i < Size; ++i){
		retVal[i] = BaseClass::GetElement(i) / value;
	}

	return retVal;
}


template <int Size>
inline const TComposedColor<Size>& TComposedColor<Size>::operator=(const TComposedColor<Size>& color)
{
	for (int i = 0; i < Size; ++i){
		BaseClass::SetElement(i, color[i]);
	}

	return *this;
}


template <int Size>
inline const TComposedColor<Size>& TComposedColor<Size>::operator=(double colorValue)
{
	std::memset(&BaseClass::GetElementsRef()[0], 0.0, sizeof(double) * Size);

	BaseClass::SetElement(0, colorValue);

	return *this;
}


template <int Size>
inline const TComposedColor<Size>& TComposedColor<Size>::operator+=(const TComposedColor<Size>& color)
{
	BaseClass::operator+=(color);

	return *this;
}


template <int Size>
inline const TComposedColor<Size>& TComposedColor<Size>::operator-=(const TComposedColor<Size>& color)
{
	BaseClass::operator-=(color);

	return *this;
}


template <int Size>
inline const TComposedColor<Size>& TComposedColor<Size>::operator*=(const TComposedColor<Size>& color)
{
	Elements& elements = BaseClass::GetElementsRef();

	for (int index = 0; index < Size; ++index){
		elements[index] *= color[index];
	}

	return *this;
}


template <int Size>
inline const TComposedColor<Size>& TComposedColor<Size>::operator/=(const TComposedColor<Size>& color)
{
	Elements& elements = BaseClass::GetElementsRef();

	for (int index = 0; index < Size; ++index){
		elements[index] /= color[index];
	}

	return *this;
}


template <int Size>
inline const TComposedColor<Size>& TComposedColor<Size>::operator*=(double value)
{
	BaseClass::operator*=(value);

	return *this;
}


template <int Size>
inline const TComposedColor<Size>& TComposedColor<Size>::operator/=(double value)
{
	BaseClass::operator/=(value);

	return *this;
}


// public normal methods

template <int Size>
void TComposedColor<Size>::GetRounded(const imath::IDoubleManip& manipulator, TComposedColor<Size>& result)
{
	for (int i = 0; i < Size; ++i){
		result[i] = manipulator.GetRounded(BaseClass::GetElement(i));
	}
}


template <int Size>
bool TComposedColor<Size>::IsRoundedEqual(const TComposedColor<Size>& color, const imath::IDoubleManip& manipulator) const
{
	for (int i = 0; i < Size; ++i){
		if (!manipulator.AreValuesEqual(BaseClass::GetElement(i), color[i])){
			return false;
		}
	}

	return true;
}


template <int Size>
void TComposedColor<Size>::Normalize()
{
	Elements& elements = BaseClass::GetElementsRef();

	for (int i = 0; i < Size; ++i){
		if (elements[i] < 0.0){
			elements[i] = 0.0;
		}

		if (elements[i] > 1.0){
			elements[i] = 1.0;
		}
	}
}


template <int Size>
void TComposedColor<Size>::GetNormalized(TComposedColor<Size>& result) const
{
	result = *this;

	result.Normalize();
}


// reimplemented (iser::ISerializable)

template <int Size>
bool TComposedColor<Size>::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag colorComponentsTag("ColorComponents", "List of color components", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag componentTag("Component", "Single component", iser::CArchiveTag::TT_LEAF, &colorComponentsTag);

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, &GetAllChanges());
	Q_UNUSED(notifier);

	bool retVal = true;

	int elementsCount = BaseClass::GetElementsCount();

	retVal = retVal && archive.BeginMultiTag(colorComponentsTag, componentTag, elementsCount);

	if ((!retVal) || (!archive.IsStoring() && (elementsCount != Size))){
		return false;
	}

	for (int i = 0; i < elementsCount; ++i){
		retVal = retVal && archive.BeginTag(componentTag);
		retVal = retVal && archive.Process(BaseClass::GetElementRef(i));
		retVal = retVal && archive.EndTag(componentTag);
	}

	retVal = retVal && archive.EndTag(colorComponentsTag);

	return retVal;
}


// related global functions

template <int Size>
uint qHash(const TComposedColor<Size>& color, uint seed = 0)
{
	quint64 retVal = seed;

	union{
		double value;
		quint64 raw;
	} element;
	element.raw = 0;

	int elementsCount = color.GetElementsCount();
	for (int i = 0; i < elementsCount; ++i){
		element.value = color[i];

		retVal = (retVal >> 1) ^ (element.raw + 1);
	}

	return uint(retVal);
}


} // namespace icmm


