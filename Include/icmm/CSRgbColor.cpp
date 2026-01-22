// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icmm/CSRgbColor.h>


// ACF includes
#include <icmm/CRgb.h>
#include <icmm/CTristimulusSpecification.h>

// ACF includes
#include <icmm/CRgbColorModel.h>


namespace icmm
{

CSRgbColor::CSRgbColor(const icmm::CRgb& rgb)
	:BaseClass(rgb)
{
	CTristimulusSpecification spec(ObserverType::TwoDegree, AstmTableType::E308Table5, std::make_shared<CIlluminant>(StandardIlluminant::D65));

	SetSpecification(spec);
}


} // namespace icmm


