// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iqt/CIniSettingsProviderComp.h>



namespace iqt
{
	
	
// reimplemented (iqt::ISettingsProvider)

QSettings& CIniSettingsProviderComp::GetSettings() const
{
	Q_ASSERT(m_settingsPtr.IsValid());

	return *m_settingsPtr.GetPtr();
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CIniSettingsProviderComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_settingsPtr.SetPtr(new QSettings(*m_iniFilePathAttrPtr, QSettings::IniFormat));
}


} // namespace iqt


