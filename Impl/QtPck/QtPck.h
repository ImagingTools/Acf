// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


#include <icomp/TModelCompWrap.h>
#include <icomp/TMakeComponentWrap.h>

#include <iqt/CSettingsSerializerComp.h>
#include <iqt/CApplicationSettingsProviderComp.h>
#include <iqt/CIniSettingsProviderComp.h>
#include <iqt/CClipboardSerializerComp.h>
#include <iqt/CTranslationManagerComp.h>



/**
	Standard Qt package.
*/
namespace QtPck
{



typedef iqt::CSettingsSerializerComp SettingsSerializer;
typedef iqt::CApplicationSettingsProviderComp ApplicationSettingsProvider;
typedef iqt::CIniSettingsProviderComp IniSettingsProvider;
typedef iqt::CClipboardSerializerComp ClipboardSerializer;
typedef icomp::TModelCompWrap<iqt::CTranslationManagerComp> TranslationManager;


} // namespace QtPck




