#include "idoc/CSerializableDocument.h"


#include "iser/ISerializable.h"

#include "istd/TChangeNotifier.h"
 

namespace idoc
{		


// reimplemented (idoc::CDocument)

void CSerializableDocument::SetContent(imod::IModel* modelPtr)
{
	I_ASSERT(modelPtr != NULL);

	iser::ISerializable* serializablePtr = dynamic_cast<iser::ISerializable*>(modelPtr);
	
	I_ASSERT(serializablePtr != NULL);

	modelPtr->AttachObserver(&m_undoManager);
}


// reimplemented (idoc::IDocument)

bool CSerializableDocument::IsModified() const
{

}


imod::IUndoManager* CSerializableDocument::GetUndoManager() const
{
	return m_undoManager;
}


} // namespace idoc

