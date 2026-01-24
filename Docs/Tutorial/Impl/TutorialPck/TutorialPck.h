// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


#include <icomp/TModelCompWrap.h>
#include <ibase/TModelObserverCompWrap.h>

#include <DocView/CTextEditorComp.h>

#include <ModelObserver/CAbComp.h>
#include <ModelObserver/CAbGuiComp.h>

#include <itutacf/CInterpolationTestComp.h>


/**
	Base system-undependent general package.
*/
namespace TutorialPck
{


typedef CTextEditorComp TextEditor;

typedef icomp::TModelCompWrap<CAbComp> AB;
typedef CAbGuiComp AbGui;

typedef ibase::TModelObserverCompWrap<itutacf::CInterpolationTestComp> InterpolationTest;


} // namespace TutorialPck




