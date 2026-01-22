// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <ifile/IFileListProvider.h>
#include <iqtgui/TDesignerGuiObserverCompBase.h>

#include <GeneratedFiles/ifilegui/ui_CFileListProviderGuiComp.h>


namespace ifilegui
{


class CFileListProviderGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CFileListProviderGuiComp, ifile::IFileListProvider>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CFileListProviderGuiComp, ifile::IFileListProvider> BaseClass;

	I_BEGIN_COMPONENT(CFileListProviderGuiComp);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet) override;
};


} // namespace ifilegui




