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

HRESULT ParseFile(std::istream& inStr, std::function<void(const BufferData&)> bufferCallback)
{
	GLTFFileData gltfFile;

	gltfFile.Buffers = [bufferCallback](const BufferData& data)
	{
		bufferCallback(data);
	};

	gltfFile.Read(inStr);
	return S_OK;
}


