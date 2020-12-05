#include "wbpch.h"
#include "Core.h"
#include "Utils.h"
#include "Platform/Windows/WindowsUtility.h"
#include "Logger/Logger.h"

	pCom<ID3DBlob> CompileShader(
		const std::wstring& filename,
		const D3D_SHADER_MACRO* defines,
		const std::string& entrypoint,
		const std::string& target)
	{
		UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
		compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		HRESULT hr = S_OK;

		pCom<ID3DBlob> byteCode = nullptr;
		pCom<ID3DBlob> errors;
		hr = D3DCompileFromFile(filename.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entrypoint.c_str(), target.c_str(), compileFlags, 0, &byteCode, &errors);

		if (hr != S_OK) {
			WB_RENDERER_ERROR("Error");
		}

		if (errors != nullptr) {
			WB_RENDERER_ERROR((char*)errors->GetBufferPointer());
		}

		return byteCode;
	}