#include "ToolDefines.h"
#include "TAssimpModel.h"
#include "TAssimpAnimation.h"
#include "TAssimpBoneNode.h"
#include "TAssimpChannel.h"
#include "UGameInstance.h"
#include "TAssimpMeshContainer.h"
#include "UTexture.h"
#include "UTexGroup.h"
#include "UMethod.h"
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

TAssimpModel::TAssimpModel(CSHPTRREF<UDevice> _spDevice) :
	UResource(_spDevice),
	m_spImporter{ nullptr },
	m_vecMeshContainers{},
	m_iNumMeshContainers{ 0 },
	m_vecBoneNodes{},
	m_iNumBoneNodes{ 0 },
	m_vecAnimations{},
	m_iNumAnimation{ 0 },
	m_vecMaterials{},
	m_iNumMaterials{ 0 },
	m_mPivotMatrix{ _float4x4::Identity },
	m_spFileGroup{ nullptr },
	m_spFileData{ nullptr },
	m_eModelType{ TYPE::NONANIM },
	m_wstrModelName{ L"" },
	m_wstrTextureFolderName{ L"" },
	m_pAIScene{ nullptr },
	m_wstrTexturePath{ L"" }
{
}

TAssimpModel::TAssimpModel(const TAssimpModel& _rhs) :
	UResource(_rhs),
	m_spImporter{ _rhs.m_spImporter },
	m_vecMeshContainers{ _rhs.m_vecMeshContainers },
	m_iNumMeshContainers{ _rhs.m_iNumMeshContainers },
	m_vecBoneNodes{ _rhs.m_vecBoneNodes },
	m_iNumBoneNodes{ _rhs.m_iNumBoneNodes },
	m_vecAnimations{ _rhs.m_vecAnimations },
	m_iNumAnimation{ _rhs.m_iNumAnimation },
	m_vecMaterials{ _rhs.m_vecMaterials },
	m_iNumMaterials{ _rhs.m_iNumMaterials },
	m_mPivotMatrix{ _rhs.m_mPivotMatrix },
	m_spFileGroup{ _rhs.m_spFileGroup },
	m_spFileData{ _rhs.m_spFileData },
	m_eModelType{ _rhs.m_eModelType },
	m_wstrModelName{ L"" },
	m_wstrTextureFolderName{ L"" },
	m_pAIScene{ nullptr },
	m_wstrTexturePath{ L"" }
{
}

SHPTR<TAssimpBoneNode> TAssimpModel::GetBoneNode(const _wstring& _strBoneNode)
{
	const BONENODES::iterator& it = std::find_if(m_vecBoneNodes.begin(), m_vecBoneNodes.end(), [&](const SHPTR<TAssimpBoneNode>& p) {
		if (p->GetBoneName() == _strBoneNode)
			return true;

		return false;
		});
	RETURN_CHECK(m_vecBoneNodes.end() == it, nullptr);
	return *it;
}

void TAssimpModel::Free()
{
	m_vecMeshContainers.clear();
}

HRESULT TAssimpModel::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT TAssimpModel::NativeConstruct(const TYPE& _eModelType, const _wstring& _wstrPath, const _float4x4& _mPivotMatrix, const _wstring& _wstrTextureFolder)
{
	RETURN_CHECK_FAILED(NativeConstruct(), E_FAIL);
	m_wstrTextureFolderName = _wstrTextureFolder;
	m_eModelType = _eModelType;
	m_mPivotMatrix = _mPivotMatrix;
	return 	CreateModel(_wstrPath);
}

HRESULT TAssimpModel::NativeConstruct(const TYPE& _eModelType, CSHPTRREF<FILEGROUP> _spFileGroup, CSHPTRREF<FILEDATA> _spFileData, const _float4x4& _mPivotMatrix, const _wstring& _wstrTextureFolder)
{
	RETURN_CHECK_FAILED(NativeConstruct(), E_FAIL);
	m_wstrTextureFolderName = _wstrTextureFolder;
	RETURN_CHECK(nullptr == _spFileGroup || nullptr == _spFileData, E_FAIL);
	m_spFileGroup = _spFileGroup; m_spFileData = _spFileData;
	m_eModelType = _eModelType;
	m_mPivotMatrix = _mPivotMatrix;
	return 	CreateModel(m_spFileData->wstrfilePath);
}

