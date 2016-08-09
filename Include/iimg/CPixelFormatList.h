#ifndef iimg_CPixelFormatList_included
#define iimg_CPixelFormatList_included


// ACF includes
#include "iprm/IOptionsList.h"


namespace iimg
{


class CPixelFormatList: virtual public iprm::IOptionsList
{
public:
	typedef QSet<int> ActiveFormats;

	CPixelFormatList();

	ActiveFormats GetActiveFormats() const;
	void SetActiveFormats(const ActiveFormats& formats);

	// reimplemented (iprm::IOptionsList)
	virtual int GetOptionsFlags() const;
	virtual int GetOptionsCount() const;
	virtual QString GetOptionName(int index) const;
	virtual QString GetOptionDescription(int index) const;
	virtual QByteArray GetOptionId(int index) const;
	virtual bool IsOptionEnabled(int index) const;

private:
	ActiveFormats m_activeFormats;
};


} // namespace iimg


#endif // !iimg_CPixelFormatList_included


