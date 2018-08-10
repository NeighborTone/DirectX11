#include "Model.h"
#include "../Utilitys/StringConverter.hpp"
/*~~~~~~~~~~~~~~~~~Memo~~~~~~~~~~~~~~~~~*/
//FBXはマネージャーがすべてのノード(各機能)を管理する
//コントロールポイントとはインデックスバッファの指し示すデータ(モデルの頂点)
//頂点の情報はFbxMesh に格納
//FbxMeshはFbxScene::GetRootNode関数で取得したFbxNodeにある
//アニメーションはシーンに登録される
//GetPolygonVertexCount() はインデックスデータの数を取得
//GetPolygonVertices()でインデックスデータそのものを取得

//----------//
//-publics-//
//----------//
Model::Model():
	pos(0,0,0),
	angle(0,0,0),
	scale(1,1,1),
	startTime(0),
	animName(0)
{
	Engine::COMInitialize();
}

Model::Model(const std::string filePath) :
	pos(0, 0, 0),
	angle(0, 0, 0),
	scale(1, 1, 1),
	startTime(0),
	animName(0)
{
	Engine::COMInitialize();
	Load(filePath);
}

Model::~Model()
{
	animations.clear();
	animations.shrink_to_fit();
	FbxArrayDelete(animStackList);	//これがないとリークが発生
}

void Model::Load(const std::string filePath)
{
	static std::unique_ptr<FbxManager, FbxManagerDeleter> manager(FbxManager::Create());

	//ワイド文字でないと正しく読み込めないので変換
	std::wstring path  = StringConverter::StringToWString(filePath);

	//ファイル名取得
	size_t length = wcslen(path.c_str()) + 1;
	std::unique_ptr<char[]> charFilePath(new char[length]);
	wcstombs_s(nullptr, charFilePath.get(), length, path.c_str(), _TRUNCATE);

	//FbxImporterオブジェクトを作成
	std::unique_ptr<FbxImporter, FbxImporterDeleter> importer(FbxImporter::Create(manager.get(), ""));
	//読み込みと初期化
	importer->Initialize(charFilePath.get(), -1, manager->GetIOSettings());
	//sceneにインポート
	std::unique_ptr<FbxScene, FbxSceneDeleter> scene(FbxScene::Create(manager.get(), ""));
	importer->Import(scene.get());
	//DirectXの座標系に合わせる
	FbxAxisSystem::DirectX.ConvertScene(scene.get());
	//ジオメトリノードの属性(FbxMesh、FbxNurbs、およびFbxPatch)を変換する機能
	FbxGeometryConverter converter(manager.get());
	//三角形分割できるシーン内のすべてのノード属性を三角形分割
	converter.Triangulate(scene.get(), true);

	SearchNode(scene.get(), scene->GetRootNode());
}

void Model::Play(int animName)
{
	this->animName = animName;
	startTime = Engine::GetFps().GetTime();
}

void Model::Draw(bool wireframeEnable)
{
	float time = Engine::GetFps().GetTime() - startTime;
	int frame = (int)(time * 60.0f);
	//60フレーム基準で指定したアニメーションを再生する
	frame %= animations[animName].frames.size();

	for (UINT i = 0; i < animations[animName].frames[frame].bones.size(); ++i)
	{
		constant.bones[i] = XMMatrixTranspose(
			animations[animName].frames[frame].bones[i]
		);
	}

	if (wireframeEnable)
	{
		for (UINT i = 0; i < meshes.size(); ++i)
		{
			meshes[i]->pos = pos;
			meshes[i]->angle = angle;
			meshes[i]->scale = scale;
			meshes[i]->SetDrawMode(D3D11_CULL_BACK, D3D11_FILL_WIREFRAME);
			meshes[i]->Draw();
		}
	}
	else
	{
		for (UINT i = 0; i < meshes.size(); ++i)
		{
			meshes[i]->pos = pos;
			meshes[i]->angle = angle;
			meshes[i]->scale = scale;
			meshes[i]->SetDrawMode(D3D11_CULL_BACK, D3D11_FILL_SOLID);
			meshes[i]->Draw();
		}
	}
	
}

