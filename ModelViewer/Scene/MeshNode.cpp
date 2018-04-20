#include "pch.h"
#include "MeshNode.h"
#include "Common\DirectXHelper.h"
#include "BufferManager.h"
#include "ImgUtils.h"
#include "DxUtils.h"

using namespace Platform;

const char *defineLookup[] = 
{
	"HAS_BASECOLORMAP",
	"HAS_NORMALMAP",
	"HAS_EMISSIVEMAP",
	"HAS_OCCLUSIONMAP",
	"HAS_METALROUGHNESSMAP"
};

const char *one = "1";

MeshNode::MeshNode(int index) :
	GraphContainerNode(index)
{
}

MeshNode::~MeshNode()
{
}

void MeshNode::Initialise(const std::shared_ptr<DX::DeviceResources>& deviceResources)
{
	GraphContainerNode::Initialise(deviceResources);
	CreateDeviceDependentResources();
}

void MeshNode::AfterLoad()
{
	CompileAndLoadVertexShader();
	CompileAndLoadPixelShader();
	GraphContainerNode::AfterLoad();
}

void MeshNode::CompileAndLoadVertexShader()
{
	// Compile vertex shader
	ID3DBlob *vsBlob = nullptr;

	// Work out the path to the shader...
	auto sf = Windows::ApplicationModel::Package::Current->InstalledLocation;
	String^ path(L"\\Assets\\Shaders\\");
	String^ filePath = sf->Path + path + "pbrvertex.hlsl";

	// Currently the compiler defines consist of UV and NORMALS...
	// Currently always define NORMALS but only define UV if we have tex coords buffer
	auto texcoords = _buffers.find(L"TEXCOORD_0");
	if (texcoords != _buffers.end())
	{
		m_hasUVs = true;
	}

	int defineCount = 1;
	if (m_hasUVs)
	{
		defineCount++;
	}

	auto defines = make_unique<D3D_SHADER_MACRO[]>(defineCount + 1);

	int idx = 0;
	(defines.get())[idx].Name = "NORMALS";
	(defines.get())[idx].Definition = one;
	idx++;
	if (m_hasUVs)
	{
		(defines.get())[idx].Name = "UV";
		(defines.get())[idx].Definition = one;
		idx++;
	}
	(defines.get())[idx].Name = nullptr;
	(defines.get())[idx].Definition = nullptr;

	auto hr = DXUtils::CompileShader(filePath->Data(), defines.get(), "main", "vs_5_0", &vsBlob);
	if (FAILED(hr))
	{
		Utility::Out(L"Failed compiling vertex shader %08X\n", hr);
		return;
	}

	DX::ThrowIfFailed(
		DevResources()->GetD3DDevice()->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(),
			nullptr, &m_vertexShader));

	Utility::Out(L"Loaded Vertex Shader");

	static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0,  DXGI_FORMAT_R32G32B32_FLOAT,	1,	0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0,  DXGI_FORMAT_R32G32_FLOAT,		2,	0,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	auto dynamicVertexDesc = make_unique<D3D11_INPUT_ELEMENT_DESC[]>(m_hasUVs ? 3 : 2);
	(dynamicVertexDesc.get())[0] = vertexDesc[0];
	(dynamicVertexDesc.get())[1] = vertexDesc[1];
	if (m_hasUVs)
	{
		(dynamicVertexDesc.get())[2] = vertexDesc[2];
	}

	// Now create an Input layout that matches..
	DX::ThrowIfFailed(
		DevResources()->GetD3DDevice()->CreateInputLayout(
			dynamicVertexDesc.get(),
			m_hasUVs ? 3 : 2,
			//ARRAYSIZE(dynamicVertexDesc.get()),
			vsBlob->GetBufferPointer(), 
			vsBlob->GetBufferSize(),
			&m_inputLayout));
}

