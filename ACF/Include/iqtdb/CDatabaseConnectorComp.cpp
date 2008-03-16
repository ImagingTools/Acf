#include "iqtdb/CDatabaseConnectorComp.h"

#include "iqt/iqt.h"


namespace iqtdb
{


CDatabaseConnectorComp::CDatabaseConnectorComp()
:	BaseClass(),
	m_databaseDriverNameAttr("ODBC", this, "DatabaseDriver"),
	m_databaseNameAttr("", this, "DatabaseName"),
	m_hostNameAttr("localhost", this, "DatabaseHost"),
	m_userNameAttr("", this, "User"),
	m_passwordAttr("", this, "Password")
{
	RegisterInterface<idb::IDatabaseConnector>(this);
}


// reimplemented (acf::IDatabaseConnector)

bool CDatabaseConnectorComp::IsDatabaseConnected() const
{
	return m_database.isOpen();
}


bool CDatabaseConnectorComp::ConnectToDatabase()
{
	bool retVal = m_database.open();

	return retVal && m_database.isOpen();
}


bool CDatabaseConnectorComp::DisconnectFromDatabase()
{
	m_database.close();

	return !m_database.isOpen();
}


// reimplemented (acf::ComponentInterface)

bool CDatabaseConnectorComp::OnInitialize(acf::ComponentManagerInterface* managerPtr)
{
	if (BaseClass::OnInitialize(managerPtr)){
		m_database = QSqlDatabase::addDatabase(iqt::GetQString(m_databaseDriverNameAttr.GetValue()));

		m_database.setHostName(iqt::GetQString(m_hostNameAttr.GetValue()));
		m_database.setDatabaseName(iqt::GetQString(m_databaseNameAttr.GetValue()));
		m_database.setUserName(iqt::GetQString(m_userNameAttr.GetValue()));
		m_database.setPassword(iqt::GetQString(m_passwordAttr.GetValue()));

		return true;
	}

	return false;
}


void CDatabaseConnectorComp::OnDeinitialize(acf::ComponentManagerInterface* managerPtr)
{
	DisconnectFromDatabase();

	BaseClass::OnDeinitialize(managerPtr);
}


} // namespace iqtdb
