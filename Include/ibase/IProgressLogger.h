// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// ACF includes
#include <istd/IPolymorphic.h>


namespace ibase
{


/**
	Consume information about progress of some process.
*/
class IProgressLogger: virtual public istd::IPolymorphic
{
public:
	/**
		Callback function for a progress event.
		\param	currentProgress	progress value in the range [0; 1].
	*/
	virtual void OnProgress(double currentProgress) = 0;

	/**
		Check if this processing operation should be canceled.
	*/
	virtual bool IsCanceled() const = 0;
};


} // namespace ibase




