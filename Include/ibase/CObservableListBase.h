// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <istd/IChangeable.h>


namespace ibase
{


/**
	Base implementation for creating simple observable collection.
	Contains helper methods for constructing ChangeSets.

	\sa CListObserver
 */
class CObservableListBase
{
public:
	enum ChangeFlags
	{
		CF_ELEMENT_ADDED = 0x382b230,
		CF_ELEMENT_REMOVED,
		CF_ELEMENT_UPDATED,
		CF_ELEMENT_MOVED,
		CF_RESET
	};

	inline static const QByteArray s_CN_INDEX_ID = QByteArrayLiteral("INDEX");
	inline static const QByteArray s_CN_INDEX_FROM_ID = QByteArrayLiteral("INDEX_FROM");
	inline static const QByteArray s_CN_INDEX_TO_ID = QByteArrayLiteral("INDEX_TO");

protected:
	static istd::IChangeable::ChangeSet ElementAddChanges(qsizetype index);
	static istd::IChangeable::ChangeSet ElementRemoveChanges(qsizetype index);
	static istd::IChangeable::ChangeSet ElementUpdatedChanges(qsizetype index);
	static istd::IChangeable::ChangeSet ElementMoveChanges(qsizetype fromIndex, qsizetype toIndex);
};


} // namespace ibase


