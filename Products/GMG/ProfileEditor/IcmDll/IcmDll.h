// IcmDll.h : main header file for the IcmDll DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include "cp_mx.h"

#include "IccToMxProfileMap.h"

// STL Includes
#include <map>

// ACF Includes
#include "iwin/CWinCriticalSection.h"
#include "iwin/CDllFunctionsProvider.h"

#define DLL_EXPORT _stdcall
#define DLL_IMPORT _stdcall


extern "C"
{

	
typedef BOOL (DLL_IMPORT *CheckColorsPtr)(
			HCMTRANSFORM hcmTransform,
			LPCOLOR lpaInputColors,
			DWORD nColors,
			COLORTYPE ctInput,
			LPBYTE lpaResult
);

typedef BOOL (DLL_IMPORT *CheckColorsInGamutPtr)(
			HCMTRANSFORM hcmTransform,  // transform handle
			RGBTRIPLE *lpaRGBTriple,    // RGB triple array
			LPBYTE lpaResult,           // buffer for results
			UINT nCount                 // result buffer size
);

typedef BOOL (DLL_IMPORT *CheckRGBsPtr)(
			HCMTRANSFORM hcmTransform,  // transform handle
			LPVOID lpSrcBits,           // source bitmap bits
			BMFORMAT bmInput,           // source bitmap format
			DWORD dwWidth,              // source bitmap width
			DWORD dwHeight,             // source bitmap hight
			DWORD dwStride,             // source bitmap delta
			LPBYTE lpaResult,           // buffer for results
			PBMCALLBACKFN pfnCallback,  // pointer to callback function
			LPARAM ulCallbackData       // caller-defined parameter to callback
);

typedef BOOL (DLL_IMPORT *ConvertColorNameToIndexPtr)(
			HPROFILE hProfile,
			PCOLOR_NAME paColorName,
			PDWORD paIndex,
			DWORD dwCount
);

typedef BOOL (DLL_IMPORT *ConvertIndexToColorNamePtr)(
			HPROFILE hProfile,
			PDWORD paIndex,
			PCOLOR_NAME paColorName,
			DWORD dwCount
);

typedef BOOL (DLL_IMPORT *CreateDeviceLinkProfilePtr)(
			PHPROFILE pahProfiles,    // array of profile handles
			DWORD nProfiles,          // profile handle array size
			PDWORD padwIntents,       // array of rendering intents
			DWORD nIntents,           // intent array size
			DWORD dwFlags,            // transform creation flags
			LPBYTE *lpProfileData     // pointer to pointer to buffer
);

typedef HCMTRANSFORM (DLL_IMPORT *CreateMultiProfileTransformPtr)(
			PHPROFILE pahProfiles,    // array of profile handles
			DWORD nProfiles,          // profile handle array size
			PDWORD padwIntents,       // array of rendering intents
			DWORD nIntents,           // intent array size
			DWORD dwFlags             // transform creation flags
);

typedef BOOL (DLL_IMPORT *CreateProfilePtr)(
			LPLOGCOLORSPACEA lpColorSpace,  // pointer to a logical color space
			LPDEVCHARACTER *lpProfileData   // pointer to pointer to buffer
);

typedef BOOL (DLL_IMPORT *CreateProfileWPtr)(
			LPLOGCOLORSPACEW lpColorSpace,  // pointer to a logical color space
			LPDEVCHARACTER *lpProfileData   // pointer to pointer to buffer
);

typedef HCMTRANSFORM (DLL_IMPORT *CreateTransformPtr)(
			LPLOGCOLORSPACEA lpColorSpace,       // pointer to logical color space
			LPDEVCHARACTER lpDevCharacter,       // profile data
			LPDEVCHARACTER lpTargetDevCharacter  // target profile data
);

typedef HCMTRANSFORM (DLL_IMPORT *CreateTransformWPtr)(
			LPLOGCOLORSPACEW lpColorSpace,       // pointer to logical color space
			LPDEVCHARACTER lpDevCharacter,       // profile data
			LPDEVCHARACTER lpTargetDevCharacter  // target profile data
);

typedef HCMTRANSFORM (DLL_IMPORT *CreateTransformExtPtr)(
			LPLOGCOLORSPACEA lpColorSpace,        // pointer to logical color space
			LPDEVCHARACTER lpDevCharacter,        // profile data
			LPDEVCHARACTER lpTargetDevCharacter,  // target profile data
			DWORD dwFlags                         // creation flags
);

typedef HCMTRANSFORM (DLL_IMPORT *CreateTransformExtWPtr)(
			LPLOGCOLORSPACEW lpColorSpace,        // pointer to logical color space
			LPDEVCHARACTER lpDevCharacter,        // profile data
			LPDEVCHARACTER lpTargetDevCharacter,  // target profile data
			DWORD dwFlags                         // creation flags
);

typedef BOOL (DLL_IMPORT *DeleteTransformPtr)(
			HCMTRANSFORM hcmTransform             // transform handle to be deleted.
);

typedef DWORD (DLL_IMPORT *GetInfoPtr)(DWORD dwInfo);

typedef BOOL (DLL_IMPORT *GetNamedProfileInfoPtr)(
			HPROFILE hProfile,                    // profile handle
			PNAMED_PROFILE_INFO pNamedProfileInfo // pointer to named profile info
);

typedef BOOL (DLL_IMPORT *GetPS2ColorRenderingDictionaryPtr)(
			HPROFILE hProfile,
			DWORD dwIntent,
			LPBYTE lpBuffer,
			LPDWORD lpcbSize,
			LPBOOL lpbBinary
);

typedef BOOL (DLL_IMPORT *GetPS2ColorRenderingIntentPtr)(
			HPROFILE hProfile,
			DWORD dwIntent,
			LPBYTE lpBuffer,
			LPDWORD lpcbSize
);

typedef BOOL (DLL_IMPORT *GetPS2ColorSpaceArrayPtr)(
			HPROFILE hProfile,
			DWORD dwIntent,
			DWORD dwCSAType,
			LPBYTE lpBuffer,
			LPDWORD lpcbSize,
			LPBOOL lpbBinary
);

typedef BOOL (DLL_IMPORT *IsProfileValidPtr)(
			HPROFILE hProfile,                  // proflle handle
			LPBOOL lpbValid                     // buffer for result.
);

typedef BOOL (DLL_IMPORT *TranslateColorsPtr)(
			HCMTRANSFORM hcmTransform,          // transform handle
			LPCOLOR lpaInputColors,             // pointer to input color array
			DWORD nColors,                      // number of color in color array
			COLORTYPE ctInput,                  // input color type
			LPCOLOR lpaOutputColors,            // pointer to output color array
			COLORTYPE ctOutput                  // output color type
);

typedef BOOL (DLL_IMPORT *TranslateRGBPtr)(
			HCMTRANSFORM hcmTransform,
			COLORREF ColorRef,
			LPCOLORREF lpColorRef,
			DWORD dwFlags
);

typedef BOOL (DLL_IMPORT *TranslateRGBsPtr)(
			HCMTRANSFORM hcmTransform,
			LPVOID lpSrcBits,
			BMFORMAT bmInput,
			DWORD dwWidth,
			DWORD dwHeight,
			DWORD dwStride,
			LPVOID lpDestBits,
			BMFORMAT bmOutput,
			DWORD dwTranslateDirection
);

typedef BOOL (DLL_IMPORT *TranslateRGBsExtPtr)(
			HCMTRANSFORM hcmTransform,
			LPVOID lpSrcBits,
			BMFORMAT bmInput,
			DWORD dwWidth,
			DWORD dwHeight,
			DWORD dwInputStride,
			LPVOID lpDestBits,
			BMFORMAT bmOutput,
			DWORD dwOutputStride,
			LPBMCALLBACKFN lpfnCallback,
			LPARAM ulCallbackData
);


}

