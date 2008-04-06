#ifndef CSerializableDocument_included
#define CSerializableDocument_included


#include "imod/CSerializedUndoManager.h"

#include "idoc/CDocument.h"


namespace idoc
{		


class CSerializableDocument: public idoc::CDocument
{
public:
	CSerializableDocument();
	virtual ~CSerializableDocument();
	
	// reimplemented (idoc::CDocument)
	virtual void SetContent(imod::IModel* modelPtr);

	// reimplemented (idoc::IDocument)
	virtual bool IsModified() const = 0;
	virtual imod::IUndoManager* GetUndoManager() const;

private:
	imod::CSerializedUndoManager m_undoManager;
};


} // namespace idoc


#endif // !CSerializableDocument_included

