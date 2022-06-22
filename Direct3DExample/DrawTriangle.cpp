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
	// Vextex ���� �߿�, �븻�� ������ 
	// V2, V3 �ٲٴ� ȭ�鿡 �ȳ��� ( �ﰢ���� �޸��� ���°� ) 

	CD3D11_BUFFER_DESC bd(
		sizeof(VERTEX) * 3,			// byte
		D3D11_BIND_VERTEX_BUFFER,	// bind flag
		D3D11_USAGE_DYNAMIC,		// usage : DYNMAIC ( GPU �б�, CPU ���� )
		D3D11_CPU_ACCESS_WRITE		// CPU Access
	);

	mspDevice->CreateBuffer(&bd, nullptr, mspVertexBuffer.ReleaseAndGetAddressOf());

	// CPU ����, GPU�� ���߿� �о
	// CPU�� Buffer�� ������ ��ĥ������ ��ٷ���� ��
	// Map(Lock) - CPU�� ������ ���� : GPU�� ���� ����
	// UnMap(UnLock) - CPU�� ������ ��ġ�� : GPU�� ���� ����
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
	// ���̴�
		// HLSL -> ������ -> ���� (Binary Data0
	// IA

	Microsoft::WRL::ComPtr<ID3DBlob> spVS;
	Microsoft::WRL::ComPtr<ID3DBlob> spPS;

	D3DCompileFromFile(
		L"VertexShader.hlsl", // ���̴� ���� �̸�
		nullptr,			  // ��ũ�� ������ ������
		nullptr,		      // ��� ���� ������
		"main",				  // entry point
		"vs_4_0_level_9_3",	  // HLSL ���α׷� ��� ����
		0,					  // compile option
		0,
		spVS.GetAddressOf(),  // ������ ���̴� Binary Code
		nullptr				  // ������ ���� �޽���
	);

	D3DCompileFromFile(
		L"PixelShader.hlsl", // ���̴� ���� �̸�
		nullptr,			  // ��ũ�� ������ ������
		nullptr,		      // ��� ���� ������
		"main",				  // entry point
		"ps_4_0_level_9_3",	  // HLSL ���α׷� ��� ����
		0,					  // compile option
		0,
		spPS.GetAddressOf(),  // ������ ���̴� Binary Code
		nullptr				  // ������ ���� �޽���
	);

	mspDevice->CreateVertexShader(
		spVS->GetBufferPointer(),
		spVS->GetBufferSize(),
		nullptr,				// Linker ����
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

	// Input Assembler Stage ����
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
