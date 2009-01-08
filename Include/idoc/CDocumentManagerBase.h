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

#include "imod/IModel.h"
#include "imod/TMultiModelObserverBase.h"
#include "imod/CSingleModelObserverBase.h"

#include "idoc/IDocumentManager.h"


namespace idoc
{


class IDocumentTemplate;


class CDocumentManagerBase: public idoc::IDocumentManager,
							public imod::TMultiModelObserverBase<imod::IModel>
{
public:
	typedef imod::TMultiModelObserverBase<imod::IModel> BaseClass;

	CDocumentManagerBase();

	void SetDocumentTemplate(const idoc::IDocumentTemplate* documentTemplatePtr);

	// reimplemented (idoc::IDocumentManager)
	virtual const idoc::IDocumentTemplate* GetDocumentTemplate() const;
	imod::IUndoManager* GetUndoManagerForDocument(const istd::IChangeable* documentPtr) const;
	virtual int GetDocumentsCount() const;
	virtual istd::IChangeable& GetDocumentFromIndex(int index) const;
	virtual istd::IPolymorphic* GetActiveView() const;
	virtual void SetActiveView(istd::IPolymorphic* viewPtr);
	virtual istd::IChangeable* GetDocumentFromView(const istd::IPolymorphic& view) const;
	virtual std::string GetDocumentTypeId(const istd::IChangeable& document) const;
	virtual istd::IChangeable* FileNew(const std::string& documentTypeId, bool createView = true, const std::string& viewTypeId = "");
	virtual bool FileOpen(
				const std::string* documentTypeIdPtr,
				const istd::CString* fileNamePtr = NULL,
				bool createView = true,
				const std::string& viewTypeId = "",
				FileToTypeMap* loadedMapPtr = NULL);
	virtual bool FileSave(
				bool requestFileName = false,
				FileToTypeMap* savedMapPtr = NULL);
	virtual bool FileClose();

protected:
	typedef istd::TDelPtr<istd::IChangeable> DocumentPtr;
	typedef istd::TDelPtr<imod::IUndoManager> UndoManagerPtr;
	typedef istd::TDelPtr<istd::IPolymorphic> ViewPtr;
	typedef std::list<ViewPtr> Views;

	struct DocumentInfo: public imod::CSingleModelObserverBase
	{
		DocumentInfo(
					CDocumentManagerBase* parentPtr,
					const std::string& documentTypeId,
					istd::IChangeable* documentPtr,
					imod::IUndoManager* undoManagerPtr)
		:	isDirty(false)
		{
			this->parentPtr = parentPtr;
			this->documentTypeId = documentTypeId;
			this->documentPtr.SetPtr(documentPtr);
			this->undoManagerPtr.SetPtr(undoManagerPtr);
		}

		CDocumentManagerBase* parentPtr;
		istd::CString filePath;
		std::string documentTypeId;
		DocumentPtr documentPtr;
		UndoManagerPtr undoManagerPtr;
		Views views;

		bool isDirty;

	protected:
		// reimplemented (imod::CSingleModelObserverBase)
		virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);
	};

	/**
		Open single document using its file path.
		\param	filePath		file path.
		\param	createView		if true, instance of view will be created.
		\param	viewTypeId		optional view type ID should be created.
		\param	documentTypeId	output parameter returning loaded ducument type ID.
		\return	instance of created document or NULL if error is occured.
	*/
	virtual istd::IChangeable* OpenDocument(
				const istd::CString& filePath,
				bool createView,
				const std::string& viewTypeId,
				std::string& documentTypeId);

	void CloseAllDocuments();

	DocumentInfo& GetDocumentInfo(int index) const;

	/**
		Get document info assigned to active view.
	*/
	DocumentInfo* GetActiveDocumentInfo() const;

	/**
		Get document info assigned to specified view.
	*/
	DocumentInfo* GetDocumentInfoFromView(const istd::IPolymorphic& view) const;

	/**
		Get document info assigned to specified file.
	*/
	DocumentInfo* GetDocumentInfoFromPath(const istd::CString& filePath) const;

	/**
		Create instance of specified document without attaching to this manager.
	*/
	DocumentInfo* CreateDocument(const std::string& documentTypeId, bool createView, const std::string& viewTypeId) const;

	/**
		Register (attach) created document as new working document.
	*/
	bool RegisterDocument(DocumentInfo* documentPtr);

	/**
		Called after view is registered.
	*/
	virtual void OnViewRegistered(istd::IPolymorphic* viewPtr);

	/**
		Called before view is removed.
	*/
	virtual void OnViewRemoved(istd::IPolymorphic* viewPtr);

	// abstract methods
	
	/**
		Gets open file names.
	*/
	virtual istd::CStringList GetOpenFileNames(const std::string* documentTypeIdPtr = NULL) const  = 0;

	/**
		Gets save file name.
	*/
	virtual istd::CString GetSaveFileName(const std::string& documentTypeId) const = 0;

	/**
		Query user if this document can be closed.
		\return	true, if document can be closed.
	*/
	virtual bool QueryDocumentClose(const DocumentInfo& info) = 0;

private:
	/**
		Do updated of recent file list with the new file \c requestedFilePath.
	*/
	void UpdateRecentFileList(const istd::CString& requestedFilePath, const std::string& documentTypeId, bool wasSuccess);

	typedef istd::TPointerVector<DocumentInfo> DocumentInfos;

	const IDocumentTemplate* m_documentTemplatePtr;
	DocumentInfos m_documentInfos;

	istd::IPolymorphic* m_activeViewPtr;
};


} // namespace idoc


#endif // idoc_CDocumentManagerBase_included


