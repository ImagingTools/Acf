#include <iprm/CNameParam.h>


// Qt includes
#include <QtCore/QObject>

// ACF includes
#include <istd/TDelPtr.h>
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


namespace iprm
{


// static constants
static const istd::IChangeable::ChangeSet s_renameChangeSet(iprm::INameParam::CF_RENAME, QObject::tr("Change name"));


// reimplemented (iprm::INameParam)

const QString& CNameParam::GetName() const
{
	return m_name;
}


void CNameParam::SetName(const QString& name)
{
	if (m_name != name){
		istd::CChangeNotifier notifier(this, &s_renameChangeSet);
		Q_UNUSED(notifier);

		m_name = name;
	}
}


bool CNameParam::IsNameFixed() const
{
	return false;
}


// reimplemented (iser::ISerializable)

bool CNameParam::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag nameTag("Name", "Object name", iser::CArchiveTag::TT_LEAF);

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);
	Q_UNUSED(notifier);

	bool retVal = archive.BeginTag(nameTag);
	retVal = retVal && archive.Process(m_name);
	retVal = retVal && archive.EndTag(nameTag);

	return retVal;
}


// reimplemented (istd::IChangeable)

bool CNameParam::CopyFrom(const IChangeable& object, CompatibilityMode /*mode*/)
{
	const INameParam* sourcePtr = dynamic_cast<const INameParam*>(&object);
	if (sourcePtr != NULL){
		SetName(sourcePtr->GetName());

		return true;
	}

	return false;
}


istd::IChangeable* CNameParam::CloneMe(istd::IChangeable::CompatibilityMode mode) const
{
	istd::TDelPtr<CNameParam> clonePtr(new CNameParam);
	if (clonePtr->CopyFrom(*this, mode)){
		return clonePtr.PopPtr();
	}

	return NULL;
}


} // namespace iprm


