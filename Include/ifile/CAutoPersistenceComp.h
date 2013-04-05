#ifndef ifile_CAutoPersistenceComp_included
#define ifile_CAutoPersistenceComp_included


// ACF includes
#include "istd/IChangeable.h"
#include "icomp/CComponentBase.h"
#include "ifile/IFilePersistence.h"
#include "ifile/IFileNameParam.h"


namespace ifile
{


/**
	Allow automatic object persistence.
	Referenced object will be restored during creation and stored during destruction.
*/
class CAutoPersistenceComp: public icomp::CComponentBase
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CAutoPersistenceComp);
		I_ASSIGN(m_objectCompPtr, "Object", "Object will be restored and stored", true, "Object");
		I_ASSIGN(m_fileLoaderCompPtr, "FileLoader", "File loader used to store and restore object", true, "FileLoader");
		I_ASSIGN(m_filePathCompPtr, "FilePath", "File path where object data will be stored if non anonymous loader is used", false, "FilePath");
		I_ASSIGN(m_restoreOnBeginAttrPtr, "RestoreonBegin", "Flag indicating that object should be restored on begin", true, true);
		I_ASSIGN(m_storeOnEndAttrPtr, "StoreOnEnd", "Flag indicating that object should be stored on end", true, true);
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_REF(istd::IChangeable, m_objectCompPtr);
	I_REF(ifile::IFilePersistence, m_fileLoaderCompPtr);
	I_REF(ifile::IFileNameParam, m_filePathCompPtr);
	I_ATTR(bool, m_restoreOnBeginAttrPtr);
	I_ATTR(bool, m_storeOnEndAttrPtr);
};


} // namespace ifile


#endif // !ifile_CAutoPersistenceComp_included


