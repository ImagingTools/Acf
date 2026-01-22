// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iview/CShapeInfoGuiComp.h>


namespace iview
{


// protected methods

void CShapeInfoGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	IShapeStatusInfo* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		QString infoText = objectPtr->GetInfoText();

		InfoText->setText(infoText);
	}
}


} // namespace iview


