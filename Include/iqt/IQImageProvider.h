#ifndef iqt_IQImageProvider_included
#define iqt_IQImageProvider_included


// Qt includes
#include <QImage>


// ACF includes
#include "istd/IPolymorphic.h"

#include "iqt/iqt.h"


namespace iqt
{


class IQImageProvider: virtual public istd::IPolymorphic
{
public:
	virtual const QImage& GetQImage() const = 0;
	virtual bool CopyImageFrom(const QImage& image) = 0;
};


} // namespace iqt


#endif // !iqt_IQImageProvider_included


