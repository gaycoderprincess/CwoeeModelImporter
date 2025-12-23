#include <iostream>
#include <cstdint>
#include <format>
#include <fstream>
#include "assimp/Importer.hpp"
#include "assimp/Exporter.hpp"
#include "assimp/Logger.hpp"
#include "assimp/DefaultLogger.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

const int nFormatVersion = 1;

std::string GetMaterialFilename(aiMaterial* material) {
	aiString tmp;
	material->GetTexture(aiTextureType_DIFFUSE, 0, &tmp);
	std::string str = tmp.C_Str();
	while (str.find('\\') != std::string::npos) {
		str.erase(str.begin());
	}
	while (str.find('/') != std::string::npos) {
		str.erase(str.begin());
	}
	return str;
}

void WriteStringToFile(std::ofstream& file, const char* string) {
	int len  = strlen(string) + 1;
	file.write((char*)&len, sizeof(len));
	file.write(string, len);
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cout << "Usage: CwoeeModelImporter.exe <filename>";
		return 0;
	}

	Assimp::Logger::LogSeverity severity = Assimp::Logger::VERBOSE;
	Assimp::DefaultLogger::create("model_import_log.txt", severity, aiDefaultLogStream_FILE);

	uint32_t flags = 0;
	flags |= aiProcess_CalcTangentSpace;
	flags |= aiProcess_GenSmoothNormals;
	flags |= aiProcess_JoinIdenticalVertices;
	flags |= aiProcess_RemoveRedundantMaterials;
	flags |= aiProcess_Triangulate;
	flags |= aiProcess_GenUVCoords;
	flags |= aiProcess_GenBoundingBoxes;

	static Assimp::Importer importer;
	if (auto scene = importer.ReadFile(argv[1], flags)) {
		std::ofstream out(std::format("{}.cwo", argv[1]), std::iostream::out | std::iostream::binary);
		out.write((char*)&nFormatVersion, sizeof(nFormatVersion));

		out.write((char*)&scene->mNumMeshes, sizeof(scene->mNumMeshes));

		for (int i = 0; i < scene->mNumMeshes; i++) {
			auto mesh = scene->mMeshes[i];
			WriteStringToFile(out, GetMaterialFilename(scene->mMaterials[mesh->mMaterialIndex]).c_str());
			out.write((char*)&mesh->mNumVertices, sizeof(mesh->mNumVertices));
			out.write((char*)&mesh->mNumFaces, sizeof(mesh->mNumFaces));
			for (int j = 0; j < mesh->mNumVertices; j++) {
				auto v = mesh->mVertices[j];
				out.write((char*)&v.x, sizeof(v.x));
				out.write((char*)&v.y, sizeof(v.y));
				out.write((char*)&v.z, sizeof(v.z));
				v = mesh->mNormals[j];
				out.write((char*)&v.x, sizeof(v.x));
				out.write((char*)&v.y, sizeof(v.y));
				out.write((char*)&v.z, sizeof(v.z));
				v = mesh->mTangents[j];
				out.write((char*)&v.x, sizeof(v.x));
				out.write((char*)&v.y, sizeof(v.y));
				out.write((char*)&v.z, sizeof(v.z));
				v = mesh->mBitangents[j];
				out.write((char*)&v.x, sizeof(v.x));
				out.write((char*)&v.y, sizeof(v.y));
				out.write((char*)&v.z, sizeof(v.z));
				if (mesh->mTextureCoords[0]) {
					v = mesh->mTextureCoords[0][j];

					out.write((char*)&v.x, sizeof(v.x));
					out.write((char*)&v.y, sizeof(v.y));
					out.write((char*)&v.z, sizeof(v.z));
				}
				else {
					float f = 0;
					out.write((char*)&f, sizeof(f));
					out.write((char*)&f, sizeof(f));
					out.write((char*)&f, sizeof(f));
				}
				if (mesh->mTextureCoords[1]) {
					v = mesh->mTextureCoords[1][j];

					out.write((char*)&v.x, sizeof(v.x));
					out.write((char*)&v.y, sizeof(v.y));
					out.write((char*)&v.z, sizeof(v.z));
				}
				else {
					float f = 0;
					out.write((char*)&f, sizeof(f));
					out.write((char*)&f, sizeof(f));
					out.write((char*)&f, sizeof(f));
				}
				if (mesh->mColors[0]) {
					struct tTmp {
						uint8_t r;
						uint8_t g;
						uint8_t b;
						uint8_t a;
					};

					auto rgb = mesh->mColors[0][j];
					tTmp rgb32;
					rgb32.r = std::min(rgb.r * 255, 255.0f);
					rgb32.g = std::min(rgb.g * 255, 255.0f);
					rgb32.b = std::min(rgb.b * 255, 255.0f);
					rgb32.a = std::min(rgb.a * 255, 255.0f);

					out.write((char*)&rgb32, sizeof(rgb32));
				}
				else {
					uint32_t f = 0xFFFFFFFF;
					out.write((char*)&f, sizeof(f));
				}
			}
			for (int j = 0; j < mesh->mNumFaces; j++) {
				auto& f = mesh->mFaces[j];
				if (f.mNumIndices != 3) {
					std::cout << "\nERROR: Model has a non-tri!";
					exit(0);
				}
				out.write((char*)&f.mIndices[0], sizeof(f.mIndices[0]));
				out.write((char*)&f.mIndices[1], sizeof(f.mIndices[1]));
				out.write((char*)&f.mIndices[2], sizeof(f.mIndices[2]));
			}
		}

		out.flush();
	}
	return 0;
}
