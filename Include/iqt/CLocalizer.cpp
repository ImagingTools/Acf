#include "iqt/CLocalizer.h"


// Qt includes
#include <QCoreApplication>


namespace iqt
{


// public methods

// reimplemented (istd::ILocalizer)

istd::CString CLocalizer::GetTranslatedText(
			const std::string& context,
			const istd::CString& text,
			const std::string& disambiguation) const
{
	std::string textKey = text.ToString();

	return iqt::GetCString(
				QCoreApplication::translate(context.c_str(), textKey.c_str(), disambiguation.c_str()));
}


} // namespace iqt


