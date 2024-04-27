#pragma once

struct ID3D12Device;
struct ID3D12Fence1;
struct ID3D12CommandQueue;
struct ID3D12GraphicsCommandList6;
struct ID3D12CommandAllocator;

enum D3D12_COMMAND_LIST_TYPE : int;

constexpr uint32_t FRAME_BUFFER_COUNT{ 3 };

class DX12Command
{
public:
	DX12Command() = default;
	
	explicit DX12Command(const DX12Command&) = delete;
	DX12Command& operator=(const DX12Command&) = delete;

	explicit DX12Command(DX12Command&&) = delete;
	DX12Command& operator=(DX12Command&&) = delete;
	

	explicit DX12Command(ID3D12Device* const aDevice, D3D12_COMMAND_LIST_TYPE aType);
	~DX12Command();

	bool BeginFrame();
	void EndFrame();
	void Release();

	ID3D12CommandQueue* const GetCommandQueue() const { return commandQueue; }
	ID3D12GraphicsCommandList6* const GetCommandList() const { return commandList; }
	uint64_t GetFrameIndex() const { return frameIndex; }
private:
	struct CommandFrame
	{
		ID3D12CommandAllocator* allocator{ nullptr };
		uint64_t				fenceValue{ 0 };

		void Wait(HANDLE aFenceEvent, ID3D12Fence1* const aFence);
		void Release();
	};

	ID3D12CommandQueue*			commandQueue{ nullptr };
	ID3D12GraphicsCommandList6*	commandList{ nullptr };
	CommandFrame				commandFrames[FRAME_BUFFER_COUNT];
	uint32_t					frameIndex{ 0 };

	ID3D12Fence1*				fence{ nullptr };
	HANDLE						fenceEvent{ nullptr };
	uint64_t					fenceValue{ 0 };
};

