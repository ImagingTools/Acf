#ifndef iqtproc_CGeneralSupplierGuiComp_included
#define iqtproc_CGeneralSupplierGuiComp_included


#include "iproc/ISupplier.h"

#include "iqtproc/TSupplierGuiCompBase.h"

#include "iqtproc/Generated/ui_CGeneralSupplierGuiComp.h"


namespace iqtproc
{


class CGeneralSupplierGuiComp: public iqtproc::TSupplierGuiCompBase<
			Ui::CGeneralSupplierGuiComp,
			iproc::ISupplier>
{
	Q_OBJECT

public:
	typedef iqtproc::TSupplierGuiCompBase<
				Ui::CGeneralSupplierGuiComp,
				iproc::ISupplier> BaseClass;

	I_BEGIN_COMPONENT(CGeneralSupplierGuiComp);
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(int updateFlags = 0);

protected slots:
	void on_TestButton_clicked();
	void on_LoadParamsButton_clicked();
	void on_SaveParamsButton_clicked();

protected:
	// reimplemented (iqtproc::TSupplierGuiCompBase)
	virtual QWidget* GetParamsWidget() const;

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
};


} // namespace iqtproc


#endif // !iqtproc_CGeneralSupplierGuiComp_included


