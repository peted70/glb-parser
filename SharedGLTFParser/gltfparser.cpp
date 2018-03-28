// gltfparser.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "GLBGLTFConverter.h"

using namespace GLTFParser;

BufferData::BufferData()
{
	ZeroMemory(&subresource, sizeof(SubresourceData));
	ZeroMemory(&desc, sizeof(BufferDesc));
}

HRESULT ParseFile(istream& inStr, function<void(const BufferData&)> bufferCallback,
				  function<void(const MaterialData&)> materialCallback,
				  function<void(const TextureData&)> textureCallback,
				  function<void(const TransformData&)> transformCallback)
{
	try
	{
		GLTFFileData gltfFile;

		gltfFile.Buffers = [bufferCallback](const BufferData& data)
		{
			bufferCallback(data);
		};

		gltfFile.Textures = [textureCallback](const TextureData& data)
		{
			textureCallback(data);
		};

		gltfFile.Materials = [materialCallback](const MaterialData& data)
		{
			materialCallback(data);
		};

		gltfFile.Transform = [transformCallback](const TransformData& data)
		{
			transformCallback(data);
		};

		gltfFile.Read(inStr);
	}
	catch (const std::exception& ex)
	{
		return E_FAIL;
	}
	return S_OK;
}


