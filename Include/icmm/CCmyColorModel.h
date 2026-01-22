// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// ACF includes
#include <icmm/CSubstractiveColorModelBase.h>


namespace icmm
{


class CCmyColorModel: public CSubstractiveColorModelBase
{
public:
	static ColorantId GetCyan();
	static ColorantId GetMagenta();
	static ColorantId GetYellow();

	// reimplemented (icmm::IColorantList)
	virtual ColorantIds GetColorantIds() const override;
	virtual ColorantUsage GetColorantUsage(const ColorantId& colorantId) const override;

	// reimplemented ISubstractiveColorModel
	virtual std::unique_ptr<ISubstractiveColorModel> CreateSubspaceModel(const QStringList& colorantIds) const override;

	// reimplemented (istd::IChangeable)
	virtual istd::IChangeableUniquePtr CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;
};


} // namespace icmm


