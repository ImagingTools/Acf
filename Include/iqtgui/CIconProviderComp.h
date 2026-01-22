// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <icomp/CComponentBase.h>
#include <iqtgui/IIconProvider.h>
#include <iqtgui/TMakeIconProviderCompWrap.h>


namespace iqtgui
{


class CIconProviderComp: public iqtgui::TMakeIconProviderCompWrap<icomp::CComponentBase>, virtual public iqtgui::IIconProvider
{
public:
	typedef iqtgui::TMakeIconProviderCompWrap<icomp::CComponentBase> BaseClass;

	I_BEGIN_COMPONENT(CIconProviderComp);
		I_REGISTER_INTERFACE(iqtgui::IIconProvider);
		I_ASSIGN_MULTI_0(m_iconFilesAttrPtr, "IconFiles", "Names of the icon files", true);
		I_ASSIGN(m_iconPathAttrPtr, "IconsPath", "Path to the icon files", true, ".");
	I_END_COMPONENT;

	// reimplemented (iqtgui::IIconProvider)
	virtual int GetIconCount() const override;
	virtual QIcon GetIcon(int index) const override;

private:
	I_MULTIATTR(QString, m_iconFilesAttrPtr);
	I_ATTR(QString, m_iconPathAttrPtr);
};


} // namespace iqtgui



