#ifndef icam_CSnapBitmapSupplierComp_included
#define icam_CSnapBitmapSupplierComp_included


#include "istd/TDelPtr.h"

#include "iimg/IBitmap.h"

#include "iprm/IParamsSet.h"

#include "iproc/TSupplierCompWrap.h"

#include "icam/IBitmapAcquisition.h"
#include "icam/IBitmapSupplier.h"


namespace icam
{


/**
	Implementation of bitmap supplier based on image acquisition.
*/
class CSnapBitmapSupplierComp: public iproc::TSupplierCompWrap<IBitmapSupplier, istd::TDelPtr<iimg::IBitmap> >
{
public:
	typedef iproc::TSupplierCompWrap<IBitmapSupplier, istd::TDelPtr<iimg::IBitmap> > BaseClass;

	I_BEGIN_COMPONENT(CSnapBitmapSupplierComp);
		I_ASSIGN(m_bitmapCompFact, "BitmapFactory", "Use to create bitmap object", true, "BitmapFactory");
		I_ASSIGN(m_bitmapAcquisitionCompPtr, "BitmapAcquisition", "Bitmap acquision object for image snap", true, "BitmapAcquisition");
		I_ASSIGN(m_paramsSetCompPtr, "ParamsSet", "Parameters set for bitmap acquisition", false, "ParamsSet");
	I_END_COMPONENT;

	iprm::IParamsSet* GetParamsSet() const;

	// reimplemented (icam::IBitmapSupplier)
	virtual const iimg::IBitmap* GetBitmap(I_DWORD objectId) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);
	virtual I_DWORD GetMinimalVersion(int versionId = iser::IVersionInfo::UserVersionId) const;

protected:
	// reimplemented (iproc::TSupplierCompWrap)
	virtual int ProduceObject(I_DWORD objectId, istd::TDelPtr<iimg::IBitmap>& result) const;

private:
	I_FACT(iimg::IBitmap, m_bitmapCompFact);

	I_REF(icam::IBitmapAcquisition, m_bitmapAcquisitionCompPtr);

	I_REF(iprm::IParamsSet, m_paramsSetCompPtr);
};


} // namespace icam


#endif // !icam_CSnapBitmapSupplierComp_included


