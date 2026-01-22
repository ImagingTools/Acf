// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <i3d/Geometry.h>


namespace i3d
{
namespace Geometry
{


bool AreIntersecting(const CLine3d& line1, const CLine3d& line2, double tolerance)
{
	CVector3d p1, p2;
	double distance = GetClosestPoints(line1, line2, p1, p2);
	return distance <= tolerance;
}


bool GetIntersection(const CLine3d& line, const CPlane3d& plane, CVector3d& result, double tolerance)
{
	CVector3d direction = line.GetDirection();
	double denominator = direction.GetDotProduct(plane.GetNormal());
	
	// Check if line is parallel to plane
	if (qAbs(denominator) < tolerance){
		return false;
	}
	
	// Calculate intersection parameter
	CVector3d toPlane = plane.GetPoint() - line.GetPoint1();
	double t = toPlane.GetDotProduct(plane.GetNormal()) / denominator;
	
	result = line.GetPointAt(t);
	return true;
}


double GetClosestPoints(const CLine3d& line1, const CLine3d& line2, CVector3d& point1, CVector3d& point2)
{
	CVector3d p1 = line1.GetPoint1();
	CVector3d d1 = line1.GetPoint2() - line1.GetPoint1();
	
	CVector3d p2 = line2.GetPoint1();
	CVector3d d2 = line2.GetPoint2() - line2.GetPoint1();
	
	CVector3d w0 = p1 - p2;
	
	double a = d1.GetDotProduct(d1);
	double b = d1.GetDotProduct(d2);
	double c = d2.GetDotProduct(d2);
	double d = d1.GetDotProduct(w0);
	double e = d2.GetDotProduct(w0);
	
	double denominator = a * c - b * b;
	
	double t1, t2;
	
	if (qAbs(denominator) < I_BIG_EPSILON){
		// Lines are parallel
		t1 = 0.0;
		t2 = (b > c ? d / b : e / c);
	}
	else{
		t1 = (b * e - c * d) / denominator;
		t2 = (a * e - b * d) / denominator;
	}
	
	// Clamp to line segments
	t1 = qMax(0.0, qMin(1.0, t1));
	t2 = qMax(0.0, qMin(1.0, t2));
	
	point1 = line1.GetPointAt(t1);
	point2 = line2.GetPointAt(t2);
	
	return GetDistance(point1, point2);
}


bool GetBarycentricCoordinates(
	const CVector3d& point,
	const CVector3d& p1,
	const CVector3d& p2,
	const CVector3d& p3,
	double& u,
	double& v,
	double& w)
{
	CVector3d v0 = p2 - p1;
	CVector3d v1 = p3 - p1;
	CVector3d v2 = point - p1;
	
	double d00 = v0.GetDotProduct(v0);
	double d01 = v0.GetDotProduct(v1);
	double d11 = v1.GetDotProduct(v1);
	double d20 = v2.GetDotProduct(v0);
	double d21 = v2.GetDotProduct(v1);
	
	double denominator = d00 * d11 - d01 * d01;
	
	if (qAbs(denominator) < I_BIG_EPSILON){
		// Degenerate triangle
		return false;
	}
	
	v = (d11 * d20 - d01 * d21) / denominator;
	w = (d00 * d21 - d01 * d20) / denominator;
	u = 1.0 - v - w;
	
	return true;
}


bool IsPointInTriangle(
	const CVector3d& point,
	const CVector3d& p1,
	const CVector3d& p2,
	const CVector3d& p3)
{
	double u, v, w;
	
	if (!GetBarycentricCoordinates(point, p1, p2, p3, u, v, w)){
		return false;
	}
	
	// Check if all barycentric coordinates are non-negative
	return (u >= 0.0) && (v >= 0.0) && (w >= 0.0);
}


} // namespace Geometry
} // namespace i3d
