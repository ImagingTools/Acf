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


IColorantList::ColorantType icmm::CCompositeDeviceColorModel::GetColorantType(const QByteArray& colorantId) const
{
	for (const ColorantInfo& colorant : m_colorants){
		if (colorantId == colorant.id){
			return colorant.type;
		}
	}

	return CT_NONE;
}


IColorantList::ProcessColorantUsage icmm::CCompositeDeviceColorModel::GetProcessColorantUsage(const QByteArray& colorantId) const
{
	for (const ColorantInfo& colorant : m_colorants){
		if (colorantId == colorant.id){
			return colorant.usage;
		}
	}

	return PCU_NONE;
}


} // namespace icmm


