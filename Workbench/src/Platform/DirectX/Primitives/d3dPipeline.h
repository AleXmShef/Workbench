#pragma once
#include "wbpch.h"
#include "Utils/Utils.h"
#include "Platform/DirectX/Utils.h"

struct d3dPipeline {
	pCom<ID3D12PipelineState> pso;
	std::unordered_map<std::string, pCom<ID3DBlob>> shaders;
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
	pCom<ID3D12RootSignature> rootSignature;
};
