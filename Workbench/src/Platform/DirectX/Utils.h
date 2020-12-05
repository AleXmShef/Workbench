#pragma once

//<-- DirectX specific includes -->
#include <d3d12.h>
#include <dxgi1_6.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <wrl.h>
#include "d3dx12.h"
#include "D3D12MemoryAllocator/D3D12MemAlloc.h"
#include "Utils/Utils.h"

#define D3D12MA_CREATE_RESOURCE(allocator, allocDesc, bufferDesc, resourceState, optClear, bufferAlloc, buffer)\
D3D12MA::Allocation* _alloc;\
allocator->CreateResource(\
	allocDesc,\
	bufferDesc,\
	resourceState,\
	optClear,\
	&_alloc,\
	IID_PPV_ARGS(buffer.GetAddressOf()));\
bufferAlloc.reset(_alloc);

inline uint64_t CalcConstantBufferByteSize(uint64_t byteSize)
{
	return (byteSize + 255) & ~255;
}

template<typename T>
using pCom = Microsoft::WRL::ComPtr<T>;
template<typename T>
using paCom = shared_ptr_with_custom_deleter < T, D3D12MA::Deleter>;

pCom<ID3DBlob> CompileShader(
	const std::wstring& filename,
	const D3D_SHADER_MACRO* defines,
	const std::string& entrypoint,
	const std::string& target);
