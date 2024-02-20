#pragma once


// ACF includes
#include <istd/IChangeable.h>


namespace icmm
{


/**
	Common interface for a colorant list used in a substractive color model.
*/
class IColorantList : virtual public istd::IChangeable
{
public:
	enum ColorantType
	{
		CT_NONE,
		CT_PROCESS,
		CT_SPOT
	};

	I_DECLARE_ENUM(ColorantType, CT_NONE, CT_PROCESS, CT_SPOT);

	/**
		Usage of a process colorant.
	*/
	enum ProcessColorantUsage
	{
		/**
			No special usage of the process colorant.
		*/
		PCU_NONE,

		/**
			Colorant is used as a darker version of another process colorant.
		*/
		PCU_DARK,

		/**
			Colorant is used as a lighter version of another process colorant.
		*/
		PCU_LIGHT,

		/**
			Colorant is used for expanding the gamut of the printing device.
		*/
		PCU_ECG
	};

	I_DECLARE_ENUM(ProcessColorantUsage, PCU_NONE, PCU_DARK, PCU_LIGHT, PCU_ECG);

	/**
		Get the list of colorant-IDs.
	*/
	virtual QByteArrayList GetColorantIds() const = 0;

	/**
		Get display name of the colorant with the given ID.
	*/
	virtual QString GetColorantName(const QByteArray& colorantId) const = 0;

	/**
		Get type of the colorant with the given ID.
	*/
	virtual ColorantType GetColorantType(const QByteArray& colorantId) const = 0;

	/**
		Get type of the colorant with the given ID. 
	*/
	virtual ProcessColorantUsage GetProcessColorantUsage(const QByteArray& colorantId) const = 0;
};


} // namespace icmm


