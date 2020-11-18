#pragma once
#include "wbpch.h"
#include "Renderer/Renderer.h"
#include "Renderer/Mesh.h"
#include "Utils/Utils.h"


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

//<-- DirectX specific linking -->
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")


//<-- define left-hand coordinate system -->
#define WB_HANDEDNESS "left"

namespace Workbench {
	class d3dRenderer : public Renderer {
	public:
		virtual void Init(std::shared_ptr<Window> window) override;
		~d3dRenderer();

		virtual void Begin() override;
		virtual void Draw() override;
		//virtual void DrawMesh(Mesh* mesh);
		virtual void End() override;

	protected:
		HRESULT CreateCommandObjects();
		HRESULT CreateSwapChain();
		HRESULT CreateRtvAndDsvDescriptorHeaps();

		void FlushCommandQueue();

		//window event callback
		void OnResize() override;

		void ResizeSwapchain();

		std::vector<IDXGIAdapter*> GetAdapters();
		std::vector<IDXGIOutput*> GetAdapterOutputs(IDXGIAdapter* adapter);
		std::vector<DXGI_MODE_DESC> GetOutputDisplayMode(IDXGIOutput* output, DXGI_FORMAT format);
	protected:
		template<typename T>
		using pCom = Microsoft::WRL::ComPtr<T>;
		template<typename T>
		using paCom = shared_ptr_with_custom_deleter < T, D3D12MA::Deleter>;


		//Window
		HINSTANCE m_hInstance;
		HWND m_hWnd;

		bool m_isFullScreen;


		//Adapter and assotiated output
		IDXGIAdapter* m_currentAdapter;
		IDXGIOutput* m_currentAdapterOutput;


		//AA settings
		bool      m_4xMsaaState = false;    // 4X MSAA enabled
		UINT      m_4xMsaaQuality = 0;      // quality level of 4X MSAA


		//D3D Essentials
		pCom<IDXGIFactory4> m_dxgiFactory;
		pCom<IDXGISwapChain> m_SwapChain;
		pCom<ID3D12Device> m_d3dDevice;

		paCom<D3D12MA::Allocator> m_Allocator;


		//GPU Fence
		Microsoft::WRL::ComPtr<ID3D12Fence> m_Fence;
		uint64_t m_CurrentFence = 0;


		//Command objects
		pCom<ID3D12CommandQueue> m_CommandQueue;
		pCom<ID3D12CommandAllocator> m_DirectCmdListAlloc;
		pCom<ID3D12GraphicsCommandList> m_CommandList;


		//Swapchain buffers
		static const int s_SwapChainBufferCount = 2;
		int m_CurrentBackBuffer = 0;
		pCom<ID3D12Resource> m_SwapChainBuffer[s_SwapChainBufferCount];
		paCom<D3D12MA::Allocation> m_DepthStencilBufferAllocation;
		pCom<ID3D12Resource> m_DepthStencilBuffer;



		//RTV and DSV Heaps
		pCom<ID3D12DescriptorHeap> m_RtvHeap;
		pCom<ID3D12DescriptorHeap> m_DsvHeap;


		//viewport and scissor rect
		D3D12_VIEWPORT m_ScreenViewport;
		D3D12_RECT m_ScissorRect;


		//Descriptor sizes
		UINT m_RtvDescriptorSize = 0;
		UINT m_DsvDescriptorSize = 0;
		UINT m_CbvSrvUavDescriptorSize = 0;


		//Formats specifications
		D3D_DRIVER_TYPE m_d3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
		DXGI_FORMAT m_BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		DXGI_FORMAT m_DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	};
}

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
