// IcmDll.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"

#include "IcmDll.h"

#include "cp_mx_i.c"
#include "icc34.h"

#include "IccHelper.h"

// STL Includes
#include <vector>

// ACF Includes
#include "isys/CSectionBlocker.h"
#include "iser/CXmlFileReadArchive.h"
#include "iser/CXmlFileWriteArchive.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CStdCmmCaller, CWinApp)
END_MESSAGE_MAP()


// CStdCmmCaller construction

CStdCmmCaller::CStdCmmCaller()
:	m_dllProvider("C:\\Windows\\System32\\icm32.dll"),
	m_nextHandle(0x200),
	m_isInitialized(false)
{
	m_CheckColorsPtr = NULL;
	m_CheckColorsInGamutPtr = NULL;
	m_CheckRGBsPtr = NULL;
	m_ConvertColorNameToIndexPtr = NULL;
	m_ConvertIndexToColorNamePtr = NULL;
	m_CreateDeviceLinkProfilePtr = NULL;
	m_CreateMultiProfileTransformPtr = NULL;
	m_CreateProfilePtr = NULL;
	m_CreateProfileWPtr = NULL;
	m_CreateTransformPtr = NULL;
	m_CreateTransformWPtr = NULL;
	m_CreateTransformExtPtr = NULL;
	m_CreateTransformExtWPtr = NULL;
	m_DeleteTransformPtr = NULL;
	m_GetInfoPtr = NULL;
	m_GetNamedProfileInfoPtr = NULL;
	m_GetPS2ColorRenderingDictionaryPtr = NULL;
	m_GetPS2ColorRenderingIntentPtr = NULL;
	m_GetPS2ColorSpaceArrayPtr = NULL;
	m_IsProfileValidPtr = NULL;
	m_TranslateColorsPtr = NULL;
	m_TranslateRGBPtr = NULL;
	m_TranslateRGBsPtr = NULL;
	m_TranslateRGBsExtPtr = NULL;
}


CStdCmmCaller::~CStdCmmCaller()
{
	m_handlerMap.clear();

	m_mxManagerPtr.Release();

	::CoUninitialize();
}


// The one and only CStdCmmCaller object

CStdCmmCaller caller;


// CStdCmmCaller initialization

BOOL CStdCmmCaller::InitInstance()
{
	CWinApp::InitInstance();

	if (m_dllProvider.IsValid()){
		m_CheckColorsPtr = (CheckColorsPtr)m_dllProvider.GetFunction("CMCheckColors");
		m_CheckColorsInGamutPtr = (CheckColorsInGamutPtr)m_dllProvider.GetFunction("CMCheckColorsInGamut");
		m_CheckRGBsPtr = (CheckRGBsPtr)m_dllProvider.GetFunction("CMCheckRGBs");
		m_ConvertColorNameToIndexPtr = (ConvertColorNameToIndexPtr)m_dllProvider.GetFunction("CMConvertColorNameToIndex");
		m_ConvertIndexToColorNamePtr = (ConvertIndexToColorNamePtr)m_dllProvider.GetFunction("CMConvertIndexToColorName");
		m_CreateDeviceLinkProfilePtr = (CreateDeviceLinkProfilePtr)m_dllProvider.GetFunction("CMCreateDeviceLinkProfile");
		m_CreateMultiProfileTransformPtr = (CreateMultiProfileTransformPtr)m_dllProvider.GetFunction("CMCreateMultiProfileTransform");
		m_CreateProfilePtr = (CreateProfilePtr)m_dllProvider.GetFunction("CMCreateProfile");
		m_CreateProfileWPtr = (CreateProfileWPtr)m_dllProvider.GetFunction("CMCreateProfileW");
		m_CreateTransformPtr = (CreateTransformPtr)m_dllProvider.GetFunction("CMCreateTransform");
		m_CreateTransformWPtr = (CreateTransformWPtr)m_dllProvider.GetFunction("CMCreateTransformW");
		m_CreateTransformExtPtr = (CreateTransformExtPtr)m_dllProvider.GetFunction("CMCreateTransformExt");
		m_CreateTransformExtWPtr = (CreateTransformExtWPtr)m_dllProvider.GetFunction("CMCreateTransformExtW");
		m_DeleteTransformPtr = (DeleteTransformPtr)m_dllProvider.GetFunction("CMDeleteTransform");
		m_GetInfoPtr = (GetInfoPtr)m_dllProvider.GetFunction("CMGetInfo");
		m_GetNamedProfileInfoPtr = (GetNamedProfileInfoPtr)m_dllProvider.GetFunction("CMGetNamedProfileInfo");
		m_GetPS2ColorRenderingDictionaryPtr = (GetPS2ColorRenderingDictionaryPtr)m_dllProvider.GetFunction("CMGetPS2ColorRenderingDictionary");
		m_GetPS2ColorRenderingIntentPtr = (GetPS2ColorRenderingIntentPtr)m_dllProvider.GetFunction("CMGetPS2ColorRenderingIntent");
		m_GetPS2ColorSpaceArrayPtr = (GetPS2ColorSpaceArrayPtr)m_dllProvider.GetFunction("CMGetPS2ColorSpaceArray");
		m_IsProfileValidPtr = (IsProfileValidPtr)m_dllProvider.GetFunction("CMIsProfileValid");
		m_TranslateColorsPtr = (TranslateColorsPtr)m_dllProvider.GetFunction("CMTranslateColors");
		m_TranslateRGBPtr = (TranslateRGBPtr)m_dllProvider.GetFunction("CMTranslateRGB");
		m_TranslateRGBsPtr = (TranslateRGBsPtr)m_dllProvider.GetFunction("CMTranslateRGBs");
		m_TranslateRGBsExtPtr = (TranslateRGBsExtPtr)m_dllProvider.GetFunction("CMTranslateRGBsExt");
	}

	iser::CXmlFileReadArchive readArchive("C:\\Work\\Projects\\GMG\\Other\\MxMapList.xml", false);
	bool isSerialized = m_profileMap.Serialize(readArchive);

	return TRUE;
}


