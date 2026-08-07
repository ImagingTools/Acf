// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#include <icmm/CXyz.h>


#include <iser/CArchiveTag.h>


namespace icmm
{


// static constants
static const iser::CArchiveTag s_xTag("X", "X component of XYZ", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_yTag("Y", "Y component of XYZ", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_zTag("Z", "Z component of XYZ", iser::CArchiveTag::TT_LEAF);


// public methods

bool CXyz::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	retVal = retVal && archive.BeginTag(s_xTag);
	retVal = retVal && archive.Process(GetElementRef(CI_X));
	retVal = retVal && archive.EndTag(s_xTag);

	retVal = retVal && archive.BeginTag(s_yTag);
	retVal = retVal && archive.Process(GetElementRef(CI_Y));
	retVal = retVal && archive.EndTag(s_yTag);

	retVal = retVal && archive.BeginTag(s_zTag);
	retVal = retVal && archive.Process(GetElementRef(CI_Z));
	retVal = retVal && archive.EndTag(s_zTag);

	return retVal;
}


} // namespace icmm

