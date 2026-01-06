#pragma once


// ACF includes
#include <iser/ISerializable.h>
#include <imath/IDoubleManip.h>
#include <imath/CVarVector.h>
#include <icmm/icmm.h>


namespace icmm
{


/**
	Generic color implementation with variable number of color components.
	
	\section VarColorPurpose Purpose
	CVarColor provides a flexible color representation that can accommodate any color
	space with any number of components. Unlike fixed-size color classes (CRgb, CCmyk),
	CVarColor's dimensionality is determined at runtime, making it ideal for generic
	color processing, color transformations, and working with exotic or custom color spaces.

	\section VarColorFlexibility Flexibility and Use Cases
	CVarColor is used when:
	- Color space is not known at compile time
	- Working with transformations between different color models
	- Implementing generic color algorithms
	- Supporting custom or extended color spaces (5+ components)
	- Storing colors in collections with mixed dimensionality

	\section VarColorExamples Usage Examples
	\code
	// Example 1: Creating colors of different dimensions
	icmm::CVarColor rgb(3);           // RGB (3 components)
	rgb.SetElement(0, 1.0);           // Red
	rgb.SetElement(1, 0.5);           // Green
	rgb.SetElement(2, 0.0);           // Blue
	
	icmm::CVarColor cmyk(4);          // CMYK (4 components)
	cmyk.SetElement(0, 0.0);          // Cyan
	cmyk.SetElement(1, 0.5);          // Magenta
	cmyk.SetElement(2, 1.0);          // Yellow
	cmyk.SetElement(3, 0.0);          // Black
	
	icmm::CVarColor spectral(31);     // Spectral (many components)

	// Example 2: Working with generic transformations
	void TransformColor(const icmm::CVarColor& input,
	                   icmm::CVarColor& output,
	                   const icmm::IColorTransformation* transform)
	{
		if (transform && transform->Calculate(input, output)) {
			// Transformation successful
			qDebug() << "Transformed" << input.GetElementsCount() 
			         << "component color to" 
			         << output.GetElementsCount() << "components";
		}
	}

	// Example 3: Color comparison with tolerance
	bool ColorsMatch(const icmm::CVarColor& color1,
	                const icmm::CVarColor& color2,
	                double tolerance = 0.01)
	{
		return color1.IsSimilar(color2, tolerance);
	}
	
	icmm::CVarColor c1(3);
	c1.SetElement(0, 0.5);
	c1.SetElement(1, 0.5);
	c1.SetElement(2, 0.5);
	
	icmm::CVarColor c2(3);
	c2.SetElement(0, 0.501);
	c2.SetElement(1, 0.499);
	c2.SetElement(2, 0.502);
	
	bool match = ColorsMatch(c1, c2, 0.01);  // true

	// Example 4: Normalization (clamping to [0,1])
	icmm::CVarColor color(3, 1.5);  // All components = 1.5 (out of range)
	
	if (!color.IsNormalized()) {
		color.Normalize();  // Clamp to [0, 1]
		// Now all components are 1.0
	}

	// Example 5: Color arithmetic
	icmm::CVarColor a(3);
	a.SetElement(0, 0.8);
	a.SetElement(1, 0.3);
	a.SetElement(2, 0.2);
	
	icmm::CVarColor b(3);
	b.SetElement(0, 0.2);
	b.SetElement(1, 0.5);
	b.SetElement(2, 0.7);
	
	// Component-wise operations
	icmm::CVarColor sum = a + b;          // {1.0, 0.8, 0.9}
	icmm::CVarColor diff = a - b;         // {0.6, -0.2, -0.5}
	icmm::CVarColor product = a * b;      // {0.16, 0.15, 0.14}
	icmm::CVarColor scaled = a * 0.5;     // {0.4, 0.15, 0.1}

	// Example 6: Rounded comparison for precision control
	icmm::CVarColor precise1(3, 0.123456789);
	icmm::CVarColor precise2(3, 0.123456788);
	
	// Create rounding manipulator (e.g., 6 decimal places)
	// imath::CDoubleRounder rounder(6);
	// bool equalRounded = precise1.IsRoundedEqual(precise2, rounder);

	// Example 7: Converting from fixed-size vectors
	imath::TVector<3> vec3;
	vec3.SetElement(0, 1.0);
	vec3.SetElement(1, 0.0);
	vec3.SetElement(2, 0.0);
	
	icmm::CVarColor fromVector(vec3);  // Convert TVector to CVarColor

	// Example 8: Generic color palette
	QList<icmm::CVarColor> CreatePalette(int colorCount, int components)
	{
		QList<icmm::CVarColor> palette;
		
		for (int i = 0; i < colorCount; ++i) {
			icmm::CVarColor color(components);
			
			// Generate color values
			for (int c = 0; c < components; ++c) {
				color.SetElement(c, 
					static_cast<double>(i) / colorCount);
			}
			
			palette.append(color);
		}
		
		return palette;
	}

	// Example 9: Serialization
	void SaveColor(const icmm::CVarColor& color, const QString& filename)
	{
		ifile::CFileWriteArchive archive(filename);
		if (archive.IsOpen()) {
			icmm::CVarColor copy = color;
			copy.Serialize(archive);
		}
	}
	\endcode

	\section VarColorPerformance Performance Considerations
	- **Dynamic allocation**: CVarColor allocates memory dynamically, unlike fixed-size colors
	- **Overhead**: Small overhead compared to CRgb, CHsv, etc.
	- **Flexibility**: Worth the cost when color space is not known at compile time
	- **Use fixed-size**: Prefer CRgb, CHsv, CCmyk, CLab when dimensions are known

	\section VarColorBestPractices Best Practices
	- Use CVarColor for generic color transformation pipelines
	- Use fixed-size classes (CRgb, etc.) when dimensions are known at compile time
	- Check IsNormalized() before operations requiring [0,1] range
	- Use IsSimilar() for fuzzy color comparisons (handles floating-point errors)
	- Cache component count to avoid repeated GetElementsCount() calls in loops

	\note The dimension (number of components) is set at construction time and can be
	      changed later, unlike fixed-size color classes.

	\sa icmm::TComposedColor, icmm::CRgb, icmm::CHsv, icmm::CCmyk, icmm::CLab,
	    imath::CVarVector, icmm::IColorTransformation

	\ingroup Color
*/
class CVarColor: public imath::CVarVector, virtual public iser::ISerializable
{
public:
	typedef imath::CVarVector BaseClass;

