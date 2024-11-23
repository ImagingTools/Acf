#pragma once


// ACF includes
#include <icmm/CSubstractiveColorModelBase.h>


namespace icmm
{


class CSubstractiveColorModel: public CSubstractiveColorModelBase
{
public:
	// reimplemented (icmm::IColorantList)
	virtual ColorantIds GetColorantIds() const override;
	virtual ColorantUsage GetColorantUsage(const ColorantId& colorantId) const override;

protected:
	struct ColorantInfo
	{
		QByteArray id;
		ColorantUsage usage = CU_NONE;
	};

	typedef QVector<ColorantInfo> ColorantInfoList;

private:
	ColorantInfoList m_colorants;
};


} // namespace icmm


