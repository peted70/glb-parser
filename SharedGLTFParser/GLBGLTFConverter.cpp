// GLBGLTFConverter.cpp : Defines the entry point for the console application.
//
#pragma once

#include "stdafx.h"
#include "GLBGLTFConverter.h"

#include <strsafe.h>

const wchar_t *newline = L"\n";

void Out(LPCTSTR sFormat, ...)
{
#ifndef NO_OUTPUT
	va_list argptr;
	va_start(argptr, sFormat);
	wchar_t buffer[2000];
	HRESULT hr = StringCbVPrintf(buffer, sizeof(buffer), sFormat, argptr);
	if (STRSAFE_E_INSUFFICIENT_BUFFER == hr || S_OK == hr)
	{
		wcsncat_s(buffer, newline, (rsize_t)sizeof(buffer));
		OutputDebugString(buffer);
	}
	else
		OutputDebugString(L"StringCbVPrintf error.");
#endif
}

using namespace GLTFParser;
using namespace std;
using namespace rapidjson;

enum class TextureType
{
	BaseColour = 0,
	Normal = 1,
	Emission = 2,
	Occlusion = 3,
	MetallicRoughness = 4
};

/**	Header
The 20 - byte header consists of the following five 4 - byte entries :
magic is the ASCII string 'glTF', and can be used to identify the arraybuffer as Binary glTF.
version is an uint32 that indicates the version of the Binary glTF container format.Currently available versions are shown in Table 1.
length is the total length of the Binary glTF, including header, content, and body, in bytes.
contentLength is the length, in bytes, of the glTF content.It must be greater than zero.
contentFormat specifies the format of the glTF content.A list of all valid values currently available is in Table 2.
Table 1: Valid values for version
Decimal
Hex
Description
1
0x00000001
Version 1
Table 2: Valid values for contentFormat
Decimal
Hex
Description
0
0x00000000
JSON */
GLTFHeader::GLTFHeader() :
	_version(0),
	_length(0)
{
}

void GLTFHeader::Read(istream& file)
{
	file.read((char *)(&_magic[0]), 4);
	file.read((char *)&_version, sizeof(unsigned int));
	file.read((char *)&_length, sizeof(unsigned int));
}

struct Component
{
	const char *Type;
	int NumBytes;
};

Component NumComponentsByType[] =
{
	{ "SCALAR", 1 },
	{ "VEC2", 2 },
	{ "VEC3", 3 },
	{ "VEC4", 4 },
	{ "MAT2", 4 },
	{ "MAT3", 9 },
	{ "MAT4", 16 }
};

GLTFChunk *GLTFChunk::Read(istream& file)
{
	// Ensure we are on a 4-byte boundary..
	streamoff currPos = file.tellg();
	streamoff padding = currPos % 4;
	if (padding > 0)
	{
		file.seekg(padding, file.cur);
	}
	else
	{
		//file.seekg((streamoff)4, file.cur);
	}
		
	file.read((char *)(&_chunkLength), sizeof(unsigned int));
	file.read((char *)&_chunkType, sizeof(unsigned int));

	if (_chunkType != ChunkType::BIN && _chunkType != ChunkType::JSON)
	{
		// Unknown chunk type just skip over it..
		file.seekg(_chunkLength);
	}
	else
	{
		_chunkData = make_unique<unsigned char[]>(_chunkLength);
		file.read((char *)_chunkData.get(), _chunkLength);

		int toAdd = _chunkLength % 4;
		if (toAdd > 0)
		{
			file.seekg((streamoff)toAdd, file.cur);
		}

		return this;
	}
	return nullptr;
}

typedef vector<shared_ptr<GLTFChunk>> ChunkList;

void GLTFFileData::PopulateDocument()
{
	const char *str = (const char *)_headerChunk->ChunkData();
	_document.Parse((const char *)_headerChunk->ChunkData(), 
		_headerChunk->ChunkLength());
}

void GLTFFileData::Read(istream& file)
{
	_header = make_unique<GLTFHeader>();
	_header->Read(file);

	_chunks = make_unique<ChunkList>();

	while (!file.eof())
	{
		streamoff pos = file.tellg();
		int length = _header->length();

		if (pos >= (streamoff)length)
			break;
			
		auto chunk = make_shared<GLTFChunk>();
		auto chunkData = chunk.get()->Read(file);
		if (chunkData == nullptr)
			continue;
		if (chunkData->IsJson())
			_headerChunk = chunkData;
		else if (chunkData->IsBinary())
			_binaryChunk = chunkData;

		_chunks.get()->push_back(chunk);
	}

	PopulateDocument();
	CheckExtensions(document());
	ParseDocument(document(), EventHandlers());
}

