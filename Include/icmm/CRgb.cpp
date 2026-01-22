// SPDX-License-Identifier: LGPL-2.1-only
#include <icmm/CRgb.h>


// ACF includes
#include <iser/CArchiveTag.h>


namespace icmm
{


// static constants
static const iser::CArchiveTag s_redTag("Red", "Red component", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_greenTag("Green", "Green component", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_blueTag("Blue", "Blue component", iser::CArchiveTag::TT_LEAF);


// public methods

bool CRgb::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	retVal = retVal && archive.BeginTag(s_redTag);
	retVal = retVal && archive.Process(GetElementRef(CI_RED));
	retVal = retVal && archive.EndTag(s_redTag);

	retVal = retVal && archive.BeginTag(s_greenTag);
	retVal = retVal && archive.Process(GetElementRef(CI_GREEN));
	retVal = retVal && archive.EndTag(s_greenTag);

	retVal = retVal && archive.BeginTag(s_blueTag);
	retVal = retVal && archive.Process(GetElementRef(CI_BLUE));
	retVal = retVal && archive.EndTag(s_blueTag);

	return retVal;
}


} // namespace icmm


