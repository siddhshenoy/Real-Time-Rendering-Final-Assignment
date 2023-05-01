#pragma once

#include <vector>
#include <string>
#include <Rendering/Model.h>

typedef struct MeshSkeleton {
	std::string MeshName;
	Model* MeshModel;
}
MeshSkeleton;

typedef struct ObjectSkeleton
{
	std::string ObjectName;
	std::string ObjectBaseName;
	std::vector<MeshSkeleton*> mChildMeshes;
}
ObjectSkeleton;