void MeshNode::CompileAndLoadPixelShader()
{
	// Compile pixel shader shader
	ID3DBlob *psBlob = nullptr;

	// Work out the path to the shader...
	auto sf = Windows::ApplicationModel::Package::Current->InstalledLocation;
	String^ path(L"\\Assets\\Shaders\\");
	String^ filePath = sf->Path + path + "pbrpixel.hlsl";

	auto textures = _material->Textures();
	
	// Allocate the defines map...
	int count = textures.size();
	auto defines = make_unique<D3D_SHADER_MACRO[]>(count + 1);

	// Iterate through all textures and set them as shader resources...
	int idx = 0;
	for (auto txItr = textures.begin(); txItr != textures.end(); ++txItr)
	{
		auto textureWrapper = txItr->second;
		auto type = textureWrapper->Type();

		const char *define = defineLookup[type];

		(defines.get())[idx].Name = define;
		(defines.get())[idx].Definition = one;

		idx++;
	}

	(defines.get())[idx].Name = nullptr;
	(defines.get())[idx].Definition = nullptr;

	auto hr = DXUtils::CompileShader(filePath->Data(), defines.get(), "main", "ps_5_0", &psBlob);
	if (FAILED(hr))
	{
		Utility::Out(L"Failed compiling pixel shader %08X\n", hr);
		return;
	}

	DX::ThrowIfFailed(
		DevResources()->GetD3DDevice()->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(),
		nullptr, &m_pixelShader));

	Utility::Out(L"Loaded Pixel Shader");
}

void MeshNode::CreateDeviceDependentResources()
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

	DX::ThrowIfFailed(DevResources()->GetD3DDevice()->CreateSamplerState(&samplerDesc, _spSampler.ReleaseAndGetAddressOf()));

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
	DX::ThrowIfFailed(DevResources()->GetD3DDevice()->CreateRasterizerState(&rasterizerState, _pRasterState.ReleaseAndGetAddressOf()));
}

void MeshNode::Draw(SceneContext& context, XMMATRIX model)
{
	if (!m_loadingComplete)
		return;

	//XMMATRIX mat;
	//if (_hasMatrix)
	//{ 
	//	mat = _matrix;
	//}
	//else
	//{
	//	mat = XMMatrixTranspose(XMMatrixAffineTransformation(XMLoadFloat3(&_scale), XMLoadFloat3(&emptyVector), XMLoadFloat4(&_rotation), XMLoadFloat3(&_translation)));
	//}

	//// Prepare to pass the updated model matrix to the shader 
	//XMStoreFloat4x4(&BufferManager::Instance().MVPBuffer().BufferData().model, /*XMMatrixTranspose(*/mat/*)*/);

	//BufferManager::Instance().MVPBuffer().Update(*(DevResources()));

	unsigned int indexCount = 0;
	bool indexed = false;

	// Get POSITIONS & NORMALS..
	auto pos = _buffers.find(L"POSITION");
	auto posBuffer = pos->second.Buffer();

	auto texcoords = _buffers.find(L"TEXCOORD_0");
	ComPtr<ID3D11Buffer> texcoordBuffer;
	if (texcoords != _buffers.end())
	{
		texcoordBuffer = texcoords->second.Buffer();
	}

	auto normals = _buffers.find(L"NORMAL");
	ComPtr<ID3D11Buffer> normalBuffer;
	if (normals != _buffers.end())
	{
		normalBuffer = normals->second.Buffer();
	}

	ID3D11Buffer *vbs[] =
	{
		*(posBuffer.GetAddressOf()),
		*(normalBuffer.GetAddressOf()),
		*(texcoordBuffer.GetAddressOf())
	};

	unsigned int strides[] = { 3 * sizeof(float), 3 * sizeof(float), 2 * sizeof(float) };
	unsigned int offsets[] = { 0, 0, 0 };
	context.context().IASetVertexBuffers(0, 3, vbs, strides, offsets);

	auto indices = _buffers.find(L"INDICES");
	if (indices != _buffers.end())
	{
		indexed = true;
		m_indexCount = indices->second.Data()->BufferDescription->Count;
		context.context().IASetIndexBuffer(
			indices->second.Buffer().Get(),
			DXGI_FORMAT_R16_UINT, // Each index is one 16-bit unsigned integer (short).
			0
		);
	}

	context.context().IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context.context().IASetInputLayout(m_inputLayout.Get());

	// Attach our vertex shader.
	assert(m_vertexShader.Get());
	context.context().VSSetShader(m_vertexShader.Get(), nullptr, 0);

	// Send the constant buffer to the graphics device.
	context.context().VSSetConstantBuffers1(0, 1, BufferManager::Instance().MVPBuffer().ConstantBuffer().GetAddressOf(),
		nullptr, nullptr);

	// Attach our pixel shader.
	context.context().PSSetShader(m_pixelShader.Get(), nullptr, 0);
	context.context().PSSetConstantBuffers(0, 1, BufferManager::Instance().PerFrameBuffer().ConstantBuffer().GetAddressOf());
	context.context().PSSetConstantBuffers(1, 1, BufferManager::Instance().PerObjBuffer().ConstantBuffer().GetAddressOf());

	// Iterate through all textures and set them as shader resources...
	auto textures = _material->Textures();

	// Setting the textures and samplers using the GLTF index as the destination slot
	// We load some textures twice here so maybe re-visit..
	for (auto txItr = textures.begin(); txItr != textures.end(); ++txItr)
	{
		auto textureWrapper = txItr->second;

		// Set texture and sampler.
		auto sampler = textureWrapper->GetSampler().Get();
		context.context().PSSetSamplers(textureWrapper->Type(), 1, &sampler);
		
		//Utility::Out(L"Set texture sampler at slot %d", textureWrapper->Type());
		auto texture = textureWrapper->GetShaderResourceView().Get();
		context.context().PSSetShaderResources(textureWrapper->Type(), 1, &texture);
	}

	if (indexed)
	{
		context.context().DrawIndexed(m_indexCount, 0, 0);
	}
	else
	{
		context.context().Draw(m_indexCount, 0);
	}

	GraphContainerNode::Draw(context, model);
}

