// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icmm/CCieLabColor.h>


// ACF includes
#include <icmm/CCieLabColorModel.h>


namespace icmm
{


CCieLabColor::CCieLabColor(const CCieLabColorModel* modelPtr)
	: m_modelPtr(nullptr)
{
	if (modelPtr != nullptr){
		m_modelPtr = std::make_shared<CCieLabColorModel>(modelPtr->GetTristimulusSpecification());
	}
}


CCieLabColor::CCieLabColor(const icmm::CLab& lab, const ITristimulusSpecification& spec)
	: m_lab(lab),
	m_modelPtr(std::make_shared<icmm::CCieLabColorModel>(spec))
{
}


CCieLabColor::CCieLabColor(const CCieLabColor& color)
	: m_lab(color.m_lab),
	m_modelPtr(color.m_modelPtr)
{
}


// reimplemented (icmm::ICieLabColor)

bool CCieLabColor::Initialize(const icmm::CLab& value, const ITristimulusSpecification& spec)
{
	m_lab = value;

	if (m_modelPtr){
		m_modelPtr->SetTristimulusSpecification(spec);
	}
	else{
		m_modelPtr = std::make_shared<icmm::CCieLabColorModel>(spec);
	}

	return true;
}


const icmm::CLab& CCieLabColor::GetLab() const
{
	return m_lab;
}


IColorSpecification::ConstColorSpecPtr CCieLabColor::GetSpecification() const
{
	return m_modelPtr ? m_modelPtr->GetSpecification() : nullptr;
}

// reimplemented (icmm::IColorObject)

icmm::CVarColor CCieLabColor::GetColor() const
{
	icmm::CVarColor varColor(3);

	varColor.SetElement(0, m_lab.GetL());
	varColor.SetElement(1, m_lab.GetA());
	varColor.SetElement(2, m_lab.GetB());

	return varColor;
}


ConstColorModelPtr CCieLabColor::GetColorModel() const
{
	return m_modelPtr;
}


std::unique_ptr<IColorObject> CCieLabColor::CloneIntoUniquePtr() const
{
	return std::make_unique<CCieLabColor>(*this);
}


// reimplemented (ISerializable)

bool CCieLabColor::Serialize(iser::IArchive& /*archive*/)
{
	return false;
}


} // namespace icmm


