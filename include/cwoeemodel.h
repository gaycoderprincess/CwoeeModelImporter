class CwoeeModelMesh {
public:
	std::string sMaterialName;
	uint32_t nNumVertices;
	uint32_t nNumFaces;
	NyaVec3 vAABBMin;
	NyaVec3 vAABBMax;
	NyaVec3* aVertices;
	NyaVec3* aNormals;
	NyaVec3* aTangents;
	NyaVec3* aBitangents;
	NyaVec3* aUVs1;
	NyaVec3* aUVs2;
	NyaDrawing::CNyaRGBA32* aColors;
	int* aIndices;

	void Destroy() {
		delete[] aVertices;
		delete[] aNormals;
		delete[] aTangents;
		delete[] aBitangents;
		delete[] aUVs1;
		delete[] aUVs2;
		delete[] aColors;
		delete[] aIndices;
	}
};

std::vector<CwoeeModelMesh> ReadCwoeeModel(std::filesystem::path path) {
	std::vector<CwoeeModelMesh> models;

	std::ifstream in(path, std::iostream::in | std::iostream::binary);
	if (!in.is_open()) return {};

	int version = 0;
	in.read((char*)&version, sizeof(version));
	if (version > 1) return {};

	int numMeshes = 0;
	in.read((char*)&numMeshes, sizeof(numMeshes));
	for (int i = 0; i < numMeshes; i++) {
		CwoeeModelMesh mesh;
		mesh.sMaterialName = ReadStringFromFile(in);
		in.read((char*)&mesh.nNumVertices, sizeof(mesh.nNumVertices));
		in.read((char*)&mesh.nNumFaces, sizeof(mesh.nNumFaces));
		in.read((char*)&mesh.vAABBMin, sizeof(mesh.vAABBMin));
		in.read((char*)&mesh.vAABBMax, sizeof(mesh.vAABBMax));
		mesh.aVertices = new NyaVec3[mesh.nNumVertices];
		mesh.aNormals = new NyaVec3[mesh.nNumVertices];
		mesh.aTangents = new NyaVec3[mesh.nNumVertices];
		mesh.aBitangents = new NyaVec3[mesh.nNumVertices];
		mesh.aUVs1 = new NyaVec3[mesh.nNumVertices];
		mesh.aUVs2 = new NyaVec3[mesh.nNumVertices];
		mesh.aColors = new NyaDrawing::CNyaRGBA32[mesh.nNumVertices];
		mesh.aIndices = new int[mesh.nNumFaces*3];
		for (int j = 0; j < mesh.nNumVertices; j++) {
			in.read((char*)&mesh.aVertices[j], sizeof(mesh.aVertices[j]));
			in.read((char*)&mesh.aNormals[j], sizeof(mesh.aNormals[j]));
			in.read((char*)&mesh.aTangents[j], sizeof(mesh.aTangents[j]));
			in.read((char*)&mesh.aBitangents[j], sizeof(mesh.aBitangents[j]));
			in.read((char*)&mesh.aUVs1[j], sizeof(mesh.aUVs1[j]));
			in.read((char*)&mesh.aUVs2[j], sizeof(mesh.aUVs2[j]));
			in.read((char*)&mesh.aColors[j], sizeof(mesh.aColors[j]));
		}
		for (int j = 0; j < mesh.nNumFaces*3; j++) {
			in.read((char*)&mesh.aIndices[j], sizeof(mesh.aIndices[j]));
		}
		models.push_back(mesh);
	}
	return models;
}