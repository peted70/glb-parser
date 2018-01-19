﻿#include "pch.h"
#include "Sample3DSceneRenderer.h"

#include "..\Common\DirectXHelper.h"
#include "Utility.h"

#include "..\EventShim.h"
#include "SceneManager.h"
#include "BufferManager.h"

// Please move me :)
static float lastPosX;
static float lastPosY;
static float lastY;

using namespace ModelViewer;

using namespace DirectX;
using namespace Windows::Foundation;
using namespace Microsoft::WRL;

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
Sample3DSceneRenderer::Sample3DSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_loadingComplete(false),
	m_degreesPerSecond(45),
	m_indexCount(0),
	m_tracking(false),
	m_deviceResources(deviceResources),
	_yaw(0.0f),
	_pitch(0.0f),
	_roll(0.0f),
	_zoom(1.0f)
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	BufferManager::Instance().MVPBuffer().BufferData().light_direction = XMFLOAT4(1.7f, 11.0f, 5.7f, 1.0f);

	SceneManager::Instance().SetDevResources(deviceResources);

	_grid = make_unique<DXGrid>();
	_grid->Initialise(deviceResources->GetD3DDevice());
	_mainAxes = make_unique<Axis>(1000.0f);
	_mainAxes->Initialise(deviceResources->GetD3DDevice());
}

