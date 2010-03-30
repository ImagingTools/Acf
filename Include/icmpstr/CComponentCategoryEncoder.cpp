#include "icmpstr/CComponentCategoryEncoder.h"


namespace icmpstr
{


// public methods

CComponentCategoryEncoder::CComponentCategoryEncoder()
{
	m_categoriesMap[icomp::IComponentStaticInfo::CCT_APPLICATION] = "Application";
	m_categoriesMap[icomp::IComponentStaticInfo::CCT_MULTIMEDIA] = "Multimedia";
	m_categoriesMap[icomp::IComponentStaticInfo::CCT_DATA] = "Data";
	m_categoriesMap[icomp::IComponentStaticInfo::CCT_PERSISTENCY] = "Persistency";
	m_categoriesMap[icomp::IComponentStaticInfo::CCT_GUI] = "Gui";
	m_categoriesMap[icomp::IComponentStaticInfo::CCT_DATA_PRESENTATION] = "Data Presentation";
	m_categoriesMap[icomp::IComponentStaticInfo::CCT_DATA_PROCESSING] = "Data Proceassing";
	m_categoriesMap[icomp::IComponentStaticInfo::CCT_APPLICATION_LOGIC] = "Application Logic";
	m_categoriesMap[icomp::IComponentStaticInfo::CCT_SERVICE] = "Service";
}


QStringList CComponentCategoryEncoder::GetCategoryNames(int category) const
{
	QStringList retVal;

	for (		CategoriesMap::const_iterator iterator = m_categoriesMap.begin();
				iterator != m_categoriesMap.end();
				iterator++){		
		if ((iterator.key() & category) != 0){
			retVal.push_back(iterator.value());
		}
	}

	return retVal;
}


int CComponentCategoryEncoder::GetCategoryFromNames(const QStringList& categoriesList) const
{
	int retVal = icomp::IComponentStaticInfo::CCT_NONE;

	for (int index = 0; index < categoriesList.count(); index++){
		int singleCategory = m_categoriesMap.key(categoriesList[index], -1);
		if (singleCategory != -1){
			retVal |= singleCategory;
		}
	}

	return retVal;
}


const CComponentCategoryEncoder::CategoriesMap& CComponentCategoryEncoder::GetCategoriesMap() const
{
	return m_categoriesMap;
}


CComponentCategoryEncoder::CategoriesList CComponentCategoryEncoder::GetSplitedCategories(int categoryFlags) const
{
	CComponentCategoryEncoder::CategoriesList splitedCategories;

	int categoriesCount = m_categoriesMap.size();

	// skip CCT_NONE
	for (int categoriesIndex = 0; categoriesIndex < categoriesCount; categoriesIndex++){
		int currentFlag = 1 << categoriesIndex;
		if ((categoryFlags & currentFlag) != 0){
			splitedCategories.push_back(currentFlag);
		}
	}

	return splitedCategories;
}



} // namespace icmpstr


