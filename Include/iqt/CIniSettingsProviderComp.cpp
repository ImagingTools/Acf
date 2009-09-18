#include "iqt/CIniSettingsProviderComp.h"



namespace iqt
{
	
	
// reimplemented (icomp::IRegistryLoaderProvider)

QSettings& CIniSettingsProviderComp::GetApplicationSettings() const
{
	I_ASSERT(m_settingsPtr.IsValid());

	return *m_settingsPtr.GetPtr();
}


// reimplemented (icomp::IComponent)

void CIniSettingsProviderComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	m_settingsPtr.SetPtr(new QSettings(iqt::GetQString(*m_iniFilePathAttrPtr), QSettings::IniFormat));
}


} // namespace iqt


