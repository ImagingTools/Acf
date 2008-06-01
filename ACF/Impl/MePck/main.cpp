#include <QDir>

#include "icomp/export.h"

#include "MePck.h"

#include "iqtdoc/iqtdoc.h"


static struct StaticInit
{
	StaticInit()
	{
		Q_INIT_RESOURCE(iqtdoc);
	}
} instance;


namespace QtPck
{


I_EXPORT_PACKAGE("Meilhaus", "Meilhaus package", "Meilhaus Hardware Card IO Qt");

//I_EXPORT_COMPONENT(LoginGui, "Gui used to user login", "User Login Qt GUI");


} // namespace QtPck


