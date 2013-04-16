#include "iqtgui/CGuiApplicationComp.h"


// Qt includes
#include <QtCore/QString>
#include <QtGui/QApplication>
#include <QtGui/QVBoxLayout>
#include <QtGui/QIcon>


namespace iqtgui
{


// public methods

CGuiApplicationComp::CGuiApplicationComp()
	:m_defaultWidgetFlags(0)
{	
}


// reimplemented (iqtgui::IGuiApplication)

const iqtgui::IGuiObject* CGuiApplicationComp::GetApplicationGui() const
{
	if (m_mainGuiCompPtr.IsValid()){
		return m_mainGuiCompPtr.GetPtr();
	}

	return NULL;
}


// reimplemented (ibase::IApplication)

int CGuiApplicationComp::Execute(int argc, char** argv)
{
	int retVal = -1;

	if (BaseClass::InitializeApplication(argc, argv)){
		m_runtimeStatus.SetRuntimeStatus(ibase::IRuntimeStatusProvider::RS_STARTING);
	
		QByteArray appStyle;

		// parse arguments:
		for (int index = 1; index < argc; index++){
			QByteArray arg = argv[index];
			if (arg == "-style"){
				if (index+1 < argc){
					appStyle = argv[index+1];
				}
			}
		}
		QApplication::setStyle(appStyle);

		TryShowSplashScreen();

		if (m_mainGuiCompPtr.IsValid()){
			if (m_frameSpaceSizeAttrPtr.IsValid()){
				m_mainWidgetPtr.SetPtr(new QWidget());
				QVBoxLayout* frameLayout = new QVBoxLayout(m_mainWidgetPtr.GetPtr());

				frameLayout->setMargin(*m_frameSpaceSizeAttrPtr);

				// create application's main widget:
				m_mainGuiCompPtr->CreateGui(m_mainWidgetPtr.GetPtr());
			}
			else{
				m_mainGuiCompPtr->CreateGui(NULL);

				m_mainWidgetPtr.SetPtr(m_mainGuiCompPtr->GetWidget());
			}

			m_mainWidgetPtr->setWindowTitle(QCoreApplication::applicationName());
			m_mainWidgetPtr->setWindowIcon(QApplication::windowIcon());
		}

		HideSplashScreen();

		if (m_mainWidgetPtr.IsValid()){
			m_defaultWidgetFlags = m_mainWidgetPtr->windowFlags();

			UpdateMainWidgetDecorations();

			ShowWindow();

			m_lastWidgetGeometry = m_mainWidgetPtr->geometry();
		}

		if (m_mainWidgetPtr.IsValid()){
			m_runtimeStatus.SetRuntimeStatus(ibase::IRuntimeStatusProvider::RS_RUNNING);

			// start application loop:
			retVal = QApplication::exec();

			m_runtimeStatus.SetRuntimeStatus(ibase::IRuntimeStatusProvider::RS_SHUTDOWN);

			Q_ASSERT(m_mainGuiCompPtr.IsValid());
			m_mainGuiCompPtr->DestroyGui();

			m_mainWidgetPtr.Reset();
		}
	}

	return retVal;
}


QString CGuiApplicationComp::GetHelpText() const
{
	return "-style QtStyle\tname of Qt-specified style";
}


// protected methods

// reimplemented (imod::TSingleModelObserverBase)

void CGuiApplicationComp::OnUpdate(int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if (m_mainWidgetPtr.IsValid()){
		m_lastWidgetGeometry = m_mainWidgetPtr->geometry();

		UpdateMainWidgetDecorations();

		ShowWindow();

		if (		!m_mainWidgetPtr->isMaximized() &&
					!m_mainWidgetPtr->isMinimized() &&
					!m_mainWidgetPtr->isFullScreen()){
			m_mainWidgetPtr->setGeometry(m_lastWidgetGeometry);
		}
	}
}


// reimplemented (icomp::CComponentBase)

void CGuiApplicationComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_allowApplicationCloseModelCompPtr.IsValid()){
		m_allowApplicationCloseModelCompPtr->AttachObserver(this);
	}
}


void CGuiApplicationComp::OnComponentDestroyed()
{
	BaseClass2::EnsureModelDetached();

	BaseClass::OnComponentDestroyed();
}


// private methods

void CGuiApplicationComp::UpdateMainWidgetDecorations()
{
	if (m_allowApplicationCloseCompPtr.IsValid() && m_mainWidgetPtr.IsValid())
	{
		Qt::WindowFlags windowFlags = m_defaultWidgetFlags;
	
		if (!m_allowApplicationCloseCompPtr->IsEnabled()){
			windowFlags = (m_mainWidgetPtr->windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowCloseButtonHint;
		}

		if (windowFlags != m_mainWidgetPtr->windowFlags()){
			m_mainWidgetPtr->setWindowFlags(windowFlags);
		}
	}
}


void CGuiApplicationComp::ShowWindow()
{
	Q_ASSERT(m_mainWidgetPtr.IsValid());

	int uiStartMode = 0;
	if (m_uiStartModeAttrPtr.IsValid()){
		uiStartMode = *m_uiStartModeAttrPtr;
	}

	switch (uiStartMode){
		case 1:
			m_mainWidgetPtr->showFullScreen();
			break;

		case 2:
			m_mainWidgetPtr->showMinimized();
			break;

		case 3:
			m_mainWidgetPtr->showMaximized();
			break;

		default:
			m_mainWidgetPtr->show();
	}
}


// public methods of the embedded class RuntimeStatus

CGuiApplicationComp::RuntimeStatus::RuntimeStatus()
	:m_status(RS_NONE)
{
}


void CGuiApplicationComp::RuntimeStatus::SetRuntimeStatus(IRuntimeStatusProvider::RuntimeStatus runtimeStatus)
{
	if (m_status != runtimeStatus){
		istd::CChangeNotifier changePtr(this);
		
		m_status = runtimeStatus;
	}
}


// reimplemented (ibase::IRuntimeStatusProvider)

ibase::IRuntimeStatusProvider::RuntimeStatus CGuiApplicationComp::RuntimeStatus::GetRuntimeStatus() const
{
	return m_status;
}


} // namespace iqtgui


