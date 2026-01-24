// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <istd/IPolymorphic.h>


namespace ibase
{		


/**
	Interface for the main entry point of a component-based application.

	\ingroup Main
	\ingroup ComponentConcept
*/
class IApplication: virtual public istd::IPolymorphic
{
public:
	/**
		Initialize the application object.
	*/
	virtual bool InitializeApplication(int argc, char** argv) = 0;

	/**
		Start execution loop of the application.
	*/
	virtual int Execute(int argc, char** argv) = 0;

	/**
		Get information about the possible command line arguments of this application.
	*/
	virtual QString GetHelpText() const = 0;

	/**
		Get the command line arguments of this application.
	*/
	virtual QStringList GetApplicationArguments() const = 0;
};


} // namespace ibase