bool CStdCmmCaller::EnsureProofInitialized()
{
	if (!m_isInitialized){
		m_initializationStatus = !FAILED(::CoInitialize(NULL));
		m_initializationStatus = m_initializationStatus && !FAILED(m_mxManagerPtr.CoCreateInstance(CLSID_MxManager));

		m_isInitialized = true;
	}

	return m_initializationStatus;
}


HCMTRANSFORM CStdCmmCaller::AddNativeHandler(HCMTRANSFORM nativeHandler)
{
	if (nativeHandler > (HCMTRANSFORM)0x100){
		HCMTRANSFORM newHandler((HCMTRANSFORM)m_nextHandle++);

		HandlerElement& newElement = m_handlerMap[newHandler];

		newElement.nativeHandler = nativeHandler;

		return newHandler;
	}

	return 0;
}


HCMTRANSFORM CStdCmmCaller::AddProofHandler(const ::std::string& profileName, HCMTRANSFORM nativeHandler)
{
	if (EnsureProofInitialized()){
		CComBSTR profilName(profileName.c_str());

		CComPtr<IUnknown>     pIUnknown;

		HRESULT hr = m_mxManagerPtr->Init1(profilName, NULL, CLSID_ContoneProofProfile, &pIUnknown, NULL);
		if (!FAILED(hr) && (pIUnknown != NULL)){
			HCMTRANSFORM newHandler((HCMTRANSFORM)m_nextHandle++);

			HandlerElement& newElement = m_handlerMap[newHandler];

			newElement.nativeHandler = nativeHandler;
			newElement.proofPtr = pIUnknown;

			return newHandler;
		}
	}

	return 0;
}


bool CStdCmmCaller::RemoveHandler(HCMTRANSFORM handler)
{
	return m_handlerMap.erase(handler) > 0;
}


bool CStdCmmCaller::IsNativeHandler(HCMTRANSFORM handler) const
{
	return GetNativeHandler(handler) != 0;
}


bool CStdCmmCaller::IsProofHandler(HCMTRANSFORM handler) const
{
	return GetProofPtr(handler) != NULL;
}


HCMTRANSFORM CStdCmmCaller::GetNativeHandler(HCMTRANSFORM handler) const
{
	HandlerMap::const_iterator iter = m_handlerMap.find(handler);

	if (iter != m_handlerMap.end()){
		const HandlerElement& element = iter->second;

		return element.nativeHandler;
	}

	return 0;
}