HRESULT TAssimpModel::NativeConstruct(const TYPE& _eModelType, const PATHS& _vecPaths, const _wstring& _wstrFileName, const _float4x4& _mPivotMatrix, const _wstring& _wstrTextureFolder)
{
	RETURN_CHECK_FAILED(NativeConstruct(), E_FAIL);
	m_wstrTextureFolderName = _wstrTextureFolder;
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	m_spFileGroup = spGameInstance->FindFolder(_vecPaths);
	RETURN_CHECK(nullptr == m_spFileGroup, E_FAIL);
	m_spFileData = m_spFileGroup->FindData(_wstrFileName);
	RETURN_CHECK(nullptr == m_spFileData, E_FAIL);
	m_eModelType = _eModelType;
	m_mPivotMatrix = _mPivotMatrix;
	return 	CreateModel(m_spFileData->wstrfilePath);
}

HRESULT TAssimpModel::NativeConstruct(const TYPE& _eModelType, const _wstring& _wstrModelFolder, const _wstring& _wstrFileName, const _float4x4& _mPivotMatrix, const _wstring& _wstrTextureFolder)
{
	RETURN_CHECK_FAILED(NativeConstruct(), E_FAIL);
	m_wstrTextureFolderName = _wstrTextureFolder;
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	m_spFileGroup = spGameInstance->FindFolder(_wstrModelFolder);
	RETURN_CHECK(nullptr == m_spFileGroup, E_FAIL);
	m_spFileData = m_spFileGroup->FindData(_wstrFileName);
	RETURN_CHECK(nullptr == m_spFileData, E_FAIL);
	m_mPivotMatrix = _mPivotMatrix;
	return 	CreateModel(m_spFileData->wstrfilePath);
}

HRESULT TAssimpModel::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_vecDatas), E_FAIL);
	return S_OK;
}


void TAssimpModel::Render(const _uint _iMeshIndex, const SRV_REGISTER _eRegister, const aiTextureFlags& _eTextureType, CSHPTRREF<UShader> _spShade)
{

}

void TAssimpModel::GetData(MODELDESC& _tModelDesc)
{
	// MeshContainery
	for (auto& iter : m_vecMeshContainers) {
		MESHDESC tMeshDesc;
		iter->GetData(tMeshDesc);
		_tModelDesc.Meshes.push_back(tMeshDesc);
	}
	// Materials
	int i = 0;
	for (auto& iter : m_vecMaterials) {
		VECTOR<_wstring> strTextureFileName{};
		for (auto& value : iter->arrMaterialTexture) {
			if (nullptr != value)
			{
				// Extenstion 
				_wstring ext = m_lsTextureExts.front();

				_wstring TexturePath = m_wstrTexturePath;
				TexturePath += value->GetTextureName(0);
				TexturePath += ext;
				strTextureFileName.push_back(TexturePath);
				// 앞을 날린다. 
				m_lsTextureExts.pop_front();
			}
			else
			{
				strTextureFileName.push_back(L"NULL");
			}
		}
		_tModelDesc.MatrialData.insert(std::pair<_uint, VECTOR<_wstring>>(i++, strTextureFileName));
	}
	// BoneNode
	for (auto& iter : m_vecBoneNodes) {
		BONENODEDESC tDesc;
		iter->GetData(tDesc);
		_tModelDesc.BNodeDatas.push_back(tDesc);
	}
	_tModelDesc.iNodeCnt = (_uint)m_vecBoneNodes.size();
	_tModelDesc.iMeshContainersCount = m_iNumMeshContainers;
	_tModelDesc.iMatrialCount = m_iNumMaterials;
}

