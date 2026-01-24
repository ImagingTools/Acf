// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <icomp/CComponentBase.h>
#include <ibase/IApplicationInfo.h>
#include <iqt/ISettingsProvider.h>


namespace iqt
{

	
/**
	An application settings provider component.

	This component provides access to the application's specific settings object.
*/
class CApplicationSettingsProviderComp:
			public icomp::CComponentBase,
			virtual public iqt::ISettingsProvider
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CApplicationSettingsProviderComp);
		I_REGISTER_INTERFACE(iqt::ISettingsProvider);
		I_ASSIGN(m_applicationInfoCompPtr, "ApplicationInfo", "Application info", true, "ApplicationInfo");
	I_END_COMPONENT;

	// reimplemented (iqt::ISettingsProvider)
	virtual QSettings& GetSettings() const override;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;
	
private:
	I_REF(ibase::IApplicationInfo, m_applicationInfoCompPtr);

	istd::TDelPtr<QSettings> m_settingsPtr;
};


} // namespace iqt

