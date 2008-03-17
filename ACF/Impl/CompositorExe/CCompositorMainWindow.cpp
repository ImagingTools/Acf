#include <QDockWidget>
#include <QCompleter>


#include "iqt/iqt.h"
#include "iqt/IGuiObject.h"

#include "CCompositorMainWindow.h"
#include "CPackageOverview.h"
#include "CCompositorAttributeEditor.h"
#include "CRegistryView.h"
#include "CCompositorWorkspace.h"


CCompositorMainWindow::CCompositorMainWindow(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
	setupUi(this);

	m_workspace = new CCompositorWorkspace(this);
	setCentralWidget( m_workspace );
	setIconSize(QSize(16,16));

	CreateDockWindows();
	CreateToolBars();
	CreateMenus();

	setWindowTitle(tr("ACF Compositor"));

	connect(actionOpenComponent, SIGNAL(triggered()), m_workspace, SLOT(onComponentOpen()));
	actionOpenComponent->setShortcut(Qt::CTRL + 'O');

	connect(actionNewComponent, SIGNAL(triggered()), m_workspace, SLOT(onComponentNew()));
	actionNewComponent->setShortcut(Qt::CTRL + 'N');

	connect(actionAddComponent, SIGNAL(triggered()),this, SLOT(onAddComponent()));
	actionAddComponent->setShortcut(Qt::CTRL + 'A');
	actionAddComponent->setDisabled(true);

	connect(actionRemoveComponent, SIGNAL(triggered()),this, SLOT(onRemoveComponent()));
	actionRemoveComponent->setShortcut(Qt::Key_Delete);

	connect(actionSave, SIGNAL(triggered()), m_workspace, SLOT(onComponentSave()));
	actionSave->setShortcut(Qt::CTRL + 'S');
	actionSave->setDisabled(true);

	connect(actionSaveAs, SIGNAL(triggered()), m_workspace, SLOT(onComponentSaveAs()));
	actionSaveAs->setShortcut(Qt::CTRL + Qt::SHIFT + 'S');
	actionSaveAs->setDisabled(true);

	connect(actionPreview, SIGNAL(triggered()), m_workspace, SLOT(onPreview()));
	actionPreview->setShortcut(Qt::Key_F3);

	connect(actionStartApplication, SIGNAL(triggered()), m_workspace, SLOT(onStartApplication()));
	
	connect(actionKillApplication, SIGNAL(triggered()), m_workspace, SLOT(onKillApplication()));
	actionKillApplication->setShortcut(Qt::CTRL + 'T');

	actionStartApplication->setDisabled(true);
	actionKillApplication->setDisabled(true);

	connect(actionTileVertically, SIGNAL(triggered()), m_workspace, SLOT(tile()));

	connect(m_workspace, 
		SIGNAL(applicationCanBeStarted(bool)), 
		actionStartApplication,
		SLOT(setEnabled(bool)));

	connect(m_workspace, 
		SIGNAL(applicationCanBeStarted(bool)), 
		actionKillApplication,
		SLOT(setDisabled(bool)));


	connect(m_workspace, 
		SIGNAL(currentView(CRegistryView*)), 
		m_propertyEditor,
		SLOT(SetComponentViewPtr(CRegistryView*)));

	connect(m_workspace, 
		SIGNAL(currentView(CRegistryView*)), 
		this,
		SLOT(SetComponentViewPtr(CRegistryView*)));

	connect(m_workspace, 
		SIGNAL(componentSelected(const icomp::IRegistryElement*)),
		this,
		SLOT(OnComponentSelected(const icomp::IRegistryElement*)));

	connect(m_propertyEditor, 
		SIGNAL(selectedDependencyInterface(const QString&)),
		m_packageView,
		SLOT(HighlightComponents(const QString&)));
			 
}


CCompositorMainWindow::~CCompositorMainWindow()
{

}


void CCompositorMainWindow::CreateDockWindows()
{
	QDockWidget *packageViewDock = new QDockWidget(tr("Components"), this);
    packageViewDock->setAllowedAreas(Qt::LeftDockWidgetArea );
	m_packageView = new CPackageOverview(packageViewDock);

	packageViewDock->setWidget(m_packageView);
	addDockWidget(Qt::LeftDockWidgetArea, packageViewDock);
	packageViewDock ->setFeatures(QDockWidget::AllDockWidgetFeatures);

	QDockWidget *objectsDockWindow = new QDockWidget(tr("Property Editor"), this);
	objectsDockWindow->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea | Qt::BottomDockWidgetArea);
	m_propertyEditor = new CCompositorAttributeEditor(objectsDockWindow);
	
	objectsDockWindow->setWidget(m_propertyEditor);
	addDockWidget(Qt::LeftDockWidgetArea, objectsDockWindow);
	objectsDockWindow ->setFeatures(QDockWidget::AllDockWidgetFeatures);
}