const Value& FindFirstMesh(const Value& root, const Value& nodeList)
{
	if (root.HasMember("mesh"))
	{
		return root;
	}

	if (root.HasMember("children"))
	{
		const Value& children = root["children"];

		for (SizeType p = 0; p < children.Size(); p++)
		{
			const Value& child = children[p];
			int nodeIdx = child.GetInt();
			const Value& childNode = nodeList[nodeIdx];
			return FindFirstMesh(childNode, nodeList);
		}
	}
}

void GetBufferFromBufferView(const Value& bufferView, unsigned char *BinaryChunkStart, int byteOffset, 
							 unsigned char **buffer, unsigned int& bufferLength)
{
	int bufferIdx = bufferView["buffer"].GetInt();
	int bufferViewByteOffset = 0;
	if (bufferView.HasMember("byteOffset"))
		bufferViewByteOffset = bufferView["byteOffset"].GetInt();
	int byteLength = bufferView["byteLength"].GetInt();
	
	*buffer = BinaryChunkStart + byteOffset + bufferViewByteOffset;
}

int ComponentTypeToNumBytes(int compType)
{
	switch (compType)
	{
	case 5120:
	case 5121:
		return 1;
	case 5122:
	case 5123:
		return 2;
	case 5125:
	case 5126:
		return 4;
	}
	return 4;
}

void FindBufferAndNotify(const Value& bufferView, const Value& bufferList, const char *contentType, unsigned char *BinaryChunkStart,
						 function<void(const BufferData&)> OnBuffer, int count, int byteOffset, int componentType)
{
	int bufferIdx = bufferView["buffer"].GetInt();
	int bufferViewByteOffset = 0;
	if (bufferView.HasMember("byteOffset"))
		bufferViewByteOffset = bufferView["byteOffset"].GetInt();
	int byteStride = 0;
	if (bufferView.HasMember("byteStride"))
		byteStride = bufferView["byteStride"].GetInt();
	int byteLength = bufferView["byteLength"].GetInt();
	int storageSize = ComponentTypeToNumBytes(componentType);

	// Finally, we have enough info to retrieve the buffer data...
	assert(bufferList.IsArray());
	const Value& buff = bufferList[bufferIdx];
	//byteLength = buff["byteLength"].GetInt();

	auto binaryChunkBuffer = BinaryChunkStart;

	BufferData data;
	data.subresource.pSysMem = binaryChunkBuffer + byteOffset + bufferViewByteOffset;
	data.subresource.SysMemPitch = 0;
	data.subresource.SysMemSlicePitch = 0;
	data.desc.ByteWidth = byteLength - byteOffset;
	data.desc.BufferContentType = contentType;
	data.desc.StructureByteStride = byteStride;

	//data.desc.Count = byteLength;
	data.desc.Count = count;

	// Notify that we have a buffer and pass parameters required to create it
	stringstream debugText;
	debugText << "Buffer:" << contentType << endl;
	OutputDebugStringA(debugText.str().c_str());
	OnBuffer(data);
}

void LoadTextureFromIdx(unsigned int textureId, TextureType type, const Document& document, 
						unsigned char *BinaryChunkStart, 
					    function<void(const TextureData&)> OnTexture)
{
	Out(L"Loading texture [type - %d, index - [%d]", type, textureId);

	const Value& textures = document["textures"];
	assert(textures.IsArray());

	const Value& tex = textures[textureId];
	if (tex.HasMember("source"))
	{
		int texSourceId = tex["source"].GetInt();
		const Value& images = document["images"];
		assert(images.IsArray());
		const Value& image = images[texSourceId];
		if (image.HasMember("bufferView"))
		{
			int bufferViewId = image["bufferView"].GetInt();
			const Value& bufferViews = document["bufferViews"];
			assert(bufferViews.IsArray());
			const Value& bufferView = bufferViews[bufferViewId];

			int bufferIdx = bufferView["buffer"].GetInt();
			int bufferViewByteOffset = 0;
			if (bufferView.HasMember("byteOffset"))
				bufferViewByteOffset = bufferView["byteOffset"].GetInt();
			int byteLength = bufferView["byteLength"].GetInt();

			//*buffer = BinaryChunkStart + byteOffset + bufferViewByteOffset;

			if (image.HasMember("mimeType"))
			{
				auto mimeTypeStr = image["mimeType"].GetString();
				TextureData data;
				data.pSysMem = BinaryChunkStart + bufferViewByteOffset;
				data.dataSize = byteLength;
				data.idx = textureId;
				data.type = (unsigned int)type;
				OnTexture(data);
			}
		}
	}
}

