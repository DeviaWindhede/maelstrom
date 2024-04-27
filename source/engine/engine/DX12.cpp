#include "pch.h"
#include "DX12.h"
#include "DX12_inc.h"
//#include <dxgi1_7.h>

#include "Window.h"

constexpr D3D_FEATURE_LEVEL MIN_FEATURE_LEVEL{ D3D_FEATURE_LEVEL_11_0 };


DX12::DX12()
{
}

DX12::~DX12()
{
	Shutdown();
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

		// this could run forever:( surely the user doesn't have more than 8 gpus

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

			::d3d12_common::Release(pAdapterTemp);

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
		NAME_D3D12_OBJECT(
			device,
			L"Main Device"
		);

		device->SetName(L"MAIN DEVICE");
	}

	new (&command) DX12Command(device, D3D12_COMMAND_LIST_TYPE_DIRECT);
	if (!command.GetCommandQueue())
	{
		return false;
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

	return true;
}

void DX12::Render()
{
	BeginFrame();
	{
		ID3D12GraphicsCommandList6* commandList{ command.GetCommandList() };
		commandList;
	}
	EndFrame();
}

bool DX12::BeginFrame()
{
	return command.BeginFrame();
}

void DX12::EndFrame()
{
	command.EndFrame();
}

void DX12::Shutdown()
{
	command.Release();
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
		::d3d12_common::Release(device);
		DXCall(debugDevice->ReportLiveDeviceObjects(
			D3D12_RLDO_SUMMARY | D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL
		));
	}
#endif

	::d3d12_common::Release(factory);
	::d3d12_common::Release(device);
}
