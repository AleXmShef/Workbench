#include "wbpch.h"
#include "d3dRenderer.h"

#include "Logger/Logger.h"

namespace Workbench {
	d3dRenderer::~d3dRenderer() {
		if (m_d3dDevice != nullptr)
			FlushCommandQueue();
		//m_Allocator->Release();
	}

	void d3dRenderer::Init(std::shared_ptr<Window> window) {

		Renderer::Init(window);

		if (m_window != nullptr) {
			m_hInstance = (HINSTANCE)m_window->GetNativeWindow();
			m_hWnd = ((WindowsWindow*)m_window.get())->GetHWND();
		}
		else {
			WB_RENDERER_CRITICAL("Trying to initialize DirectX with non-WIN32 API window");
			return;
		}

		m_isFullScreen = m_window->IsFullscreen();

		//initialize DirectX debug layer when in debug build
#ifdef WB_DEBUG
		pCom<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
			debugController->EnableDebugLayer();
		}
#endif // DEBUG

		//Begin Initialization
		//Create DXGI Factory
		HRESULT createdFactory = CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory));
		if (FAILED(createdFactory)) {
			WB_RENDERER_CRITICAL("Failed to create DXGIFactory");
			return;
		}


		//Get current adapter
		auto adapters = GetAdapters();
		m_currentAdapter = adapters[0];

		//Get current output
		//auto outputs = GetAdapterOutputs(m_currentAdapter);
		//m_currentAdapterOutput = outputs[0];

		//Create hardware device
		HRESULT createdHardwareDevice = D3D12CreateDevice(m_currentAdapter, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&m_d3dDevice));
		if (FAILED(createdHardwareDevice)) {
			WB_RENDERER_CRITICAL("Failed to create hardware device");
		}

		DXGI_ADAPTER_DESC desc;
		m_currentAdapter->GetDesc(&desc);
		WB_RENDERER_LOG("Using adapter:");
		WB_RENDERER_LOG("\t{}", ws2s(desc.Description));

		//Create D3D12 Memory Allocator
		D3D12MA::ALLOCATOR_DESC allocatorDesc = {};
		allocatorDesc.pDevice = m_d3dDevice.Get();
		allocatorDesc.pAdapter = m_currentAdapter;

		D3D12MA::Allocator* _alloc;
		
		HRESULT createdMemotyAllocator = D3D12MA::CreateAllocator(&allocatorDesc, &_alloc);
		if (FAILED(createdMemotyAllocator)) {
			WB_RENDERER_CRITICAL("Failed to create D3D12 Memory Allocator");
		}

		m_Allocator.reset(_alloc);

		//Create fence object
		m_d3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence));

		//Get descriptor sizes
		m_RtvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		m_DsvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		m_CbvSrvUavDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		//Check 4x MSAA quality support
		D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
		msQualityLevels.Format = m_BackBufferFormat;
		msQualityLevels.SampleCount = 4;
		msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
		msQualityLevels.NumQualityLevels = 0;

		m_d3dDevice->CheckFeatureSupport(
			D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
			&msQualityLevels,
			sizeof(msQualityLevels));

		m_4xMsaaQuality = msQualityLevels.NumQualityLevels;

		CreateCommandObjects();
		CreateSwapChain();
		CreateRtvAndDsvDescriptorHeaps();

		OnResize();
		
		//temporarily disable Alt+Enter combo for going fullscreen
		m_dxgiFactory->MakeWindowAssociation(m_hWnd, DXGI_MWA_NO_ALT_ENTER);

		WB_RENDERER_INFO("DirectX successfully initialized");
	}

	HRESULT d3dRenderer::CreateCommandObjects() {

		//poopulate command queue descriptor struct
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

		//create command queue
		HRESULT createdCommandQueue = m_d3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_CommandQueue));
		if (FAILED(createdCommandQueue)) {
			WB_RENDERER_CRITICAL("Failed to create command queue");
			return createdCommandQueue;
		}

		//create command allocator
		HRESULT createdCommandAllocator = m_d3dDevice->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(m_DirectCmdListAlloc.GetAddressOf())
		);
		if (FAILED(createdCommandAllocator)) {
			WB_RENDERER_CRITICAL("Failed to create command allocator");
			return createdCommandAllocator;
		}

		//create command list
		HRESULT createdCommandList = m_d3dDevice->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			m_DirectCmdListAlloc.Get(), // Associated command allocator
			nullptr,                   // Initial PipelineStateObject
			IID_PPV_ARGS(m_CommandList.GetAddressOf())
		);
		if (FAILED(createdCommandList)) {
			return createdCommandList;
		}

		// Start off in a closed state.  This is because the first time we refer 
		// to the command list we will Reset it, and it needs to be closed before
		// calling Reset.
		m_CommandList->Close();
		return createdCommandList;
	}

	HRESULT d3dRenderer::CreateSwapChain() {
		m_SwapChain.Reset();

		auto [windowWidth, windowHeight] = m_window->GetDimensions();

		//populate swap chain descriptor struct
		DXGI_SWAP_CHAIN_DESC sd;
		sd.BufferDesc.Width = windowWidth;											//window width
		sd.BufferDesc.Height = windowHeight;										//window height
		sd.BufferDesc.RefreshRate.Numerator = 0;									//monitor refresh rate
		sd.BufferDesc.RefreshRate.Denominator = 0;									//
		sd.BufferDesc.Format = m_BackBufferFormat;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.SampleDesc.Count = m_4xMsaaState ? 4 : 1;								//antialiasing sample count
		sd.SampleDesc.Quality = m_4xMsaaState ? (m_4xMsaaQuality - 1) : 0;			//antialiasing quality level
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = s_SwapChainBufferCount;									//buffer count (2 or 3)
		sd.OutputWindow = m_hWnd;													//windows hwnd
		sd.Windowed = true;															//is windowed
		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		return m_dxgiFactory->CreateSwapChain(
			m_CommandQueue.Get(),
			&sd,
			m_SwapChain.GetAddressOf()
		);
	}

	HRESULT d3dRenderer::CreateRtvAndDsvDescriptorHeaps() {
		//populate rtv descriptor heap struct
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
		rtvHeapDesc.NumDescriptors = s_SwapChainBufferCount;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rtvHeapDesc.NodeMask = 0;

		//create rtv descriptor heap
		HRESULT createdRtvDescriptorHeap = m_d3dDevice->CreateDescriptorHeap(
			&rtvHeapDesc, 
			IID_PPV_ARGS(m_RtvHeap.GetAddressOf())
		);
		if (FAILED(createdRtvDescriptorHeap)) {
			WB_RENDERER_CRITICAL("Failed to create rtv descriptor heap");
			return createdRtvDescriptorHeap;
		}

		//populate dsv descriptor heap struct
		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
		dsvHeapDesc.NumDescriptors = 1;
		dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		dsvHeapDesc.NodeMask = 0;

		//create dsv descriptor heap
		HRESULT createdDsvDescriptorHeap = m_d3dDevice->CreateDescriptorHeap(
			&dsvHeapDesc, 
			IID_PPV_ARGS(m_DsvHeap.GetAddressOf())
		);
		if (FAILED(createdDsvDescriptorHeap)) {
			WB_RENDERER_CRITICAL("Failed to create dsv descriptor heap");
			return createdRtvDescriptorHeap;
		}

		return S_OK;
	}

	void d3dRenderer::FlushCommandQueue() {
		// Advance the fence value to mark commands up to this fence point.
		m_CurrentFence++;

		// Add an instruction to the command queue to set a new fence point.  Because we 
		// are on the GPU timeline, the new fence point won't be set until the GPU finishes
		// processing all the commands prior to this Signal().
		m_CommandQueue->Signal(m_Fence.Get(), m_CurrentFence);

		// Wait until the GPU has completed commands up to this fence point.
		if (m_Fence->GetCompletedValue() < m_CurrentFence)
		{
			HANDLE eventHandle = CreateEventEx(nullptr, NULL, false, EVENT_ALL_ACCESS);
			if (eventHandle != 0) {
				// Fire event when GPU hits current fence.  
				m_Fence->SetEventOnCompletion(m_CurrentFence, eventHandle);

				// Wait until the GPU hits current fence event is fired.
				WaitForSingleObject(eventHandle, INFINITE);
				CloseHandle(eventHandle);
			}
		}
	}


	void d3dRenderer::OnResize() {
		ResizeSwapchain();
	};

	void d3dRenderer::ResizeSwapchain() {

		auto [newWidth, newHeight] = m_window->GetDimensions();

		//flush before changing any resources
		FlushCommandQueue();

		//reset command list
		m_CommandList->Reset(m_DirectCmdListAlloc.Get(), nullptr);

		//reset buffer com pointers (= nullptr)
		for (int i = 0; i < s_SwapChainBufferCount; ++i)
			m_SwapChainBuffer[i].Reset();
		m_DepthStencilBuffer.Reset();

		//resize sawpchain buffers
		m_SwapChain->ResizeBuffers(
			s_SwapChainBufferCount,
			newWidth, newHeight,
			m_BackBufferFormat,
			DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
		);

		m_CurrentBackBuffer = 0;

		//NEED TO UNDERSTAND THIS

		//create back buffer views (since buffers themselves were already created by the swapchain???)
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_RtvHeap->GetCPUDescriptorHandleForHeapStart());
		for (UINT i = 0; i < s_SwapChainBufferCount; i++)
		{
			m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_SwapChainBuffer[i]));
			m_d3dDevice->CreateRenderTargetView(m_SwapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
			rtvHeapHandle.Offset(1, m_RtvDescriptorSize);
		}

		//create depth/stencil buffer
		D3D12_RESOURCE_DESC depthStencilDesc;
		depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		depthStencilDesc.Alignment = 0;
		depthStencilDesc.Width = newWidth;
		depthStencilDesc.Height = newHeight;
		depthStencilDesc.DepthOrArraySize = 1;
		depthStencilDesc.MipLevels = 1;

		// Correction 11/12/2016: SSAO chapter requires an SRV to the depth buffer to read from 
		// the depth buffer.  Therefore, because we need to create two views to the same resource:
		//   1. SRV format: DXGI_FORMAT_R24_UNORM_X8_TYPELESS
		//   2. DSV Format: DXGI_FORMAT_D24_UNORM_S8_UINT
		// we need to create the depth buffer resource with a typeless format.  
		depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;

		depthStencilDesc.SampleDesc.Count = m_4xMsaaState ? 4 : 1;
		depthStencilDesc.SampleDesc.Quality = m_4xMsaaState ? (m_4xMsaaQuality - 1) : 0;
		depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE optClear;
		optClear.Format = m_DepthStencilFormat;
		optClear.DepthStencil.Depth = 1.0f;
		optClear.DepthStencil.Stencil = 0;

		//<-- legacy memory allocation -->
		//m_d3dDevice->CreateCommittedResource(
		//	&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		//	D3D12_HEAP_FLAG_NONE,
		//	&depthStencilDesc,
		//	D3D12_RESOURCE_STATE_COMMON,
		//	&optClear,
		//	IID_PPV_ARGS(m_DepthStencilBuffer.GetAddressOf()));

		//<-- D3D12 Memory Allocator allocation
		D3D12MA::ALLOCATION_DESC desc = {};
		desc.HeapType = D3D12_HEAP_TYPE_DEFAULT;

		D3D12MA::Allocation* alloc;

		m_Allocator->CreateResource(
			&desc,
			&depthStencilDesc,
			D3D12_RESOURCE_STATE_COMMON,
			&optClear,
			&alloc,
			IID_PPV_ARGS(m_DepthStencilBuffer.GetAddressOf())
		);

		m_DepthStencilBufferAllocation.reset(alloc);

		//create depth/stencil view
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Format = m_DepthStencilFormat;
		dsvDesc.Texture2D.MipSlice = 0;
		m_d3dDevice->CreateDepthStencilView(m_DepthStencilBuffer.Get(), &dsvDesc, m_DsvHeap->GetCPUDescriptorHandleForHeapStart());

		// Transition the resource from its initial state to be used as a depth buffer.
		m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_DepthStencilBuffer.Get(),
			D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));

		// Execute the resize commands.
		m_CommandList->Close();
		ID3D12CommandList* cmdsLists[] = { m_CommandList.Get() };
		m_CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

		// Wait until resize is complete.
		FlushCommandQueue();

		// Update the viewport transform to cover the client area.
		m_ScreenViewport.TopLeftX = 0;
		m_ScreenViewport.TopLeftY = 0;
		m_ScreenViewport.Width = static_cast<float>(newWidth);
		m_ScreenViewport.Height = static_cast<float>(newHeight);
		m_ScreenViewport.MinDepth = 0.0f;
		m_ScreenViewport.MaxDepth = 1.0f;

		m_ScissorRect = { 0, 0, (LONG)newWidth, (LONG)newHeight };
	};

	void d3dRenderer::Begin() {
		m_DirectCmdListAlloc->Reset();

		m_CommandList->Reset(m_DirectCmdListAlloc.Get(), nullptr);

		m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_SwapChainBuffer[m_CurrentBackBuffer].Get(),
			D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

		m_CommandList->RSSetViewports(1, &m_ScreenViewport);
		m_CommandList->RSSetScissorRects(1, &m_ScissorRect);

		m_CommandList->ClearRenderTargetView(CD3DX12_CPU_DESCRIPTOR_HANDLE(
			m_RtvHeap->GetCPUDescriptorHandleForHeapStart(),
			m_CurrentBackBuffer,
			m_RtvDescriptorSize), DirectX::Colors::LightSteelBlue, 0, nullptr);
		m_CommandList->ClearDepthStencilView(m_DsvHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

		m_CommandList->OMSetRenderTargets(1, &CD3DX12_CPU_DESCRIPTOR_HANDLE(
			m_RtvHeap->GetCPUDescriptorHandleForHeapStart(),
			m_CurrentBackBuffer,
			m_RtvDescriptorSize), true, &m_DsvHeap->GetCPUDescriptorHandleForHeapStart());
	}

	void d3dRenderer::Draw() {
		
	}

	//void d3dRenderer::DrawMesh(Mesh* mesh) {
	//
	//}

	void d3dRenderer::End() {
		// Indicate a state transition on the resource usage.
		m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_SwapChainBuffer[m_CurrentBackBuffer].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

		// Done recording commands.
		m_CommandList->Close();

		// Add the command list to the queue for execution.
		ID3D12CommandList* cmdsLists[] = { m_CommandList.Get() };
		m_CommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

		// swap the back and front buffers
		m_SwapChain->Present(m_window->IsVSync(), 0);
		m_CurrentBackBuffer = (m_CurrentBackBuffer + 1) % s_SwapChainBufferCount;

		// Wait until frame commands are complete.  This waiting is inefficient and is
		// done for simplicity.  Later we will show how to organize our rendering code
		// so we do not have to wait per frame.
		FlushCommandQueue();
	}

	std::vector<IDXGIAdapter*> d3dRenderer::GetAdapters() {
		UINT i = 0;
		IDXGIAdapter* adapter = nullptr;
		std::vector<IDXGIAdapter*> adapterList;

		WB_RENDERER_LOG("Available adapters:");

		while (m_dxgiFactory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND) {
			DXGI_ADAPTER_DESC desc;
			adapter->GetDesc(&desc);
			std::wstring desc_ = L"";
			desc_ += desc.Description;
			WB_RENDERER_LOG("\t{0}", ws2s(desc_));
			adapterList.push_back(adapter);

			++i;
		}

		return adapterList;
	}
	std::vector<IDXGIOutput*> d3dRenderer::GetAdapterOutputs(IDXGIAdapter* adapter) {
		std::vector<IDXGIOutput*> output_vec;
		
		UINT i = 0;
		IDXGIOutput* output = nullptr;
		WB_RENDERER_LOG("Available outputs:");

		while (adapter->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND) {
			DXGI_OUTPUT_DESC desc;
			output->GetDesc(&desc);

			std::wstring desc_ = L"";
			desc_ += desc.DeviceName;

			WB_RENDERER_LOG("\t{}", ws2s(desc_));

			output_vec.push_back(output);

			++i;
		}
		
		return output_vec;
	}
}