void Model::Draw(DirectX::XMMATRIX rota, bool wireframeEnable)
{
	float time = Engine::GetFps().GetTime() - startTime;
	int frame = (int)(time * 60.0f);
	//60フレーム基準で指定したアニメーションを再生する
	frame %= animations[animName].frames.size();

	for (UINT i = 0; i < animations[animName].frames[frame].bones.size(); ++i)
	{
		constant.bones[i] = XMMatrixTranspose(
			animations[animName].frames[frame].bones[i]
		);
	}

	if (wireframeEnable)
	{
		for (UINT i = 0; i < meshes.size(); ++i)
		{
			meshes[i]->pos = pos;
			meshes[i]->angle = angle;
			meshes[i]->scale = scale;
			meshes[i]->SetDrawMode(D3D11_CULL_BACK, D3D11_FILL_WIREFRAME);
			meshes[i]->Draw(rota);
		}
	}
	else
	{
		for (UINT i = 0; i < meshes.size(); ++i)
		{
			meshes[i]->pos = pos;
			meshes[i]->angle = angle;
			meshes[i]->scale = scale;
			meshes[i]->SetDrawMode(D3D11_CULL_BACK, D3D11_FILL_SOLID);
			meshes[i]->Draw(rota);
		}
	}

}

//----------//
/*privates*/
//----------//
void Model::SearchNode(FbxScene* scene, FbxNode* node)
{
	//ノードをすべて探し出す
	LoadNode(scene, node);

	for (int i = 0; i < node->GetChildCount(); ++i)
	{
		SearchNode(scene, node->GetChild(i));
	}
}

