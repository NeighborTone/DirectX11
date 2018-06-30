//---------------------//
//FBXSDKが必要です//
//---------------------//
#pragma once
#pragma warning (disable : 4458)	//thisを使うため消す
#include <vector>
#include <memory>
#include <fbxsdk.h>
#include <DirectXMath.h>
#include "../Engine.h"

#if defined(_DLL)
#pragma comment(lib, "libfbxsdk-md.lib")
#else
#pragma comment(lib, "libfbxsdk-mt.lib")
#endif

class Model
{
private:
	struct FbxManagerDeleter
	{
		void operator()(FbxManager* fbxManager) const
		{
			fbxManager->Destroy();
		}
	};
	struct FbxImporterDeleter
	{
		void operator()(FbxImporter* fbxImporter) const
		{
			fbxImporter->Destroy();
		}
	};
	struct FbxSceneDeleter
	{
		void operator()(FbxScene* fbxScene) const
		{
			fbxScene->Destroy();
		}
	};
	struct Constant
	{
		DirectX::XMMATRIX bones[200];
	};
	struct Animation
	{
		struct Frame
		{
			std::vector<DirectX::XMMATRIX> bones;
		};
		std::vector<Frame> frames;
	};

	Constant constant;
	std::vector<std::unique_ptr<Mesh>> meshes;
	std::vector<Animation> animations;
	FbxArray<FbxString*> animStackList;		//アニメ情報リスト、連番で入る
	float startTime;
	int animName;

	void SearchNode(FbxScene* scene, FbxNode* node);
	void LoadNode(FbxScene* scene, FbxNode* node);
	bool IsOptimized(FbxMesh* mesh);
	void LoadMesh(FbxScene* scene, FbxMesh* mesh);
	void LoadMeshWithControlPoint(FbxMesh* mesh, Mesh* item);
	void LoadMeshWithPolygonVertex(FbxMesh* mesh, Mesh* item);
	void LoadAnim(FbxScene* scene, FbxMesh* mesh, Mesh* item, bool isOptimized);
	void LoadSkin(FbxMesh* mesh, FbxAnimStack* animStack, int animNum, Mesh* item, std::vector<std::vector<int>>& controlPointIndices, bool isOptimized);
	void LoadBones(FbxMesh* mesh, int animNum, int skinNum, int frameCount, Mesh* item, FbxTime start, std::vector<std::vector<int>>& controlPointIndices, bool isOptimized);
	void AddBlendInControlPoint(int blendIndex, FbxCluster* cluster, Mesh* item);
	void AddBlendInPolygonVertex(int blendIndex, FbxCluster* cluster, Mesh* item, std::vector<std::vector<int>>& controlPointIndices);
	void SearchBlendIndex(int vertexIndex, int blendIndex, float blendWeight, Mesh* item);
	static DirectX::XMMATRIX FbxMatrixToXMMatrix(FbxMatrix source);

public:
	Vec3 pos;
	Vec3 angle;
	Vec3 scale;
	Model();
	Model(const std::string filePath);
	~Model();
	void Load(const std::string filePath);
	void Play(int animName);
	void Draw(bool wireframeEnable = false);
	void Draw(DirectX::XMMATRIX rota,bool wireframeEnable = false);
};