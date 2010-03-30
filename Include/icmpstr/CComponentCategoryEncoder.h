#ifndef icmpstr_CComponentCategoryEncoder_included
#define icmpstr_CComponentCategoryEncoder_included


// Qt includes
#include <QString>
#include <QStringList>
#include <QMap>


// ACF includes
#include "icomp/IComponentStaticInfo.h"


namespace icmpstr
{


/**
	Class to decode/encode the component category into human readable text.
*/
class CComponentCategoryEncoder
{
public:
	typedef QMap<int, QString> CategoriesMap;
	typedef QList<int> CategoriesList;

	CComponentCategoryEncoder();

	QStringList GetCategoryNames(int category) const;
	int GetCategoryFromNames(const QStringList& categoriesList) const;
	const CategoriesMap& GetCategoriesMap() const;
	CategoriesList GetSplitedCategories(int categoryFlags) const;


private:
	CategoriesMap m_categoriesMap;
};


} // namespace icmpstr


#endif // !icmpstr_CComponentCategoryEncoder_included