// CStdCmmCaller
// See IcmDll.cpp for the implementation of this class
//

class CStdCmmCaller: public CWinApp
{
public:
	CStdCmmCaller();
	virtual ~CStdCmmCaller();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()

public:
	bool EnsureProofInitialized();

	HCMTRANSFORM AddNativeHandler(HCMTRANSFORM nativeHandler);
	HCMTRANSFORM AddProofHandler(const ::std::string& profileName, HCMTRANSFORM nativeHandler = 0);
	bool RemoveHandler(HCMTRANSFORM handler);
	bool IsNativeHandler(HCMTRANSFORM handler) const;
	bool IsProofHandler(HCMTRANSFORM handler) const;
	HCMTRANSFORM GetNativeHandler(HCMTRANSFORM handler) const;
	const CComQIPtr<IContoneProof>& GetProofPtr(HCMTRANSFORM handler) const;

	// FuncionPointers
	CheckColorsPtr m_CheckColorsPtr;
	CheckColorsInGamutPtr m_CheckColorsInGamutPtr;
	CheckRGBsPtr m_CheckRGBsPtr;
	ConvertColorNameToIndexPtr m_ConvertColorNameToIndexPtr;
	ConvertIndexToColorNamePtr m_ConvertIndexToColorNamePtr;
	CreateDeviceLinkProfilePtr m_CreateDeviceLinkProfilePtr;
	CreateMultiProfileTransformPtr m_CreateMultiProfileTransformPtr;
	CreateProfilePtr m_CreateProfilePtr;
	CreateProfileWPtr m_CreateProfileWPtr;
	CreateTransformPtr m_CreateTransformPtr;
	CreateTransformWPtr m_CreateTransformWPtr;
	CreateTransformExtPtr m_CreateTransformExtPtr;
	CreateTransformExtWPtr m_CreateTransformExtWPtr;
	DeleteTransformPtr m_DeleteTransformPtr;
	GetInfoPtr m_GetInfoPtr;
	GetNamedProfileInfoPtr m_GetNamedProfileInfoPtr;
	GetPS2ColorRenderingDictionaryPtr m_GetPS2ColorRenderingDictionaryPtr;
	GetPS2ColorRenderingIntentPtr m_GetPS2ColorRenderingIntentPtr;
	GetPS2ColorSpaceArrayPtr m_GetPS2ColorSpaceArrayPtr;
	IsProfileValidPtr m_IsProfileValidPtr;
	TranslateColorsPtr m_TranslateColorsPtr;
	TranslateRGBPtr m_TranslateRGBPtr;
	TranslateRGBsPtr m_TranslateRGBsPtr;
	TranslateRGBsExtPtr m_TranslateRGBsExtPtr;

