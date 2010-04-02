#ifndef icomp_CComponentMetaDescriptionEncoder_included
#define icomp_CComponentMetaDescriptionEncoder_included


// STL includes
#include <map>

// ACF includes
#include "istd/CString.h"


namespace icomp
{


/**
	Class to decode/encode the component category into human readable text.
*/
class CComponentMetaDescriptionEncoder
{
public:
	CComponentMetaDescriptionEncoder(const istd::CString& metaDescription);

	/**
		Get meta keys using in the meta-description.
	*/
	istd::CStringList GetMetaKeys() const;

	/**
		Get value list for the given key.
	*/
	istd::CStringList GetValues(const istd::CString& key = istd::CString()) const;

	/**
		Set meta values for the given key.		
	*/
	void SetValues(const istd::CString& key, const istd::CStringList& values);

	/**
		Get full meta description text.
	*/
	istd::CString GetMetaDescription() const;

private:
	void CalculateMetaValuesMap();

	static void RemoveUnusedCharacters(istd::CString& string, const istd::CString& characters);
	static istd::CStringList SplitString(istd::CString& string);

private:
	typedef std::map<istd::CString, istd::CStringList> MetaValuesMap;

	MetaValuesMap m_metaValuesMap;

	istd::CString m_metaDescription;
};


} // namespace icomp


#endif // !icomp_CComponentMetaDescriptionEncoder_included


