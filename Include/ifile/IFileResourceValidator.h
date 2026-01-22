// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <istd/IInformationProvider.h>
#include <ifile/IFileResourcesManager.h>


namespace ifile
{


/**
	Interface for validation of the file-based resources.
*/
class IFileResourceValidator: virtual public istd::IPolymorphic
{
public:
	/**
		Do validation of the given resource.
		\param resourceId			ID of the resource in the repository.
		\param messageConsumerPtr	Consumer of operation messages (optionally).
	*/
	virtual istd::IInformationProvider::InformationCategory ValidateResource(
				const QByteArray& resourceId,
				const ifile::IFileResourcesManager* fileRepositoryPtr,
				ilog::IMessageConsumer* messageConsumerPtr = NULL) const = 0;
};


} // namespace ifile


