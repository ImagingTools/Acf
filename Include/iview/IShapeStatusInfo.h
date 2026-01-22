// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <istd/IChangeable.h>


namespace iview
{


/**
	Interface for access local bitmap information.
*/
class IShapeStatusInfo: virtual public istd::IChangeable
{
public:
	/**
		Get information text for the current position.
	*/
	virtual QString GetInfoText() const = 0;

	/**
		Set information text for the current position.
	*/
	virtual void SetInfoText(const QString& infoText) = 0;
};


} // namespace iview




