#include <vector>
#include <fstream>
#include "DrawSquare.h"


void DrawSquare::Initialize(HINSTANCE hInstance, int width, int height)
{
	D3DFramework::Initialize(hInstance, width, height);

	InitPipeline();
	InitSquare();

	CreateTextureFromBMP();
}

void DrawSquare::Destroy()
{
	mspBlendState.Reset();
	mspSamplerState.Reset();
	mspTextureView.Reset();
	mspTexture.Reset();
	mspVertexBuffer.Reset();
	mspInputLayout.Reset();
	mspPixelShader.Reset();
	mspVertexShader.Reset();

	D3DFramework::Destroy();
}

void DrawSquare::InitSquare()
{
	VERTEX vertices[]
	{
		{ -0.5f, 0.5f, 0.0f, 0.0f, 0.0f },
		{ 0.5f, 0.5f, 0.0f, 1.0f, 0.0f },
		{ -0.5f, -0.5f, 0.0f, 0.0f, 1.0f },
		{ 0.5f, -0.5f, 0.0f, 1.0f,1.0f }
	};

	CD3D11_BUFFER_DESC bd(
		sizeof(VERTEX) * 4,			// byte
		D3D11_BIND_VERTEX_BUFFER,	// bind flag
		D3D11_USAGE_DYNAMIC,		// usage : DYNMAIC ( GPU 읽기, CPU 쓰기 )
		D3D11_CPU_ACCESS_WRITE		// CPU Access
	);

	mspDevice->CreateBuffer(&bd, nullptr, mspVertexBuffer.ReleaseAndGetAddressOf());

	D3D11_MAPPED_SUBRESOURCE ms;
	mspDeviceContext->Map(
		mspVertexBuffer.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&ms
	);

	memcpy(ms.pData, vertices, sizeof(vertices));

	mspDeviceContext->Unmap(mspVertexBuffer.Get(), 0);

	float border[4]{ 0.0f, 0.0f, 0.0f, 0.0f };
	CD3D11_SAMPLER_DESC sd(
		D3D11_FILTER_MIN_MAG_MIP_POINT,
		D3D11_TEXTURE_ADDRESS_CLAMP,
		D3D11_TEXTURE_ADDRESS_CLAMP,
		D3D11_TEXTURE_ADDRESS_CLAMP,
		0.0f,
		1,
		D3D11_COMPARISON_ALWAYS,
		border,
		0,
		1
	);

	mspDevice->CreateSamplerState(&sd, mspSamplerState.ReleaseAndGetAddressOf());

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	mspDevice->CreateBlendState(&blendDesc, mspBlendState.ReleaseAndGetAddressOf());
}

