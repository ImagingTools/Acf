#include <icmm/CCompositeDeviceColorModel.h>


namespace icmm
{


// public methods

// reimplemented (icmm::IColorantList)

QByteArrayList icmm::CCompositeDeviceColorModel::GetColorantIds() const
{
	QByteArrayList retVal;

	for (const ColorantInfo& colorant : m_colorants){
		retVal.push_back(colorant.id);
	}

	return retVal;
}


QString icmm::CCompositeDeviceColorModel::GetColorantName(const QByteArray& colorantId) const
{
	for (const ColorantInfo& colorant : m_colorants){
		if (colorantId == colorant.id){
			return colorant.name;
		}
	}

	return QString();
}


icmm::ColorantUsage icmm::CCompositeDeviceColorModel::GetColorantUsage(const QByteArray& colorantId) const
{
	for (const ColorantInfo& colorant : m_colorants){
		if (colorantId == colorant.id){
			return colorant.usage;
		}
	}

	return CU_NONE;
}


} // namespace icmm


