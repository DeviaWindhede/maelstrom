#pragma once
#include "DX12Command.h"

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

	DX12Command		command;
	ID3D12Device*	device{ nullptr };
	IDXGIFactory7*	factory{ nullptr };

	bool BeginFrame();
	void EndFrame();
	void Render();

	//IDXGISwapChain1* swapChain;
	//ID3D12CommandQueue* commandQueue;
	//ID3D12CommandAllocator* commandAllocator;
	//ID3D12GraphicsCommandList* commandList;
	//ID3D12Fence* fence;
private:
	void Shutdown();
};

