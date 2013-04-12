#include "ilog/CConsoleLogComp.h"


// STL includes
#include <iostream>

// ACF includes
#include "istd/IInformationProvider.h"


namespace ilog
{


// protected methods

// reimplemented (CStreamLogCompBase)

void CConsoleLogComp::WriteText(const QString& text)
{
	std::wcout << text.toLocal8Bit().constData();
}


void CConsoleLogComp::NewLine()
{
	std::wcout << std::endl;
}


} // namespace ilog


