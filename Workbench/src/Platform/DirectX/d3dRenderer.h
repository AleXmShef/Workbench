#pragma once
#include "wbpch.h"
#include "Renderer/Renderer.h"
#include "Platform/Windows/WindowsWindow.h"

#include <d3d12.h>
#include <dxgi1_6.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <wrl.h>
#include "d3dx12.h"

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

namespace Workbench {
	class d3dRenderer : public Renderer {
	public:
		virtual void Init(std::shared_ptr<Window> window) override;
		~d3dRenderer();

		virtual void Draw() override;
	protected:
		HRESULT CreateCommandObjects();
		HRESULT CreateSwapChain();
		HRESULT CreateRtvAndDsvDescriptorHeaps();

		void FlushCommandQueue();

		//window event callbacks
		void onResize() override;

		std::vector<IDXGIAdapter*> GetAdapters();
		std::vector<IDXGIOutput*> GetAdapterOutputs(IDXGIAdapter* adapter);
		std::vector<DXGI_MODE_DESC> GetOutputDisplayMode(IDXGIOutput* output, DXGI_FORMAT format);
	protected:
		template<typename T>
		using pCom = Microsoft::WRL::ComPtr<T>;

		HINSTANCE m_hInstance;
		HWND m_hWnd;

		bool      m_4xMsaaState = false;    // 4X MSAA enabled
		UINT      m_4xMsaaQuality = 0;      // quality level of 4X MSAA

		pCom<IDXGIFactory4> m_dxgiFactory;
		pCom<IDXGISwapChain> m_SwapChain;
		pCom<ID3D12Device> m_d3dDevice;

		Microsoft::WRL::ComPtr<ID3D12Fence> m_Fence;
		uint64_t m_CurrentFence = 0;

		pCom<ID3D12CommandQueue> m_CommandQueue;
		pCom<ID3D12CommandAllocator> m_DirectCmdListAlloc;
		pCom<ID3D12GraphicsCommandList> m_CommandList;

		static const int s_SwapChainBufferCount = 2;
		int m_CurrentBackBuffer = 0;
		pCom<ID3D12Resource> m_SwapChainBuffer[s_SwapChainBufferCount];
		pCom<ID3D12Resource> m_DepthStencilBuffer;

		pCom<ID3D12DescriptorHeap> m_RtvHeap;
		pCom<ID3D12DescriptorHeap> m_DsvHeap;

		D3D12_VIEWPORT m_ScreenViewport;
		D3D12_RECT m_ScissorRect;

		UINT m_RtvDescriptorSize = 0;
		UINT m_DsvDescriptorSize = 0;
		UINT m_CbvSrvUavDescriptorSize = 0;

		D3D_DRIVER_TYPE m_d3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
		DXGI_FORMAT m_BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		DXGI_FORMAT m_DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	};
}

