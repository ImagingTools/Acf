// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <istd/TArray.h>
#include <istd/TIndex.h>
#include <imath/TISampledFunction.h>


namespace imath
{


/**
	Implementation of 1D sampled function with uniform sampling grid.
	
	\section SampledFunctionPurpose Purpose
	CSampledFunction represents a one-dimensional function sampled at regular intervals
	over a logical domain. It stores function values at discrete sample points and provides
	interpolation for continuous evaluation. This class is fundamental for representing
	digitized signals, sensor data, lookup tables, and discretized mathematical functions.
	
	\section SampledFunctionConcepts Key Concepts
	- **Logical Range**: The continuous domain [min, max] over which the function is defined
	- **Samples**: Discrete function values stored at regular intervals
	- **Sampling Step**: Distance between consecutive sample points (computed automatically)
	- **Sample Coordinate**: The logical domain position of a given sample index
	- **Interpolation**: Evaluating function at arbitrary points (typically linear)
	
	\section SampledFunctionUsageExamples Usage Examples
	\code
	// Create function with 100 samples
	imath::CSampledFunction func(100, 0.0);
	
	// Set logical range [0, 10]
	func.SetLogicalRange(istd::CRange(0.0, 10.0));
	
	// Fill with sine wave data
	for (int i = 0; i < 100; ++i) {
		double x = func.GetSampleCoordinate(i);  // Get position in [0, 10]
		func.SetSampleValue(i, qSin(x));
	}
	
	// Evaluate at arbitrary point (with interpolation)
	double result;
	if (func.GetValueAt(3.7, result)) {
		// result contains interpolated value at x=3.7
	}
	
	// Get sampling information
	double step = func.GetSamplingStep();  // 10.0 / 99 ≈ 0.101
	int count = func.GetTotalSamplesCount();  // 100
	
	// Copy from another sampled function
	imath::CSampledFunction func2;
	func2.CopyFrom(func);
	
	// Access raw samples
	std::vector<double> samples = func.GetSamples();
	\endcode
	
	\section SampledFunctionVsInterpolator Sampled Function vs Interpolator
	- **CSampledFunction**: Stores and owns the sample data, uniform grid, IChangeable
	- **Interpolators** (CLinearInterpolator, CAkimaInterpolator): Reference external data, 
	  provide interpolation algorithms, don't store data
	
	Use CSampledFunction when:
	- You need to store and manage sampled data
	- Working with uniform sampling grids
	- Need IChangeable interface (undo/redo, serialization)
	- Building signal processing or data analysis systems
	
	\section SampledFunctionInterfaces Implemented Interfaces
	- **ISampledFunction**: Core sampled function interface
	- **TIMathFunction**: Mathematical function evaluation
	- **IChangeable**: Change tracking, cloning, serialization
	
	\sa imath::ISampledFunction, imath::CSampledFunction2d, imath::CLinearInterpolator, 
	    imath::CAkimaInterpolator
	
	\ingroup Functions
*/
class CSampledFunction: virtual public ISampledFunction
{
public:
	typedef istd::TIndex<1> Index;

	CSampledFunction();
	CSampledFunction(const CSampledFunction& function);
	explicit CSampledFunction(const imath::ISampledFunction& function);
	explicit CSampledFunction(int size, double defaultValue = 0);

	bool operator==(const CSampledFunction& other) const;

	void Reset();
	bool Initialize(int size, double defaultValue = 0);
	double GetSampleValue(int index) const;
	void SetSampleValue(int index, double value);
	void SetLogicalRange(const istd::CRange& logicalRange);
	double GetSamplingStep() const;
	double GetSampleCoordinate(int index) const;
	
	// reimplemented (imath::ISampledFunction)
	virtual bool CreateFunction(double* dataPtr, const ArgumentType& sizes) override;
	virtual int GetTotalSamplesCount() const override;
	virtual double GetSampleAt(const SampleIndex& index) const override;
	virtual int GetGridSize(int dimensionIndex) const override;
	virtual istd::CRange GetLogicalRange(int dimensionIndex) const override;
	virtual istd::CRange GetResultValueRange(int dimensionIndex, int resultDimension = -1) const override;
	virtual std::vector<double> GetSamples() const override;

	// reimplemented (imath::TIMathFunction)
	virtual bool GetValueAt(const ArgumentType& argument, ResultType& result) const override;
	virtual ResultType GetValueAt(const ArgumentType& argument) const override;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const override;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual bool IsEqual(const IChangeable& object) const override;
	virtual istd::IChangeableUniquePtr CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;
	virtual bool ResetData(CompatibilityMode mode = CM_WITHOUT_REFS) override;

private:
	typedef std::vector<double> SamplesContainer;
	SamplesContainer m_samplesContainer;
	istd::CRange m_logicalRange;
};


// public inline methods

inline double CSampledFunction::GetSampleValue(int index) const
{
	Q_ASSERT(index>= 0 && index < m_samplesContainer.size());

	return m_samplesContainer[index];
}


inline void CSampledFunction::SetSampleValue(int index, double value)
{
	Q_ASSERT(index >= 0 && index < m_samplesContainer.size());

	m_samplesContainer[index] = value;
}


inline void CSampledFunction::SetLogicalRange(const istd::CRange& logicalRange)
{
	m_logicalRange = logicalRange;
}


inline double CSampledFunction::GetSamplingStep() const
{
	if (m_samplesContainer.size() >= 2){
		return m_logicalRange.GetLength() / (m_samplesContainer.size() - 1);
	}

	return 0;
}


inline double CSampledFunction::GetSampleCoordinate(int index) const
{
	Q_ASSERT(m_samplesContainer.size() > 0);
	Q_ASSERT(0 <= index);
	Q_ASSERT(index < m_samplesContainer.size());

	return m_logicalRange.GetValueFromAlpha(double(index) / double(m_samplesContainer.size() - 1));
}


} // namespace imath