// Initializes view parameters when the window size changes.
void Sample3DSceneRenderer::CreateWindowSizeDependentResources()
{
	Size outputSize = m_deviceResources->GetOutputSize();
	float aspectRatio = outputSize.Width / outputSize.Height;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	// This is a simple example of change that can be made when the app is in
	// portrait or snapped view.
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	// Note that the OrientationTransform3D matrix is post-multiplied here
	// in order to correctly orient the scene to match the display orientation.
	// This post-multiplication step is required for any draw calls that are
	// made to the swap chain render target. For draw calls to other targets,
	// this transform should not be applied.

	// This sample makes use of a right-handed coordinate system using row-major matrices.
	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovRH(
		fovAngleY,
		aspectRatio,
		0.01f,
		1000.0f
		);

	XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();

	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

	XMStoreFloat4x4(
		&BufferManager::Instance().MVPBuffer().BufferData().projection,
		XMMatrixTranspose(perspectiveMatrix * orientationMatrix)
		);

	static const XMVECTORF32 at = { 0.0f, 0.0f, 0.0f, 0.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	auto camMatrix = XMMatrixRotationRollPitchYaw(_pitch, _yaw, _roll);

	XMVECTORF32 alongZ = { 0.0f, 0.0f, _zoom };

	auto eye = XMVector3TransformCoord(alongZ, camMatrix);
	XMStoreFloat4x4(&BufferManager::Instance().MVPBuffer().BufferData().view, 
		XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
}

// Called once per frame, rotates the cube and calculates the model and view matrices.
void Sample3DSceneRenderer::Update(DX::StepTimer const& timer)
{
	if (!m_tracking)
	{
		// Convert degrees to radians, then convert seconds to rotation angle
		//float radiansPerSecond = XMConvertToRadians(m_degreesPerSecond);
		//double totalRotation = timer.GetTotalSeconds() * radiansPerSecond;
		//float radians = static_cast<float>(fmod(totalRotation, XM_2PI));

		Rotate(0.0f);
	}
}

// Rotate the 3D cube model a set amount of radians.
void Sample3DSceneRenderer::Rotate(float radians)
{
	// Prepare to pass the updated model matrix to the shader
	XMStoreFloat4x4(&BufferManager::Instance().MVPBuffer().BufferData().model, XMMatrixTranspose(XMMatrixRotationY(radians)));
}

void Sample3DSceneRenderer::StartTracking(float positionX, float positionY, VirtualKeyModifiers mod)
{
	Utility::Out(L"StartTracking [%f %f]", positionX, positionY);

	m_tracking = true;
	lastPosY = positionY;
	lastPosX = positionX;
}

// When tracking, the 3D cube can be rotated around its Y axis by tracking pointer position relative to the output screen width.
void Sample3DSceneRenderer::TrackingUpdate(float positionX, float positionY, VirtualKeyModifiers mod)
{
	if (m_tracking)
	{
		Utility::Out(L"TrackingUpdate [%f %f] %d", positionX, positionY, mod);

		if ((int)(mod & VirtualKeyModifiers::Control) != 0)
		{
			_zoom += (positionY - lastPosY) / 10.0f;
		}
		else
		{
			_pitch += (positionY - lastPosY) / 100.0f;
			_yaw += (positionX - lastPosX) / 100.0f;
		}

		lastPosY = positionY;
		lastPosX = positionX;

		CreateWindowSizeDependentResources();
	}
}

void Sample3DSceneRenderer::StopTracking(float positionX, float positionY, VirtualKeyModifiers mod)
{
	Utility::Out(L"StopTracking [%f %f]", positionX, positionY);
	m_tracking = false;
}

// Renders one frame using the vertex and pixel shaders.
void Sample3DSceneRenderer::Render()
{
	// Loading is asynchronous. Only draw geometry after it's loaded.
	if (!m_loadingComplete)
	{
		return;
	}

	auto context = m_deviceResources->GetD3DDeviceContext();

	DrawGrid(context);
	DrawAxis(context, _mainAxes.get());

	context->RSSetState(_pRasterState);

	SceneManager::Instance().Current()->Draw(context);
	return;
}

void Sample3DSceneRenderer::DrawGrid(ID3D11DeviceContext2 *context)
{
	BufferManager::Instance().MVPBuffer().BufferData().color.x = 0.45f;
	BufferManager::Instance().MVPBuffer().BufferData().color.y = 0.45f;
	BufferManager::Instance().MVPBuffer().BufferData().color.z = 0.45f;

	BufferManager::Instance().MVPBuffer().Update(*m_deviceResources);

	_grid->RenderBuffers(context);

	context->IASetInputLayout(_lineDrawingInputLayout.Get());

	// Attach our vertex shader.
	context->VSSetShader(_simpleVertexShader.Get(), nullptr, 0);

	// Send the constant buffer to the graphics device.
	context->VSSetConstantBuffers(0, 1, BufferManager::Instance().MVPBuffer().ConstantBuffer().GetAddressOf());

	// Attach our pixel shader.
	context->PSSetShader(_simplePixelShader.Get(), nullptr, 0);

	// Draw the objects.
	context->DrawIndexed(_grid->IndexCount(), 0, 0);
}

void Sample3DSceneRenderer::DrawAxis(ID3D11DeviceContext2 *context, Axis *axis)
{
	BufferManager::Instance().MVPBuffer().BufferData().color.x = 0.15f;
	BufferManager::Instance().MVPBuffer().BufferData().color.y = 0.15f;
	BufferManager::Instance().MVPBuffer().BufferData().color.z = 0.15f;

	BufferManager::Instance().MVPBuffer().Update(*m_deviceResources);

	axis->RenderBuffers(context);

	context->IASetInputLayout(_lineDrawingInputLayout.Get());

	// Attach our vertex shader.
	context->VSSetShader(_simpleVertexShader.Get(), nullptr, 0);

	// Send the constant buffer to the graphics device.
	context->VSSetConstantBuffers(0, 1, BufferManager::Instance().MVPBuffer().ConstantBuffer().GetAddressOf());

	// Attach our pixel shader.
	context->PSSetShader(_simplePixelShader.Get(), nullptr, 0);

	// Draw the objects.
	context->DrawIndexed(axis->IndexCount(), 0, 0);
}

void Sample3DSceneRenderer::CreateDeviceDependentResources()
{
	// Create sampler.
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	DX::ThrowIfFailed(
		m_deviceResources->GetD3DDevice()->CreateSamplerState(&samplerDesc,
			_spSampler.ReleaseAndGetAddressOf()));

	D3D11_RASTERIZER_DESC rasterizerState;
	rasterizerState.FillMode = D3D11_FILL_SOLID;
	rasterizerState.CullMode = D3D11_CULL_BACK;
	rasterizerState.FrontCounterClockwise = true;
	rasterizerState.DepthBias = false;
	rasterizerState.DepthBiasClamp = 0;
	rasterizerState.SlopeScaledDepthBias = 0;
	rasterizerState.DepthClipEnable = false;
	rasterizerState.ScissorEnable = false;
	rasterizerState.MultisampleEnable = true;
	rasterizerState.AntialiasedLineEnable = true;
	m_deviceResources->GetD3DDevice()->CreateRasterizerState(&rasterizerState, &_pRasterState);

	// Load shaders asynchronously for model rendering...
	auto loadVSTask = DX::ReadDataAsync(L"SampleVertexShader.cso");
	auto loadPSTask = DX::ReadDataAsync(L"SamplePixelShader.cso");

	// After the vertex shader file is loaded, create the shader and input layout.
	auto createVSTask = loadVSTask.then([this](const std::vector<byte>& fileData) {
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateVertexShader(
				&fileData[0],
				fileData.size(),
				nullptr,
				&m_vertexShader
				)
			);

		static const D3D11_INPUT_ELEMENT_DESC vertexDesc [] =
		{
			{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",		0,  DXGI_FORMAT_R32G32B32_FLOAT,	1,	0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",	0,  DXGI_FORMAT_R32G32_FLOAT,		2,	0,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateInputLayout(
				vertexDesc,
				ARRAYSIZE(vertexDesc),
				&fileData[0],
				fileData.size(),
				&m_inputLayout
				)
			);
	});

	// After the pixel shader file is loaded, create the shader and constant buffer.
	auto createPSTask = loadPSTask.then([this](const std::vector<byte>& fileData) {
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreatePixelShader(
				&fileData[0],
				fileData.size(),
				nullptr,
				&m_pixelShader
				)
			);

		BufferManager::Instance().MVPBuffer().Initialise(*m_deviceResources);
	});

	// Load shaders asynchronously for line rendering...
	auto loadVSTask2 = DX::ReadDataAsync(L"SimpleVertexShader.cso");
	auto loadPSTask2 = DX::ReadDataAsync(L"SimplePixelShader.cso");

	// After the vertex shader file is loaded, create the shader and input layout.
	auto createVSTask2 = loadVSTask2.then([this](const std::vector<byte>& fileData) {
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateVertexShader(
				&fileData[0],
				fileData.size(),
				nullptr,
				&_simpleVertexShader
			)
		);

		static const D3D11_INPUT_ELEMENT_DESC SimpleVertexDesc[] =
		{
			{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0,  DXGI_FORMAT_R32G32B32_FLOAT,	1,	0,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateInputLayout(
				SimpleVertexDesc,
				ARRAYSIZE(SimpleVertexDesc),
				&fileData[0],
				fileData.size(),
				&_lineDrawingInputLayout
			)
		);
	});

	// After the pixel shader file is loaded, create the shader and constant buffer.
	auto createPSTask2 = loadPSTask2.then([this](const std::vector<byte>& fileData) {
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreatePixelShader(
				&fileData[0],
				fileData.size(),
				nullptr,
				&_simplePixelShader
			)
		);

		CD3D11_BUFFER_DESC lineDrawingConstantBufferDesc(sizeof(LineDrawingConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&lineDrawingConstantBufferDesc,
				nullptr,
				&_lineDrawingConstantBuffer
			)
		);
	});

	(createPSTask && createVSTask && createPSTask2 && createVSTask2).then([this]()
	{
		m_loadingComplete = true;
	});
}

void Sample3DSceneRenderer::ReleaseDeviceDependentResources()
{
	m_loadingComplete = false;
	m_vertexShader.Reset();
	m_inputLayout.Reset();
	m_pixelShader.Reset();

	BufferManager::Instance().MVPBuffer().Release();

	_lineDrawingConstantBuffer.Reset();

	m_vertexBuffer.Reset();
	m_indexBuffer.Reset();

	for (auto& buffer : _buffers)
	{
		buffer.second.Buffer().Reset();
	}

	_buffers.clear();
	_spTexture.Reset();
}