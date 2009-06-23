#include "iabc/CServiceApplicationComp.h"


// Qt includes
#include <QVector>
#include <QMessageBox>
#include <QCoreApplication>


namespace iabc
{


// reimplemented (ibase::IApplication)

bool CServiceApplicationComp::InitializeApplication(int/* argc*/, char** /* argv*/)
{
	return true;
}


int CServiceApplicationComp::Execute(int argc, char** argv)
{
	I_ASSERT(m_serviceNameAttrPtr.IsValid());
	I_ASSERT(m_serviceDescriptionAttrPtr.IsValid());

	if (!m_serviceDescriptionAttrPtr.IsValid() || !m_serviceNameAttrPtr.IsValid()){
		return -1;
	}

	QString serviceName = iqt::GetQString(*m_serviceNameAttrPtr);

	if (m_applicationCompPtr.IsValid()){
		// check whether the service already installed, if not, then install it:
		QtServiceController controller(serviceName);
		if (!controller.isInstalled()){
			// for installation we need 3 additional arguments: -i for install, account and password (last both we leave always empty)
			// standard Qt mechanisms don't allow definition of a service with command line parameters, so we need some workaround:
			QVector<char*> instalArguments(argc + 3); 
			QList<QByteArray> argvData;
			for (int i = 0; i < argc; ++i){
				argvData.append(argv[i]);
			}

			// copy first argument:
			instalArguments[0] = argv[0];

			// insert install arguments needed by Qt for service creation:
			instalArguments[1] = "-i";
			instalArguments[2] = "";
			instalArguments[3] = "";
			
			// append user defined application's arguments:
			for (int i = 4; i < argc + 3; ++i){
				instalArguments[i] = argvData[i - 3].data();
			}

			m_servicePtr.SetPtr(new CService(*this, *m_applicationCompPtr.GetPtr(), argc + 3, instalArguments.data(), serviceName));
			m_servicePtr->setServiceDescription(iqt::GetQString(*m_serviceDescriptionAttrPtr));

			int retVal = m_servicePtr->exec();
			if (retVal == 0){
				QMessageBox::information(
							NULL,
							QCoreApplication::tr("ACF Service Application"),
							QString(QCoreApplication::tr("Service: %1 was succesfully installed.\nNow you can start the service from the Service Manager.")).arg(serviceName));
			}

			return retVal;
		}

		// service is installed and can be started. ALL Possible parameters are in the argv[0] string defined.


		
		argc = 1;
		m_servicePtr.SetPtr(new CService(*this, *m_applicationCompPtr.GetPtr(), argc, argv, serviceName));

		return m_servicePtr->exec();
	}

	return -1;
}


istd::CString CServiceApplicationComp::GetHelpText() const
{
	// TODO: Combine service and application related  usages.
	if (m_applicationCompPtr.IsValid()){
		return m_applicationCompPtr->GetHelpText();
	}

	return istd::CString();
}


// public methods of embedded class CService

CServiceApplicationComp::CService::CService(
			CServiceApplicationComp& parent,
			ibase::IApplication& application,
			int argc,
			char **argv,
			const QString &name)
	:QtServiceBase(argc, argv, name),
	m_parent(parent),
	m_application(application),
	m_argc(argc),
	m_argv(argv)
{
	setServiceFlags(QtServiceBase::CanBeSuspended | QtServiceBase::CannotBeStopped);
}


// protected methods of embedded class CService

// reimplemented (QtServiceBase)

void CServiceApplicationComp::CService::start()
{
}


void CServiceApplicationComp::CService::createApplication(int& argc, char** argv)
{
	m_application.InitializeApplication(argc, argv);
}


int CServiceApplicationComp::CService::executeApplication()
{
	return m_application.Execute(m_argc, m_argv);
}


} // namespace iabc



