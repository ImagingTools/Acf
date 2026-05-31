// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QByteArray>
#include <QtCore/QMap>

// ACF includes
#include <memory>
#include <icomp/IElementStaticInfo.h>


namespace icomp
{


/**
	Lightweight implementation of IElementStaticInfo for cached subelement meta-info.
	Stores interface IDs and nested subelement info loaded from a binary cache file.
*/
class CCachedElementStaticInfo: virtual public IElementStaticInfo
{
public:
	CCachedElementStaticInfo();

	/**
		Add an interface ID to this cached subelement's interface list.
	*/
	void AddInterfaceId(const QByteArray& interfaceId);

	/**
		Register a nested subelement info with ownership transfer.
		\param	subelementId	ID of the subelement.
		\param	subelementInfoPtr	subelement info object. Ownership is transferred.
	*/
	void RegisterSubelementInfo(const QByteArray& subelementId, CCachedElementStaticInfo* subelementInfoPtr);

	// reimplemented (icomp::IElementStaticInfo)
	virtual Ids GetMetaIds(int metaGroupId) const override;
	virtual const IElementStaticInfo* GetSubelementInfo(const QByteArray& subcomponentId) const override;

private:
	Ids m_interfaceIds;

	typedef QMap<QByteArray, std::unique_ptr<CCachedElementStaticInfo>> SubelementInfos;
	SubelementInfos m_subelementInfos;
};


} // namespace icomp


