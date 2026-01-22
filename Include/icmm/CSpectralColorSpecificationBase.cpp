// SPDX-License-Identifier: LGPL-2.1-only
#include <icmm/CSpectralColorSpecificationBase.h>


// ACF includes
#include <iser/IArchive.h>
#include <istd/CChangeNotifier.h>
#include <iser/CArchiveTag.h>
#include <iser/CPrimitiveTypesSerializer.h>


namespace icmm
{



CSpectralColorSpecificationBase::CSpectralColorSpecificationBase(const ISpectrumInfo& spectrumInfo)
	: m_spectrumType(Reflective)
{
	m_info.CopyFrom(spectrumInfo);
}


CSpectralColorSpecificationBase::CSpectralColorSpecificationBase(istd::CIntRange range, int step)
	: m_spectrumType(Reflective)
{
	m_info.SetRange(range);
	m_info.SetStep(step);
}


CSpectralColorSpecificationBase::CSpectralColorSpecificationBase(const CSpectralColorSpecificationBase& spec)
	: m_spectrumType(spec.GetSpectrumType())
{
	m_info.CopyFrom(*spec.GetSpectrumInfo());
}


// reimplemented (ISpectrumInfoProvider)

const ISpectrumInfo* CSpectralColorSpecificationBase::GetSpectrumInfo() const
{
	return &m_info;
}



ISpectralColorSpecification::SpectrumType CSpectralColorSpecificationBase::GetSpectrumType() const
{
	return m_spectrumType;
}



// reimplemented (iser::ISerializable)

bool CSpectralColorSpecificationBase::Serialize(iser::IArchive& archive)
{
	istd::CChangeNotifier notifier(archive.IsStoring() ? nullptr : this, &GetAllChanges());
	Q_UNUSED(notifier);

	bool retVal = true;

	iser::CArchiveTag spectrumTypeTag("SpectrumType", "Type if used spectrum", iser::CArchiveTag::TT_LEAF);
	retVal = retVal && archive.BeginTag(spectrumTypeTag);
	retVal = retVal && iser::CPrimitiveTypesSerializer::SerializeEnum(archive, m_spectrumType, &icmm::staticMetaObject);
	retVal = retVal && archive.EndTag(spectrumTypeTag);

	iser::CArchiveTag spectrumInfoTag("SpectrumInfo", "Information about spectrum properties", iser::CArchiveTag::TT_LEAF);
	retVal = retVal && archive.BeginTag(spectrumInfoTag);
	retVal = retVal && m_info.Serialize(archive);
	retVal = retVal && archive.EndTag(spectrumInfoTag);

	return retVal;
}


// reimplemented (istd::IChangeable)

int CSpectralColorSpecificationBase::GetSupportedOperations() const
{
	return SO_CLONE | SO_COMPARE | SO_COPY;
}


bool CSpectralColorSpecificationBase::CopyFrom(const IChangeable& object, CompatibilityMode /*mode*/)
{
	const CSpectralColorSpecificationBase* objectPtr = dynamic_cast<const CSpectralColorSpecificationBase*>(&object);
	if (objectPtr != nullptr){
		istd::CChangeNotifier notifier(this);

		m_spectrumType = objectPtr->m_spectrumType;
		m_info.CopyFrom(*objectPtr->GetSpectrumInfo());

		return true;
	}

	return false;
}


bool CSpectralColorSpecificationBase::IsEqual(const IChangeable& other) const
{
	const CSpectralColorSpecificationBase* objectPtr = dynamic_cast<const CSpectralColorSpecificationBase*>(&other);
	if (objectPtr == nullptr){
		return false;
	}

	bool isTypeSame = m_spectrumType == objectPtr->GetSpectrumType();
	bool isInfoSame = m_info.IsEqual(*objectPtr->GetSpectrumInfo());

	return isTypeSame && isInfoSame;
}


} // namespace icmm


