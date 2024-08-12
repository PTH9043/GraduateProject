#include "ServerDefines.h"
#include "CMobLayOutSaver.h"

namespace Server {

	CMobLayOutSaver::CMobLayOutSaver(const _string& _strPath, const VECTOR< MOBSERVERDATA>& _MobData)
	{
		std::ofstream save{ _strPath, std::ios::binary };
		assert(false == save.fail());

		size_t MobDatsSize = _MobData.size();
		save.write((_char*)&MobDatsSize, sizeof(size_t));
		for (auto& iter : _MobData)
		{
			save.write((_char*)&iter.iStartAnimIndex, sizeof(_int));
			save.write((_char*)&iter.iMobType, sizeof(_int));
			save.write((_char*)&iter.mWorldMatrix, sizeof(_float4x4));
			save.write((_char*)&iter.iMobID, sizeof(_int));
			save.write((_char*)&iter.iMobParentsID, sizeof(_int));
			SaveString(save, iter.strRoomName);
		}
	}

	CStaticObjLayoutSaver::CStaticObjLayoutSaver(const _string& _strPath, const VECTOR<STATICOBJSERVERDATA>& _ObjData)
	{
		std::ofstream save{ _strPath, std::ios::binary };
		assert(false == save.fail());

		size_t MobDatsSize = _ObjData.size();
		save.write((_char*)&MobDatsSize, sizeof(size_t));
		for (auto& iter : _ObjData)
		{
			save.write((_char*)&iter.mWorldMatrix, sizeof(_float4x4));
			save.write((_char*)&iter.iServerID, sizeof(_int));
			save.write((_char*)&iter.iObjType, sizeof(_int));
			SaveString(save, iter.strRoomName);
		}
	}
}