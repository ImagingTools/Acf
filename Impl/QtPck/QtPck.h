// SPDX-License-Identifier: LGPL-2.1-only
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




