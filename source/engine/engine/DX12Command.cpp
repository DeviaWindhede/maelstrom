#include "pch.h"
#include "DX12Command.h"
#include "DX12_inc.h"

DX12Command::DX12Command(ID3D12Device * const aDevice, D3D12_COMMAND_LIST_TYPE aType)
{
	HRESULT hr{ S_OK };

	// Create command queue
	D3D12_COMMAND_QUEUE_DESC desc{};
	desc.Type = aType;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.NodeMask = 0; // TODO: If multi-GPU, set to corresponding mask
	DXCall(hr = aDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(&commandQueue)));
	if (FAILED(hr))
	{
		Release();
		return;
	}
	// WE WILL MOST LIKELY ONLY USE DIRECT AND COMPUTE COMMAND QUEUES
	// SO MAYBE WE COULD LOG THE DIFFERENCE
	NAME_D3D12_OBJECT(
		commandQueue,
		aType == D3D12_COMMAND_LIST_TYPE_DIRECT ?
		L"Direct Queue" :
		aType == D3D12_COMMAND_LIST_TYPE_COMPUTE ?
		L"Compute Queue" :
		L"Command Queue"
	);

	// Create command allocator
	for (uint32_t i = 0; i < FRAME_BUFFER_COUNT; ++i)
	{
		DXCall(hr = aDevice->CreateCommandAllocator(aType, IID_PPV_ARGS(&commandFrames[i].allocator)));
		if (FAILED(hr))
		{
			Release();
			return;
		}

		NAME_D3D12_OBJECT_INDEXED(
			commandFrames[i].allocator,
			i,
			aType == D3D12_COMMAND_LIST_TYPE_DIRECT ?
			L"Direct Allocator" :
			aType == D3D12_COMMAND_LIST_TYPE_COMPUTE ?
			L"Compute Allocator" :
			L"Command Allocator"
		);
	}

	// Create command list
	DXCall(hr = aDevice->CreateCommandList(
		0, // since we have only one GPU, the node mask is 0
		aType,
		commandFrames[0].allocator,
		nullptr,
		IID_PPV_ARGS(&commandList)
	));
	NAME_D3D12_OBJECT(
		commandList,
		aType == D3D12_COMMAND_LIST_TYPE_DIRECT ?
		L"Direct Command List" :
		aType == D3D12_COMMAND_LIST_TYPE_COMPUTE ?
		L"Compute Command List" :
		L"Command Command List"
	);
	commandList->Close();

	if (FAILED(hr))
	{
		Release();
		return;
	}

	// Create synchronization objects
	DXCall(hr = aDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));
	if (FAILED(hr))
	{
		Release();
		return;
	}
	NAME_D3D12_OBJECT(fence, L"Fence");

	fenceEvent = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
	assert(fenceEvent != nullptr);
}

DX12Command::~DX12Command()
{
	assert(commandQueue == nullptr && commandList == nullptr && fence == nullptr);
}

bool DX12Command::BeginFrame()
{
	CommandFrame& frame = commandFrames[frameIndex];
	frame.Wait(fenceEvent, fence);

	HRESULT hr{ S_OK };

	// Resetting the command list means that we can reuse the memory that the command list was using
	DXCall(hr = frame.allocator->Reset());
	if (FAILED(hr))
	{
		return false;
	}

	DXCall(hr = commandList->Reset(frame.allocator, nullptr));
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

void DX12Command::EndFrame()
{
	// Closing the command list means that we can't add any more commands to it
	DXCall(commandList->Close());

	ID3D12CommandList* const commandLists[] = { commandList }; // TODO: Add more command lists for multi-threading at a later date
	commandQueue->ExecuteCommandLists(_countof(commandLists), &commandLists[0]);

	++fenceValue;
	commandFrames[frameIndex].fenceValue = fenceValue;
	commandQueue->Signal(fence, fenceValue);

	frameIndex = (frameIndex + 1) % FRAME_BUFFER_COUNT;
}

void DX12Command::Release()
{
	for (uint32_t i = 0; i < FRAME_BUFFER_COUNT; i++)
	{
		commandFrames[i].Wait(fenceEvent, fence);
	}
	::d3d12_common::Release(fence);
	fenceValue = 0;

	CloseHandle(fenceEvent);
	fenceEvent = nullptr;

	for (uint32_t i = 0; i < FRAME_BUFFER_COUNT; i++)
	{
		commandFrames[i].Release();
	}

	::d3d12_common::Release(commandQueue);
	::d3d12_common::Release(commandList);
}

void DX12Command::CommandFrame::Wait(HANDLE aFenceEvent, ID3D12Fence1 * const aFence)
{
	assert(aFenceEvent && aFence != nullptr);

	auto val = aFence->GetCompletedValue();
	val;
	// The GPU has not finished executing all active command lists if completed value is less than the fence value
	if (aFence->GetCompletedValue() < fenceValue)
	{
		// Registers the fence to be signaled when the fence value reaches the specified value
		DXCall(aFence->SetEventOnCompletion(fenceValue, aFenceEvent));

		// Wait until the command queue has finished execution
		WaitForSingleObject(aFenceEvent, INFINITE);
	}
}

void DX12Command::CommandFrame::Release()
{
	::d3d12_common::Release(allocator);
}
