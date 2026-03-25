// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


#include <icomp/IPackagesManager.h>
#include <icomp/IRegistriesManager.h>
#include <icomp/IMetaInfoManager.h>


namespace icomp
{


class IComponentEnvironmentManager:
			virtual public IPackagesManager,
			virtual public IRegistriesManager,
			virtual public IMetaInfoManager
{
public:
	/**
		Get file path of real used config file.
		If no config file was used, it returns empty string.
	*/
	virtual QString GetConfigFilePath() const = 0;

	/**
		Get the list of project targets. A project target is a file path to the root registry of an executable, defined in the environment configuration.
	*/
	virtual QStringList GetProjectTargets() const = 0;
};


} // namespace icomp




