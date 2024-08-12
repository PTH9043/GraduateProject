#include "ServerDefines.h"
#include "CMobLayoutLoader.h"

namespace Server {

	CMobLayoutLoader::CMobLayoutLoader(const _string& _strPath, const _string& _layoutName)
	{
		std::ifstream read{ _strPath, std::ios::binary };
		RETURN_CHECK(true == read.fail(), ;);

		size_t LayOutSize;
		read.read((_char*)&LayOutSize, sizeof(size_t));
		m_MobData.reserve(LayOutSize);

		for (size_t i = 0; i < LayOutSize; ++i)
		{
			MOBDATA MobData;

			size_t ObjectNameSize = 0;
			read.read((_char*)&ObjectNameSize, sizeof(size_t));

			size_t ObjectAnimNameSize = 0;
			read.read((_char*)&ObjectAnimNameSize, sizeof(size_t));

			_string ObjName;
			_string AnimName;
			MobData.strAnimModelName.resize(ObjectNameSize);
			read.read(&MobData.strAnimModelName[0], ObjectNameSize);

			MobData.strAnimName.resize(ObjectAnimNameSize);
			read.read(&MobData.strAnimName[0], ObjectAnimNameSize);

			read.read((_char*) & MobData.mWorldMatrix, sizeof(_float4x4));

			MobData.strRoomName = _layoutName;
			m_MobData.push_back(MobData);
		}
	}


	CStaticObjectLayoutLoader::CStaticObjectLayoutLoader(const _string& _strPath, const _string& _layoutName)
	{
		std::ifstream read{ _strPath, std::ios::binary };
		RETURN_CHECK(true == read.fail(), ;);

		size_t LayOutSize;
		read.read((_char*)&LayOutSize, sizeof(size_t));
		m_MobData.reserve(LayOutSize);

		for (size_t i = 0; i < LayOutSize; ++i)
		{
			MOBDATA MobData;

			size_t ObjectNameSize = 0;
			read.read((_char*)&ObjectNameSize, sizeof(size_t));

			_string ObjName;
			MobData.strAnimModelName.resize(ObjectNameSize);
			read.read(&MobData.strAnimModelName[0], ObjectNameSize);

			read.read((_char*)&MobData.mWorldMatrix, sizeof(_float4x4));
			MobData.strRoomName = _layoutName;
			m_MobData.push_back(MobData);
		}
	}

	CMobServerLayoutLoader::CMobServerLayoutLoader(const _string& _strPath)
	{
		std::ifstream read{ _strPath, std::ios::binary };
		ASSERT_CRASH(false == read.fail());

		size_t MobDatsSize = 0;
		read.read((_char*)&MobDatsSize, sizeof(size_t));
		m_MobData.reserve(MobDatsSize);
		for (_int i = 0; i < MobDatsSize; ++i)
		{
			MOBSERVERDATA mobServerData;
			read.read((_char*)&mobServerData.iStartAnimIndex, sizeof(_int));
			read.read((_char*)&mobServerData.iMobType, sizeof(_int));
			read.read((_char*)&mobServerData.mWorldMatrix, sizeof(_float4x4));
			read.read((_char*)&mobServerData.iMobID, sizeof(_int));
			read.read((_char*)&mobServerData.iMobParentsID, sizeof(_int));
			ReadString(read, mobServerData.strRoomName);
			m_MobData.emplace_back(mobServerData);
		}
	}


	CStaticObjServerLayoutLoader::CStaticObjServerLayoutLoader(const _string& _strPath)
	{
		std::ifstream read{ _strPath, std::ios::binary };
		ASSERT_CRASH(false == read.fail());

		size_t StaticObjDataSize = 0;
		read.read((_char*)&StaticObjDataSize, sizeof(size_t));
		m_StaticObjData.reserve(StaticObjDataSize);
		for (_int i = 0; i < StaticObjDataSize; ++i)
		{
			STATICOBJSERVERDATA staticObjServerData;
			read.read((_char*)&staticObjServerData.mWorldMatrix, sizeof(_float4x4));
			read.read((_char*)&staticObjServerData.iServerID, sizeof(_int));
			read.read((_char*)&staticObjServerData.iObjType, sizeof(_int));
			ReadString(read, staticObjServerData.strRoomName);
			m_StaticObjData.emplace_back(staticObjServerData);
		}
	}
}