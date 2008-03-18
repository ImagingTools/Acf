#ifndef imath_TSplineGridFunctionBase_included
#define imath_TSplineGridFunctionBase_included


#include "istd/TArray.h"

#include "imath/TVector.h"
#include "imath/TFulcrumGridFunctionBase.h"


namespace imath
{


/**
	Spline interpolation function using polynomial 3 degree segments.
	To reduce number of coefficients only precalculated derrivative values for each fulcrum will be stored.
*/
template <class Element, int Dimensions, class Fulcrum = Element, int DerivativesCount = Dimensions>
class TSplineGridFunctionBase: public TFulcrumGridFunctionBase<Element, Dimensions>
{
public:
	typedef TFulcrumGridFunctionBase<Element, Dimensions> BaseClass;

	// reimplemented (imath::TIMathFunction<imath::TVector<Dimensions>, Element>)
	virtual bool GetValueAt(const imath::TVector<Dimensions>& argument, Element& result) const;
	virtual Element GetValueAt(const imath::TVector<Dimensions>& argument) const;

protected:
	typedef imath::TVector<DerivativesCount, Element> Derivatives;
	typedef istd::TArray<Derivatives, Dimensions> DerivativesGrid;

	/**
		Calculate interpolated value at specified recursion level.
		\param	argument	position, where interpolation should be calculated.
		\param	index		multidimensional index pointing at cuboid element in fulcrum grid.
		\param	sizes		size fulcrum grid.
		\param	dimension	working dimension and recursion level.
		\param	result		returned object.
	*/
	void CalcRecursiveValueAt(
				const TVector<Dimensions>& argument,
				istd::TIndex<Dimensions>& index,
				const istd::TIndex<Dimensions>& sizes,
				int dimension,
				Element& result) const;

	/**
		Calculate interpolated derivative at specified recursion level.
		\param	argument			position, where interpolation should be calculated.
		\param	index				multidimensional index pointing at cuboid element in fulcrum grid.
		\param	sizes				size fulcrum grid.
		\param	derivativeDimension	dimension index of requested derivative.
		\param	dimension			working dimension and recursion level.
		\param	result				returned object.
	*/
	void CalcRecursiveDerivativeAt(
				const TVector<Dimensions>& argument,
				istd::TIndex<Dimensions>& index,
				const istd::TIndex<Dimensions>& sizes,
				int derivativeDimension,
				int dimension,
				Element& result) const;

	const DerivativesGrid& GetDerivativesGrid() const;
	DerivativesGrid& GetDerivativesGrid();

	// reimplemented (istd::IChangeable)
	virtual void OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);

	// static methods
	static double GetValueKernelAt(double alpha);
	static double GetDerivativeKernelAt(double alpha);

	// abstract methods

	/**
		Calc values of derrivatives after fulcrum grid changes.
	*/
	virtual void CalcDerivativesGrid(DerivativesGrid& result) = 0;

private:
	DerivativesGrid m_derrivativesGrid;
};


// protected inline methods

template <class Element, int Dimensions, class Fulcrum, int DerivativesCount>
inline typename const TSplineGridFunctionBase<Element, Dimensions, Fulcrum, DerivativesCount>::DerivativesGrid&
			TSplineGridFunctionBase<Element, Dimensions, Fulcrum, DerivativesCount>::GetDerivativesGrid() const
{
	return m_derrivativesGrid;
}


template <class Element, int Dimensions, class Fulcrum, int DerivativesCount>
inline typename TSplineGridFunctionBase<Element, Dimensions, Fulcrum, DerivativesCount>::DerivativesGrid&
			TSplineGridFunctionBase<Element, Dimensions, Fulcrum, DerivativesCount>::GetDerivativesGrid()
{
	return m_derrivativesGrid;
}


// static protected inline methods

template <class Element, int Dimensions, class Fulcrum, int DerivativesCount>
inline double TSplineGridFunctionBase<Element, Dimensions, Fulcrum, DerivativesCount>::GetValueKernelAt(double alpha)
{
	return 2 * alpha * alpha * alpha - 3 * alpha * alpha + 1;
}


template <class Element, int Dimensions, class Fulcrum, int DerivativesCount>
inline double TSplineGridFunctionBase<Element, Dimensions, Fulcrum, DerivativesCount>::GetDerivativeKernelAt(double alpha)
{
	return alpha * alpha * alpha - 2 * alpha * alpha + alpha;
}


// public methods

// reimplemented (imath::TIMathFunction<imath::TVector<Dimensions>, Element>)

template <class Element, int Dimensions, class Fulcrum, int DerivativesCount>
bool TSplineGridFunctionBase<Element, Dimensions, Fulcrum, DerivativesCount>::GetValueAt(const imath::TVector<Dimensions>& argument, Element& result) const
{
	istd::TIndex<Dimensions> index = FindIndices(argument);

	CalcRecursiveValueAt(argument, index, GetGridSize(), Dimensions - 1, result);

	return true;
}


template <class Element, int Dimensions, class Fulcrum, int DerivativesCount>
typename Element TSplineGridFunctionBase<Element, Dimensions, Fulcrum, DerivativesCount>::GetValueAt(const imath::TVector<Dimensions>& argument) const
{
	ResultType retVal;

	GetValueAt(argument, retVal);

	return retVal;
}