void DrawSquare::InitPipeline()
{
	Microsoft::WRL::ComPtr<ID3DBlob> spVS;
	Microsoft::WRL::ComPtr<ID3DBlob> spPS;

	D3DCompileFromFile(
		L"VertexShader1.hlsl", // 셰이더 파일 이름
		nullptr,			  // 매크로 데이터 포인터
		nullptr,		      // 헤더 파일 포인터
		"main",				  // entry point
		"vs_4_0_level_9_3",	  // HLSL 프로그램 언어 버전
		0,					  // compile option
		0,
		spVS.GetAddressOf(),  // 생성된 셰이더 Binary Code
		nullptr				  // 컴파일 에러 메시지
	);

	D3DCompileFromFile(
		L"PixelShader1.hlsl", // 셰이더 파일 이름
		nullptr,			  // 매크로 데이터 포인터
		nullptr,		      // 헤더 파일 포인터
		"main",				  // entry point
		"ps_4_0_level_9_3",	  // HLSL 프로그램 언어 버전
		0,					  // compile option
		0,
		spPS.GetAddressOf(),  // 생성된 셰이더 Binary Code
		nullptr				  // 컴파일 에러 메시지
	);

	mspDevice->CreateVertexShader(
		spVS->GetBufferPointer(),
		spVS->GetBufferSize(),
		nullptr,				// Linker 정보
		mspVertexShader.ReleaseAndGetAddressOf()
	);
	mspDevice->CreatePixelShader(
		spPS->GetBufferPointer(),
		spPS->GetBufferSize(),
		nullptr,
		mspPixelShader.ReleaseAndGetAddressOf()
	);

	mspDeviceContext->VSSetShader(mspVertexShader.Get(), nullptr, 0);
	mspDeviceContext->PSSetShader(mspPixelShader.Get(), nullptr, 0);

	// Input Assembler Stage 설정
	D3D11_INPUT_ELEMENT_DESC ied[]{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	mspDevice->CreateInputLayout(ied, 2, spVS->GetBufferPointer(), spVS->GetBufferSize(), mspInputLayout.ReleaseAndGetAddressOf());

	mspDeviceContext->IASetInputLayout(mspInputLayout.Get());
}

HRESULT DrawSquare::CreateTextureFromBMP()
{
	std::ifstream file;
	file.open("Data/32.bmp", std::ios::binary);

	BITMAPFILEHEADER bmh;
	BITMAPINFOHEADER bmi;

	// 2. BITMAPFILEHEADER 읽기
	file.read(reinterpret_cast<char*>(&bmh), sizeof(BITMAPFILEHEADER));
	// 3. BITMAPINFOGHEADER 읽기
	file.read(reinterpret_cast<char*>(&bmi), sizeof(BITMAPINFOHEADER));
	if (bmh.bfType != 0x4D42)
	{
		return E_FAIL;
	}
	if (bmi.biBitCount != 32)
	{
		return E_FAIL;
	}

	std::vector<char> pPixels(bmi.biSizeImage);
	// 4. 픽셀로 건너뛰기
	file.seekg(bmh.bfOffBits);
	// 5. 비트맵 읽기
	int pitch = bmi.biWidth * (bmi.biBitCount / 8);
	int index{};
	char r{}, g{}, b{}, a{};
	for (int y {bmi.biHeight - 1}; y >= 0; y--)
	{
		index = y * pitch;
		for (int x = 0; x < bmi.biWidth; x++)
		{
			file.read(&b, 1);
			file.read(&g, 1);
			file.read(&r, 1);
			file.read(&a, 1);

			if (static_cast<unsigned char>(r) == 30 &&
				static_cast<unsigned char>(g) == 199 &&
				static_cast<unsigned char>(b) == 250)
			{
				pPixels[index] = 0;
				pPixels[index+1] = 0;
				pPixels[index+2] = 0;
				pPixels[index+3] = 0;
			}
			else
			{
				pPixels[index] = b;
				pPixels[index + 1] = g;
				pPixels[index + 2] = r;
				pPixels[index + 3] = a;
			}

			index += 4;
		}
	}

	file.close();

	CD3D11_TEXTURE2D_DESC td(
		DXGI_FORMAT_B8G8R8A8_UNORM,
		bmi.biWidth,
		bmi.biHeight,
		1,
		1
	);

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &pPixels[0];
	initData.SysMemPitch = pitch;
	initData.SysMemSlicePitch = 0;

	mspDevice->CreateTexture2D(&td, &initData, mspTexture.ReleaseAndGetAddressOf());

	// ResourceView - ShaderResourceView
	CD3D11_SHADER_RESOURCE_VIEW_DESC srvd(
		D3D11_SRV_DIMENSION_TEXTURE2D,
		td.Format,
		0,
		1
	);

	mspDevice->CreateShaderResourceView(mspTexture.Get(), &srvd, mspTextureView.ReleaseAndGetAddressOf());

	return S_OK;
}

void DrawSquare::Render()
{
	UINT offset = 0;
	UINT stride = sizeof(VERTEX);

	mspDeviceContext->IASetVertexBuffers(0, 1, mspVertexBuffer.GetAddressOf(), &stride, &offset);

	mspDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
//	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
//	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP

	mspDeviceContext->PSSetSamplers(0, 1, mspSamplerState.GetAddressOf());

	mspDeviceContext->PSSetShaderResources(0, 1, mspTextureView.GetAddressOf());

	mspDeviceContext->OMSetBlendState(mspBlendState.Get(), nullptr, 0xffffffff);

	mspDeviceContext->Draw(4, 0);
}
