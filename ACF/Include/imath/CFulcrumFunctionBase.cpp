#include "imath/CFulcrumFunctionBase.h"


#include <algorithm>

#include "iser/CArchiveTag.h"

#include "istd/TChangeNotifier.h"


namespace imath
{


void CFulcrumFunctionBase::Reset()
{
	istd::CChangeNotifier notifier(this);

	m_fulcrums.clear();
}


int CFulcrumFunctionBase::GetFulcrumsCount() const
{
	return int(m_fulcrums.size());
}


void CFulcrumFunctionBase::SetFulcrumsCount(int count)
{
	istd::CChangeNotifier notifier(this);

	m_fulcrums.resize(count);
}


const CVector2d& CFulcrumFunctionBase::GetFulcrum(int index) const
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < int(m_fulcrums.size()));

	return m_fulcrums[index];
}


void CFulcrumFunctionBase::SetFulcrum(int index, const CVector2d& fulcrum)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < int(m_fulcrums.size()));

	istd::CChangeNotifier notifier(this);

	m_fulcrums[index] = fulcrum;
}


void CFulcrumFunctionBase::RemoveFulcrum(int index)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < int(m_fulcrums.size()));

	istd::CChangeNotifier notifier(this);

	m_fulcrums.erase(m_fulcrums.begin() + index);
}


// reimplemented (iser::ISerializable)

bool CFulcrumFunctionBase::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	iser::CArchiveTag fulcrumsTag("Fulcrums", "List of interpolation fulcrum points");
	iser::CArchiveTag fulcrumTag("Fulcrum", "Single interpolation fulcrum point");

	bool isStoring = archive.IsStoring();

	if (isStoring){
		int fulcrumsCount = int(m_fulcrums.size());
		retVal = retVal && archive.BeginMultiTag(fulcrumsTag, fulcrumTag, fulcrumsCount);

		for (int i = 0; i < fulcrumsCount; i++){
			CVector2d& point = m_fulcrums[i];

			retVal = retVal && archive.BeginTag(fulcrumTag);
			retVal = retVal && point.Serialize(archive);
			retVal = retVal && archive.EndTag(fulcrumTag);
		}

		retVal = retVal && archive.EndTag(fulcrumsTag);
	}
	else{
		istd::CChangeNotifier notifier(this);

		int fulcrumsCount;
		retVal = retVal && archive.BeginMultiTag(fulcrumsTag, fulcrumTag, fulcrumsCount);

		if (!retVal){
			return false;
		}

		SetFulcrumsCount(fulcrumsCount);

		for (int i = 0; i < fulcrumsCount; i++){
			CVector2d& point = m_fulcrums[i];

			retVal = retVal && archive.BeginTag(fulcrumTag);
			retVal = retVal && point.Serialize(archive);
			retVal = retVal && archive.EndTag(fulcrumTag);
		}

		retVal = retVal && archive.EndTag(fulcrumsTag);
	}

	return retVal;
}


// protected methods

// reimplemented (istd::IChangeable)

void CFulcrumFunctionBase::OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr)
{
	BaseClass::OnEndChanges(changeFlags, changeParamsPtr);

	SortFulcrums();
}


void CFulcrumFunctionBase::SortFulcrums()
{
	::std::sort(m_fulcrums.begin(), m_fulcrums.end(), CFulcrumFunctionBase::SortFulcrumFunc);
}


} // namespace imath





