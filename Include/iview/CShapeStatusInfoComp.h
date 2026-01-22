// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <icomp/CComponentBase.h>

#include <iview/IShapeStatusInfo.h>


namespace iview
{


/**
	Standard implementation of IShapeStatusInfo interface.
*/
class CShapeStatusInfoComp: public icomp::CComponentBase, virtual public IShapeStatusInfo
{
public:
	typedef icomp::CComponentBase BaseClass;
	I_BEGIN_COMPONENT(CShapeStatusInfoComp);
		I_REGISTER_INTERFACE(IShapeStatusInfo);
	I_END_COMPONENT;

	// reimplemented (iview::IShapeStatusInfo)
	virtual QString GetInfoText() const override;
	virtual void SetInfoText(const QString& infoText) override;

private:
	QString m_infoText;
};


} // namespace iview




