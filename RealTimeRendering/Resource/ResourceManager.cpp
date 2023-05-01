#include "ResourceManager.h"

Map<std::string, ObjectSkeleton*> ResourceManager::gObjectCache;
std::vector<Object*> ResourceManager::gObjectList;
std::vector<std::string> ResourceManager::gObjectNameList;
Map<std::string, Object*> ResourceManager::gObjectMap;

bool ResourceManager::EnableCaching = true;

Object* ResourceManager::LoadObject(const std::string& str, float alpha) {
	Object* result = new Object();
	if (ResourceManager::gObjectCache.HasKey(str) && EnableCaching) {
		ObjectSkeleton* skeleton = ResourceManager::gObjectCache.Get(str);
		spdlog::info(std::string(__FUNCTION__) + " CACHE HIT, Loading the skeleton from cache");
		//std::cout << __FUNCTION__ << " Cache HIT, Loading the skeleton from address: " << skeleton << std::endl;
		std::string name_ext = "";
		int i = 0;
		while (std::find(gObjectNameList.begin(), gObjectNameList.end(), skeleton->ObjectBaseName + name_ext) != gObjectNameList.end())
		{
			name_ext = " (" + std::to_string(i) + ")";
			i++;
		}
		
		skeleton->ObjectName = skeleton->ObjectBaseName + name_ext;
		for (MeshSkeleton* meshSkeleton : skeleton->mChildMeshes) {
			Mesh* mesh = new Mesh();
			mesh->LoadMesh(meshSkeleton->MeshModel);
			mesh->SetName(meshSkeleton->MeshName.c_str());
			result->AddChild(mesh);
		}
		result->SetName(skeleton->ObjectName.c_str());
		gObjectNameList.push_back(result->GetName());
	}
	else {
		if (EnableCaching) {
		//	std::cout << __FUNCTION__ << " Cache Miss, Loading the object from file '" << str << "'" << std::endl;
			spdlog::info(std::string(__FUNCTION__) + " CACHE MISS, Loading the object from file '" + str + "'");
		}
		else {
			spdlog::warn(std::string(__FUNCTION__) + " Caching is disabled, loading the object from file everytime!");
			//std::cout << __FUNCTION__ << " Caching is disabled, loading the object from file everytime!" << std::endl;
		}
		ObjectSkeleton* skeleton = LoadObjectFromPath(str, alpha);
		for (MeshSkeleton* meshSkeleton : skeleton->mChildMeshes) {
			Mesh* mesh = new Mesh();
			mesh->LoadMesh(meshSkeleton->MeshModel);
			mesh->SetName(meshSkeleton->MeshName.c_str());
			result->AddChild(mesh);
		}
		result->SetName(skeleton->ObjectName.c_str());
		gObjectNameList.push_back(result->GetName());
		ResourceManager::gObjectCache.Set(str, skeleton);
	}
	ResourceManager::gObjectList.push_back(result);
	ResourceManager::gObjectMap.Add(result->GetName(), result);
	return result;
}

ObjectSkeleton* ResourceManager::LoadObjectFromPath(const std::string& object_file_path, float alpha) {
	ObjectSkeleton* ResultSkeleton = nullptr;
	Assimp::Importer aImporter;
	const aiScene* scene = aImporter.ReadFile(object_file_path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs  | aiProcess_CalcTangentSpace);
	if (scene != NULL) {
		ResultSkeleton = new ObjectSkeleton;
		std::vector<MeshSkeleton*> Meshes = LoadMeshesFromScene(scene->mRootNode, scene, alpha);
		ResultSkeleton->mChildMeshes = Meshes;
		std::string file_name = scene->GetShortFilename(object_file_path.c_str());
		ResultSkeleton->ObjectBaseName = file_name;
		int i = 0;
		while(std::find(gObjectNameList.begin(), gObjectNameList.end(), file_name) != gObjectNameList.end())
		{
			i++;
		}
		file_name += " (" + std::to_string(i) + ")";
		ResultSkeleton->ObjectName = file_name;
		
	}
	return ResultSkeleton;
}

std::vector<MeshSkeleton*> ResourceManager::LoadMeshesFromScene(const aiNode* RootNode, const aiScene* Scene, float alpha)
{
	std::vector<MeshSkeleton*> Meshes;
	int TotalMeshes = RootNode->mNumMeshes;
	int TotalChildren = RootNode->mNumChildren;
	int MeshIndex = -1;
	for (int i = 0; i < TotalMeshes; i++) {
		MeshIndex = RootNode->mMeshes[i];
		Model* MeshModel = LoadModelFromMesh(Scene->mMeshes[MeshIndex], alpha);
		MeshSkeleton* ResultMeshSkeleton = new MeshSkeleton;
		ResultMeshSkeleton->MeshModel = MeshModel;
		ResultMeshSkeleton->MeshName = Scene->mMeshes[MeshIndex]->mName.C_Str();
		//std::cout << __FUNCTION__ << ": Loading Mesh '" << ResultMeshSkeleton->MeshName << "' from scene" << std::endl;
		Meshes.push_back(ResultMeshSkeleton);
	}
	for (int i = 0; i < TotalChildren; i++) {
		std::vector<MeshSkeleton*> ChildMeshes = LoadMeshesFromScene(RootNode->mChildren[i], Scene, alpha);
		if (ChildMeshes.size() > 0) {
			for (MeshSkeleton* Skeleton : ChildMeshes) {
				Meshes.push_back(Skeleton);
			}
		}
	}
	return Meshes;
}
Model* ResourceManager::LoadModelFromMesh(const aiMesh* mesh, float alpha)
{
	Model* ResultModel = new Model;
	for (int vIdx = 0; vIdx < mesh->mNumVertices; vIdx++) {
		Vertex vertex_data;
		vertex_data.Position = glm::vec3(mesh->mVertices[vIdx].x, mesh->mVertices[vIdx].y, mesh->mVertices[vIdx].z);
		vertex_data.Color = glm::vec4(1.0f, 1.0f, 1.0f, alpha);
		vertex_data.Normal = glm::vec3(mesh->mNormals[vIdx].x, mesh->mNormals[vIdx].y, mesh->mNormals[vIdx].z);
		if (mesh->HasTextureCoords(0)) {
			vertex_data.TexCoord = glm::vec2(mesh->mTextureCoords[0][vIdx].x, mesh->mTextureCoords[0][vIdx].y);
		}
		else {
			vertex_data.TexCoord = glm::vec2(0.0f, 0.0f);
		}
		vertex_data.Tangent = glm::vec3(mesh->mTangents[vIdx].x, mesh->mTangents[vIdx].y, mesh->mTangents[vIdx].z);
		ResultModel->VertexData.push_back(vertex_data);
	}
	for (int fIdx = 0; fIdx < mesh->mNumFaces; fIdx++) {
		for (int Idx = 0; Idx < mesh->mFaces[fIdx].mNumIndices; Idx++) {
			ResultModel->IndexData.push_back(mesh->mFaces[fIdx].mIndices[Idx]);
		}
	}
	return ResultModel;
}
void ResourceManager::DestroyObject(Object* o) {
	if(std::find(ResourceManager::gObjectList.begin(), ResourceManager::gObjectList.end(), o) != ResourceManager::gObjectList.end())
		ResourceManager::gObjectList.erase(std::find(ResourceManager::gObjectList.begin(), ResourceManager::gObjectList.end(), o));
}
Object* ResourceManager::FindObjectWithName(const std::string& name) {
	return ResourceManager::gObjectMap.Get(name);
}