void TAssimpModel::GetData(ANIMMODELDESC& _tAnimModelDesc)
{
	// MeshContainery
	for (auto& iter : m_vecMeshContainers) {
		ANIMMESHDESC tMeshDesc;
		iter->GetData(tMeshDesc);
		_tAnimModelDesc.Meshes.push_back(tMeshDesc);
	}
	// Materials
	int i = 0;
	for (auto& iter : m_vecMaterials) {
		VECTOR<_wstring> strTextureFileName{};
		for (auto& value : iter->arrMaterialTexture) {
			if (nullptr != value)
			{
				// Extenstion 
				_wstring ext = m_lsTextureExts.front();

				_wstring TexturePath = m_wstrTexturePath;
				TexturePath += value->GetTextureName(0);
				TexturePath += ext;
				strTextureFileName.push_back(TexturePath);
				// 앞을 날린다. 
				m_lsTextureExts.pop_front();
			}
			else
			{
				strTextureFileName.push_back(L"NULL");
			}
		}
		_tAnimModelDesc.MatrialData.insert(std::pair<_uint, VECTOR<_wstring>>(i++, strTextureFileName));
	}
	// BoneNode
	for (auto& iter : m_vecBoneNodes) {
		BONENODEDESC tDesc;
		iter->GetData(tDesc);
		_tAnimModelDesc.BNodeDatas.push_back(tDesc);
	}
	// Animation
	for (auto& iter : m_vecAnimations) {
		ANIMDESC tDesc;
		iter->GetData(tDesc);
		_tAnimModelDesc.Animes.push_back(tDesc);
	}

	_tAnimModelDesc.iNodeCnt = (_uint)m_vecBoneNodes.size();
	_tAnimModelDesc.iMeshContainersCount = m_iNumMeshContainers;
	_tAnimModelDesc.iMatrialCount = m_iNumMaterials;
}

void TAssimpModel::SaveNonAnimModel(const _wstring& _wstrPath, _wstring& _wstrConvertPath)
{
	MODELDESC stModelDesc;
	GetData(stModelDesc);
	_wstring dir = _wstrPath;
	ConvertPath(dir);
	_wstrConvertPath = dir;

	using namespace std;
	ofstream save{ dir, ios::binary };
	RETURN_CHECK(!save, ;);
	{
		// MESH
		{
			size_t SIZE = stModelDesc.Meshes.size();
			save.write((_char*)&SIZE, sizeof(size_t));
			if (0 != SIZE)
			{
				for (auto& iter : stModelDesc.Meshes) {
					// Name 
					UMethod::SaveString(save, UMethod::ConvertWToS(iter.wstrName));
					// Vertex Model
					save.write((_char*)&iter.iNumVertices, sizeof(iter.iNumVertices));
					// Index Model
					save.write((_char*)&iter.iNumIndices, sizeof(iter.iNumIndices));
					for (_uint i = 0; i < iter.iNumVertices; ++i) {
						save.write((_char*)&iter.pVtxModel[i], sizeof(VTXMODEL));
					}
					for (_uint i = 0; i < iter.iNumIndices; ++i) {
						save.write((_char*)&iter.pIndices[i], sizeof(INDICIES32));
					}
					for (_uint i = 0; i < iter.iNumVertices; ++i) {
						save.write((_char*)&iter.pVtxModel[i].vPosition, sizeof(_float3));
					}
					{
						save.write((_char*)&iter.vMinVertex, sizeof(_float3));
						save.write((_char*)&iter.vMaxVertex, sizeof(_float3));
					}
					// BonCnt
					save.write((_char*)&iter.iBoneNodeCnt, sizeof(iter.iBoneNodeCnt));
					if (0 != iter.iBoneNodeCnt) {
						for (auto& BoneNodeNames : iter.BoneNodeNameList)
						{
							UMethod::SaveString(save, UMethod::ConvertWToS(BoneNodeNames));
						}
					}
					// Material Index
					save.write((_char*)&iter.iMatrialIndex, sizeof(iter.iMatrialIndex));
				}
			}
		}
		// BoneNode
		{
			size_t SIZE = stModelDesc.BNodeDatas.size();
			save.write((_char*)&SIZE, sizeof(size_t));
			if (0 != SIZE) {
				for (auto& iter : stModelDesc.BNodeDatas) {
					// Name
					save.write((_char*)&iter.mOffsetMatrix, sizeof(_float4x4));
					save.write((_char*)&iter.mTransformMatrix, sizeof(_float4x4));
					save.write((_char*)&iter.fDepth, sizeof(_float));
					UMethod::SaveString(save, UMethod::ConvertWToS(iter.wstrParentsName));
					UMethod::SaveString(save, UMethod::ConvertWToS(iter.wstrName));
				}
			}
		}
		// Material
		{
			size_t SIZE = stModelDesc.MatrialData.size();
			save.write((_char*)&SIZE, sizeof(size_t));
			if (0 != SIZE) {
				for (auto& iter : stModelDesc.MatrialData) {
					save.write((_char*)&iter.first, sizeof(iter.first));
					// Materials
					size_t STRING_SIZE = iter.second.size();
					save.write((_char*)&STRING_SIZE, sizeof(size_t));

					for (auto& TextureName : iter.second) {
						UMethod::SaveString(save, UMethod::ConvertWToS(TextureName));
					}
				}
			}
		}
	}
}