void GLTFFileData::LoadTransform(const Document& document, const Value& mNode, const Callbacks& callbacks)
{
	TransformData tdata;
	tdata.hasMatrix = false;
	if (mNode.HasMember("rotation"))
	{
		tdata.rotation[0] = mNode["rotation"][0].GetDouble();
		tdata.rotation[1] = mNode["rotation"][1].GetDouble();
		tdata.rotation[2] = mNode["rotation"][2].GetDouble();
		tdata.rotation[3] = mNode["rotation"][3].GetDouble();
	}

	if (mNode.HasMember("translation"))
	{
		tdata.translation[0] = mNode["translation"][0].GetDouble();
		tdata.translation[1] = mNode["translation"][1].GetDouble();
		tdata.translation[2] = mNode["translation"][2].GetDouble();
	}

	if (mNode.HasMember("scale"))
	{
		tdata.scale[0] = mNode["scale"][0].GetDouble();
		tdata.scale[1] = mNode["scale"][1].GetDouble();
		tdata.scale[2] = mNode["scale"][2].GetDouble();
	}

	if (mNode.HasMember("matrix"))
	{
		tdata.hasMatrix = true;

		tdata.matrix[0] = mNode["matrix"][0].GetDouble();
		tdata.matrix[1] = mNode["matrix"][1].GetDouble();
		tdata.matrix[2] = mNode["matrix"][2].GetDouble();
		tdata.matrix[3] = mNode["matrix"][3].GetDouble();

		tdata.matrix[4] = mNode["matrix"][4].GetDouble();
		tdata.matrix[5] = mNode["matrix"][5].GetDouble();
		tdata.matrix[6] = mNode["matrix"][6].GetDouble();
		tdata.matrix[7] = mNode["matrix"][7].GetDouble();

		tdata.matrix[8] = mNode["matrix"][8].GetDouble();
		tdata.matrix[9] = mNode["matrix"][9].GetDouble();
		tdata.matrix[10] = mNode["matrix"][10].GetDouble();
		tdata.matrix[11] = mNode["matrix"][11].GetDouble();

		tdata.matrix[12] = mNode["matrix"][12].GetDouble();
		tdata.matrix[13] = mNode["matrix"][13].GetDouble();
		tdata.matrix[14] = mNode["matrix"][14].GetDouble();
		tdata.matrix[15] = mNode["matrix"][15].GetDouble();
	}

	callbacks.Transform(tdata);
}