const CComQIPtr<IContoneProof>& CStdCmmCaller::GetProofPtr(HCMTRANSFORM handler) const
{
	HandlerMap::const_iterator iter = m_handlerMap.find(handler);

	if (iter != m_handlerMap.end()){
		const HandlerElement& element = iter->second;

		return element.proofPtr;
	}

	static CComQIPtr<IContoneProof> nullPtr;

	return nullPtr;
}


// static attributes

DWORD CStdCmmCaller::s_gmgIdent(*(DWORD*)" GMG");

extern "C"
{


// ICM implementations

BOOL DLL_EXPORT GMGCheckColors(
			HCMTRANSFORM hcmTransform,
			LPCOLOR lpaInputColors,
			DWORD nColors,
			COLORTYPE ctInput,
			LPBYTE lpaResult)
{
	isys::CSectionBlocker blocker(&caller.m_criticalSection);

	if (caller.m_CheckColorsPtr != NULL){
		return caller.m_CheckColorsPtr(hcmTransform, lpaInputColors, nColors, ctInput, lpaResult);
	}

	return FALSE;
}


BOOL DLL_EXPORT GMGCheckColorsInGamut(
			HCMTRANSFORM hcmTransform,
			RGBTRIPLE *lpaRGBTriple,
			LPBYTE lpaResult,
			UINT nCount)
{
	isys::CSectionBlocker blocker(&caller.m_criticalSection);

	if (caller.m_CheckColorsInGamutPtr != NULL){
		return caller.m_CheckColorsInGamutPtr(hcmTransform, lpaRGBTriple, lpaResult, nCount);
	}

	return FALSE;
}


BOOL DLL_EXPORT GMGCheckRGBs(
			HCMTRANSFORM hcmTransform,
			LPVOID lpSrcBits,
			BMFORMAT bmInput,
			DWORD dwWidth,
			DWORD dwHeight,
			DWORD dwStride,
			LPBYTE lpaResult,
			PBMCALLBACKFN pfnCallback,
			LPARAM ulCallbackData)
{
	isys::CSectionBlocker blocker(&caller.m_criticalSection);

	if (caller.m_CheckRGBsPtr != NULL){
		return caller.m_CheckRGBsPtr(hcmTransform, lpSrcBits, bmInput, dwWidth, dwHeight, dwStride, lpaResult, pfnCallback, ulCallbackData);
	}

	return FALSE;
}


BOOL DLL_EXPORT GMGConvertColorNameToIndex(
			HPROFILE hProfile,
			PCOLOR_NAME paColorName,
			PDWORD paIndex,
			DWORD dwCount)
{
	isys::CSectionBlocker blocker(&caller.m_criticalSection);

	if (caller.m_ConvertColorNameToIndexPtr != NULL){
		return caller.m_ConvertColorNameToIndexPtr(hProfile, paColorName, paIndex, dwCount);
	}

	return FALSE;
}


BOOL DLL_EXPORT GMGConvertIndexToColorName(
			HPROFILE hProfile,
			PDWORD paIndex,
			PCOLOR_NAME paColorName,
			DWORD dwCount)
{
	isys::CSectionBlocker blocker(&caller.m_criticalSection);

	if (caller.m_ConvertIndexToColorNamePtr != NULL){
		return caller.m_ConvertIndexToColorNamePtr(hProfile, paIndex, paColorName, dwCount);
	}

	return FALSE;
}


BOOL DLL_EXPORT GMGCreateDeviceLinkProfile(
			PHPROFILE pahProfiles,
			DWORD nProfiles,
			PDWORD padwIntents,
			DWORD nIntents,
			DWORD dwFlags,
			LPBYTE *lpProfileData)
{
	isys::CSectionBlocker blocker(&caller.m_criticalSection);

	if (caller.m_CreateDeviceLinkProfilePtr != NULL){
		return caller.m_CreateDeviceLinkProfilePtr(pahProfiles, nProfiles, padwIntents, nIntents, dwFlags, lpProfileData);
	}

	return FALSE;
}


HCMTRANSFORM DLL_EXPORT GMGCreateMultiProfileTransform(
			PHPROFILE pahProfiles,
			DWORD nProfiles,
			PDWORD padwIntents,
			DWORD nIntents,
			DWORD dwFlags)
{
	::std::vector<::std::string> profileIds;

	for (DWORD profileIndex = 0; profileIndex < nProfiles; ++profileIndex){
		HPROFILE handler = pahProfiles[profileIndex];

		DWORD profileSize = 0;
		::GetColorProfileFromHandle(handler, NULL, &profileSize);

		if (profileSize > 0){
			::std::vector<BYTE> buffer(profileSize);
			if (GetColorProfileFromHandle(handler, &buffer[0], &profileSize)){
				const icHeader* headerPtr = (const icHeader*)&buffer[0];

				if (headerPtr->colorSpace == 0x4B594D43){
					__iccInfo info;
					CIccHelper helper;
					if (helper.GetInfo(&buffer[0], profileSize, info)){
						profileIds.push_back((const char*)info.desc);
					}
				}
			}
		}
	}

	isys::CSectionBlocker blocker(&caller.m_criticalSection);

	HCMTRANSFORM nativeHandler = 0;

	if (		(profileIds.size() != nProfiles) &&
				(caller.m_CreateMultiProfileTransformPtr != NULL)){
		nativeHandler = caller.m_CreateMultiProfileTransformPtr(pahProfiles, nProfiles, padwIntents, nIntents, dwFlags);
	}

	if (profileIds.size() >= 2){
		const ::std::string& mxProfile = caller.m_profileMap.FindMxProfile(profileIds[0], profileIds[1]);

		if (!mxProfile.empty()){
			return caller.AddProofHandler(mxProfile, nativeHandler);
		}
	}

	if (nativeHandler != 0){
		return caller.AddNativeHandler(nativeHandler);
	}

	SetLastError(ERROR_INVALID_FUNCTION);

	return HCMTRANSFORM(1);
}


BOOL DLL_EXPORT GMGCreateProfile(
			LPLOGCOLORSPACEA lpColorSpace,
			LPDEVCHARACTER *lpProfileData)
{
	isys::CSectionBlocker blocker(&caller.m_criticalSection);

	if (caller.m_CreateProfilePtr != NULL){
		return caller.m_CreateProfilePtr(lpColorSpace, lpProfileData);
	}

	return FALSE;
}


BOOL DLL_EXPORT GMGCreateProfileW(
			LPLOGCOLORSPACEW lpColorSpace,
			LPDEVCHARACTER *lpProfileData)
{
	isys::CSectionBlocker blocker(&caller.m_criticalSection);

	if (caller.m_CreateProfileWPtr != NULL){
		return caller.m_CreateProfileWPtr(lpColorSpace, lpProfileData);
	}

	return FALSE;
}


HCMTRANSFORM DLL_EXPORT GMGCreateTransform(
			LPLOGCOLORSPACEA lpColorSpace,
			LPDEVCHARACTER lpDevCharacter,
			LPDEVCHARACTER lpTargetDevCharacter)
{
	isys::CSectionBlocker blocker(&caller.m_criticalSection);

	if (caller.m_CreateTransformPtr != NULL){
		return caller.m_CreateTransformPtr(lpColorSpace, lpDevCharacter, lpTargetDevCharacter);
	}

	SetLastError(ERROR_INVALID_FUNCTION);

	return HCMTRANSFORM(1);
}


HCMTRANSFORM DLL_EXPORT GMGCreateTransformW(
			LPLOGCOLORSPACEW lpColorSpace,
			LPDEVCHARACTER lpDevCharacter,
			LPDEVCHARACTER lpTargetDevCharacter)
{
	isys::CSectionBlocker blocker(&caller.m_criticalSection);

	if (caller.m_CreateTransformWPtr != NULL){
		return caller.m_CreateTransformWPtr(lpColorSpace, lpDevCharacter, lpTargetDevCharacter);
	}

	SetLastError(ERROR_INVALID_FUNCTION);

	return HCMTRANSFORM(1);
}


HCMTRANSFORM DLL_EXPORT GMGCreateTransformExt(
			LPLOGCOLORSPACEA lpColorSpace,
			LPDEVCHARACTER lpDevCharacter,
			LPDEVCHARACTER lpTargetDevCharacter,
			DWORD dwFlags)
{
	isys::CSectionBlocker blocker(&caller.m_criticalSection);

	if (caller.m_CreateTransformExtPtr != NULL){
		return caller.m_CreateTransformExtPtr(lpColorSpace, lpDevCharacter, lpTargetDevCharacter, dwFlags);
	}

	SetLastError(ERROR_INVALID_FUNCTION);

	return HCMTRANSFORM(1);
}


HCMTRANSFORM DLL_EXPORT GMGCreateTransformExtW(
			LPLOGCOLORSPACEW lpColorSpace,
			LPDEVCHARACTER lpDevCharacter,
			LPDEVCHARACTER lpTargetDevCharacter,
			DWORD dwFlags)
{
	isys::CSectionBlocker blocker(&caller.m_criticalSection);

	if (caller.m_CreateTransformExtWPtr != NULL){
		return caller.m_CreateTransformExtWPtr(lpColorSpace, lpDevCharacter, lpTargetDevCharacter, dwFlags);
	}

	SetLastError(ERROR_INVALID_FUNCTION);

	return HCMTRANSFORM(1);
}


BOOL DLL_EXPORT GMGDeleteTransform(HCMTRANSFORM hcmTransform)
{
	isys::CSectionBlocker blocker(&caller.m_criticalSection);

	BOOL retVal = FALSE;

	if (caller.IsNativeHandler(hcmTransform) && caller.m_DeleteTransformPtr != NULL){
		HCMTRANSFORM nativeHandler = caller.GetNativeHandler(hcmTransform);

		retVal = caller.m_DeleteTransformPtr(nativeHandler);
	}

	caller.RemoveHandler(hcmTransform);

	return retVal;
}


DWORD DLL_EXPORT GMGGetInfo(DWORD dwInfo)
{
	isys::CSectionBlocker blocker(&caller.m_criticalSection);

	if (caller.m_GetInfoPtr != NULL){
		switch (dwInfo){
		case CMM_IDENT:
			return CStdCmmCaller::s_gmgIdent;

		case CMM_DESCRIPTION:
			return IDS_STRING1000;
		}

		DWORD retVal = caller.m_GetInfoPtr(dwInfo);

		return retVal;
	}

	return 0;
}


BOOL DLL_EXPORT GMGGetNamedProfileInfo(HPROFILE hProfile, PNAMED_PROFILE_INFO pNamedProfileInfo)
{
	isys::CSectionBlocker blocker(&caller.m_criticalSection);

	if (caller.m_GetNamedProfileInfoPtr != NULL){
		return caller.m_GetNamedProfileInfoPtr(hProfile, pNamedProfileInfo);
	}

	return FALSE;
}


BOOL DLL_EXPORT GMGGetPS2ColorRenderingDictionary(
			HPROFILE hProfile,
			DWORD dwIntent,
			LPBYTE lpBuffer,
			LPDWORD lpcbSize,
			LPBOOL lpbBinary)
{
	isys::CSectionBlocker blocker(&caller.m_criticalSection);

	if (caller.m_GetPS2ColorRenderingDictionaryPtr != NULL){
		return caller.m_GetPS2ColorRenderingDictionaryPtr(hProfile, dwIntent, lpBuffer, lpcbSize, lpbBinary);
	}

	return FALSE;
}


BOOL DLL_EXPORT GMGGetPS2ColorRenderingIntent(
			HPROFILE hProfile,
			DWORD dwIntent,
			LPBYTE lpBuffer,
			LPDWORD lpcbSize)
{
	isys::CSectionBlocker blocker(&caller.m_criticalSection);

	if (caller.m_GetPS2ColorRenderingIntentPtr != NULL){
		return caller.m_GetPS2ColorRenderingIntentPtr(hProfile, dwIntent, lpBuffer, lpcbSize);
	}

	return FALSE;
}


BOOL DLL_EXPORT GMGGetPS2ColorSpaceArray(
			HPROFILE hProfile,
			DWORD dwIntent,
			DWORD dwCSAType,
			LPBYTE lpBuffer,
			LPDWORD lpcbSize,
			LPBOOL lpbBinary)
{
	isys::CSectionBlocker blocker(&caller.m_criticalSection);

	if (caller.m_GetPS2ColorSpaceArrayPtr != NULL){
		return caller.m_GetPS2ColorSpaceArrayPtr(hProfile, dwIntent, dwCSAType, lpBuffer, lpcbSize, lpbBinary);
	}

	return FALSE;
}


BOOL DLL_EXPORT GMGIsProfileValid(HPROFILE hProfile, LPBOOL lpbValid)
{
	isys::CSectionBlocker blocker(&caller.m_criticalSection);

	if (caller.m_IsProfileValidPtr != NULL){
		return caller.m_IsProfileValidPtr(hProfile, lpbValid);
	}

	return FALSE;
}


BOOL DLL_EXPORT GMGTranslateColors(
			HCMTRANSFORM hcmTransform,
			LPCOLOR lpaInputColors,
			DWORD nColors,
			COLORTYPE ctInput,
			LPCOLOR lpaOutputColors,
			COLORTYPE ctOutput)
{
	isys::CSectionBlocker blocker(&caller.m_criticalSection);

	if (		caller.IsProofHandler(hcmTransform) &&
				(ctInput == COLOR_CMYK) &&
				(ctOutput == COLOR_CMYK)){
		const CComQIPtr<IContoneProof>& proofPtr = caller.GetProofPtr(hcmTransform);
		I_ASSERT(proofPtr != NULL);	// if this handler is proof handler, it was correctly initialized -> proof pointer cannot be NULL

		::std::vector<I_BYTE> inputBuffer(nColors * 4);

		for (DWORD inputIndex = 0; inputIndex < nColors; ++inputIndex){
			const CMYKCOLOR& inputColor = lpaInputColors[inputIndex].cmyk;
//			inputBuffer[inputIndex * 4 + 0] = I_BYTE(inputColor.cyan >> 8);
//			inputBuffer[inputIndex * 4 + 1] = I_BYTE(inputColor.magenta >> 8);
//			inputBuffer[inputIndex * 4 + 2] = I_BYTE(inputColor.yellow >> 8);
//			inputBuffer[inputIndex * 4 + 3] = I_BYTE(inputColor.black >> 8);
			inputBuffer[inputIndex * 4 + 0] = I_BYTE(inputColor.cyan >> 8);
			inputBuffer[inputIndex * 4 + 1] = I_BYTE(inputColor.cyan >> 8);
			inputBuffer[inputIndex * 4 + 2] = 0;
			inputBuffer[inputIndex * 4 + 3] = 0;
		}

		I_ASSERT(sizeof(COLOR) == 4 * sizeof(I_WORD));

		if (!FAILED(proofPtr->Profile(nColors, &inputBuffer[0], (I_WORD*)lpaOutputColors))){
			return TRUE;
		}
	}

	if (caller.IsNativeHandler(hcmTransform) && caller.m_TranslateColorsPtr != NULL){
		HCMTRANSFORM nativeHandler = caller.GetNativeHandler(hcmTransform);

		return caller.m_TranslateColorsPtr(nativeHandler, lpaInputColors, nColors, ctInput, lpaOutputColors, ctOutput);
	}

	return FALSE;
}


BOOL DLL_EXPORT GMGTranslateRGB(
			HCMTRANSFORM hcmTransform,
			COLORREF ColorRef,
			LPCOLORREF lpColorRef,
			DWORD dwFlags)
{
	isys::CSectionBlocker blocker(&caller.m_criticalSection);

	if (caller.m_TranslateRGBPtr != NULL){
		return caller.m_TranslateRGBPtr(hcmTransform, ColorRef, lpColorRef, dwFlags);
	}

	return FALSE;
}


BOOL DLL_EXPORT GMGTranslateRGBs(
			HCMTRANSFORM hcmTransform,
			LPVOID lpSrcBits,
			BMFORMAT bmInput,
			DWORD dwWidth,
			DWORD dwHeight,
			DWORD dwStride,
			LPVOID lpDestBits,
			BMFORMAT bmOutput,
			DWORD dwTranslateDirection)
{
	isys::CSectionBlocker blocker(&caller.m_criticalSection);

	if (caller.m_TranslateRGBsPtr != NULL){
		return caller.m_TranslateRGBsPtr(
					hcmTransform,
					lpSrcBits,
					bmInput,
					dwWidth,
					dwHeight,
					dwStride,
					lpDestBits,
					bmOutput,
					dwTranslateDirection);
	}

	return FALSE;
}


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
			LPARAM ulCallbackData)
{
	isys::CSectionBlocker blocker(&caller.m_criticalSection);

	if (caller.m_TranslateRGBsExtPtr != NULL){
		return caller.m_TranslateRGBsExtPtr(
					hcmTransform,
					lpSrcBits,
					bmInput,
					dwWidth,
					dwHeight,
					dwInputStride,
					lpDestBits,
					bmOutput,
					dwOutputStride,
					lpfnCallback,
					ulCallbackData);
	}

	return FALSE;
}


}