void TAssimpModel::SaveAnimModel(const _wstring& _wstrPath, _wstring& _wstrConvertPath)
{
	ANIMMODELDESC stModelDesc;
	GetData(stModelDesc);
	_wstring dir = _wstrPath;
	ConvertPath(dir);
	_wstrConvertPath = dir;

	using namespace std;
	ofstream save{ dir, ios::binary };
	RETURN_CHECK(!save, ;);
	{
		{
			// MESH
			{
				size_t SIZE = stModelDesc.Meshes.size();
				save.write((_char*)&SIZE, sizeof(size_t));
				if (0 != SIZE)
				{
					for (auto& iter : stModelDesc.Meshes) {
						// Name 
						UMethod::SaveString(save, UMethod::ConvertWToS(iter.wstrName));
						// Vertex Model
						save.write((_char*)&iter.iNumVertices, sizeof(iter.iNumVertices));
						// Index Model
						save.write((_char*)&iter.iNumIndices, sizeof(iter.iNumIndices));
						for (_uint i = 0; i < iter.iNumVertices; ++i) {
							save.write((_char*)&iter.pVtxModel[i], sizeof(VTXANIMMODEL));
						}
						for (_uint i = 0; i < iter.iNumIndices; ++i) {
							save.write((_char*)&iter.pIndices[i], sizeof(INDICIES32));
						}
						for (_uint i = 0; i < iter.iNumVertices; ++i) {
							save.write((_char*)&iter.pVtxModel[i].vPosition, sizeof(_float3));
						}
						{
							save.write((_char*)&iter.vMinVertex, sizeof(_float3));
							save.write((_char*)&iter.vMaxVertex, sizeof(_float3));
						}
						// BonCnt
						save.write((_char*)&iter.iBoneNodeCnt, sizeof(iter.iBoneNodeCnt));
						if (0 != iter.iBoneNodeCnt) {
							for (auto& BoneNodeNames : iter.BoneNodeNameList)
							{
								UMethod::SaveString(save, UMethod::ConvertWToS(BoneNodeNames));
							}
						}
						// Material Index
						save.write((_char*)&iter.iMatrialIndex, sizeof(iter.iMatrialIndex));
					}
				}
			}
			// BoneNode
			{
				size_t SIZE = stModelDesc.BNodeDatas.size();
				save.write((_char*)&SIZE, sizeof(size_t));
				if (0 != SIZE) {
					for (auto& iter : stModelDesc.BNodeDatas) {
						// Name
						save.write((_char*)&iter.mOffsetMatrix, sizeof(_float4x4));
						save.write((_char*)&iter.mTransformMatrix, sizeof(_float4x4));
						save.write((_char*)&iter.fDepth, sizeof(_float));
						UMethod::SaveString(save, UMethod::ConvertWToS(iter.wstrParentsName));
						UMethod::SaveString(save, UMethod::ConvertWToS(iter.wstrName));
					}
				}
			}
			// Material
			{
				size_t SIZE = stModelDesc.MatrialData.size();
				save.write((_char*)&SIZE, sizeof(size_t));
				if (0 != SIZE) {
					for (auto& iter : stModelDesc.MatrialData) {
						save.write((_char*)&iter.first, sizeof(iter.first));
						// Materials
						size_t STRING_SIZE = iter.second.size();
						save.write((_char*)&STRING_SIZE, sizeof(size_t));

						for (auto& TextureName : iter.second) {
							UMethod::SaveString(save, UMethod::ConvertWToS(TextureName));
						}
					}
				}
			}
		}
		// Animation 
		{
			_uint iSize = (_uint)stModelDesc.Animes.size();
			save.write((_char*)&iSize, sizeof(_uint));
			if (iSize != 0)
			{
				for (auto& iter : stModelDesc.Animes)
				{
					save.write((_char*)&iter.stExtraData, sizeof(ANIMEXTRADATA));
					UMethod::SaveString(save, UMethod::ConvertWToS(iter.wstrName));
					_uint iChannelSize = (_uint)iter.Channels.size();
					save.write((_char*)&iChannelSize, sizeof(_uint));
					for (auto& Channel : iter.Channels)
					{
						UMethod::SaveString(save, UMethod::ConvertWToS(Channel.wstrBoneName));
						save.write((_char*)&Channel.iNumMaxKeyFrames, sizeof(_uint));
						for (_uint i = 0; i < Channel.iNumMaxKeyFrames; ++i)
						{
							save.write((_char*)&Channel.pKeyFrames[i], sizeof(KEYFRAME));
						}
					}
				}
			}
		}
	}
}

