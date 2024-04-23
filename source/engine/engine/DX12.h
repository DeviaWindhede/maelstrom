#pragma once

class Window;
struct ID3D12Device;
struct IDXGIFactory7;

class DX12
{
public:
	DX12();
	~DX12();
	DX12(const DX12&) = delete;

	bool Init(Window* aWindow);
	void BeginFrame();
	void EndFrame();

	ID3D12Device* device;
	IDXGIFactory7* factory;


	//IDXGISwapChain1* swapChain;
	//ID3D12CommandQueue* commandQueue;
	//ID3D12CommandAllocator* commandAllocator;
	//ID3D12GraphicsCommandList* commandList;
	//ID3D12Fence* fence;
private:
	void Shutdown();
};

