#ifndef iqt_ISettingsProvider_included
#define iqt_ISettingsProvider_included


#include "iqt/iqt.h"


#include <QSettings>

#include "istd/IPolymorphic.h"



namespace iqt
{

/**
	Interface for access the application settings object.
*/
class ISettingsProvider: virtual public istd::IPolymorphic
{
public:
	virtual QSettings& GetApplicationSettings() const = 0;
};


} // namespace iqt


#endif // !iqt_ISettingsProvider_included
