#ifndef iqt2d_CPolygonParamsGuiComp_included
#define iqt2d_CPolygonParamsGuiComp_included


// Qt includes
#include <QtGui/QItemDelegate>
#include <QtGui/QLineEdit>
#include <QtGui/QDoubleValidator>


// ACF includes
#include "i2d/CPolygon.h"

#include "iview/CInteractivePolygonShape.h"

#include "iqt2d/TShapeParamsGuiCompBase.h"
#include "iqt2d/Generated/ui_CPolygonParamsGuiComp.h"


namespace iqt2d
{


class CPolygonParamsGuiComp:
	public iqt2d::TShapeParamsGuiCompBase<
				Ui::CPolygonParamsGuiComp,
				iview::CInteractivePolygonShape,
				i2d::CPolygon>
{
	Q_OBJECT

public:
	typedef iqt2d::TShapeParamsGuiCompBase<
			Ui::CPolygonParamsGuiComp,
			iview::CInteractivePolygonShape,
			i2d::CPolygon> BaseClass;

	I_BEGIN_COMPONENT(CPolygonParamsGuiComp);
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiCreated();
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();
	virtual void UpdateGui(int updateFlags = 0);

protected Q_SLOTS:
	void OnParamsChanged();

	void on_InsertButton_clicked();
	void on_RemoveButton_clicked();

protected:
	/** 
		Internal item delegate class for input validation
	*/
	class CPolygonParamsGuiItemDelegate: public QItemDelegate
	{
	public:
		virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&, const QModelIndex&) const
		{
			QLineEdit* editorPtr = new QLineEdit(parent);
			editorPtr->setValidator(new QDoubleValidator());
			return editorPtr;
		}
	};
};


} // namespace iqt2d


#endif // !iqt2d_CPolygonParamsGuiComp_included
