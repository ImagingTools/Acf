#include "imath/CLinearInterpolator.h"


// Qt includes
#include <QtCore/QVector>


namespace imath
{


void CLinearInterpolator::SetNodes(double* positions, double* values, int nodesCount)
{
	m_nodes.clear();

	for (int i = 0; i < nodesCount; i++){
		m_nodes[positions[i]] = values[i];
	}
}


// reimplemented (imath::TIMathFunction<double, double>)

bool CLinearInterpolator::GetValueAt(const double& argument, double& result) const
{
	Nodes::ConstIterator nextIter = m_nodes.lowerBound(argument);
	if ((nextIter != m_nodes.constEnd()) && (nextIter != m_nodes.constBegin())){
		double nextPosition = nextIter.key();
		Q_ASSERT(nextPosition >= argument);
		double nextValue = nextIter.value();

		Nodes::ConstIterator prevIter = nextIter - 1;

		double prevPosition = prevIter.key();
		Q_ASSERT(prevPosition <= argument);
		double prevValue = prevIter.value();

		// interpolation in segment
		double nodeDiff = (nextPosition - prevPosition);
		Q_ASSERT(nodeDiff > 0);

		double alpha = (argument - prevPosition) / nodeDiff;

		result =	prevValue * (1 - alpha) +
					nextValue * alpha;

		return true;
	}

	return false;
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


