// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


template <class Archive>
class TLoggableFileReadArchive : public Archive
{
public:
	using Archive::OpenFile;

	mutable istd::IInformationProvider::InformationCategory messageCategory = istd::IInformationProvider::IC_NONE;
	mutable int messageId = 0;
	mutable QString message;

protected:
	virtual bool IsLogConsumed(
				const istd::IInformationProvider::InformationCategory* /*categoryPtr*/,
				const int* /*flagsPtr*/) const override
	{
		return true;
	}

	virtual bool SendLogMessage(
				istd::IInformationProvider::InformationCategory category,
				int id,
				const QString& messageText,
				const QString& messageSource,
				int flags = 0) const override
	{
		QString decoratedMessage = messageText;
		QString decoratedMessageSource = messageSource;
		this->DecorateMessage(category, id, flags, decoratedMessage, decoratedMessageSource);

		messageCategory = category;
		messageId = id;
		message = decoratedMessage;

		return true;
	}
};
