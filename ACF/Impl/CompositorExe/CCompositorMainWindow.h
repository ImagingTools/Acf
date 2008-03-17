#ifndef CCompositorMainWindow_included
#define CCompositorMainWindow_included


#include <QMainWindow>
#include <QLineEdit>


#include "icomp/IRegistryElement.h"

#include "Generated/ui_CCompositorMainWindow.h"


class CRegistryView;
class CCompositorWorkspace;
class CCompositorAttributeEditor;
class CPackageOverview;


class CCompositorMainWindow : public QMainWindow, public Ui::CCompositorMainWindow
{
    Q_OBJECT

public:
    CCompositorMainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~CCompositorMainWindow();

protected slots:
	void CreateDockWindows();
	void CreateToolBars();
	void CreateMenus();
	void SetComponentViewPtr(CRegistryView* viewPtr);
	void UpdateComponentSearchList(CRegistryView* viewPtr);
	
	void OnAddComponent();
	void OnRemoveComponent();
	void OnComponentSelected(const icomp::IRegistryElement* componentPtr);
	void SetZoom(const QString& value);
	void OnSearchListIndexChanged(const QString& role);

private:
	CCompositorAttributeEditor* m_propertyEditor;
	CCompositorWorkspace* m_workspace;
	CPackageOverview* m_packageView;

	QToolBar* m_graphicsToolBar;
	QComboBox* m_zoomCombo;

	QToolBar* m_searchToolBar;
	QComboBox* m_comboSearch;
};


#endif // CCompositorMainWindow_included

