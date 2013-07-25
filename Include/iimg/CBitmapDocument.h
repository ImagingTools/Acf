#ifndef iimg_CBitmapDocument_included
#define iimg_CBitmapDocument_included


// ACF includes
#include "idoc/TMultiPageDocumentWrap.h"
#include "idoc/CStandardDocumentMetaInfo.h"
#include "iimg/IMultiBitmapProvider.h"
#include "iimg/CBitmap.h"
#include "imod/TModelWrap.h"


namespace iimg
{


/**
	Definition of a multi-page bitmap document.
*/
class CBitmapDocument:
			public idoc::TMultiPageDocumentWrap<
						idoc::IMultiPageDocument,
						imod::TModelWrap<CBitmap> >,
			virtual public iimg::IMultiBitmapProvider
{
public:
	typedef idoc::TMultiPageDocumentWrap<
				idoc::IMultiPageDocument,
				imod::TModelWrap<CBitmap> > BaseClass;
	typedef imod::TModelWrap<CBitmap> Bitmap;

	// reimplemented (idoc::IMultiPageDocument)
	virtual istd::IChangeable* InsertPage(
		const QString& pageTitle = QString(),
		const QSizeF& pageSize = QSizeF(),
		int position = -1);

	// reimplemented (iimg::IMultiBitmapProvider)
	virtual const iprm::IOptionsList* GetBitmapListInfo() const;
	virtual int GetBitmapsCount() const;
	virtual const iimg::IBitmap* GetBitmap(int bitmapIndex) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);
	virtual istd::IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const;
};


} // namespace iimg


#endif // !iimg_CBitmapDocument_included


