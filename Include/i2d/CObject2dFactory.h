// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <iser/IObject.h>


namespace i2d
{


/**
	Simple factory for 2D-objects.
*/
class CObject2dFactory: virtual public iser::IObjectFactory
{
public:
	// reimplemented (istd::TIFactory<iser::IObject>)
	virtual iser::IObject* CreateInstance(const QByteArray& keyId = "") const override;

	// reimplemented (istd::IFactoryInfo)
	virtual KeyList GetFactoryKeys() const override;

	// static methods
	static const CObject2dFactory& GetInstance();

private:
	static KeyList GetInitialFactoryKeys();

	// static attributes
	static KeyList s_factoryKeys;
	static CObject2dFactory s_instance;
};


} // namespace i2d




