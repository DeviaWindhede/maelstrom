#pragma once

#include <dxgi1_6.h>
#include <d3d12.h>
#include <wrl.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")

#include <DirectXMath.h>

using namespace Microsoft::WRL;

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