void GLTFFileData::LoadMeshNode(const Document& document, const Value& mNode, const Callbacks& callbacks)
{
	LoadTransform(document, mNode, callbacks);

	int meshIndex = mNode["mesh"].GetInt();

	//string meshName = sceneNode["name"].GetString();

	// Lookup the mesh in the 'meshes' collection...
	const Value& meshesArray = document["meshes"];
	assert(meshesArray.IsArray());
	const Value& meshNode = meshesArray[meshIndex];

	// Process the meshes transform...

	const Value& primitives = meshNode["primitives"];
	//meshName = meshNode["name"].GetString();

	assert(primitives.IsArray());

	// Loop around the primitives that make up the mesh and create 
	// buffers for them...
	for (SizeType p = 0; p < primitives.Size(); p++)
	{
		const Value& primitiveObject = primitives[p];
		if (primitiveObject.HasMember("extensions"))
		{
			const Value& exts = primitiveObject["extensions"];
			if (exts.HasMember("KHR_draco_mesh_compression"))
			{
				// Read the BufferView and process the attributes
				// maybe factor the processing of attributes into a function
				// taking a parameter to determine whether or not to decompress

			}
		}

		const Value& attributesObject = primitiveObject["attributes"];
		int indicesIdx = primitiveObject["indices"].GetInt();
		int materialIdx = primitiveObject["material"].GetInt();

		const Value& materials = document["materials"];
		assert(materials.IsArray());
		const Value& material = materials[materialIdx];

		MaterialData data;
		data.MaterialName = material.HasMember("name") ? material["name"].GetString() : "unnamed-material";
		if (material.HasMember("emissiveFactor"))
		{
			const Value& emmissiveFactor = material["emissiveFactor"];
			assert(emmissiveFactor.IsArray());

			data.emmissiveFactor[0] = emmissiveFactor[0].GetFloat();
			data.emmissiveFactor[1] = emmissiveFactor[1].GetFloat();
			data.emmissiveFactor[2] = emmissiveFactor[2].GetFloat();
		}
		if (material.HasMember("pbrMetallicRoughness"))
		{
			const Value& pbrMetallicRoughness = material["pbrMetallicRoughness"];

			if (pbrMetallicRoughness.HasMember("baseColorTexture"))
			{
				const Value& baseColorTexture = pbrMetallicRoughness["baseColorTexture"];
				const Value& texture = baseColorTexture["index"];
				data.Pbrmetallicroughness_Basecolortexture = texture.GetInt();
			}

			if (pbrMetallicRoughness.HasMember("metallicRoughnessTexture"))
			{
				const Value& baseColorTexture = pbrMetallicRoughness["metallicRoughnessTexture"];
				const Value& texture = baseColorTexture["index"];
				data.Pbrmetallicroughness_Metallicroughnesstexture = texture.GetInt();
			}
		}

		if (material.HasMember("emissiveTexture"))
		{
			const Value& emissiveTexture = material["emissiveTexture"];
			const Value& texture = emissiveTexture["index"];
			data.Emissivetexture = texture.GetInt();
		}

		if (material.HasMember("occlusionTexture"))
		{
			const Value& occlusionTexture = material["occlusionTexture"];
			const Value& texture = occlusionTexture["index"];
			data.Occlusiontexture = texture.GetInt();
		}

		if (material.HasMember("normalTexture"))
		{
			const Value& normalTexture = material["normalTexture"];
			const Value& texture = normalTexture["index"];
			data.Normaltexture = texture.GetInt();
		}

		callbacks.Materials(data);

		if (material.HasMember("pbrMetallicRoughness"))
		{
			const Value& pbrMetallicRoughness = material["pbrMetallicRoughness"];
			if (pbrMetallicRoughness.HasMember("baseColorTexture"))
			{
				const Value& baseColorTexture = pbrMetallicRoughness["baseColorTexture"];
				const Value& texture = baseColorTexture["index"];
				int textureId = texture.GetInt();

				LoadTextureFromIdx(textureId, TextureType::BaseColour, document, BinaryChunk()->ChunkData(), callbacks.Textures);
			}
			if (pbrMetallicRoughness.HasMember("metallicRoughnessTexture"))
			{
				const Value& baseColorTexture = pbrMetallicRoughness["metallicRoughnessTexture"];
				const Value& texture = baseColorTexture["index"];
				int textureId = texture.GetInt();

				LoadTextureFromIdx(textureId, TextureType::MetallicRoughness, document, BinaryChunk()->ChunkData(), callbacks.Textures);
			}
		}
		if (material.HasMember("emissiveTexture"))
		{
			const Value& emissiveTexture = material["emissiveTexture"];
			const Value& texture = emissiveTexture["index"];
			LoadTextureFromIdx(texture.GetInt(), TextureType::Emission, document, BinaryChunk()->ChunkData(), callbacks.Textures);
		}

		if (material.HasMember("occlusionTexture"))
		{
			const Value& occlusionTexture = material["occlusionTexture"];
			const Value& texture = occlusionTexture["index"];
			LoadTextureFromIdx(texture.GetInt(), TextureType::Occlusion, document, BinaryChunk()->ChunkData(), callbacks.Textures);
		}

		if (material.HasMember("normalTexture"))
		{
			const Value& normalTexture = material["normalTexture"];
			const Value& texture = normalTexture["index"];
			LoadTextureFromIdx(texture.GetInt(), TextureType::Normal, document, BinaryChunk()->ChunkData(), callbacks.Textures);
		}

		if (primitiveObject.HasMember("mode"))
		{
			int mode = primitiveObject["mode"].GetInt();
		}

		// ------------------------------------------------------
		const Value& accessors = document["accessors"];
		assert(accessors.IsArray());
		const Value& Accessor = accessors[indicesIdx];

		int bufferViewIdx = Accessor["bufferView"].GetInt();
		int count = Accessor["count"].GetInt();
		int byteOffset = 0;
		if (Accessor.HasMember("byteOffset"))
			byteOffset = Accessor["byteOffset"].GetInt();
		int componentType = 0;
		if (Accessor.HasMember("componentType"))
			componentType = Accessor["componentType"].GetInt();

		const Value& bufferViews = document["bufferViews"];
		assert(bufferViews.IsArray());
		const Value& indicesBufferView = bufferViews[bufferViewIdx];

		FindBufferAndNotify(indicesBufferView, document["buffers"], "INDICES", BinaryChunk()->ChunkData(),
			callbacks.Buffers, count, byteOffset, componentType);

		// ------------------------------------------------------

		for (Value::ConstMemberIterator itr = attributesObject.MemberBegin();
			itr != attributesObject.MemberEnd(); ++itr)
		{
			int accessorIdx = itr->value.GetInt();

			// Lookup the accessor..
			const Value& accessors = document["accessors"];
			assert(accessors.IsArray());
			const Value& Accessor = accessors[accessorIdx];

			int bufferViewIdx = Accessor["bufferView"].GetInt();

			int count = Accessor["count"].GetInt();
			int byteOffset = 0;
			if (Accessor.HasMember("byteOffset"))
				byteOffset = Accessor["byteOffset"].GetInt();
			int componentType = 0;
			if (Accessor.HasMember("componentType"))
				componentType = Accessor["componentType"].GetInt();

			// Get the raw data from the binary chunk...
			const Value& bufferViews = document["bufferViews"];
			assert(bufferViews.IsArray());

			const Value& bufferView = bufferViews[bufferViewIdx];

			FindBufferAndNotify(bufferView, document["buffers"], itr->name.GetString(), BinaryChunk()->ChunkData(),
				callbacks.Buffers, count, byteOffset, componentType);
		}
	}
}

