// SPDX-License-Identifier: LGPL-2.1-only
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




