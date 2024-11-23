#include <icmm/CSubstractiveColorModel.h>


namespace icmm
{


// public methods

// reimplemented (icmm::IColorantList)

IColorantList::ColorantIds CSubstractiveColorModel::GetColorantIds() const
{
	ColorantIds retVal;

	for (const ColorantInfo& colorant : m_colorants){
		retVal.push_back(colorant.id);
	}

	return retVal;
}


icmm::ColorantUsage CSubstractiveColorModel::GetColorantUsage(const ColorantId& colorantId) const
{
	for (const ColorantInfo& colorant : m_colorants){
		if (colorantId == colorant.id){
			return colorant.usage;
		}
	}

	return CU_NONE;
}


} // namespace icmm


