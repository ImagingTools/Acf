#pragma once


// ACF includes
#include <iimgprint/iimgprint.h>
#include <icmm/CVarColor.h>
#include <icmm/CLab.h>


namespace iimgprint
{


class CPrinterProfile;


/**
	Interface for device-to-Lab color transformation based on printer profile.
	
	\section DeviceToLabPurpose Purpose
	IDeviceToLabTransformation converts device-dependent colors (CMYK, RGB) to
	device-independent CIE Lab colors using spectral measurements from the printer
	profile. This is the forward transformation: Device → Lab.
	
	\section DeviceToLabUsage Usage
	\code
	// Get device-to-Lab transformation from profile
	const IDeviceToLabTransformation* toLab = profile.GetDeviceToLabTransformation();
	
	// Convert CMYK to Lab
	icmm::CVarColor cmyk(4);
	cmyk.SetElement(0, 0.5);  // C
	cmyk.SetElement(1, 0.3);  // M
	cmyk.SetElement(2, 0.7);  // Y
	cmyk.SetElement(3, 0.1);  // K
	
	icmm::CLab lab;
	if (toLab->DeviceToLab(cmyk, lab)) {
		// Use Lab color
	}
	\endcode
	
	\sa iimgprint::ILabToDeviceTransformation, iimgprint::CPrinterProfile
	
	\ingroup Color
	\ingroup ImageProcessing
*/
class IDeviceToLabTransformation
{
public:
	/**
		Virtual destructor.
	*/
	virtual ~IDeviceToLabTransformation() {}
	
	/**
		Converts device-dependent color to CIE Lab.
		
		\param deviceColor Device color (CMYK, RGB, etc.)
		\param labColor Resulting Lab color
		\return True if conversion succeeded
	*/
	virtual bool DeviceToLab(const icmm::CVarColor& deviceColor, icmm::CLab& labColor) const = 0;
};


/**
	Interface for Lab-to-device color transformation based on printer profile.
	
	\section LabToDevicePurpose Purpose
	ILabToDeviceTransformation converts device-independent CIE Lab colors to
	device-dependent colors (CMYK, RGB) using spectral measurements and gamut
	mapping from the printer profile. This is the inverse transformation: Lab → Device.
	
	\section LabToDeviceUsage Usage
	\code
	// Get Lab-to-device transformation from profile
	const ILabToDeviceTransformation* toDevice = profile.GetLabToDeviceTransformation();
	
	// Convert Lab to CMYK
	icmm::CLab lab(50.0, 25.0, -30.0);
	
	icmm::CVarColor cmyk;
	if (toDevice->LabToDevice(lab, cmyk)) {
		// Use device color
	}
	\endcode
	
	\sa iimgprint::IDeviceToLabTransformation, iimgprint::CPrinterProfile
	
	\ingroup Color
	\ingroup ImageProcessing
*/
class ILabToDeviceTransformation
{
public:
	/**
		Virtual destructor.
	*/
	virtual ~ILabToDeviceTransformation() {}
	
	/**
		Converts CIE Lab color to device-dependent color.
		
		\param labColor Lab color
		\param deviceColor Resulting device color (CMYK, RGB, etc.)
		\return True if conversion succeeded
	*/
	virtual bool LabToDevice(const icmm::CLab& labColor, icmm::CVarColor& deviceColor) const = 0;
};


} // namespace iimgprint