void TAssimpModel::LoadAnimation(const _wstring& _wstrPath)
{
	_uint iFlag = aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_Triangulate | aiProcess_CalcTangentSpace;

	// FilePath
	_string strFilePath = UMethod::ConvertWToS(_wstrPath);

	SHPTR<Assimp::Importer>		spImporter{ nullptr };
	const aiScene* pAIScene{ nullptr };

	// Read Improter 
	spImporter = std::make_shared<Assimp::Importer>();
	pAIScene = spImporter->ReadFile(strFilePath, iFlag);
	RETURN_CHECK(nullptr == pAIScene, ;);
	// 애니메이션 만들어서 집어 넣는다.
	CreateAnimation(pAIScene);
}

HRESULT TAssimpModel::CreateModel(const _wstring& _wstrPath)
{
	std::filesystem::path PATH{ _wstrPath };
	_wstring str = PATH.extension();
	// FBX
	RETURN_CHECK(str != L".fbx" && str != L".FBX", E_FAIL);

	_wstring Path = _wstrPath;
	// FilePath
	_string strFilePath = UMethod::ConvertWToS(Path);

	_uint iFlag = 0;
	if (TYPE::NONANIM == m_eModelType)
		iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_Triangulate | aiProcess_CalcTangentSpace;
	else
		iFlag = aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_Triangulate | aiProcess_CalcTangentSpace;

	// Read Improter 
	m_spImporter = std::make_shared<Assimp::Importer>();
	m_pAIScene = m_spImporter->ReadFile(strFilePath, iFlag);
	RETURN_CHECK(nullptr == m_pAIScene, E_FAIL);
	std::filesystem::path path = Path;

	m_wstrModelName = std::filesystem::path(Path).filename();
	size_t Index = m_wstrModelName.find(L".");
	m_wstrModelName = m_wstrModelName.substr(0, Index);

	CreateBoneNodes(m_pAIScene->mRootNode);
	// Create MeshContainers
	RETURN_CHECK_FAILED(CreateMeshContainers(m_mPivotMatrix), E_FAIL);
	RETURN_CHECK_FAILED(CreateMaterials(m_spFileGroup), E_FAIL);
	RETURN_CHECK_FAILED(CreateAnimation(m_pAIScene), E_FAIL);

	std::sort(m_vecBoneNodes.begin(), m_vecBoneNodes.end(), [&](SHPTR<TAssimpBoneNode>& p1, SHPTR<TAssimpBoneNode>& p2) {
		return p1->GetDepth() < p2->GetDepth();
		});

	return S_OK;
}

HRESULT TAssimpModel::CreateMeshContainers(const _xmmatrix& _pivotMatrix)
{
	// MeshContainers
	m_iNumMeshContainers = m_pAIScene->mNumMeshes;
	m_vecMeshContainers.reserve(m_iNumMeshContainers);
	// NumMeshContainers
	for (_uint i = 0; i < m_iNumMeshContainers; ++i) {

		SHPTR<TAssimpMeshContainer> pAssimpMeshContainers =
			CreateConstructorNative<TAssimpMeshContainer>(GetDevice(), m_pAIScene->mMeshes[i],
				(_uint)m_eModelType, ThisShared<TAssimpModel>(), _pivotMatrix);

		m_vecMeshContainers.push_back(pAssimpMeshContainers);
	}
	return S_OK;
}

