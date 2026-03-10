// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <iimg/CMultiPageBitmapSequenceComp.h>


namespace iimg
{


// protected methods

// reimplemented (idoc::IMultiPageDocument)

istd::IChangeable* CMultiPageBitmapSequenceComp::InsertPage(
			const idoc::IDocumentMetaInfo* pageMetaInfoPtr,
			const iprm::IParamsSet* /*pageParameterPtr*/,
			int position)
{
	istd::TUniqueInterfacePtr<iimg::IMultiBitmapProvider> multiBitmapPtr(m_multiBitmapFactoryCompPtr.CreateInstance());
	if (multiBitmapPtr.IsValid()){
		Page newPage;
			
		if (pageMetaInfoPtr != NULL){
			newPage.pageMetaInfo.CopyFrom(*pageMetaInfoPtr);
		}

		newPage.pagePtr.MoveCastedPtr(multiBitmapPtr);

		m_documentPages.insert(position, newPage);

		return newPage.pagePtr.GetPtr();
	}

	return NULL;
}


} // namespace iimg


