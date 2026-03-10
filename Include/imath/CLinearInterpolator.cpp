// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <imath/CLinearInterpolator.h>


namespace imath
{


// public methods

CLinearInterpolator::CLinearInterpolator()
	:m_isExtrapolationEnabled(false)
{
}


CLinearInterpolator::CLinearInterpolator(double* positions, double* values, int nodesCount, bool isExtrapolationEnabled)
	:m_isExtrapolationEnabled(isExtrapolationEnabled)
{
	SetNodes(positions, values, nodesCount);
}


void CLinearInterpolator::SetNodes(double* positions, double* values, int nodesCount)
{
	m_nodes.clear();

	for (int nodeIndex = 0; nodeIndex < nodesCount; nodeIndex++){
		m_nodes[positions[nodeIndex]] = values[nodeIndex];
	}
}


bool CLinearInterpolator::InitFromFunction(const ISampledFunction& function)
{
	m_nodes.clear();

	istd::CRange logicalRange = function.GetLogicalRange(0);
	istd::CIntRange sampleRange = istd::CIntRange(0, function.GetTotalSamplesCount());

	for (int sampleIndex = 0; sampleIndex < function.GetTotalSamplesCount(); sampleIndex++){
		double sampleAlpha = sampleRange.GetAlphaFromValue(sampleIndex);

		double logicalPosition = logicalRange.GetValueFromAlpha(sampleAlpha);

		m_nodes[logicalPosition] = function.GetSampleAt(istd::TIndex<1>(sampleIndex));
	}

	return true;
}


// reimplemented (imath::TIMathFunction<double, double>)

bool CLinearInterpolator::GetValueAt(const double& argument, double& result) const
{
	if (m_nodes.isEmpty()){
		return false;
	}

	// Single node: constant function (both with and without extrapolation)
	if (m_nodes.count() == 1){
		result = m_nodes.constBegin().value();
		return true;
	}

	Nodes::ConstIterator nextIter = m_nodes.lowerBound(argument);

	// argument is <= first key
	if (nextIter == m_nodes.constBegin()){
		const double x1 = nextIter.key();
		const double y1 = nextIter.value();

		if (!m_isExtrapolationEnabled || x1 == argument){
			// clamp (or exact hit)
			result = y1;
			return true;
		}

		// extrapolate to the left using first two nodes
		Nodes::ConstIterator it2 = std::next(nextIter);
		const double x2 = it2.key();
		const double y2 = it2.value();

		const double dx = (x2 - x1);
		Q_ASSERT(dx > 0);

		const double slope = (y2 - y1) / dx;
		result = y1 + slope * (argument - x1);
		return true;
	}

	// argument is > last key
	if (nextIter == m_nodes.constEnd()){
		Nodes::ConstIterator lastIter = std::prev(m_nodes.constEnd());
		const double xN = lastIter.key();
		const double yN = lastIter.value();

		if (!m_isExtrapolationEnabled){
			// clamp to last value
			result = yN;
			return true;
		}

		// extrapolate to the right using last two nodes
		Nodes::ConstIterator prevIter = std::prev(lastIter);
		const double xNm1 = prevIter.key();
		const double yNm1 = prevIter.value();

		const double dx = (xN - xNm1);
		Q_ASSERT(dx > 0);

		const double slope = (yN - yNm1) / dx;
		result = yN + slope * (argument - xN);
		return true;
	}

	// argument is between two keys or exactly equals a key (not the first)
	const double x2 = nextIter.key();
	const double y2 = nextIter.value();

	// exact hit
	if (x2 == argument){
		result = y2;
		return true;
	}

	Nodes::ConstIterator prevIter = std::prev(nextIter);
	const double x1 = prevIter.key();
	const double y1 = prevIter.value();

	const double dx = (x2 - x1);
	Q_ASSERT(dx > 0);

	const double alpha = (argument - x1) / dx;
	result = y1 * (1.0 - alpha) + y2 * alpha;
	return true;
}


double CLinearInterpolator::GetValueAt(const double& argument) const
{
	double retVal;
	if (GetValueAt(argument, retVal)){
		return retVal;
	}
	else{
		return 0;
	}
}


} // namespace imath


