// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// Qt includes
#include <QtCore/QAbstractItemModel>

// ACF includes
#include <istd/IPolymorphic.h>


namespace ibase
{


/**
	Interface for getting access to the	Qt's item based data model.
	\deprecated
*/
class IQtItemModelProvider: virtual public istd::IPolymorphic
{
public:
	/**
		Get access to the Qt's item model.
	*/
	virtual const QAbstractItemModel* GetItemModel() const = 0;
};


} // namespace ibase




