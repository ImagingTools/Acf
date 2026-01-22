// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <istd/CSystem.h>
#include <iprm/IOptionsList.h>


namespace ifile
{


/**
	Provider of file system related informations.
*/
class IFileSystemInfoProvider: virtual public istd::IChangeable
{
public:
	/**
		Get the list of drives available on the local machine.
	*/
	virtual const iprm::IOptionsList& GetDriveList() const = 0;

	/**
		Get info about a given file drive.
		\param driveIndex	Index of the file drive specified as index of the element inside of the list returned by GetDriveList.
							Negative values mean that information about all available file drives will be cummulated.
	*/
	virtual const istd::CSystem::FileDriveInfo* GetFileDriveInfo(int driveIndex = -1) const = 0;
};


} // namespace ifile


