// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ilog/CTracingConfigurationComp.h>


// ACF includes
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>
#include <istd/CChangeNotifier.h>


namespace ilog
{

// public methods

CTracingConfigurationComp::CTracingConfigurationComp()
	: m_tracingLevel(-1)
{
}


// reimplemented (ilog::ITracingConfiguration)

int CTracingConfigurationComp::GetTracingLevel() const
{
	return m_tracingLevel;
}


void CTracingConfigurationComp::SetTracingLevel(int tracingLevel)
{
	m_tracingLevel = tracingLevel;
}


// reimplemented (iser::ISerializable)

bool CTracingConfigurationComp::Serialize(iser::IArchive& archive)
{
	const iser::IVersionInfo& versionInfo = archive.GetVersionInfo();

	QByteArray tracingLevelTagName = QByteArrayLiteral("TracingLevel");

	quint32 frameworkVersion = 0;
	if (versionInfo.GetVersionNumber(iser::IVersionInfo::AcfVersionId, frameworkVersion) && (frameworkVersion < 6447)){
		tracingLevelTagName = QByteArrayLiteral("TacingLevel");
	}

	static iser::CArchiveTag tracingLevelTag(tracingLevelTagName, "Level of message tracing", iser::CArchiveTag::TT_LEAF);

	istd::CChangeNotifier notifier(archive.IsStoring() ? nullptr : this);
	Q_UNUSED(notifier);

	bool retVal = true;

	retVal = retVal && archive.BeginTag(tracingLevelTag);
	retVal = retVal && archive.Process(m_tracingLevel);
	retVal = retVal && archive.EndTag(tracingLevelTag);

	return retVal;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CTracingConfigurationComp::OnComponentCreated()
{
	if (m_defaultTracingLevel.IsValid()){
		m_tracingLevel = *m_defaultTracingLevel;
	}
}


} // namespace ilog

