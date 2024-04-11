#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_CORESTRUCT_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_CORESTRUCT_H

namespace Core {

	/*
	@ Date: 2024-04-07, Writer: 박태현
	@ Explain: 공간에 대한 정의 
	*/
	struct SPACEINFO {
		Vector3		vCenter;
		Vector3		vExtents;
		_int				Depths;
		_int				Index;

		SPACEINFO() : vCenter{}, vExtents{}, Depths{ 0 }, Index{ 0 } {}
		SPACEINFO(const Vector3& _vCenter, const Vector3& _vExtents, _int _Depths, _int _SpaceIndex = 0) :
			vCenter{ _vCenter }, vExtents{ _vExtents }, Depths{ _Depths }, Index{ _SpaceIndex } {}
	};
}
#endif // _SERVERFRAMEWORK_CORE_PUBLIC_CORESTRUCT_H