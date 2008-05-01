#ifndef iqt_CBitmapLoaderComp_included
#define iqt_CBitmapLoaderComp_included


#include "iser/IFileLoader.h"

#include "icomp/CComponentBase.h"

#include "iqt/iqt.h"


namespace iqt
{


class CBitmapLoaderComp: public icomp::CComponentBase, virtual public iser::IFileLoader
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CBitmapLoaderComp)
		I_REGISTER_INTERFACE(iser::IFileLoader)
	I_END_COMPONENT

	virtual void SetLastLoadFileName(const istd::CString& fileName);
	virtual void SetLastSaveFileName(const istd::CString& fileName);

	// reimplemented (iser::IFileLoader)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const istd::CString* filePathPtr = NULL,
				bool forLoading = true,
				bool forSaving = true) const;
	virtual int LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const;
	virtual int SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const;
	virtual const istd::CString& GetLastLoadFileName() const;
	virtual const istd::CString& GetLastSaveFileName() const;

private:
	istd::CString m_lastLoadFileName;
	istd::CString m_lastSaveFileName;
};


// inline methods

inline void CBitmapLoaderComp::SetLastLoadFileName(const istd::CString& fileName)
{
	m_lastLoadFileName = fileName;
}


inline void CBitmapLoaderComp::SetLastSaveFileName(const istd::CString& fileName)
{
	m_lastSaveFileName = fileName;
}


} // namespace iqt


#endif // !iqt_CBitmapLoaderComp_included


