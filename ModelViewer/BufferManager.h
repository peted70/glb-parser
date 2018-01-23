#pragma once

#include "Content\ShaderStructures.h"
#include "Common\DirectXHelper.h"
#include "Common\DeviceResources.h"

using namespace ModelViewer;
using namespace Microsoft::WRL;
using namespace DX;

template<class T>
class ConstantBufferData
{
public:
	ConstantBufferData()
	{
	}

	~ConstantBufferData()
	{
		m_constantBuffer.Reset();
	}

	void Initialise(const DeviceResources& devResources)
	{
		CD3D11_BUFFER_DESC desc(sizeof(T), D3D11_BIND_CONSTANT_BUFFER);
		auto device = devResources.GetD3DDevice();
		DX::ThrowIfFailed(device->CreateBuffer(&desc, nullptr, &m_constantBuffer));
	}
	void Release() { m_constantBuffer.Reset(); }
	void Update(const DeviceResources& devResources)
	{
		assert(ConstantBuffer().Get());
		auto context = devResources.GetD3DDeviceContext();
		context->UpdateSubresource1(ConstantBuffer().Get(), 0, NULL, &(BufferData()), 0, 0, 0);
	}

	T& BufferData() { return _bufferData; }
	ComPtr<ID3D11Buffer> ConstantBuffer() { return m_constantBuffer; }

private:
	T _bufferData;
	ComPtr<ID3D11Buffer> m_constantBuffer;
};

class BufferManager
{
public:
	static BufferManager& Instance()
	{
		static BufferManager instance;
		return instance;
	}
	BufferManager(BufferManager const&) = delete;
	void operator=(BufferManager const&) = delete;

	ConstantBufferData<ModelViewProjectionConstantBuffer>& MVPBuffer()
	{
		return _mvpBuffer;
	}

private:
	BufferManager();
	ConstantBufferData<ModelViewProjectionConstantBuffer> _mvpBuffer;
};
