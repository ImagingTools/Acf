// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QMap>

// ACF includes
#include <imath/ISampledFunctionInterpolator.h>


namespace imath
{


/**
	1D interpolation using the Akima method for smooth, natural-looking curves.
	
	\section AkimaPurpose Purpose
	CAkimaInterpolator provides smooth interpolation through a set of data points using
	the Akima spline method. This method produces curves that are less prone to oscillation
	than cubic splines, making it particularly suitable for visualizing scientific data
	or creating smooth transitions in animations and simulations.
	
	\section AkimaCharacteristics Characteristics
	- **Smoothness**: C1 continuous (continuous first derivative)
	- **Locality**: Changes to one data point only affect nearby segments
	- **No overshooting**: Less prone to oscillation compared to cubic splines
	- **Natural appearance**: Produces visually pleasing curves for most datasets
	
	\section AkimaUsageExamples Usage Examples
	\code
	// Create interpolator from data arrays
	double positions[] = {0.0, 1.0, 2.0, 3.0, 4.0};
	double values[] = {0.0, 1.0, 0.5, 2.0, 1.5};
	imath::CAkimaInterpolator interpolator(positions, values, 5);
	
	// Interpolate at arbitrary positions
	double result;
	if (interpolator.GetValueAt(1.5, result)) {
		// result contains interpolated value at x=1.5
	}
	
	// Or use direct access
	double value = interpolator.GetValueAt(2.3);
	
	// Initialize from ISampledFunction
	// (if you have function data in that format)
	// interpolator.InitFromFunction(sampledFunction);
	\endcode
	
	\section AkimaVsLinear Akima vs Linear Interpolation
	- **Akima**: Smooth curves, C1 continuous, better for visualization
	- **Linear**: Piecewise linear, simpler, better for data with sharp transitions
	
	Choose Akima interpolation when:
	- You need smooth, natural-looking curves
	- Data represents continuous physical phenomena
	- Visual quality is important
	- Data points are reasonably spaced
	
	\sa imath::CLinearInterpolator, imath::ISampledFunctionInterpolator
	
	\ingroup Interpolation
*/
class CAkimaInterpolator: virtual public imath::ISampledFunctionInterpolator
{
public:
	struct Node
	{
		double value;
		double derivative;
	};

	typedef QMap<double, Node> Nodes;

	CAkimaInterpolator();
	CAkimaInterpolator(double* positions, double* values, int nodesCount);

	void SetNodes(double* positions, double* values, int nodesCount);

	// reimplemented (imath::ISampledFunctionInterpolator)
	virtual bool InitFromFunction(const ISampledFunction& function) override;

	// reimplemented (imath::TIMathFunction<double, double>)
	virtual bool GetValueAt(const double& argument, double& result) const override;
	virtual double GetValueAt(const double& argument) const override;

protected:
	Nodes m_nodes;
};


} // namespace imath


