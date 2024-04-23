#include "pch.h"
#include "DX12.h"
#include "DX12_inc.h"
//#include <dxgi1_7.h>

#include "Window.h"

constexpr D3D_FEATURE_LEVEL MIN_FEATURE_LEVEL{ D3D_FEATURE_LEVEL_11_0 };

template<typename T>
constexpr void Release(T*& aPtr)
{
	if (aPtr)
	{
		aPtr->Release();
		aPtr = nullptr;
	}
}

DX12::DX12()
{
}

DX12::~DX12()
{
}

bool DX12::Init(Window* aWindow)
{
	if (device)
	{
		Shutdown();
		return true;
	}
	aWindow;

	uint32_t dxgiFactoryFlags = 0;
#ifdef _DEBUG
	{
		ComPtr<ID3D12Debug3> debugController;
		DXCall(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
		debugController->EnableDebugLayer();
		dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
	}
#endif

	HRESULT hr{ S_OK };

	DXCall(hr = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory))); // __uuidof(IDXGIFactory7), (void**)&pFactory
	if (FAILED(hr))
	{
		return false;
	}




	// Choose best performing GPU as adapter
	ComPtr<IDXGIAdapter4> pAdapter{ nullptr };

	{
		IDXGIAdapter4* pAdapterTemp{ nullptr };

		// this could run forever:( surely the user doesn't have more than 8 monitors

		uint32_t j = 0;
		for (
			uint32_t i = 0;
			i < (unsigned int)(factory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&pAdapterTemp)) != DXGI_ERROR_NOT_FOUND);
			++i
			)
		{
			if (SUCCEEDED(D3D12CreateDevice(pAdapterTemp, MIN_FEATURE_LEVEL, __uuidof(ID3D12Device), nullptr)))
			{
				break;
			}

			Release(pAdapterTemp);

			if (j++ > 8)
			{
				break;
			}
		}

		pAdapter.Attach(pAdapterTemp);
	}

	{
		D3D_FEATURE_LEVEL maxFeatureLevel{ D3D_FEATURE_LEVEL_11_0 };

		{
			constexpr D3D_FEATURE_LEVEL featureLevels[]
			{
				D3D_FEATURE_LEVEL_12_1,
				D3D_FEATURE_LEVEL_12_0,
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0
			};

			D3D12_FEATURE_DATA_FEATURE_LEVELS featureLevelsData{};
			featureLevelsData.NumFeatureLevels = _countof(featureLevels);
			featureLevelsData.pFeatureLevelsRequested = featureLevels;

			ComPtr<ID3D12Device> pDevice{ nullptr };
			DXCall(D3D12CreateDevice(pAdapter.Get(), MIN_FEATURE_LEVEL, IID_PPV_ARGS(&pDevice)));
			DXCall(pDevice->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &featureLevelsData, sizeof(featureLevelsData)));
			maxFeatureLevel = featureLevelsData.MaxSupportedFeatureLevel;
		}

		assert(maxFeatureLevel >= MIN_FEATURE_LEVEL);
		if (maxFeatureLevel < MIN_FEATURE_LEVEL)
		{
			return false;
		}

		DXCall(hr = D3D12CreateDevice(pAdapter.Get(), maxFeatureLevel, IID_PPV_ARGS(&device)));
		if (FAILED(hr))
		{
			return false;
		}

		device->SetName(L"MAIN DEVICE");
	}


#ifdef _DEBUG
	{
		ComPtr<ID3D12InfoQueue> pInfoQueue;
		DXCall(device->QueryInterface(IID_PPV_ARGS(&pInfoQueue)));

		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
		pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);

		//D3D12_MESSAGE_SEVERITY severities[] =
		//{
		//	D3D12_MESSAGE_SEVERITY_INFO
		//};

		//D3D12_MESSAGE_ID denyIds[] =
		//{
		//	D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,
		//	D3D12_MESSAGE_ID_CLEARDEPTHSTENCILVIEW_MISMATCHINGCLEARVALUE,
		//	D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,
		//	D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE
		//};

		//D3D12_INFO_QUEUE_FILTER newFilter{};
		//newFilter.DenyList.NumSeverities = _countof(severities);
		//newFilter.DenyList.pSeverityList = severities;
		//newFilter.DenyList.NumIDs = _countof(denyIds);
		//newFilter.DenyList.pIDList = denyIds;

		//pInfoQueue->PushStorageFilter(&newFilter);
	}
#endif

	//const WindowData& windowData = aWindow->GetWindowData();

	//// Create device
	//DXCall(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_2, IID_PPV_ARGS(&device))); // D3D_FEATURE_LEVEL_12_2 
	//
	//// Create swap chain
	//DXGI_SWAP_CHAIN_DESC1 scd = {};
	//scd.BufferCount = 2;
	//scd.Width = windowData.viewPortWidth;
	//scd.Height = windowData.viewPortHeight;
	//scd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	//scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	//scd.SampleDesc.Count = 1;
	//DXCall(pFactory->CreateSwapChainForHwnd(device, aWindow->GetHWND(), &scd, nullptr, nullptr, &swapChain));

	//// Create command queue
	//D3D12_COMMAND_QUEUE_DESC cqDesc = {};
	//cqDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	//cqDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	//DXCall(device->CreateCommandQueue(&cqDesc, IID_PPV_ARGS(&commandQueue)));

	//// Create command allocator
	//DXCall(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator)));

	//// Create command list
	//DXCall(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, nullptr, IID_PPV_ARGS(&commandList)));

	//// Create synchronization objects
	//UINT64 fenceValue = 0;
	//DXCall(device->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));

	return true;
}

void DX12::Shutdown()
{
	Release(factory);
	Release(device);

#ifdef _DEBUG
	{
		{
			ComPtr<ID3D12InfoQueue> pInfoQueue;
			DXCall(device->QueryInterface(IID_PPV_ARGS(&pInfoQueue)));

			pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, FALSE);
			pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, FALSE);
			pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, FALSE);
		}

		ComPtr<ID3D12DebugDevice2> debugDevice;
		DXCall(device->QueryInterface(IID_PPV_ARGS(&debugDevice)));
		Release(device);
		DXCall(debugDevice->ReportLiveDeviceObjects(
			D3D12_RLDO_SUMMARY | D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL
		));
	}
#endif
}
