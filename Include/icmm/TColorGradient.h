#pragma once


// ACF includes
#include <imath/TIMathFunction.h>
#include <icmm/IColorTransformation.h>


namespace icmm
{


class LinearGradientFunction
{
public:
	static double GetValue(double x)
	{
		return x;
	}
};


/**
	Simple implementation of color gradient based on interpolation between start and end colors.
	
	\section ColorGradientPurpose Purpose
	TColorGradient provides smooth color transitions between two colors using linear or
	custom interpolation functions. It implements IColorTransformation, allowing it to
	generate intermediate colors based on a parameter value (typically 0 to 1).

	\section ColorGradientDesign Design
	The gradient is parameterized by:
	- **Start color**: Color at parameter value 0.0
	- **End color**: Color at parameter value 1.0
	- **Gradient function**: Controls interpolation curve (linear, exponential, etc.)

	\section ColorGradientUsage Usage Examples
	\code
	// Example 1: Simple linear gradient from red to blue
	icmm::CVarColor red(3);
	red.SetElement(0, 1.0);  // R
	red.SetElement(1, 0.0);  // G
	red.SetElement(2, 0.0);  // B
	
	icmm::CVarColor blue(3);
	blue.SetElement(0, 0.0);  // R
	blue.SetElement(1, 0.0);  // G
	blue.SetElement(2, 1.0);  // B
	
	icmm::CLinearColorGradient gradient(red, blue);
	
	// Get color at 50% (purple)
	icmm::CVarColor param(1);
	param.SetElement(0, 0.5);
	
	icmm::CVarColor midColor;
	gradient.GetValueAt(param, midColor);
	// midColor = {0.5, 0.0, 0.5} - purple

	// Example 2: Creating a color ramp
	QList<icmm::CVarColor> CreateColorRamp(
		const icmm::CVarColor& start,
		const icmm::CVarColor& end,
		int steps)
	{
		icmm::CLinearColorGradient gradient(start, end);
		QList<icmm::CVarColor> ramp;
		
		for (int i = 0; i < steps; ++i) {
			double t = static_cast<double>(i) / (steps - 1);
			
			icmm::CVarColor param(1);
			param.SetElement(0, t);
			
			icmm::CVarColor color = gradient.GetValueAt(param);
			ramp.append(color);
		}
		
		return ramp;
	}
	
	// Usage
	icmm::CVarColor white(3, 1.0);
	icmm::CVarColor black(3, 0.0);
	QList<icmm::CVarColor> grayscaleRamp = CreateColorRamp(black, white, 10);

	// Example 3: Temperature gradient (blue to red)
	icmm::CVarColor CreateTemperatureGradient()
	{
		// Cold (blue)
		icmm::CVarColor cold(3);
		cold.SetElement(0, 0.0);
		cold.SetElement(1, 0.0);
		cold.SetElement(2, 1.0);
		
		// Hot (red)
		icmm::CVarColor hot(3);
		hot.SetElement(0, 1.0);
		hot.SetElement(1, 0.0);
		hot.SetElement(2, 0.0);
		
		return icmm::CLinearColorGradient(cold, hot);
	}

	// Example 4: Using gradient for visualization
	void VisualizeData(const QVector<double>& data,
	                  const icmm::CLinearColorGradient& gradient)
	{
		// Find data range
		double minVal = *std::min_element(data.begin(), data.end());
		double maxVal = *std::max_element(data.begin(), data.end());
		double range = maxVal - minVal;
		
		for (double value : data) {
			// Normalize to [0, 1]
			double t = (value - minVal) / range;
			
			icmm::CVarColor param(1);
			param.SetElement(0, t);
			
			// Get color for this value
			icmm::CVarColor color = gradient.GetValueAt(param);
			
			// Render with this color
			DrawPoint(color);
		}
	}

	// Example 5: Custom gradient function
	class SmoothStepGradientFunction
	{
	public:
		static double GetValue(double x)
		{
			// Smooth step interpolation (ease in/out)
			return x * x * (3.0 - 2.0 * x);
		}
	};
	
	// Create gradient with smooth step function
	typedef icmm::TColorGradient<SmoothStepGradientFunction> 
		CSmoothStepGradient;
	
	icmm::CVarColor start(3, 0.0);
	icmm::CVarColor end(3, 1.0);
	CSmoothStepGradient smoothGradient(start, end);

	// Example 6: Heatmap colors
	icmm::CLinearColorGradient CreateHeatmapGradient()
	{
		// Black (cold)
		icmm::CVarColor black(3);
		black.SetElement(0, 0.0);
		black.SetElement(1, 0.0);
		black.SetElement(2, 0.0);
		
		// Yellow (hot)
		icmm::CVarColor yellow(3);
		yellow.SetElement(0, 1.0);
		yellow.SetElement(1, 1.0);
		yellow.SetElement(2, 0.0);
		
		return icmm::CLinearColorGradient(black, yellow);
	}
	\endcode

	\section ColorGradientFunctions Gradient Functions
	The template parameter GradientFunction controls interpolation:
	
	**LinearGradientFunction** (default):
	- Simple linear interpolation: f(x) = x
	- Used by CLinearColorGradient typedef
	
	**Custom functions**:
	- Implement `static double GetValue(double x)`
	- Return value in [0, 1] for input in [0, 1]
	- Examples: ease-in, ease-out, exponential, logarithmic

	\section ColorGradientApplications Applications
	- **Data visualization**: Mapping values to colors
	- **Heatmaps**: Showing intensity or temperature
	- **Color pickers**: Smooth transitions in UI
	- **Image processing**: Color space conversions
	- **Scientific visualization**: False color representations

	\section ColorGradientBestPractices Best Practices
	- Use linear gradients for general-purpose color transitions
	- Choose start/end colors in appropriate color space (HSV for hue shifts, RGB for direct mixing)
	- Clamp parameter values to [0, 1] for predictable results
	- Consider perceptual uniformity - use Lab space for perceptually smooth gradients
	- Cache gradient objects for repeated evaluations

	\note Parameter values outside [0, 1] are automatically clamped to valid range.

	\sa icmm::IColorTransformation, icmm::TComposedColorGradient,
	    icmm::CVarColor, icmm::CLinearColorGradient

	\ingroup Color
*/
template <class GradientFunction>
class TColorGradient: virtual public IColorTransformation
{
public:
	typedef IColorTransformation::ResultType GradientColor;

