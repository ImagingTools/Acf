#ifndef iqtmm_CMediaObjectComp_included
#define iqtmm_CMediaObjectComp_included


// Qt includes
#include <Phonon>


// ACF includes
#include "iqtmm/IMediaObject.h"

#include "icomp/CComponentBase.h"


namespace iqtmm
{


class CMediaObjectComp:
			public icomp::CComponentBase,
			public Phonon::MediaObject,
			virtual public iqtmm::IMediaObject
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef Phonon::MediaObject BaseClass2;

	I_BEGIN_COMPONENT(CMediaObjectComp);
		I_REGISTER_INTERFACE(iqtmm::IMediaObject);
	I_END_COMPONENT();

	// reimplemented (iqtmm::IMediaObject)
	virtual QString GetMediumUrl() const;
	virtual void SetMediumUrl(const QString& mediumUrl);
};


} // namespace iqtmm


#endif // !iqtmm_CMediaObjectComp_included