HRESULT TAssimpModel::CreateMaterials(CSHPTRREF<FILEGROUP> _spFileGroup)
{
	SHPTR<FILEGROUP> spFileGroup = _spFileGroup->FindGroup(m_wstrTextureFolderName);
	if (nullptr == spFileGroup)
	{
		spFileGroup = _spFileGroup;
		_bool isFindTexture{ false };
		// Upper Finder
		{
			while (!isFindTexture)
			{
				spFileGroup = spFileGroup->spParentsUpper;
				for (auto& iter : spFileGroup->UnderFileGroupList)
				{
					if (iter.first == m_wstrTextureFolderName)
					{
						spFileGroup = iter.second;
						isFindTexture = true;
						break;
					}
				}
				if (nullptr == _spFileGroup)
					break;
			}
		}
		if (false == isFindTexture)
			// down Folder
		{
			spFileGroup = _spFileGroup;
			for (auto& iter : _spFileGroup->UnderFileGroupList)
			{
				if (iter.first == m_wstrTextureFolderName)
				{
					spFileGroup = iter.second;
					break;
				}
			}
			RETURN_CHECK(spFileGroup == _spFileGroup, E_FAIL);
		}
	}

	m_iNumMaterials = m_pAIScene->mNumMaterials;
	m_vecMaterials.reserve(m_iNumMaterials);
	_wstring wstrPath = spFileGroup->wstrPath;
	m_wstrTexturePath = wstrPath;

	for (_uint i = 0; i < m_iNumMaterials; ++i) {
		// Matrail 
		aiMaterial* pAiMaterial = m_pAIScene->mMaterials[i];

		_uint iCnt{ 0 };
		SHPTR<MODELMATRIALDESC> pModelMaterial = std::make_shared<MODELMATRIALDESC>();
		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j) {
			aiString strTexturePath{};
			char szName[MAX_PATH]{ "" };
			if (FAILED(pAiMaterial->GetTexture(aiTextureType(j), 0, &strTexturePath)))
				continue;

			if (!strcmp(strTexturePath.C_Str(), ""))
				continue;

			fs::path ps{ strTexturePath.data };

			_wstring strRealName = ps.filename();
			if (ps.extension() == L".TGA" || ps.extension() == L".tga")
			{
				size_t iIndex = strRealName.find(L".");
				strRealName = strRealName.substr(0, iIndex);
				strRealName += L".dds";
				m_lsTextureExts.push_back(L".dds");
			}
			else
			{
				m_lsTextureExts.push_back(ps.extension());
			}

			_wstring Path = wstrPath;
			Path += L"\\";
			Path += strRealName;
			// Create Texture 
			pModelMaterial->arrMaterialTexture[j] = CreateConstructorNativeNotMsg<UTexGroup>(GetDevice(), Path);
			if (nullptr == pModelMaterial->arrMaterialTexture[j]) {
				continue;
			}
			++iCnt;
		}
		if (iCnt > 0)
			m_vecMaterials.push_back(pModelMaterial);
	}

	m_wstrTexturePath += L"\\";
	return S_OK;
}

HRESULT TAssimpModel::CreateBoneNodes(aiNode* _pNode, SHPTR<TAssimpBoneNode> _pParents, const _float _fDepths)
{
	RETURN_CHECK(nullptr == _pNode, E_FAIL);

	_float4x4 TransformMatrix;
	memcpy(&TransformMatrix, &_pNode->mTransformation, sizeof(_float4x4));

	SHPTR<TAssimpBoneNode> pBoneNode = CreateNative<TAssimpBoneNode>(_pParents,
		_pNode->mName.data, TransformMatrix, _fDepths);

	m_vecBoneNodes.push_back({ pBoneNode });

	for (_uint i = 0; i < _pNode->mNumChildren; ++i) {
		CreateBoneNodes(_pNode->mChildren[i], pBoneNode, _fDepths + 0.1f);
	}
	return S_OK;
}

HRESULT TAssimpModel::CreateAnimation(const aiScene* _pAIScene)
{
	RETURN_CHECK(nullptr == _pAIScene, E_FAIL);

	m_iNumAnimation = _pAIScene->mNumAnimations;
	SHPTR<TAssimpModel> pModel = ThisShared<TAssimpModel>();
	for (_uint i = 0; i < m_iNumAnimation; ++i)
	{
		aiAnimation* pAIAnim = _pAIScene->mAnimations[i];

		SHPTR<TAssimpAnimation> pAnimation = CreateNative<TAssimpAnimation>(pAIAnim, pModel);
		if (nullptr == pAnimation)
			return S_OK;

		m_vecAnimations.push_back(pAnimation);
	}

	return S_OK;
}

void TAssimpModel::ConvertPath(_wstring& _wstrConvertPath)
{
	_wstrConvertPath.append(L"\\Convert\\");
	int MakeDir = _wmkdir(_wstrConvertPath.c_str());
	_wstrConvertPath.append(m_wstrModelName);
	_wstrConvertPath += L"_FBX";
	_wstrConvertPath += L".bin";
}
