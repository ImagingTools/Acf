// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <istd/IPolymorphic.h>
#include <iser/IVersionInfo.h>
#include <ibase/ibase.h>


namespace ibase
{


/**
	This interface provides some information about the application.

	\ingroup Main
*/
class IApplicationInfo: virtual public istd::IPolymorphic
{
public:
	enum ApplicationAttribute
	{
		AA_COMPANY_NAME,
		AA_PRODUCT_NAME,
		AA_PRODUCT_EDITION,
		AA_APPLICATION_ID,
		AA_APPLICATION_NAME,
		AA_APPLICATION_SUBNAME,
		AA_APPLICATION_TYPE,
		AA_APPLICATION_PATH,
		AA_LEGAL_COPYRIGHT,
		AA_MAIN_VERSION,
		AA_VERSION_KIND,
		AA_USER = 100
	};

	/**
		Kind of the application version.
		The version kind is selected via build pipeline.
		Building on a developer PC always results in \c VK_DEVELOPER.
	*/
	enum VersionKind
	{
		/**
			Developer version, built on a developer PC or without explicit selection.
		*/
		VK_DEVELOPER,

		/**
			Beta version.
		*/
		VK_BETA,

		/**
			Release candidate.
		*/
		VK_RELEASE_CANDIDATE,

		/**
			Release version.
		*/
		VK_RELEASE
	};

	/**
		Get value of application attribute.
		\param	attributeId	ID of application attribute \sa ApplicationAttribute.
	*/
	virtual QString GetApplicationAttribute(int attributeId, bool allowTranslation = true) const = 0;

	/**
		Get kind of the application version.
		The default implementation returns \c VK_DEVELOPER,
		which is also the version kind of any build done on a developer PC.
		\sa VersionKind.
	*/
	virtual VersionKind GetVersionKind() const
	{
		return VK_DEVELOPER;
	}

	/**
		Get ID of main module version ID for \c iser::IVersionInfo.
		This ID is used to identify which module is main application module.
		The meaning is the same as meaning of \c versionId parameters in interface \c iser::IVersionInfo.
		\return	ID of main module, or negative value if it is undefined.
	*/
	virtual int GetMainVersionId() const = 0;

	/**
		Get access to application version info.
	*/
	virtual const iser::IVersionInfo& GetVersionInfo() const = 0;
};


} // namespace ibase