void BoundingSphereFromBoundingBox()
{

}

void MeshNode::CreateBuffer(WinRTGLTFParser::GLTF_BufferData ^ data)
{
	int bindFlags = 0;
	if (data->BufferDescription->BufferContentType == L"POSITION" ||
		data->BufferDescription->BufferContentType == L"NORMAL" ||
		data->BufferDescription->BufferContentType == L"TEXCOORD_0")
	{
		bindFlags = D3D11_BIND_VERTEX_BUFFER;
	}
	else if (data->BufferDescription->BufferContentType == L"INDICES")
	{
		bindFlags = D3D11_BIND_INDEX_BUFFER;
	}
	else
	{
		return;
	}

	if (data->BufferDescription->BufferContentType == L"POSITION")
	{
		_bbox = BoundingBox<float>::CreateBoundingBoxFromVertexBuffer((void *)data->BufferDescription->pSysMem, data->SubResource->ByteWidth);
	}

	// Create the buffers...
	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = (void *)data->BufferDescription->pSysMem;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;

	std::wstring type(data->BufferDescription->BufferContentType->Data());
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;

	CD3D11_BUFFER_DESC vertexBufferDesc(data->SubResource->ByteWidth, bindFlags);
	vertexBufferDesc.StructureByteStride = data->SubResource->StructureByteStride;

	auto device = DevResources()->GetD3DDevice();
	DX::ThrowIfFailed(
		device->CreateBuffer(
			&vertexBufferDesc,
			&vertexBufferData,
			&buffer
		)
	);

	BufferWrapper bw(data, buffer);
	_buffers[type] = bw;
}

void MeshNode::CreateMaterial(GLTF_MaterialData ^ data)
{
	_material = make_shared<NodeMaterial>();
	_material->Initialise(data);
}

