#pragma once

#include <sstream>

#include <Structures/Map.h>

#include <Rendering/Object.h>
#include <Rendering/ObjectStructures.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <spdlog/spdlog.h>

class ResourceManager
{
public:
	static bool EnableCaching;
	static std::vector<std::string> gObjectNameList;
	static Map<std::string, ObjectSkeleton*> gObjectCache;
	static std::vector<Object*> gObjectList;
	static Map<std::string, Object*> gObjectMap;
public:
	static Object* LoadObject(const std::string& str, float alpha = 1.0f);
	static ObjectSkeleton* LoadObjectFromPath(const std::string& str, float alpha = 1.0f);
	static std::vector<MeshSkeleton*> LoadMeshesFromScene(const aiNode* RootNode, const aiScene* Scene, float alpha = 1.0f);
	static Model* LoadModelFromMesh(const aiMesh* mesh, float alpha = 1.0f);
	static void DestroyObject(Object* o);
	static Object* FindObjectWithName(const std::string& name);

};
