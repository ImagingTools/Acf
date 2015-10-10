#include <ibase/CConsoleApplicationComp.h>


// ACF includes
#include <icomp/CCompositeComponent.h>


namespace ibase
{


// public methods

// reimplemented (ibase::IApplication)

bool CConsoleApplicationComp::InitializeApplication(int argc, char** argv)
{
	m_applicationArguments.clear();

	// parse arguments:
	for (int argIndex = 0; argIndex < argc; argIndex++){
		QByteArray arg = argv[argIndex];

		m_applicationArguments << QString::fromLocal8Bit(argv[argIndex]);
	}

	if (!m_applicationPtr.IsValid()){
		if (QCoreApplication::instance() != NULL)
		{
			m_applicationPtr.SetPtr(QCoreApplication::instance(), false);
		}
		else
		{
			m_applicationPtr.SetPtr(new QCoreApplication(argc, argv), true);
			if (!m_applicationPtr.IsValid()){
				return false;
			}
		}
	}

	connect(&m_consoleReader, SIGNAL(KeyPressedSignal(char)), this, SLOT(OnKeyPressed(char)));

	m_consoleReader.Start();

	if (m_applicationInfoCompPtr.IsValid())
	{
		QCoreApplication::setOrganizationName(m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_COMPANY_NAME));
		QCoreApplication::setApplicationName(m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_NAME));
	}

	icomp::CCompositeComponent* rootComponentPtr = NULL;

	for (		icomp::ICompositeComponent* componentPtr = const_cast<icomp::ICompositeComponent*>(GetParentComponent(true));
				componentPtr != NULL;
				componentPtr = const_cast<icomp::ICompositeComponent*>(componentPtr->GetParentComponent(true))){
		rootComponentPtr = dynamic_cast<icomp::CCompositeComponent*>(componentPtr);
	}


	if (rootComponentPtr != NULL){
		rootComponentPtr->EnsureAutoInitComponentsCreated();
	}

	for (int i = 0; i < m_componentsToInitializeCompPtr.GetCount(); ++i)
	{
		istd::IPolymorphic* componentPtr = m_componentsToInitializeCompPtr[i];
		Q_UNUSED(componentPtr);
	}

	return true;
}


int CConsoleApplicationComp::Execute(int argc, char** argv)
{
	if (!InitializeApplication(argc, argv))
	{
		return -1;
	}

	return m_applicationPtr->exec();
}


QString CConsoleApplicationComp::GetHelpText() const
{
	return QString();
}


QStringList CConsoleApplicationComp::GetApplicationArguments() const
{
	return m_applicationArguments;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CConsoleApplicationComp::OnComponentDestroyed()
{
	m_consoleReader.Stop();

	BaseClass::OnComponentDestroyed();
}


// private slots

void CConsoleApplicationComp::OnKeyPressed(char ch)
{
	if (ch == 'Q' || ch == 'q' ) //if Q key pressed exit the application
	{
		QCoreApplication::exit();
	}
}


} // namespace ibase