	iwin::CWinCriticalSection m_criticalSection;

	CIccToMxProfileMap m_profileMap;

	static DWORD s_gmgIdent;

private:
	iwin::CDllFunctionsProvider m_dllProvider;

	bool m_isInitialized;
	bool m_initializationStatus;

	CComQIPtr<IMxManager> m_mxManagerPtr;
	CComQIPtr<ISetStatus> m_pStatusServer;	///< interface of the status server of the module

	struct HandlerElement
	{
		CComQIPtr<IContoneProof> proofPtr;
		HCMTRANSFORM nativeHandler;
	};

	typedef ::std::map<HCMTRANSFORM, HandlerElement> HandlerMap;

	HandlerMap m_handlerMap;

	I_DWORD m_nextHandle;
};


extern "C"
{


// ICM function declarations

BOOL DLL_EXPORT GMGCheckColors(
    HCMTRANSFORM hcmTransform,  // transform handle
    LPCOLOR lpaInputColors,     // array of COLORs
    DWORD nColors,              // COLOR array size
    COLORTYPE ctInput,          // input color type
    LPBYTE lpaResult            // buffer for results
);

BOOL DLL_EXPORT GMGCheckColorsInGamut(
    HCMTRANSFORM hcmTransform,  // transform handle
    RGBTRIPLE *lpaRGBTriple,    // RGB triple array
    LPBYTE lpaResult,           // buffer for results
    UINT nCount                 // result buffer size
);

BOOL DLL_EXPORT GMGCheckRGBs(
    HCMTRANSFORM hcmTransform,  // transform handle
    LPVOID lpSrcBits,           // source bitmap bits
    BMFORMAT bmInput,           // source bitmap format
    DWORD dwWidth,              // source bitmap width
    DWORD dwHeight,             // source bitmap hight
    DWORD dwStride,             // source bitmap delta
    LPBYTE lpaResult,           // buffer for results
    PBMCALLBACKFN pfnCallback,  // pointer to callback function
    LPARAM ulCallbackData       // caller-defined parameter to callback
);

BOOL DLL_EXPORT GMGConvertColorNameToIndex(
    HPROFILE hProfile,
    PCOLOR_NAME paColorName,
    PDWORD paIndex,
    DWORD dwCount
);

BOOL DLL_EXPORT GMGConvertIndexToColorName(
    HPROFILE hProfile,
    PDWORD paIndex,
    PCOLOR_NAME paColorName,
    DWORD dwCount
);

BOOL DLL_EXPORT GMGCreateDeviceLinkProfile(
    PHPROFILE pahProfiles,    // array of profile handles
    DWORD nProfiles,          // profile handle array size
    PDWORD padwIntents,       // array of rendering intents
    DWORD nIntents,           // intent array size
    DWORD dwFlags,            // transform creation flags
    LPBYTE *lpProfileData     // pointer to pointer to buffer
);

HCMTRANSFORM DLL_EXPORT GMGCreateMultiProfileTransform(
    PHPROFILE pahProfiles,    // array of profile handles
    DWORD nProfiles,          // profile handle array size
    PDWORD padwIntents,       // array of rendering intents
    DWORD nIntents,           // intent array size
    DWORD dwFlags             // transform creation flags
);

BOOL DLL_EXPORT GMGCreateProfile(
    LPLOGCOLORSPACEA lpColorSpace,  // pointer to a logical color space
    LPDEVCHARACTER *lpProfileData   // pointer to pointer to buffer
);

BOOL DLL_EXPORT GMGCreateProfileW(
    LPLOGCOLORSPACEW lpColorSpace,  // pointer to a logical color space
    LPDEVCHARACTER *lpProfileData   // pointer to pointer to buffer
);

HCMTRANSFORM DLL_EXPORT GMGCreateTransform(
    LPLOGCOLORSPACEA lpColorSpace,       // pointer to logical color space
    LPDEVCHARACTER lpDevCharacter,       // profile data
    LPDEVCHARACTER lpTargetDevCharacter  // target profile data
);

HCMTRANSFORM DLL_EXPORT GMGCreateTransformW(
    LPLOGCOLORSPACEW lpColorSpace,       // pointer to logical color space
    LPDEVCHARACTER lpDevCharacter,       // profile data
    LPDEVCHARACTER lpTargetDevCharacter  // target profile data
);

HCMTRANSFORM DLL_EXPORT GMGCreateTransformExt(
    LPLOGCOLORSPACEA lpColorSpace,        // pointer to logical color space
    LPDEVCHARACTER lpDevCharacter,        // profile data
    LPDEVCHARACTER lpTargetDevCharacter,  // target profile data
    DWORD dwFlags                         // creation flags
);

HCMTRANSFORM DLL_EXPORT GMGCreateTransformExtW(
    LPLOGCOLORSPACEW lpColorSpace,        // pointer to logical color space
    LPDEVCHARACTER lpDevCharacter,        // profile data
    LPDEVCHARACTER lpTargetDevCharacter,  // target profile data
    DWORD dwFlags                         // creation flags
);

BOOL DLL_EXPORT GMGDeleteTransform(
    HCMTRANSFORM hcmTransform             // transform handle to be deleted.
);

DWORD DLL_EXPORT GMGGetInfo(
    DWORD dwInfo
);

BOOL DLL_EXPORT GMGGetNamedProfileInfo(
    HPROFILE hProfile,                    // profile handle
    PNAMED_PROFILE_INFO pNamedProfileInfo // pointer to named profile info
);

BOOL DLL_EXPORT GMGGetPS2ColorRenderingDictionary(
    HPROFILE hProfile,
    DWORD dwIntent,
    LPBYTE lpBuffer,
    LPDWORD lpcbSize,
    LPBOOL lpbBinary
);

BOOL DLL_EXPORT GMGGetPS2ColorRenderingIntent(
    HPROFILE hProfile,
    DWORD dwIntent,
    LPBYTE lpBuffer,
    LPDWORD lpcbSize
);

BOOL DLL_EXPORT GMGGetPS2ColorSpaceArray(
    HPROFILE hProfile,
    DWORD dwIntent,
    DWORD dwCSAType,
    LPBYTE lpBuffer,
    LPDWORD lpcbSize,
    LPBOOL lpbBinary
);

BOOL DLL_EXPORT GMGIsProfileValid(
    HPROFILE hProfile,                  // proflle handle
    LPBOOL lpbValid                     // buffer for result.
);

BOOL DLL_EXPORT GMGTranslateColors(
    HCMTRANSFORM hcmTransform,          // transform handle
    LPCOLOR lpaInputColors,             // pointer to input color array
    DWORD nColors,                      // number of color in color array
    COLORTYPE ctInput,                  // input color type
    LPCOLOR lpaOutputColors,            // pointer to output color array
    COLORTYPE ctOutput                  // output color type
);

BOOL DLL_EXPORT GMGTranslateRGB(
    HCMTRANSFORM hcmTransform,
    COLORREF ColorRef,
    LPCOLORREF lpColorRef,
    DWORD dwFlags
);

BOOL DLL_EXPORT GMGTranslateRGBs(
    HCMTRANSFORM hcmTransform,
    LPVOID lpSrcBits,
    BMFORMAT bmInput,
    DWORD dwWidth,
    DWORD dwHeight,
    DWORD dwStride,
    LPVOID lpDestBits,
    BMFORMAT bmOutput,
    DWORD dwTranslateDirection
);

BOOL DLL_EXPORT GMGTranslateRGBsExt(
    HCMTRANSFORM hcmTransform,
    LPVOID lpSrcBits,
    BMFORMAT bmInput,
    DWORD dwWidth,
    DWORD dwHeight,
    DWORD dwInputStride,
    LPVOID lpDestBits,
    BMFORMAT bmOutput,
    DWORD dwOutputStride,
    LPBMCALLBACKFN lpfnCallback,
    LPARAM ulCallbackData
);


}


