#include "iqtmm/CMediaObjectComp.h"


// Qt includes
#include <QUrl>


// ACF inlcudes
#include "istd/TChangeNotifier.h"


namespace iqtmm
{


// public methods

// reimplemented (iqtmm::IMediaObject)


QString CMediaObjectComp::GetMediumUrl() const
{
	return BaseClass2::currentSource().url().path();
}


void CMediaObjectComp::SetMediumUrl(const QString& mediumUrl)
{
	if (mediumUrl != GetMediumUrl()){
		istd::CChangeNotifier changePtr(this);

		BaseClass2::setCurrentSource(mediumUrl);
	}
}


} // namespace iqtmm


