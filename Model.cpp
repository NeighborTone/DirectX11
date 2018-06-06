#include "Model.h"
/*~~~~~~~~~~~~~~~~~Memo~~~~~~~~~~~~~~~~~*/
//FBX�̓}�l�[�W���[�����ׂẴm�[�h(�e�@�\)���Ǘ�����
//�R���g���[���|�C���g�Ƃ̓C���f�b�N�X�o�b�t�@�̎w�������f�[�^(���f���̒��_)
//���_�̏���FbxMesh �Ɋi�[
//FbxMesh��FbxScene::GetRootNode�֐��Ŏ擾����FbxNode�ɂ���
//�A�j���[�V�����̓V�[���ɓo�^�����


//----------//
//-publics-//
//----------//
Model::Model():
	pos(0,0,0),
	angles(0,0,0),
	scale(0,0,0)
{
	Initialize();
}

Model::Model(const char * const filePath)
{
	Initialize();
	Load(filePath);
}

void Model::Load(const char* const filePath)
{
	static std::unique_ptr<FbxManager, FbxManagerDeleter> manager(FbxManager::Create());

	//�t�@�C�����擾
	size_t length = strlen(filePath) + 1;
	std::unique_ptr<char[]> charFilePath(new char[length]);
	//FbxImporter�I�u�W�F�N�g���쐬
	std::unique_ptr<FbxImporter, FbxImporterDeleter> importer(FbxImporter::Create(manager.get(), ""));
	//�ǂݍ��݂Ə�����
	importer->Initialize(charFilePath.get(), -1, manager->GetIOSettings());
	//scene�ɃC���|�[�g
	std::unique_ptr<FbxScene, FbxSceneDeleter> scene(FbxScene::Create(manager.get(), ""));
	importer->Import(scene.get());
	//DirectX�̍��W�n�ɍ��킹��
	FbxAxisSystem::DirectX.ConvertScene(scene.get());
	//�W�I���g���m�[�h�̑���(FbxMesh�AFbxNurbs�A�����FbxPatch)��ϊ�����@�\
	FbxGeometryConverter converter(manager.get());
	//�O�p�`�����ł���V�[�����̂��ׂẴm�[�h�������O�p�`����
	converter.Triangulate(scene.get(), true);

	SearchNode(scene.get(), scene->GetRootNode());
}

void Model::Play(int animName)
{
	this->animName = animName;
	startTime = Engine::GetFps().GetTime();

}

void Model::Draw()
{
	float time = Engine::GetFps().GetTime() - startTime;
	int frame = (int)(time * 60.0f);
	//60�t���[����Ŏw�肵���A�j���[�V�������Đ�����
	frame %= animations[animName].frames.size();
}



//----------//
/*privates*/
//----------//
void Model::Initialize()
{
	pos = Vec3(0.0f, 0.0f, 0.0f);
	angles = Vec3(0.0f, 0.0f, 0.0f);
	scale = Vec3(1.0f, 1.0f, 1.0f);
}

void Model::SearchNode(FbxScene* scene, FbxNode* node)
{
	//�m�[�h�����ׂĒT���o��
	LoadNode(scene, node);

	for (int i = 0; i < node->GetChildCount(); ++i)
	{
		SearchNode(scene, node->GetChild(i));
	}
}

void Model::LoadNode(FbxScene* scene, FbxNode* node)
{
	//�m�[�h�̓ǂݍ���
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
	//���b�V���̖@�����擾
	if (mesh->GetElementNormal() != nullptr)
	{
		if (mesh->GetElementNormal()->GetMappingMode() != FbxLayerElement::EMappingMode::eByControlPoint)
			return false;
	}
	//���b�V����UV���擾
	if (mesh->GetElementUV() != nullptr)
	{
		if (mesh->GetElementUV()->GetMappingMode() != FbxLayerElement::EMappingMode::eByControlPoint)
			return false;
	}

	return true;
}

void Model::LoadMesh(FbxScene* scene, FbxMesh* mesh)
{
	//���b�V���̓ǂݍ���.item�ɐݒ肵��Move����
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

	//�V�F�[�_�[���A�^�b�`
	item->Apply();
	item->GetMaterial().Load("Shader/test.hlsl");
	item->GetMaterial().SetBuffer(2, &constant, sizeof(Constant));

	meshes.push_back(std::move(item));
}

