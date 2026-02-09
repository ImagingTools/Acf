// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QMap>

// ACF includes
#include <imath/ISampledFunctionInterpolator.h>


namespace imath
{


/**
	Implementation of a linear interpolator for piecewise linear function approximation.
	
	\section LinearInterpPurpose Purpose
	CLinearInterpolator provides simple and efficient piecewise linear interpolation
	through a set of data points. It creates straight line segments between consecutive
	points, offering exact reproduction of linear functions and predictable behavior
	for all input data. Optional extrapolation extends the function beyond the defined
	domain using the slopes of the first and last segments.
	
	\section LinearInterpCharacteristics Characteristics
	- **Simplicity**: Straightforward, predictable behavior
	- **Efficiency**: Fast evaluation, minimal computation
	- **Exactness**: Passes exactly through all data points
	- **Continuity**: C0 continuous (continuous value but not derivative)
	- **No oscillation**: Cannot produce spurious oscillations between points
	
	\section LinearInterpUsageExamples Usage Examples
	\code
	// Create interpolator with extrapolation disabled (default)
	double positions[] = {0.0, 1.0, 2.0, 3.0};
	double values[] = {0.0, 2.0, 1.5, 3.0};
	imath::CLinearInterpolator interp1(positions, values, 4, false);
	
	// Query inside domain
	double result;
	if (interp1.GetValueAt(1.5, result)) {
		// result ≈ 1.75 (linear interpolation between 2.0 and 1.5)
	}
	
	// Query outside domain (no extrapolation)
	double value1 = interp1.GetValueAt(-1.0);  // Returns 0.0 (first value)
	double value2 = interp1.GetValueAt(5.0);   // Returns 3.0 (last value)
	
	// Create interpolator with extrapolation enabled
	imath::CLinearInterpolator interp2(positions, values, 4, true);
	double value3 = interp2.GetValueAt(-1.0);  // Extrapolates using first segment slope
	double value4 = interp2.GetValueAt(5.0);   // Extrapolates using last segment slope
	
	// Initialize from ISampledFunction
	// interp1.InitFromFunction(sampledFunction);
	\endcode
	
	\section LinearInterpWhenToUse When to Use
	Use linear interpolation when:
	- Data has sharp transitions or discontinuities in derivatives
	- Simplicity and predictability are important
	- Performance is critical (fastest interpolation method)
	- You need exact reproduction at data points
	- Data represents piecewise linear relationships
	
	Consider other methods when:
	- You need smooth curves (use CAkimaInterpolator or splines)
	- Data represents smooth continuous phenomena
	- Visual quality matters more than simplicity
	
	\sa imath::CAkimaInterpolator, imath::ISampledFunctionInterpolator
	
	\ingroup Interpolation
*/
class CLinearInterpolator: virtual public imath::ISampledFunctionInterpolator
{
public:
	CLinearInterpolator();

	/**
		\param isExtrapolationEnabled	If enabled, the values will be extrapolatied also outside of defined domain, otherwise returned value for argument outside[positionMin, positionMax] is the last value.
	*/
	CLinearInterpolator(double* positions, double* values, int nodesCount, bool isExtrapolationEnabled = false);

	void SetNodes(double* positions, double* values, int nodesCount);

	// reimplemented (imath::ISampledFunctionInterpolator)
	virtual bool InitFromFunction(const ISampledFunction& function) override;

	// reimplemented (imath::TIMathFunction<double, double>)
	virtual bool GetValueAt(const double& argument, double& result) const override;
	virtual double GetValueAt(const double& argument) const override;

private:
	typedef QMap<double, double> Nodes;
	Nodes m_nodes;
	bool m_isExtrapolationEnabled;
};


} // namespace imath