	TColorGradient(const GradientColor& startColor, const GradientColor& endColor);

	// reimplemented (icmm::IColorTransformation)
	virtual bool GetValueAt(const ArgumentType& argument, ResultType& result) const override;
	virtual ResultType GetValueAt(const ArgumentType& argument) const override;

private:
	GradientColor m_startColor;
	GradientColor m_endColor;
};


template <class GradientFunction>
TColorGradient<GradientFunction>::TColorGradient(const GradientColor& startColor, const GradientColor& endColor)
	:m_startColor(startColor),
	m_endColor(endColor)
{
}


// reimplemented (icmm::IColorTransformation)

template <class GradientFunction>
bool TColorGradient<GradientFunction>::GetValueAt(const ArgumentType& argument, ResultType& result) const
{
	Q_ASSERT(argument.GetElementsCount() == 1);

	static istd::CRange normRange(0.0, 1.0);

	double argumentValue = normRange.GetClipped(argument[0]);
	
	for (int componentIndex = 0; componentIndex < result.GetElementsCount(); componentIndex++){
		double startValue = m_startColor[componentIndex];
		double endValue = m_endColor[componentIndex];

		double gradientValue = 0.0; 
		if (startValue < endValue){
			gradientValue = (endValue - startValue) * argumentValue + startValue;
		}
		else{
			gradientValue = (startValue - endValue) * (1.0 - argumentValue) + endValue;
		}

		result[componentIndex] = GradientFunction::GetValue(gradientValue);
	}

	return true;
}


template <class GradientFunction>
typename TColorGradient<GradientFunction>::ResultType TColorGradient<GradientFunction>::GetValueAt(const ArgumentType& argument) const
{
	ResultType result(m_startColor.GetElementsCount());

	GetValueAt(argument, result);

	return result;
}


typedef TColorGradient<LinearGradientFunction> CLinearColorGradient;


} // namespace icmm


