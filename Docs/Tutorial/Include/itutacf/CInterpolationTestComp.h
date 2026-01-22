// SPDX-License-Identifier: LGPL-2.1-only
#pragma once


// ACF includes
#include <imod/TSingleModelObserverBase.h>
#include <icomp/CComponentBase.h>
#include <i2d/CPolygon.h>


namespace itutacf
{


/**
	Component for synchronization between some polyline and his interpolated slave.
*/
class CInterpolationTestComp:
			public icomp::CComponentBase,
			public imod::TSingleModelObserverBase<i2d::CPolygon>
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef imod::TSingleModelObserverBase<i2d::CPolygon> BaseClass2;

	I_BEGIN_COMPONENT(CInterpolationTestComp);
		I_ASSIGN(m_slaveObjectCompPtr, "SlaveObject", "Reference data object", true, "SlaveObject");
		I_ASSIGN(m_interpolatedNodesCountAttrPtr, "InterpolatedNodesCount", "Number of interpolated nodes", true, 100);
		I_ASSIGN(m_interpolatorTypeAttrPtr, "InterpolatorType", "Type of interpolator:\n0 - linear\n1 - Akima", true, 0);
	I_END_COMPONENT;
	
protected:
	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet) override;

private:
	I_REF(i2d::CPolygon, m_slaveObjectCompPtr);
	I_ATTR(int, m_interpolatedNodesCountAttrPtr);
	I_ATTR(int, m_interpolatorTypeAttrPtr);
};


} // namespace itutacf




