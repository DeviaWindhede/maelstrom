#pragma once

#include <dxgi1_6.h>
#include <d3d12.h>
#include <wrl.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")

#include <DirectXMath.h>

using namespace Microsoft::WRL;

namespace d3d12_common
{
	template<typename T>
	constexpr void Release(T*& aPtr)
	{
		if (aPtr)
		{
			aPtr->Release();
			aPtr = nullptr;
		}
	}
}

#ifdef _DEBUG

#ifndef DXCall
#define DXCall(x)										\
{														\
	if(FAILED(x))										\
	{													\
		char lineNumber[32];							\
		sprintf_s(lineNumber, "%d", __LINE__);			\
		OutputDebugString(L"DXCall failed in file: ");	\
		OutputDebugString(__FILEW__);					\
		OutputDebugString(L" at line: ");				\
		OutputDebugStringA(lineNumber);					\
		OutputDebugString(L"\n");						\
		__debugbreak();									\
	}													\
}
#endif

#else

#ifndef DXCall
#define DXCall(x) x
#endif

#endif

#ifdef _DEBUG
#define NAME_D3D12_OBJECT(aPtr, aName) aPtr->SetName(aName); OutputDebugString(L"|| Created D3D12 object: "); OutputDebugString(aName); OutputDebugString(L"\n");

#define NAME_D3D12_OBJECT_INDEXED(aPtr, aIndex, aName)				\
{																	\
	wchar_t name[128];												\
	if (swprintf_s(name, L"%s[%u]", aName, aIndex)) {				\
		aPtr->SetName(name);										\
		OutputDebugString(L"|| Created D3D12 object: ");			\
		OutputDebugString(name);									\
		OutputDebugString(L"\n");									\
	}																\
}
#else
#define NAME_D3D12_OBJECT(aPtr, aName)
#define NAME_D3D12_OBJECT_INDEXED(aPtr, aName, aIndex)
#endif
