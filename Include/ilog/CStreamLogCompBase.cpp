// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ilog/CStreamLogCompBase.h>


// Qt includes
#include <QtCore/QDateTime>

// ACF includes
#include <istd/IInformationProvider.h>
#include <iser/IVersionInfo.h>
#include <iqt/iqt.h>
#include <ilog/CMessage.h>


namespace ilog
{


// public methods

CStreamLogCompBase::CStreamLogCompBase()
:	BaseClass(),
	m_isVersionInfoWritten(false),
	m_isLastDotShown(false),
	m_lastDotCategory(istd::IInformationProvider::IC_NONE),
	m_worseCategory(istd::IInformationProvider::IC_NONE)
{
}


istd::IInformationProvider::InformationCategory CStreamLogCompBase::GetWorseCategory() const
{
	return m_worseCategory;
}


// reimplemented (ilog::IMessageConsumer)

bool CStreamLogCompBase::IsMessageSupported(
			int /*messageCategory*/,
			int /*messageId*/,
			const istd::IInformationProvider* /*messagePtr*/) const
{
	return true;
}


// protected methods

void CStreamLogCompBase::WriteMessageToStream(const istd::IInformationProvider& message)
{
	QString messageText = GenerateMessageText(message);
	
	WriteText(messageText + "\n", message.GetInformationCategory());
}


QString CStreamLogCompBase::GenerateMessageText(const istd::IInformationProvider& message) const
{
	QString messageText;

	if (*m_useTimeStampAttrPtr){
		QString timeStampText;

		if (m_timeFormatAttrPtr.IsValid()){
			timeStampText = message.GetInformationTimeStamp().toString((*m_timeFormatAttrPtr).constData());
		}
		else{
			timeStampText = message.GetInformationTimeStamp().toString();
		}

		messageText += "[" + timeStampText + "] ";
	}

	QString categoryCode;
	if (*m_useCategoryAttrPtr){
		switch (message.GetInformationCategory()){
		case istd::IInformationProvider::IC_WARNING:
			messageText += "Warning: ";
			categoryCode = "W";
			break;

		case istd::IInformationProvider::IC_ERROR:
			messageText += "Error: ";
			categoryCode = "E";
			break;

		case istd::IInformationProvider::IC_CRITICAL:
			messageText += "Fatal error: ";
			categoryCode = "E!";
			break;

		default:
			categoryCode = "I";
			break;
		}
	}

	if (*m_useCodeAttrPtr){
		QString codeText = QString::number(message.GetInformationId());

		messageText += categoryCode + codeText + ": ";
	}


	messageText += message.GetInformationDescription();

	return messageText;
}


// reimplemented (ilog::CLogCompBase)

void CStreamLogCompBase::WriteMessageToLog(const MessagePtr& messagePtr)
{
	if (!m_isVersionInfoWritten){
		m_isVersionInfoWritten = true;

		if (m_versionInfoCompPtr.IsValid()){
			iser::IVersionInfo::VersionIds versionIds = m_versionInfoCompPtr->GetVersionIds();
			for (int versionId : versionIds){
				quint32 versionNumber;
				if (m_versionInfoCompPtr->GetVersionNumber(versionId, versionNumber)){
					QString versionDesc = m_versionInfoCompPtr->GetVersionIdDescription(versionId);
					QString versionName = m_versionInfoCompPtr->GetEncodedVersionName(versionId, versionNumber);
					WriteText("SW Version - " + versionDesc + ": " + versionName + "\n", istd::IInformationProvider::IC_INFO);
				}
			}
		}
	}

	if (messagePtr.IsValid()){
		istd::IInformationProvider::InformationCategory category = messagePtr->GetInformationCategory();
		if (category >= *m_minPriorityAttrPtr){
			if (m_isLastDotShown){
				WriteText("\n", m_lastDotCategory);

				m_isLastDotShown = false;
			}

			WriteMessageToStream(*messagePtr);
		}
		else if (*m_isDotEnabledAttrPtr){
			WriteText(".", category);

			m_isLastDotShown = true;
			m_lastDotCategory = category;
		}

		if (category > m_worseCategory){
			m_worseCategory = category;
		}
	}
}


// reimplemented (icomp::CComponentBase)

void CStreamLogCompBase::OnComponentDestroyed()
{
	BaseClass::OnComponentDestroyed();

	if (m_isLastDotShown){
		WriteText("\n", m_lastDotCategory);

		m_isLastDotShown = false;
	}

	m_worseCategory = istd::IInformationProvider::IC_NONE;
	m_isVersionInfoWritten = false;
}

void CStreamLogCompBase::ResetIsVersionInfoWrittenFlag()
{
	m_isVersionInfoWritten = false;
}


} // namespace ilog


