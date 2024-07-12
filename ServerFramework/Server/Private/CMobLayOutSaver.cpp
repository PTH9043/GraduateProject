#include "ServerDefines.h"
#include "CMobLayOutSaver.h"

namespace Server {

	CMobLayOutSaver::CMobLayOutSaver(const _string& _strPath, const VECTOR< MOBDATA>& _MobData)
	{
		std::ofstream save{ _strPath, std::ios::binary };
		assert(false == save.fail());

		size_t MobDatsSize = _MobData.size();
		save.write((_char*)&MobDatsSize, sizeof(size_t));

		for (auto& iter : _MobData)
		{
			SaveString(save, iter.strAnimName);
			SaveString(save, iter.strAnimModelName);
			save.write((_char*)&iter.mWorldMatrix, sizeof(_float4x4));
			save.write((_char*)&iter.iConnectIndex, sizeof(_int));
		}
	}
}