void GLTFFileData::LoadSceneNode(const Document& document, const Value& sceneNode, const Callbacks& callbacks,
	int nodeIndex, int parentIndex)
{
	SceneNodeData data;
	data.nodeIndex = nodeIndex;
	data.parentIndex = parentIndex;

	string name = "node-" + std::to_string(nodeIndex);

	if (sceneNode.HasMember("name"))
	{
		name = sceneNode["name"].GetString();
	}

	if (sceneNode.HasMember("mesh"))
	{
		Out(L"Loading %S", name.c_str());

		data.isMesh = true;
		data.Name = name.c_str();
		callbacks.SceneNode(data);

		// Can it have children too?
		LoadMeshNode(document, sceneNode, callbacks);
	}
	else if (sceneNode.HasMember("children"))
	{
		// Notify a new node and load it's transforms...
		string name = "node-" + std::to_string(nodeIndex);

		if (sceneNode.HasMember("name"))
		{
			name = sceneNode["name"].GetString();
		}
	
		Out(L"Loading scene node %S", name.c_str());

		data.isMesh = false;
		data.Name = name.c_str();
		callbacks.SceneNode(data);
		
		LoadTransform(document, sceneNode, callbacks);

		auto& children = sceneNode["children"];
		// iterate children and recurse for each child node
		assert(children.IsArray());
		for (SizeType j = 0; j < children.Size(); j++)
		{
			int childIndex = children[j].GetInt();
			const Value& nodesArray = document["nodes"];
			assert(nodesArray.IsArray());
			const Value& childNode = nodesArray[childIndex];

			LoadSceneNode(document, childNode, callbacks, childIndex, nodeIndex);
		}
	}
}

void GLTFFileData::LoadScene(const Document& document, const Value& scene, const Callbacks& callbacks)
{
	const Value& nodes = scene["nodes"];
	assert(nodes.IsArray());
	for (SizeType j = 0; j < nodes.Size(); j++)
	{
		int nodeIndex = nodes[j].GetInt();
		const Value& nodesArray = document["nodes"];
		assert(nodesArray.IsArray());
		const Value& sceneNode = nodesArray[nodeIndex];

		string name = "node-" + std::to_string(nodeIndex);

		if (sceneNode.HasMember("name"))
		{
			name = sceneNode["name"].GetString();
		}
		Out(L"Loading %S", name.c_str());

		// This call will recurse down the scene hierarchy and load the whole tree..
		LoadSceneNode(document, sceneNode, callbacks, nodeIndex, -1);
	}
}

void GLTFFileData::CheckExtensions(const Document& document)
{
	if (document.HasMember("extensionsUsed"))
	{
		Out(L"Extensions Used");

		for (auto& ext: document["extensionsUsed"].GetArray())
		{
			Out(L"Ext: %S", ext.GetString());
		}
	}

	if (document.HasMember("extensionsRequired"))
	{
		Out(L"Extensions Required");

		for (auto& ext : document["extensionsRequired"].GetArray())
		{
			Out(L"Ext: %S", ext.GetString());
		}
	}
}

void GLTFFileData::ParseDocument(const Document& document, const Callbacks& callbacks)
{
	// Just start with the current scene for now..
	const Value& scene = document["scene"];
	int currentScene = scene.GetInt();
	const Value& a = document["scenes"];
	assert(a.IsArray());

	const Value& currentSceneNode = a[currentScene];

	Out(L"Loading scene %d", currentScene);
	LoadScene(document, currentSceneNode, callbacks);
}