void CCompositorMainWindow::CreateToolBars()
{
	m_graphicsToolBar = new QToolBar(this);
	m_graphicsToolBar->setObjectName(QString::fromUtf8("ZoomToolBar"));
	m_graphicsToolBar->setOrientation(Qt::Horizontal);
	QStringList zooms;
	zooms << "10" << "25" << "33" << "50" << "75" << "100" << "125" << "150" << "175" << "200" << "300" << "500";

	QLabel* zoomLabel = new QLabel(m_graphicsToolBar);
	zoomLabel->setPixmap(QString::fromUtf8(":/images/images/viewmag.png"));
	m_zoomCombo = new QComboBox(m_graphicsToolBar);
	m_zoomCombo->addItems(zooms);
	m_zoomCombo->setCurrentIndex(3);
	m_zoomCombo->setEditable(true); 

	connect(m_zoomCombo, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(setZoom(const QString&)));
	m_zoomCombo->setEnabled(false);

	addToolBar(static_cast<Qt::ToolBarArea>(4), m_graphicsToolBar);

	QLabel* searchLabel = new QLabel(m_graphicsToolBar);
	searchLabel->setText(tr("Go to:"));
	m_comboSearch = new QComboBox(m_graphicsToolBar);
	m_comboSearch->setEditable(true); 
	m_comboSearch->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	connect(m_comboSearch,
		SIGNAL(currentIndexChanged(const QString&)),
		this,
		SLOT(onSearchListIndexChanged(const QString&)));

	QLayout* layout = m_graphicsToolBar->layout();
	if (layout != NULL){
		m_graphicsToolBar->addWidget(zoomLabel);
		m_graphicsToolBar->addWidget(m_zoomCombo);
		m_graphicsToolBar->addWidget(searchLabel);
		m_graphicsToolBar->addWidget(m_comboSearch);
	}

	m_graphicsToolBar->setEnabled(false);
}


void CCompositorMainWindow::CreateMenus()
{

}


void CCompositorMainWindow::SetComponentViewPtr(CRegistryView* viewPtr)
{
	m_zoomCombo->setEnabled(viewPtr != NULL);
	m_graphicsToolBar->setEnabled(viewPtr != NULL);

	if (viewPtr == NULL){
		actionKillApplication->setDisabled(true);
		actionAddComponent->setDisabled(true);
		actionSave->setDisabled(true);
		actionSaveAs->setDisabled(true);
	}
	else{
		viewPtr->disconnect(this, SLOT(UpdateComponentSearchList(CRegistryView*)));
		m_propertyEditor->disconnect(viewPtr, SLOT(UpdateConnections()));

		connect(viewPtr,
			SIGNAL(viewChanged(CRegistryView*)), 
			this, 
			SLOT(UpdateComponentSearchList(CRegistryView*)));

		connect(m_propertyEditor, 
		SIGNAL(DoUpdate()), 
		viewPtr,
		SLOT(UpdateConnections()));

		actionAddComponent->setEnabled(true);
		actionSave->setEnabled(true);
		actionSaveAs->setEnabled(true);
	}

	UpdateComponentSearchList(viewPtr);
}


void CCompositorMainWindow::UpdateComponentSearchList(CRegistryView* viewPtr)
{
	m_comboSearch->clear();
/*
	QCompleter *completer = new QCompleter(this);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	completer->setModel(m_comboSearch->model());
	m_comboSearch->setCompleter(completer);

	if (viewPtr != NULL){
		const acf::CompositeComponent& composite = viewPtr->Get();
		for (int componentIndex = 0; componentIndex < composite.GetComponentCount(); componentIndex++){
			m_comboSearch->addItem(iqt::GetQString(composite.componentRole(componentIndex)));
		}
	}
	*/
}


void CCompositorMainWindow::OnAddComponent()
{
	icomp::IComponentStaticInfo* componentPtr = m_packageView->GetSelectedComponent();
	if (componentPtr != NULL){
		CRegistryView* compositeView = dynamic_cast<CRegistryView*>(m_workspace->activeWindow());

		I_ASSERT(compositeView != NULL);

		if (compositeView != NULL){
			compositeView->AddComponent(*componentPtr);
		}
	}
}


void CCompositorMainWindow::OnRemoveComponent()
{
	CRegistryView* compositeView = dynamic_cast<CRegistryView*>(m_workspace->activeWindow());
	if (compositeView != NULL){
		compositeView->RemoveSelectedComponent();
	}
}


void CCompositorMainWindow::OnComponentSelected(const icomp::IRegistryElement* componentPtr)
{
	const icomp::IComponentStaticInfo& staticInfo = componentPtr->GetComponentStaticInfo();
	std::string guiInterfaceId = typeid(iqt::IGuiObject).name();

	icomp::IComponentStaticInfo::InterfaceExtractors interfaceExtractors = staticInfo.GetInterfaceExtractors();
	const icomp::IComponentStaticInfo::InterfaceExtractorPtr* extractorPtr = interfaceExtractors.FindElement(guiInterfaceId);
	if (extractorPtr != NULL){
		actionRemoveComponent->setEnabled(true);
		if (extractorPtr != NULL){
			actionPreview->setEnabled(true);
		}
		else{
			actionRemoveComponent->setEnabled(false);
			actionPreview->setEnabled(false);
		}
	}
}


void CCompositorMainWindow::SetZoom(const QString& value)
{
	CRegistryView* compositeView = dynamic_cast<CRegistryView*>(m_workspace->activeWindow());
	if (compositeView != NULL){
		QMatrix newMatrix;
		newMatrix.scale(value.toInt()/100.0, value.toInt()/100.0);
		compositeView->setMatrix(newMatrix);
	}
}


void CCompositorMainWindow::OnSearchListIndexChanged(const QString& role)
{
	CRegistryView* compositeView = dynamic_cast<CRegistryView*>(m_workspace->activeWindow());
	if (compositeView != NULL){
		compositeView->SetCenterOn(role);
	}
}


