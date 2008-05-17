#ifndef IRegistryGeometryProvider_included
#define IRegistryGeometryProvider_included


#include <QPoint>


#include "istd/CString.h"
#include "istd/IPolymorphic.h"


class IRegistryGeometryProvider: virtual public istd::IPolymorphic
{
public:
	/**
		Get the view position for the component \c componentRole.
	*/
	virtual QPoint GetComponentPosition(const istd::CString& componentRole) const = 0;

	/**
		Set the view position for the component \c componentRole.
	*/
	virtual void SetComponentPosition(const istd::CString& componentRole, const QPoint& point) = 0;
};


#endif // !IRegistryGeometryProvider_included