void Model::LoadNode(FbxScene* scene, FbxNode* node)
{
	//ノードの読み込む
	if (!node->GetVisibility())
		return;

	FbxNodeAttribute* attribute = node->GetNodeAttribute();

	if (attribute == nullptr)
		return;

	if (attribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		LoadMesh(scene, node->GetMesh());
}

bool Model::IsOptimized(FbxMesh* mesh)
{
	//メッシュの法線を取得
	if (mesh->GetElementNormal() != nullptr)
	{
		if (mesh->GetElementNormal()->GetMappingMode() != FbxLayerElement::EMappingMode::eByControlPoint)
			return false;
	}
	//メッシュのUVを取得
	if (mesh->GetElementUV() != nullptr)
	{
		if (mesh->GetElementUV()->GetMappingMode() != FbxLayerElement::EMappingMode::eByControlPoint)
			return false;
	}

	return true;
}

void Model::LoadMesh(FbxScene* scene, FbxMesh* mesh)
{
	//メッシュの読み込み.itemに設定してMoveする
	std::unique_ptr<Mesh> item(new Mesh());
	item->vertices.clear();
	item->indices.clear();

	bool isOptimized = IsOptimized(mesh);

	if (isOptimized)
	{
		LoadMeshWithControlPoint(mesh, item.get());
	}
	else
	{
		LoadMeshWithPolygonVertex(mesh, item.get());
	}

	LoadAnim(scene, mesh, item.get(), isOptimized);

	//シェーダーをアタッチ
	item->Apply();
	item->GetMaterial().Load("Resource//Shader/test.hlsl");
	item->GetMaterial().SetBuffer(2, &constant, sizeof(Constant));

	meshes.push_back(std::move(item));
}

void Model::LoadMeshWithControlPoint(FbxMesh* mesh, Mesh* item)
{
	//モデルの頂点数を数えて格納する
	item->vertices.resize(mesh->GetControlPointsCount());

	for (int i = 0; i < mesh->GetControlPointsCount(); ++i)
	{
		//座標
		FbxVector4 position = mesh->GetControlPointAt(i);
		item->vertices[i].pos = Vec3((float)position[0], (float)position[1], (float)position[2]);
		//法線
		FbxGeometryElementNormal* normalElement = mesh->GetElementNormal();
		if (normalElement != nullptr)
		{
			FbxVector4 normal = normalElement->GetDirectArray().GetAt(i);
			item->vertices[i].normal = Vec3((float)normal.mData[0], (float)normal.mData[1], (float)normal.mData[2]);
		}
		//UV
		FbxGeometryElementUV* uvElement = mesh->GetElementUV();
		if (uvElement != nullptr)
		{
			FbxVector2 uv = uvElement->GetDirectArray().GetAt(i);
			item->vertices[i].uv = Vec2((float)uv.mData[0], 1.0f - (float)uv.mData[1]);
		}
	}

	item->indices.resize(mesh->GetPolygonVertexCount());

	for (int i = 0; i < mesh->GetPolygonVertexCount(); ++i)
	{
		item->indices[i] = mesh->GetPolygonVertices()[i];
	}
}

void Model::LoadMeshWithPolygonVertex(FbxMesh* mesh, Mesh* item)
{
	FbxStringList uvSetNames;
	mesh->GetUVSetNames(uvSetNames);

	for (int i = 0; i < mesh->GetPolygonCount(); ++i)
	{
		for (int j = 0; j < mesh->GetPolygonSize(i); ++j)
		{
			Vertex vertex;

			int v = mesh->GetPolygonVertex(i, j);
			FbxVector4 position = mesh->GetControlPoints()[v];
			vertex.pos = Vec3((float)position.mData[0], (float)position.mData[1], (float)position.mData[2]);

			FbxVector4 normal;
			mesh->GetPolygonVertexNormal(i, j, normal);
			vertex.normal = Vec3((float)normal.mData[0], (float)normal.mData[1], (float)normal.mData[2]);

			FbxVector2 uv;
			bool isMapped;
			if (mesh->GetElementUVCount() > 0)
				mesh->GetPolygonVertexUV(i, j, uvSetNames[0], uv, isMapped);
			vertex.uv = Vec2((float)uv.mData[0], 1.0f - (float)uv.mData[1]);
		
			item->vertices.push_back(vertex);
		}
	}
}

void Model::LoadAnim(FbxScene* scene, FbxMesh* mesh, Mesh* item, bool isOptimized)
{
	std::vector<std::vector<int>> controlPointIndices;
	//頂点数取得
	if (!isOptimized)
	{
		controlPointIndices.resize(mesh->GetControlPointsCount());
		for (int i = 0; i < mesh->GetPolygonVertexCount(); ++i)
		{
			controlPointIndices[mesh->GetPolygonVertices()[i]].push_back(i);
		}
	}

	//アニメーション情報取得
	scene->FillAnimStackNameArray(animStackList);
	/*printf("anim %d\n", animStackList.Size());*/

	animations.resize(animStackList.Size());	//取得したアニメ数まで領域を変える

	for (int i = 0; i < animStackList.Size(); i++)
	{
		FbxAnimStack* animStack = scene->FindMember<FbxAnimStack>(animStackList[i]->Buffer());
		scene->SetCurrentAnimationStack(animStack);		//ここでアニメの切り替え
		
		LoadSkin(mesh, animStack, i, item, controlPointIndices, isOptimized);
	}
}

void Model::LoadSkin(FbxMesh* mesh, FbxAnimStack* animStack, int animNum, Mesh* item, std::vector<std::vector<int>>& controlPointIndices, bool isOptimized)
{
	FbxTime start = animStack->GetLocalTimeSpan().GetStart();
	FbxTime stop = animStack->GetLocalTimeSpan().GetStop();
	float length = (stop.GetMilliSeconds() - start.GetMilliSeconds()) / 1000.0f;
	int frameCount = (int)(length * 60.0f);
	animations[animNum].frames.resize(frameCount);

	/*printf("deformer %d\n", mesh->GetDeformerCount(FbxDeformer::eSkin));*/
	for (int i = 0; i < mesh->GetDeformerCount(FbxDeformer::eSkin); ++i)
	{
		LoadBones(mesh, animNum, i, frameCount, item, start, controlPointIndices, isOptimized);
	}
}

void Model::LoadBones(FbxMesh* mesh, int animNum, int skinNum, int frameCount, Mesh* item, FbxTime start, std::vector<std::vector<int>>& controlPointIndices, bool isOptimized)
{
	FbxSkin* skin = (FbxSkin*)mesh->GetDeformer(skinNum, FbxDeformer::eSkin);

	for (int i = 0; i < frameCount; ++i)
	{
		FbxTime time;
		time.SetMilliSeconds(fbxsdk::FbxLongLong((i / 60.0f * 1000.0f)) + start.GetMilliSeconds());

		FbxMatrix globalPosition = mesh->GetNode()->EvaluateGlobalTransform(time);
		FbxVector4 translation = mesh->GetNode()->GetGeometricTranslation(FbxNode::eSourcePivot);
		FbxVector4 rotation = mesh->GetNode()->GetGeometricRotation(FbxNode::eSourcePivot);
		FbxVector4 scaling = mesh->GetNode()->GetGeometricScaling(FbxNode::eSourcePivot);
		FbxAMatrix geometryOffset = FbxAMatrix(translation, rotation, scaling);

		animations[animNum].frames[i].bones.resize(skin->GetClusterCount());

		for (int j = 0; j < skin->GetClusterCount(); ++j)
		{
			FbxCluster* cluster = skin->GetCluster(j);

			FbxMatrix vertexTransformMatrix;
			FbxAMatrix referenceGlobalInitPosition;
			FbxAMatrix clusterGlobalInitPosition;
			FbxMatrix clusterGlobalCurrentPosition;
			FbxMatrix clusterRelativeInitPosition;
			FbxMatrix clusterRelativeCurrentPositionInverse;
			cluster->GetTransformMatrix(referenceGlobalInitPosition);
			referenceGlobalInitPosition *= geometryOffset;
			cluster->GetTransformLinkMatrix(clusterGlobalInitPosition);
			clusterGlobalCurrentPosition = cluster->GetLink()->EvaluateGlobalTransform(time);
			clusterRelativeInitPosition = clusterGlobalInitPosition.Inverse() * referenceGlobalInitPosition;
			clusterRelativeCurrentPositionInverse = globalPosition.Inverse() * clusterGlobalCurrentPosition;
			vertexTransformMatrix = clusterRelativeCurrentPositionInverse * clusterRelativeInitPosition;

			animations[animNum].frames[i].bones[j] = FbxMatrixToXMMatrix(vertexTransformMatrix);

			if (isOptimized)
				AddBlendInControlPoint(j, cluster, item);
			else
				AddBlendInPolygonVertex(j, cluster, item, controlPointIndices);
		}
	}
}

void Model::AddBlendInControlPoint(int blendIndex, FbxCluster* cluster, Mesh* item)
{
	for (int i = 0; i < cluster->GetControlPointIndicesCount(); ++i)
	{
		int index = cluster->GetControlPointIndices()[i];
		float weight = (float)cluster->GetControlPointWeights()[i];

		SearchBlendIndex(index, blendIndex, weight, item);
	}
}

void Model::AddBlendInPolygonVertex(int blendIndex, FbxCluster* cluster, Mesh* item, std::vector<std::vector<int>>& controlPointIndices)
{
	for (int i = 0; i < cluster->GetControlPointIndicesCount(); ++i)
	{
		int index = cluster->GetControlPointIndices()[i];
		float weight = (float)cluster->GetControlPointWeights()[i];

		for (UINT v = 0; v < controlPointIndices[index].size(); ++v)
		{
			SearchBlendIndex(controlPointIndices[index][v], blendIndex, weight, item);
		}
	}
}

void Model::SearchBlendIndex(int vertexIndex, int blendIndex, float blendWeight, Mesh* item)
{
	for (int i = 0; i < 8; ++i)
	{
		if (item->vertices[vertexIndex].blendIndices[i] == (unsigned)blendIndex)
			return;

		if (item->vertices[vertexIndex].blendIndices[i] < 999)
			continue;

		item->vertices[vertexIndex].blendIndices[i] = blendIndex;
		item->vertices[vertexIndex].blendWeights[i] = blendWeight;
		return;
	}
}

DirectX::XMMATRIX Model::FbxMatrixToXMMatrix(FbxMatrix source)
{
	DirectX::XMMATRIX destination;
	for (int x = 0; x < 4; ++x)
	{
		for (int y = 0; y < 4; ++y)
		{
			destination.r[x].m128_f32[y] = static_cast<float>(source.mData[x][y]);
		}
	}
	return destination;
}

