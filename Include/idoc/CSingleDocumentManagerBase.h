#ifndef idoc_CDocumentManagerBase_included
#define idoc_CDocumentManagerBase_included


// STL includes
#include <set>
#include <map>
#include <list>

#include "istd/TIFactory.h"
#include "istd/TPointerVector.h"
#include "istd/TDelPtr.h"

#include "iser/IArchive.h"

#include "imod/CSingleModelObserverBase.h"

#include "idoc/IDocumentManager.h"


namespace idoc
{


class IDocumentTemplate;


class CSingleDocumentManagerBase:
			virtual public idoc::IDocumentManager,
			protected imod::CSingleModelObserverBase
{
public:
	CSingleDocumentManagerBase();

	void SetDocumentTemplate(const idoc::IDocumentTemplate* documentTemplatePtr);

	// reimplemented (idoc::IDocumentManager)
	virtual const idoc::IDocumentTemplate* GetDocumentTemplate() const;
	virtual imod::IUndoManager* GetUndoManagerForDocument(const istd::IChangeable* documentPtr) const;
	virtual int GetDocumentsCount() const;
	virtual istd::IChangeable& GetDocumentFromIndex(int index, DocumentInfo* documentInfoPtr) const;
	virtual int GetViewsCount(int documentIndex) const;
	virtual istd::IPolymorphic* GetViewFromIndex(int documentIndex, int viewIndex) const;
	virtual istd::IPolymorphic* GetActiveView() const;
	virtual istd::IChangeable* GetDocumentFromView(const istd::IPolymorphic& view) const;
	virtual std::string GetDocumentTypeId(const istd::IChangeable& document) const;
	virtual bool FileNew(
				const std::string& documentTypeId, 
				bool createView = true, 
				const std::string& viewTypeId = "",
				istd::IChangeable** newDocumentPtr = NULL);
	virtual bool FileOpen(
				const std::string* documentTypeIdPtr,
				const istd::CString* fileNamePtr = NULL,
				bool createView = true,
				const std::string& viewTypeId = "",
				FileToTypeMap* loadedMapPtr = NULL);
	virtual bool FileSave(
				bool requestFileName = false,
				FileToTypeMap* savedMapPtr = NULL);
	virtual void FileClose(bool* ignoredPtr = NULL);

protected:
	/**
		Open single document using its file path.
		\param	filePath		file path.
		\param	createView		if true, instance of view will be created.
		\param	viewTypeId		optional view type ID should be created.
		\param	documentTypeId	output parameter returning loaded ducument type ID.
		\return	instance of created document or NULL if error is occured.
	*/
	virtual bool OpenDocument(
				const istd::CString& filePath,
				bool createView,
				const std::string& viewTypeId,
				std::string& documentTypeId);

	/**
		Create instance of specified document without attaching to this manager.
	*/
	bool NewDocument(const std::string& documentTypeId, bool createView, const std::string& viewTypeId);
	void EnsureViewRemoved();

	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	// abstract methods

	/**
		Called after view is registered.
	*/
	virtual void OnViewRegistered(istd::IPolymorphic* viewPtr) = 0;

	/**
		Called before view is removed.
	*/
	virtual void OnViewRemoved(istd::IPolymorphic* viewPtr) = 0;

	/**
		Gets open file names.
	*/
	virtual istd::CString GetOpenFileName(const std::string* documentTypeIdPtr = NULL) const  = 0;

	/**
		Gets save file name.
	*/
	virtual istd::CString GetSaveFileName(const std::string& documentTypeId) const = 0;

	/**
		Query user if this document can be closed.
		\param	ignoredPtr	optional return flag indicating that user ignored this close operation.
	*/
	virtual void QueryDocumentClose(bool* ignoredPtr) = 0;

private:
	const IDocumentTemplate* m_documentTemplatePtr;

	istd::CString m_filePath;
	std::string m_documentTypeId;
	std::string m_viewTypeId;
	istd::TDelPtr<istd::IChangeable> m_documentPtr;
	istd::TDelPtr<imod::IUndoManager> m_undoManagerPtr;
	istd::TDelPtr<istd::IPolymorphic> m_viewPtr;

	bool m_isDirty;
};


} // namespace idoc


#endif // idoc_CDocumentManagerBase_included


