// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once

// STL includes
#include <vector>


namespace i2d
{


/** 
	Calculates points on the given arc using Bresenham's integer algorithm.
	No antialiasing is used.
*/
class CArcPointsCalculator
{
public:
	typedef std::vector<int> Coordinates;

	/** 
		Fast implementation of the arc points calculation.

		Inputs: 
			centerX, centerY:		arc center in pixel
			radius:					arc radius in pixel
			startAngle, endAngle:	start and end angle of the arc in grad

		Outputs:
			xPoints, yPoints:		vectors of the calculated points coordinates

		Returns:
			true if arc points have been calculated, otherwise false
	*/
	static bool GetArcPoints(
				int centerX,
				int centerY,
				int radius,
				int startAngle,
				int endAngle,
				Coordinates& xPoints,
				Coordinates& yPoints);
};


} // namespace i2d


