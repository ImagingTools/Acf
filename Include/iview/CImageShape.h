// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtGui/QImage>
#include <QtGui/QPixmap>
#include <QtCore/QPoint>

// ACF includes
#include <icmm/IColorTransformation.h>
#include <iimg/IBitmap.h>
#include <iview/CShapeBase.h>


namespace iview
{


class CImageShape: public CShapeBase
{
public:
	typedef CShapeBase BaseClass;

	explicit CImageShape(const icmm::IColorTransformation* colorTransformationPtr = NULL);

	// reimplemented (iview::IShape)
	virtual void Draw(QPainter& drawContext) const override;

	// reimplemented (imod::IObserver)
	virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask) override;
	virtual void AfterUpdate(imod::IModel* modelPtr, const istd::IChangeable::ChangeSet& changeSet) override;

	// reimplemented (iview::CShapeBase)
	virtual i2d::CRect CalcBoundingBox() const override;

	// reimplemented (iview::ITouchable)
	virtual TouchState IsTouched(istd::CIndex2d position) const override;
	virtual QString GetShapeDescriptionAt(istd::CIndex2d position) const override;

protected:
	virtual void DrawPixmap(
				QPainter& painter,
				const QPixmap& pixmap,
				const i2d::CRect& bitmapArea,
				const i2d::CAffine2d& destTransform) const;
private:
	void SetLookupTableToImage(QImage& image, const icmm::IColorTransformation& colorTransformation);

private:
	QPixmap m_pixmap;
	QPoint m_pixmapOffset;
	bool m_ignoreTransformation = false;

	const icmm::IColorTransformation* m_colorTransformationPtr = nullptr;
};


} // namespace iview





