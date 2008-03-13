#include "iqt/CIconProviderComp.h"


namespace iqt
{


// public methods

// reimplemented (iqt::IIconProvider)

int CIconProviderComp::GetIconCount() const
{
	return m_iconFilesAttr.GetCount();
}


QIcon CIconProviderComp::GetIcon(int iconIndex) const
{
	static QIcon emptyIcon;

	I_ASSERT(iconIndex < m_iconFilesAttr.GetCount());
	I_ASSERT(iconIndex >= 0);

	if (index >= 0 && index < m_iconFilesAttr.GetCount()){
		return QIcon(iqt::GetQString(m_iconPathAttr.GetValue() + istd::CString("/") + m_iconFilesAttr.GetValue(index)));
	}

	return emptyIcon;
}


} // namespace iqt