	/**
		Default constructor.
		It set number of elements to 0.
	*/
	CVarColor();

	/**
		Constructor with explicit initialization of number of elements.
	*/
	explicit CVarColor(int componentsCount, double value = 0);

	/**
		Copy constructor.
	*/
	CVarColor(const CVarColor& color);

	/**
		Template conversion constructor.
	*/
	template <int Size>
	CVarColor(const imath::TVector<Size>& vector)
	:	BaseClass(vector)
	{
	}

	/**
		Get color after components value rounding with specified precision.
		\param	manipulator	manipulator object used to round single components.
		\param	result		result object.
	*/
	void GetRounded(const imath::IDoubleManip& manipulator, CVarColor& result);

	/**
		Check if two values are equal after rounding.
		\param	color		second color to comparision.
		\param	manipulator	value manipulator used to round single components.
	*/
	bool IsRoundedEqual(const CVarColor& color, const imath::IDoubleManip& manipulator) const;

	/**
		Allows to compare two colors with tolerance.
	*/
	bool IsSimilar(const CVarColor& color, double tolerance = I_BIG_EPSILON) const;

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
	void GetNormalized(CVarColor& result) const;

	CVarColor operator+(const CVarColor& color) const;
	CVarColor operator-(const CVarColor& color) const;
	CVarColor operator*(const CVarColor& color) const;
	CVarColor operator/(const CVarColor& color) const;

	CVarColor operator*(double value) const;
	CVarColor operator/(double value) const;

	CVarColor& operator=(const CVarColor& color);

	const CVarColor& operator+=(const CVarColor& color);
	const CVarColor& operator-=(const CVarColor& color);
	const CVarColor& operator*=(const CVarColor& color);
	const CVarColor& operator/=(const CVarColor& color);

	const CVarColor& operator*=(double value);
	const CVarColor& operator/=(double value);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;
};


// inline methods

inline bool CVarColor::IsSimilar(const CVarColor& color, double tolerance) const
{
	return GetDistance(color) <= tolerance;
}


inline bool CVarColor::IsNormalized() const
{
	int elementsCount = GetElementsCount();

	for (int i = 0; i < elementsCount; ++i){
		double component = GetElement(i);

		if ((component < 0) || (component > 1)){
			return false;
		}
	}

	return true;
}


inline CVarColor CVarColor::operator+(const CVarColor& color) const
{
	CVarColor retVal(*this);

	retVal += color;

	return retVal;
}


inline CVarColor CVarColor::operator-(const CVarColor& color) const
{
	CVarColor retVal(*this);

	retVal -= color;

	return retVal;
}


inline CVarColor CVarColor::operator*(const CVarColor& color) const
{
	CVarColor retVal(*this);

	retVal *= color;

	return retVal;
}


inline CVarColor CVarColor::operator/(const CVarColor& color) const
{
	CVarColor retVal(*this);

	retVal /= color;

	return retVal;
}


inline CVarColor CVarColor::operator*(double value) const
{
	int elementsCount = GetElementsCount();

	CVarColor retVal(elementsCount);

	for (int i = 0; i < elementsCount; ++i){
		retVal.SetElement(i, GetElement(i) * value);
	}

	return retVal;
}


inline CVarColor CVarColor::operator/(double value) const
{
	int elementsCount = GetElementsCount();

	CVarColor retVal(elementsCount);

	for (int i = 0; i < elementsCount; ++i){
		retVal.SetElement(i, GetElement(i) / value);
	}

	return retVal;
}


inline const CVarColor& CVarColor::operator*=(double value)
{
	BaseClass::operator*=(value);

	return *this;
}


inline const CVarColor& CVarColor::operator/=(double value)
{
	BaseClass::operator/=(value);

	return *this;
}


} // namespace icmm


