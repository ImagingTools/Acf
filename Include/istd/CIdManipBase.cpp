// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <istd/CIdManipBase.h>


namespace istd
{


// static methods

bool CIdManipBase::SplitId(const QByteArray& complexId, QByteArray& baseId, QByteArray& subId)
{
	int separatorPos = complexId.indexOf('/');
	if (separatorPos >= 0){
		baseId = complexId.left(separatorPos);
		subId = complexId.mid(separatorPos + 1);

		return true;
	}
	else{
		baseId = complexId;
		subId = "";

		return false;
	}
}


QByteArray CIdManipBase::JoinId(const QByteArray& baseId, const QByteArray& subId)
{
	if (!subId.isEmpty()){
		return baseId + "/" + subId;
	}
	else{
		return baseId;
	}
}


}//namespace istd