void Model::LoadMeshWithControlPoint(FbxMesh* mesh, Mesh* item)
{
	//���f���̒��_���𐔂��Ċi�[����
	item->vertices.resize(mesh->GetControlPointsCount());

	for (int i = 0; i < mesh->GetControlPointsCount(); i++)
	{
		//���W
		FbxVector4 position = mesh->GetControlPointAt(i);
		item->vertices[i].pos = Vec3((float)position[0], (float)position[1], (float)position[2]);
		//�@��
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

	for (int i = 0; i < mesh->GetPolygonVertexCount(); i++)
	{
		item->indices[i] = mesh->GetPolygonVertices()[i];
	}
}

void Model::LoadMeshWithPolygonVertex(FbxMesh* mesh, Mesh* item)
{
	FbxStringList uvSetNames;
	mesh->GetUVSetNames(uvSetNames);

	for (int i = 0; i < mesh->GetPolygonCount(); i++)
	{
		for (int j = 0; j < mesh->GetPolygonSize(i); j++)
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
	//���_���擾
	if (!isOptimized)
	{
		controlPointIndices.resize(mesh->GetControlPointsCount());
		for (int i = 0; i < mesh->GetPolygonVertexCount(); i++)
		{
			controlPointIndices[mesh->GetPolygonVertices()[i]].push_back(i);
		}
	}
	//�A�j���[�V�������擾
	FbxArray<FbxString*> animStackList;		//�A�j����񃊃X�g�A�A�Ԃœ���
	scene->FillAnimStackNameArray(animStackList);
	printf("anim %d\n", animStackList.Size());

	animations.resize(animStackList.Size());	//�擾�����A�j�����܂ŗ̈��ς���

	for (int i = 0; i < animStackList.Size(); i++)
	{
		FbxAnimStack* animStack = scene->FindMember<FbxAnimStack>(animStackList[i]->Buffer());
		scene->SetCurrentAnimationStack(animStack);		//�����ŃA�j���̐؂�ւ�

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

	printf("deformer %d\n", mesh->GetDeformerCount(FbxDeformer::eSkin));
	for (int i = 0; i < mesh->GetDeformerCount(FbxDeformer::eSkin); i++)
	{
		LoadBones(mesh, animNum, i, frameCount, item, start, controlPointIndices, isOptimized);
	}
}

void Model::LoadBones(FbxMesh* mesh, int animNum, int skinNum, int frameCount, Mesh* item, FbxTime start, std::vector<std::vector<int>>& controlPointIndices, bool isOptimized)
{
	FbxSkin* skin = (FbxSkin*)mesh->GetDeformer(skinNum, FbxDeformer::eSkin);

	for (int i = 0; i < frameCount; i++)
	{
		FbxTime time;
		time.SetMilliSeconds(fbxsdk::FbxLongLong((i / 60.0f * 1000.0f)) + start.GetMilliSeconds());

		FbxMatrix globalPosition = mesh->GetNode()->EvaluateGlobalTransform(time);
		FbxVector4 translation = mesh->GetNode()->GetGeometricTranslation(FbxNode::eSourcePivot);
		FbxVector4 rotation = mesh->GetNode()->GetGeometricRotation(FbxNode::eSourcePivot);
		FbxVector4 scaling = mesh->GetNode()->GetGeometricScaling(FbxNode::eSourcePivot);
		FbxAMatrix geometryOffset = FbxAMatrix(translation, rotation, scaling);

		animations[animNum].frames[i].bones.resize(skin->GetClusterCount());

		for (int j = 0; j < skin->GetClusterCount(); j++)
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
	for (int i = 0; i < cluster->GetControlPointIndicesCount(); i++)
	{
		int index = cluster->GetControlPointIndices()[i];
		float weight = (float)cluster->GetControlPointWeights()[i];

		SearchBlendIndex(index, blendIndex, weight, item);
	}
}

void Model::AddBlendInPolygonVertex(int blendIndex, FbxCluster* cluster, Mesh* item, std::vector<std::vector<int>>& controlPointIndices)
{
	for (int i = 0; i < cluster->GetControlPointIndicesCount(); i++)
	{
		int index = cluster->GetControlPointIndices()[i];
		float weight = (float)cluster->GetControlPointWeights()[i];

		for (UINT v = 0; v < controlPointIndices[index].size(); v++)
		{
			SearchBlendIndex(controlPointIndices[index][v], blendIndex, weight, item);
		}
	}
}

void Model::SearchBlendIndex(int vertexIndex, int blendIndex, float blendWeight, Mesh* item)
{
	for (int i = 0; i < 8; i++)
	{
		if (item->vertices[vertexIndex].blendIndices[i] == blendIndex)
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
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			destination.r[x].m128_f32[y] = static_cast<float>(source.mData[x][y]);
		}
	}
	return destination;
}

