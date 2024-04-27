#include "ClientDefines.h"
#include "CNetworkClientController.h"

CNetworkClientController::CNetworkClientController()
{
}

HRESULT CNetworkClientController::NativeConstruct(const _string& _strIPAddress, const _int _PortNumber)
{
	RETURN_CHECK_FAILED(__super::NativeConstruct(_strIPAddress, _PortNumber), E_FAIL);
	return S_OK;
}

void CNetworkClientController::NativePacket()
{
	//__super::NativePacket();
	//CS_LOGIN Login;
	//Login.set_time_test(CurrentMilliseconds());
	//Login.set_user_name("Hello");
	//SendProtoData(Login, TAG_CS_LOGIN);
}

void CNetworkClientController::ProcessPacket(_char* _pPacket, PACKETHEAD _PacketHead)
{
	/*switch (_PacketHead.PacketType)
	{
	case TAG_SERVER::TAG_SC_LOGIN:
	{
		{
			SC_CHECKLOGIN Login;
			Login.ParseFromArray(_pPacket, static_cast<_int>(_PacketHead.PacketSize));
		}
		{
			CS_LOGIN Login;
			Login.set_time_test(CurrentMilliseconds());
			Login.set_user_name("Hello");
			SendProtoData<CS_LOGIN>(Login, TAG_CS_LOGIN);
		}
	}
	break;
	}*/
}

void CNetworkClientController::Free()
{
}
