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

HRESULT ParseFile(shared_ptr<istream> inStr,
				  function<void(const BufferData&)> bufferCallback,
				  function<void(const MaterialData&)> materialCallback,
				  function<void(const TextureData&)> textureCallback,
				  function<void(const TransformData&)> transformCallback,
				  function<void(const SceneNodeData&)> sceneNodeCallback)
{
	try
	{
		GLTFFileData gltfFile;

		gltfFile.EventHandlers().Buffers = [bufferCallback](const BufferData& data)
		{
			bufferCallback(data);
		};

		gltfFile.EventHandlers().Textures = [textureCallback](const TextureData& data)
		{
			textureCallback(data);
		};

		gltfFile.EventHandlers().Materials = [materialCallback](const MaterialData& data)
		{
			materialCallback(data);
		};

		gltfFile.EventHandlers().Transform = [transformCallback](const TransformData& data)
		{
			transformCallback(data);
		};

		gltfFile.EventHandlers().SceneNode = [sceneNodeCallback](const SceneNodeData& data)
		{
			sceneNodeCallback(data);
		};

		gltfFile.Read(inStr);
	}
	catch (const std::exception& ex)
	{
		string str("Exception caught: ");
		str += string(ex.what());
		OutputDebugStringA(str.c_str());
		return E_FAIL;
	}
	return S_OK;
}


