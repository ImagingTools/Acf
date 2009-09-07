#include "imath/CNormalHistogram.h"


#include "istd/TChangeNotifier.h"


namespace imath
{


CNormalHistogram::CNormalHistogram()
:	m_elementsSum(0)
{
}


void CNormalHistogram::SetElementsCount(int count)
{
	istd::CChangeNotifier notifier(this);

	if (count < GetElementsCount()){
		m_elements.resize(count, 0);

		CalcElementsSum();
	}
	else{
		m_elements.resize(count, 0);
	}
}


void CNormalHistogram::SetElement(int index, int value)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < GetElementsCount());

	istd::CChangeNotifier notifier(this);

	m_elementsSum -= m_elements[index];
	m_elements[index] = value;
	m_elementsSum += value;
}


void CNormalHistogram::IncreaseElement(int index)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < GetElementsCount());

	istd::CChangeNotifier notifier(this);

	++m_elements[index];
	++m_elementsSum;
}


bool CNormalHistogram::AddValue(double value)
{
	double alpha = m_valueRange.GetAlphaFromValue(value);
	if ((alpha < 0) || (alpha > 1) || m_elements.empty()){
		return false;
	}

	int elementIndex = int(alpha * (m_elements.size() - I_BIG_EPSILON));
	I_ASSERT(elementIndex < GetElementsCount());

	IncreaseElement(elementIndex);

	return true;
};


int CNormalHistogram::GetQuantileIndex(double quantile) const
{
	double sumThreshold = quantile * m_elementsSum;

	if (sumThreshold <= 0){
		return -1;
	}

	int sum = 0;
	int elementsCount = int(m_elements.size());
	for (int i = 0; i < elementsCount; ++i){
		sum += m_elements[i];

		if (sum > sumThreshold){
			return i;
		}
	}

	return elementsCount;
}


double CNormalHistogram::GetAproxQuantileValue(double quantile) const
{
	double sumThreshold = quantile * m_elementsSum;

	if (sumThreshold <= 0){
		return m_valueRange.GetMinValue();
	}

	int sum = 0;
	int elementsCount = int(m_elements.size());
	for (int i = 0; i < elementsCount; ++i){
		int element = m_elements[i];

		sum += element;

		if (sum > sumThreshold){
			double finePos;
			if (i > 0){
				int prevSum = sum - element;
				finePos = i + (sumThreshold - prevSum / (element)) - 0.5;
			}
			else{
				finePos = sum / sumThreshold;
			}

			return m_valueRange.GetValueFromAlpha(finePos / GetElementsCount());
		}
	}

	return m_valueRange.GetMaxValue();
}


// protected methods

void CNormalHistogram::CalcElementsSum() const
{
	m_elementsSum = 0;

	for (Elements::const_iterator iter = m_elements.begin(); iter != m_elements.end(); ++iter){
		m_elementsSum += *iter;
	}
}


} // namespace imath


