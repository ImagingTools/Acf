#pragma once


// ACF includes
#include <iimgprint/IDeviceLabTransformation.h>
#include <iimgprint/CPrinterProfile.h>
#include <imath/TMultidimensionalPolynomial.h>


namespace iimgprint
{


/**
	Device-to-Lab transformation using multi-dimensional interpolation.
	
	\section DeviceToLabImpl Implementation
	Uses multi-dimensional interpolation over test chart measurement data to
	convert device colors to Lab. The transformation is built from spectral
	measurements: for each test patch, we have both device values (CMYK/RGB)
	and measured spectral reflectance, which is converted to Lab.
	
	\sa iimgprint::IDeviceToLabTransformation
	
	\ingroup Color
	\ingroup ImageProcessing
*/
class CDeviceToLabTransformation : public IDeviceToLabTransformation
{
public:
	/**
		Constructs device-to-Lab transformation from printer profile.
		
		\param profile Printer profile with spectral measurement data
	*/
	explicit CDeviceToLabTransformation(const CPrinterProfile& profile);
	
	/**
		Destructor.
	*/
	virtual ~CDeviceToLabTransformation();
	
	// IDeviceToLabTransformation interface
	virtual bool DeviceToLab(const icmm::CVarColor& deviceColor, icmm::CLab& labColor) const override;

private:
	class Impl;
	Impl* m_impl;
};


/**
	Lab-to-device transformation using multi-dimensional interpolation.
	
	\section LabToDeviceImpl Implementation
	Uses multi-dimensional interpolation over test chart measurement data to
	convert Lab colors to device colors. The transformation is the inverse of
	device-to-Lab: given a target Lab color, find the device values that
	produce that Lab color (or closest match within gamut).
	
	\sa iimgprint::ILabToDeviceTransformation
	
	\ingroup Color
	\ingroup ImageProcessing
*/
class CLabToDeviceTransformation : public ILabToDeviceTransformation
{
public:
	/**
		Constructs Lab-to-device transformation from printer profile.
		
		\param profile Printer profile with spectral measurement data
	*/
	explicit CLabToDeviceTransformation(const CPrinterProfile& profile);
	
	/**
		Destructor.
	*/
	virtual ~CLabToDeviceTransformation();
	
	// ILabToDeviceTransformation interface
	virtual bool LabToDevice(const icmm::CLab& labColor, icmm::CVarColor& deviceColor) const override;

private:
	class Impl;
	Impl* m_impl;
};


} // namespace iimgprint