// protected methods

template <class Element, int Dimensions, class Fulcrum, int DerivativesCount>
void TSplineGridFunctionBase<Element, Dimensions, Fulcrum, DerivativesCount>::CalcRecursiveDerivativeAt(
			const TVector<Dimensions>& argument,
			istd::TIndex<Dimensions>& index,
			const istd::TIndex<Dimensions>& sizes,
			int derivativeDimension,
			int dimension,
			Element& result) const
{
	if (dimension < 0){
		result = m_derrivativesGrid[index][derivativeDimension];

		return;
	}

	int& indexElement = index[dimension];

	if (indexElement < 0){
		I_ASSERT(indexElement == -1);

		++indexElement;
		CalcRecursiveDerivativeAt(argument, index, sizes, derivativeDimension, dimension - 1, result);
		--indexElement;
	}
	else if (indexElement >= sizes[dimension] - 1){
		I_ASSERT(indexElement == sizes[dimension] - 1);

		CalcRecursiveDerivativeAt(argument, index, sizes, derivativeDimension, dimension - 1, result);
	}
	else{
		Element firstDerivative;
		CalcRecursiveDerivativeAt(argument, index, sizes, dimension, dimension - 1, firstDerivative);

		++indexElement;

		Element secondDerivative;
		CalcRecursiveDerivativeAt(argument, index, sizes, dimension, dimension - 1, secondDerivative);

		--indexElement;

		double firstPosition = GetLayerPosition(dimension, indexElement);
		double secondPosition = GetLayerPosition(dimension, indexElement + 1);
		double layersDistance = secondPosition - firstPosition;
		I_ASSERT(layersDistance >= 0);
		I_ASSERT(argument[dimension] >= firstPosition);
		I_ASSERT(argument[dimension] <= secondPosition);

		double alpha = (argument[dimension] - firstPosition) / layersDistance;
		
		result =	firstDerivative * (GetValueKernelAt(alpha) * layersDistance) +
					secondDerivative * (GetValueKernelAt(1.0 - alpha) * layersDistance);
	}
}


template <class Element, int Dimensions, class Fulcrum, int DerivativesCount>
void TSplineGridFunctionBase<Element, Dimensions, Fulcrum, DerivativesCount>::CalcRecursiveValueAt(
			const TVector<Dimensions>& argument,
			istd::TIndex<Dimensions>& index,
			const istd::TIndex<Dimensions>& sizes,
			int dimension,
			Element& result) const
{
	if (dimension < 0){
		result = GetFulcrumAtIndex(index);

		return;
	}

	int& indexElement = index[dimension];

	if (indexElement < 0){
		// element out of boundaries at this dimension
		I_ASSERT(indexElement == -1);

		++indexElement;
		CalcRecursiveValueAt(argument, index, sizes, dimension - 1, result);
		--indexElement;
	}
	else if (indexElement >= sizes[dimension] - 1){
		// element out of boundaries at this dimension
		I_ASSERT(indexElement == sizes[dimension] - 1);

		CalcRecursiveValueAt(argument, index, sizes, dimension - 1, result);
	}
	else{
		Element firstValue;
		CalcRecursiveValueAt(argument, index, sizes, dimension - 1, firstValue);

		++indexElement;

		Element secondValue;
		CalcRecursiveValueAt(argument, index, sizes, dimension - 1, secondValue);

		--indexElement;

		double firstPosition = GetLayerPosition(dimension, indexElement);
		double secondPosition = GetLayerPosition(dimension, indexElement + 1);
		double layersDistance = secondPosition - firstPosition;
		I_ASSERT(layersDistance >= 0);
		I_ASSERT(argument[dimension] >= firstPosition);
		I_ASSERT(argument[dimension] <= secondPosition);

		double alpha = (argument[dimension] - firstPosition) / layersDistance;

		if (dimension < DerivativesCount){
			// use derrivative information
			Element firstDerivative;
			CalcRecursiveDerivativeAt(argument, index, sizes, dimension, dimension - 1, firstDerivative);

			++indexElement;

			Element secondDerivative;
			CalcRecursiveDerivativeAt(argument, index, sizes, dimension, dimension - 1, secondDerivative);

			--indexElement;

			result =	firstValue * GetValueKernelAt(alpha) +
						secondValue * GetValueKernelAt(1.0 - alpha) +
						firstDerivative * (GetDerivativeKernelAt(alpha) * layersDistance) +
						secondDerivative * (GetDerivativeKernelAt(1.0 - alpha) * layersDistance);
		}
		else{
			// derrivative information is not available for this dimension
			result =	firstValue * (1.0 - alpha) +
						secondValue * alpha;
		}
	}
}


// reimplemented (istd::IChangeable)

template <class Element, int Dimensions, class Fulcrum, int DerivativesCount>
void TSplineGridFunctionBase<Element, Dimensions, Fulcrum, DerivativesCount>::OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	BaseClass::OnEndChanges(changeFlags, changeParamsPtr);

	m_derrivativesGrid.SetSizes(GetGridSize());

	CalcDerivativesGrid(m_derrivativesGrid);
}


} // namespace imath


#endif // !imath_TSplineGridFunctionBase_included


