// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QtGlobal>

// ACF includes
#include <icomp/TComponentRegistrator.h>
#include <icomp/CComponentInfoRegistrator.h>
#include <icomp/CPackageStaticInfo.h>


namespace icomp
{


#if defined _MSC_VER || defined __MINGW32__ || defined __MINGW64__
	#define I_FUNCTION_EXPORT __declspec(dllexport) // Note: actually gcc seems also to supports this syntax.
#elif defined __GNUC__
	#define I_FUNCTION_EXPORT __attribute__ ((visibility("default")))
#else
	#define I_FUNCTION_EXPORT
#endif


#ifndef QT_NO_DEBUG
	#define I_PACKAGE_EXPORT_FUNCTION GetPackageInfoDebug
	#define I_PACKAGE_EXPORT_FUNCTION_NAME "GetPackageInfoDebug"
#else // !QT_NO_DEBUG
	#define I_PACKAGE_EXPORT_FUNCTION GetPackageInfo
	#define I_PACKAGE_EXPORT_FUNCTION_NAME "GetPackageInfo"
#endif // !QT_NO_DEBUG


#ifdef I_STATIC_PACKAGE_INFO

/**
	Package source compiled into a static library: no package export and no registration,
	only the component info functions that the registry compiler declares in the generated code.
*/
#define I_EXPORT_PACKAGE(logicalName, description, keywords)

#define I_EXPORT_COMPONENT(component, description, keywords) \
	const icomp::IRealComponentStaticInfo& GetComponentInfo_##component(){\
		return component::InitStaticInfo(NULL);\
	}

#else // I_STATIC_PACKAGE_INFO

#define I_EXPORT_PACKAGE(logicalName, description, keywords) \
	static icomp::CPackageStaticInfo packageInfo(description, keywords);\
	extern "C" I_FUNCTION_EXPORT icomp::CPackageStaticInfo* I_PACKAGE_EXPORT_FUNCTION(){\
		return &packageInfo;\
	}

#ifdef I_EXTERN_COMPONENT_INFO

/**
	Package source compiled without the component headers: the component info functions are defined by the same
	source compiled with I_STATIC_PACKAGE_INFO into another object file, here they are only registered.
*/
#define I_EXPORT_COMPONENT(component, description, keywords) \
	const icomp::IRealComponentStaticInfo& GetComponentInfo_##component();\
	static icomp::CComponentInfoRegistrator component##_registrator(#component, GetComponentInfo_##component(), packageInfo, description, keywords);\

#else // I_EXTERN_COMPONENT_INFO

#define I_EXPORT_COMPONENT(component, description, keywords) \
	static icomp::TComponentRegistrator<component> component##_registrator(#component, packageInfo, description, keywords);\

#endif // I_EXTERN_COMPONENT_INFO

#endif // I_STATIC_PACKAGE_INFO

typedef icomp::CPackageStaticInfo* (*GetPackageInfoFunc)();


#define I_DATA_PERSISTENCE "\"Data Persistence\""
#define I_DATA_PROCESSING "\"Data Processing\""
#define I_SERVICE "Service"
#define I_APPLICATION "Application"
#define I_APPLICATION_LOGIC "\"Application Logic\""
#define I_DATA_MODEL "\"Data Model\""
#define I_GUI "GUI"
#define I_DATA_VISUALIZATION "Visualization"
#define I_PACKAGE "Package"

// Macros used to simplify using of meta description
#define IM_CATEGORY(keywords) " Category='" keywords "' "
#define IM_TAG(keywords) " Tag='" keywords "' "
#define IM_COMPANY(keywords) " Company='" keywords "' "
#define IM_AUTHOR(keywords) " Author='" keywords "' "
#define IM_PROJECT(keywords) " Project='" keywords "' "

#ifdef I_STATIC_PACKAGE_INFO
#define I_REGISTER_QT_RESOURCE(resourceId)
#else
#define I_REGISTER_QT_RESOURCE(resourceId)\
static struct Registrator_##resourceId\
{\
	Registrator_##resourceId()\
	{\
		Q_INIT_RESOURCE(resourceId);\
	}\
} registrator_instance_##resourceId;\

#endif // I_STATIC_PACKAGE_INFO


} // namespace icomp




