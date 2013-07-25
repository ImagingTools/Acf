#include "iimg/CBitmapDocument.h"


namespace iimg
{


// public methods

// reimplemented (idoc::IMultiPageDocument)

istd::IChangeable* CBitmapDocument::InsertPage(
			const QString& /*pageTitle*/,
			const QSizeF& /*pageSize*/,
			int position)
{
	istd::CChangeNotifier changePtr(this);

	Page newPage;

	Bitmap* bitmapPtr = new Bitmap;
	newPage.pagePtr.SetPtr(bitmapPtr);

	if (position < 0){
		m_documentPages.push_back(newPage);
	}
	else{
		position = qMin(m_documentPages.count(), position);

		m_documentPages.insert(position, newPage);
	}

	return bitmapPtr;
}


// reimplemented (iimg::IMultiBitmapProvider)

const iprm::IOptionsList* CBitmapDocument::GetBitmapListInfo() const
{
	return NULL;
}


int CBitmapDocument::GetBitmapsCount() const
{
	return GetPagesCount();
}


const iimg::IBitmap* CBitmapDocument::GetBitmap(int bitmapIndex) const
{
	return m_documentPages.at(bitmapIndex).pagePtr.GetPtr();
}


// reimplemented (iser::ISerializable)

bool CBitmapDocument::Serialize(iser::IArchive& /*archive*/)
{
	bool retVal = true;

	return retVal;
}


// reimplemented (istd::IChangeable)

bool CBitmapDocument::CopyFrom(const istd::IChangeable& object, CompatibilityMode /*mode*/)
{
	const CBitmapDocument* sourcePtr = dynamic_cast<const CBitmapDocument*>(&object);
	if (sourcePtr != NULL){
		istd::CChangeNotifier changePtr(this);

		m_documentPages.clear();

		for (int pageIndex = 0; pageIndex < sourcePtr->m_documentPages.count(); ++pageIndex){
			istd::IChangeable* pagePtr = InsertPage();
			if (pagePtr == NULL){
				return false;
			}

			const istd::IChangeable* sourcePagePtr = sourcePtr->m_documentPages.at(pageIndex).pagePtr.GetPtr();
			Q_ASSERT(sourcePagePtr != NULL);

			if (!pagePtr->CopyFrom(*sourcePagePtr)){
				return false;
			}
		}

		return true;
	}

	return false;
}


istd::IChangeable* CBitmapDocument::CloneMe(CompatibilityMode mode) const
{
	istd::TDelPtr<CBitmapDocument> clonedPtr(new CBitmapDocument);
	if (clonedPtr->CopyFrom(*this, mode)){
		return clonedPtr.PopPtr();
	}

	return NULL;
}


} // namespace iimg


