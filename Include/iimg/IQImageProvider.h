// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtGui/QImage>

// ACF includes
#include <istd/IPolymorphic.h>
#include <iimg/iimg.h>


namespace iimg
{


/**
	Interface for QImage object provider.

	\ingroup ImageProcessing
	\ingroup Geometry
*/
class IQImageProvider: virtual public istd::IPolymorphic
{
public:

	/**
		Gets a QImage object.
	*/
	virtual const QImage& GetQImage() const = 0;

	/**
		Copies the image data from a QImage object.
		\param	image	QImage object.
		\return	true if it succeeds, false if it fails.
	*/
	virtual bool CopyImageFrom(const QImage& image) = 0;
};


} // namespace iimg




