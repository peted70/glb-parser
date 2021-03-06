// GLBGLTFConverter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../SharedGLTFParser/gltfparser.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char **argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::string fileStr;
	if (argc > 1)
	{
		fileStr = argv[1];
	}
	else
	{
		std::cout << "Error: you must supply an input file";
		return -1;
	}

	//std::ifstream infile(fileStr, std::ios::binary);
	auto infile = make_shared<ifstream>(fileStr, ios::binary);

	if (infile->fail())
	{
		std::cout << "Error: can't read the file";
		return false;
	}

	ParseFile(infile, [](const BufferData& data)
	{

	},
	[](const MaterialData& data)
	{

	},
	[](const TextureData& data)
	{

	},
	[](const TransformData& data)
	{

	},
	[](const SceneNodeData& data)
	{

	});

    return 0;
}

