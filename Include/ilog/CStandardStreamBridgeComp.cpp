// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <ilog/CStandardStreamBridgeComp.h>


namespace ilog
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CStandardStreamBridgeComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_slaveMessageConsumerCompPtr.IsValid()){
		m_coutBridgePtr.reset(new CStandardStreamBridge(std::cout, *m_slaveMessageConsumerCompPtr, istd::IInformationProvider::IC_INFO));

		m_cerrBridgePtr.reset(new CStandardStreamBridge(std::cerr, *m_slaveMessageConsumerCompPtr, istd::IInformationProvider::IC_ERROR));
	}
}

void CStandardStreamBridgeComp::OnComponentDestroyed()
{
	m_coutBridgePtr.reset();
	m_cerrBridgePtr.reset();

	BaseClass::OnComponentDestroyed();
}


} // namespace ilog

