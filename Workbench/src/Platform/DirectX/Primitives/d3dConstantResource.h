#pragma once
#include "wbpch.h"
#include "Utils/Utils.h"
#include "Platform/DirectX/Utils.h"

#include "Renderer/Primitives/ConstantResource.h"


namespace Workbench {
	class d3dConstantResource : public ConstantResource {
	public:
		d3dConstantResource(
			ID3D12Device* device,
			ID3D12GraphicsCommandList* cmdList,
			D3D12MA::Allocator* allocator
		);

		void UpdateResource(const void* data, uint64_t size) override;

		void UploadResource() override;

		void ReleaseResource() override;

	protected:
		template<typename T>
		using pCom = Microsoft::WRL::ComPtr<T>;
		template<typename T>
		using paCom = shared_ptr_with_custom_deleter < T, D3D12MA::Deleter>;

		ID3D12Device* m_Device = nullptr;
		ID3D12GraphicsCommandList* m_CmdList;
		D3D12MA::Allocator* m_Allocator = nullptr;

		pCom<ID3D12Resource> m_Buffer = nullptr;
		paCom<D3D12MA::Allocation> m_BufferAllocation;
		pCom<ID3D12Resource> m_Uploader = nullptr;
		paCom<D3D12MA::Allocation> m_UploaderAllocation;

		D3D12_SUBRESOURCE_DATA m_SubResourceData = {};

		pCom<ID3DBlob> m_Data = nullptr;
	};
}

