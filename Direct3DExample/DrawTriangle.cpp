#include "DrawTriangle.h"

void DrawTriangle::Initialize(HINSTANCE hInstance, int width, int height)
{
	D3DFramework::Initialize(hInstance, width, height);

	InitPipeline();
	InitTriangle();
}

void DrawTriangle::Destroy()
{
	mspVertexBuffer.Reset();
	mspInputLayout.Reset();
	mspPixelShader.Reset();
	mspVertexShader.Reset();

	D3DFramework::Destroy();
}

void DrawTriangle::InitTriangle()
{
	VERTEX vertices[]
	{
		{ 0.0f, 0.5f, 0.0f, { 1.0f, 0.0f, 0.0f, 1.0f } },
		{ 0.45f, -0.5f, 0.0f, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ -0.45f, -0.5f, 0.0f, { 0.0f, 0.0f, 1.0f, 1.0f } }
	};
	// Vextex 순서 중요, 노말이 결정됨 
	// V2, V3 바꾸니 화면에 안나옴 ( 삼각형의 뒷면을 보는것 ) 

	CD3D11_BUFFER_DESC bd(
		sizeof(VERTEX) * 3,			// byte
		D3D11_BIND_VERTEX_BUFFER,	// bind flag
		D3D11_USAGE_DYNAMIC,		// usage : DYNMAIC ( GPU 읽기, CPU 쓰기 )
		D3D11_CPU_ACCESS_WRITE		// CPU Access
	);

	mspDevice->CreateBuffer(&bd, nullptr, mspVertexBuffer.ReleaseAndGetAddressOf());

	// CPU 저장, GPU가 나중에 읽어감
	// CPU가 Buffer에 전송을 마칠때까지 기다려줘야 함
	// Map(Lock) - CPU가 저장을 시작 : GPU는 접근 금지
	// UnMap(UnLock) - CPU가 저장을 마치면 : GPU는 접근 가능
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

}

void DrawTriangle::InitPipeline()
{
	// 셰이더
		// HLSL -> 컴파일 -> 기계어 (Binary Data0
	// IA

	Microsoft::WRL::ComPtr<ID3DBlob> spVS;
	Microsoft::WRL::ComPtr<ID3DBlob> spPS;

	D3DCompileFromFile(
		L"VertexShader.hlsl", // 셰이더 파일 이름
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
		L"PixelShader.hlsl", // 셰이더 파일 이름
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
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	mspDevice->CreateInputLayout(ied, 2, spVS->GetBufferPointer(), spVS->GetBufferSize(), mspInputLayout.ReleaseAndGetAddressOf());

	mspDeviceContext->IASetInputLayout(mspInputLayout.Get());
}

void DrawTriangle::Render()
{
	UINT offset = 0;
	UINT stride = sizeof(VERTEX);

	mspDeviceContext->IASetVertexBuffers(0, 1, mspVertexBuffer.GetAddressOf(), &stride, &offset);

	mspDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mspDeviceContext->Draw(3, 0);
}
