// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <icmm/IIlluminant.h>


namespace icmm
{


class CIlluminant: virtual public IIlluminant
{
public:
	/**
		Construct an illuminant based on a standard type.
	*/
	CIlluminant(StandardIlluminant illuminantType = StandardIlluminant::D50);

	/**
		Construct a custom Illuminant.
	*/
	CIlluminant(
		const QString& illuminantName,
		const icmm::CVarColor& whitePoint);

	/**
		Copy constructor
	*/
	CIlluminant(const CIlluminant& other);

	// reimplemented (IWhitePointProvider)
	virtual icmm::CVarColor GetWhitePoint() const override;

	// reimplemented (IIluminant)
	virtual void SetWhitePoint(const icmm::CVarColor& whitePoint) override;
	virtual QString GetIlluminantName() const override;
	virtual void SetIlluminantName(const QString& illuminantName) override;
	virtual StandardIlluminant GetIlluminantType() const override;
	virtual void SetIlluminantType(const StandardIlluminant& illuminantType) override;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const override;
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual bool IsEqual(const istd::IChangeable& other) const override;
	virtual istd::IChangeableUniquePtr CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

protected:
	void InitFromStandardIlluminant(StandardIlluminant illuminantType);

private:
	StandardIlluminant m_illuminantType = StandardIlluminant::D50;
	QString m_illuminantName = "D50";
	icmm::CVarColor m_whitePoint;
};


} // namespace icmm


