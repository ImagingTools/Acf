#ifndef iqtgui_CFileDialogSerializerComp_included
#define iqtgui_CFileDialogSerializerComp_included


#include "iser/IFileLoader.h"
#include "iser/IFileLoaderInfo.h"

#include "ibase/TLoggerCompWrap.h"

#include "icomp/CComponentBase.h"

#include "iqtgui/iqtgui.h"


namespace iqtgui
{


class CFileDialogLoaderComp: 
			public ibase::CLoggerComponentBase, 
			virtual public iser::IFileLoader,
			virtual public iser::IFileLoaderInfo
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	enum MessageId
	{
		MI_NONE_SUPPORTS = 0xf8e0
	};

	I_BEGIN_COMPONENT(CFileDialogLoaderComp)
		I_REGISTER_INTERFACE(iser::IFileLoader)
		I_REGISTER_INTERFACE(iser::IFileLoaderInfo)
		I_ASSIGN_MULTI_0(m_loadersCompPtr, "Loaders", "List of file serializers will be used as slaves", true)
	I_END_COMPONENT

	// reimplemented (iser::IFileLoader)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const istd::CString* filePathPtr = NULL,
				int flags = 0,
				bool beQuiet = true) const;
	virtual int LoadFromFile(istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const;
	virtual int SaveToFile(const istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const;
	virtual bool GetFileExtensions(istd::CStringList& result, int flags = 0, bool doAppend = false) const;
	virtual istd::CString GetTypeDescription(const istd::CString* extensionPtr = NULL) const;

	// reimplemented (iser::IFileLoaderInfo)
	virtual istd::CString GetLastLoadFileName() const;
	virtual istd::CString GetLastSaveFileName() const;

	// static methods
	/**
		Append filters extracted from file loader to Qt filter list accepted by \c QFileDialog.
		\param	loader	file loader.
		\param	allExt	output list of all filters from extensions separated using semicolon (for example '*.bmp;*.png').
		\param	result	input/output list of filters with description separated with next line character.
		\return			number of filters.
	*/
	static int AppendLoaderFilterList(const iser::IFileLoader& loader, int flags, QString& allExt, QString& result);

protected:
	virtual QString GetFileName(const istd::CString& filePath, bool isSaving, int& selectionIndex) const;

	iser::IFileLoader* GetLoaderFor(const QString& filePath, int selectionIndex, int flags, bool beQuiet) const;

private:
	I_MULTIREF(iser::IFileLoader, m_loadersCompPtr);

	mutable QString m_lastOpenPath;
	mutable QString m_lastSavePath;
};
	  

} // namespace iqtgui


#endif // !iqtgui_CFileDialogSerializerComp_included