void MeshNode::CreateTransform(GLTF_TransformData^ data)
{
	// If we are handed a matrix, just apply that, otherwise break down into scale, rotate, translate
	// and generate the matrix from those..createbuffer
	if (data->hasMatrix)
	{
		_hasMatrix = true;
		XMFLOAT4X4 mat = 
		{
			data->matrix[0], 
			data->matrix[1],
			data->matrix[2],
			data->matrix[3],
			data->matrix[4],
			data->matrix[5],
			data->matrix[6],
			data->matrix[7],
			data->matrix[8],
			data->matrix[9],
			data->matrix[10],
			data->matrix[11],
			data->matrix[12],
			data->matrix[13],
			data->matrix[14],
			data->matrix[15]
		};
		

		_matrix = XMLoadFloat4x4(&mat);
		XMStoreFloat4x4(&BufferManager::Instance().MVPBuffer().BufferData().model, XMMatrixTranspose(_matrix));
	}
	else
	{
		_scale = { data->scale[0], data->scale[1], data->scale[2] };
		_translation = { data->translation[0], data->translation[1], data->translation[2] };

		// Using the conversion from right-handed coordinate system of OpenGL to left-handed coordinate
		// system of DirectX
		// q.x, q.y, -q.z, -q.w
		//
		//_rotation = { data->rotation[0], data->rotation[1], -data->rotation[2], -data->rotation[3] };
		_rotation = { data->rotation[0], data->rotation[1], data->rotation[2], data->rotation[3] };

		//XMVECTOR scale = { 1.0, 1.0, 1.0 };
		//XMVECTOR translation = { 0.0, 0.0, 0.0 };

		//XMVECTOR ypr = { 0.0, 180.0, 0.0 };
		//// generate a quaternion from angle for testing...
		//XMVECTOR rotQuat = XMQuaternionRotationRollPitchYawFromVector(ypr);

		//auto matrix = XMMatrixRotationQuaternion(rotQuat);

		//_scale = { 1.0, 1.0, 1.0 };
		//_translation = { 0.0, 0.0, 0.0 };
		//_rotation = { 0.0, 0.0, 0.0, 0.0 };

		// Create matrix from scale
		auto matrix = XMMatrixAffineTransformation(XMLoadFloat3(&_scale), XMLoadFloat3(&emptyVector), XMLoadFloat4(&_rotation), XMLoadFloat3(&_translation));

		// Prepare to pass the updated model matrix to the shader 
		XMStoreFloat4x4(&BufferManager::Instance().MVPBuffer().BufferData().model, XMMatrixTranspose(matrix));
	}
}

void MeshNode::CreateTexture(WinRTGLTFParser::GLTF_TextureData ^ data)
{
	// Don't want to allocate textures we have already allocated..
	// Disabled temporarily
	//if (_material->HasTexture(data->Idx))
	//	return;

	// Create texture.
	D3D11_TEXTURE2D_DESC txtDesc = {};
	txtDesc.MipLevels = txtDesc.ArraySize = 1;

	// TODO: Fix this - understand when to use sRGB and RGB 
	txtDesc.Format = (data->Type == 4 || data->Type == 3) ? DXGI_FORMAT_R8G8B8A8_UNORM : DXGI_FORMAT_R8G8B8A8_UNORM;
	txtDesc.SampleDesc.Count = 1;
	txtDesc.Usage = D3D11_USAGE_IMMUTABLE;
	txtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	uint32_t width;
	uint32_t height;

	auto image = ImgUtils::LoadRGBAImage((void *)data->pSysMem, data->DataSize, width, height);

	txtDesc.Width = width;
	txtDesc.Height = height;

	D3D11_SUBRESOURCE_DATA initialData = {};
	initialData.pSysMem = image.data();
	initialData.SysMemPitch = txtDesc.Width * sizeof(uint32_t);

	ComPtr<ID3D11Texture2D> tex;
	DX::ThrowIfFailed(
		DevResources()->GetD3DDevice()->CreateTexture2D(&txtDesc, &initialData,
			tex.GetAddressOf()));

	ComPtr<ID3D11ShaderResourceView> textureResourceView;
	DX::ThrowIfFailed(
		DevResources()->GetD3DDevice()->CreateShaderResourceView(tex.Get(),
			nullptr, textureResourceView.ReleaseAndGetAddressOf()));

	// Create sampler.
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ComPtr<ID3D11SamplerState> texSampler;
	DX::ThrowIfFailed(DevResources()->GetD3DDevice()->CreateSamplerState(&samplerDesc, texSampler.ReleaseAndGetAddressOf()));

	_material->AddTexture(data->Idx, data->Type, tex, textureResourceView, texSampler);
}

