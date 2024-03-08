#include "ClientDefines.h"
#include "UNetworkClientController.h"

UNetworkClientController::UNetworkClientController()
{
}

HRESULT UNetworkClientController::NativeConstruct(const _string& _strIPAddress, const _int _PortNumber)
{
	RETURN_CHECK_FAILED(__super::NativeConstruct(_strIPAddress, _PortNumber), E_FAIL);
	return S_OK;
}

void UNetworkClientController::NativePacket()
{
	__super::NativePacket();
	CS_LOGIN Login;
	Login.set_time_test(CurrentMilliseconds());
	Login.set_user_name("Hello");
	SendProtoData(Login, TAG_CS_LOGIN);
}

void UNetworkClientController::ProcessPacket(_char* _pPacket, PACKETHEAD _PacketHead)
{
	switch (_PacketHead.PacketType)
	{
	case TAG_SERVER::TAG_SC_LOGIN:
	{
		SC_CHECKLOGIN Login;
		Login.ParseFromArray(_pPacket, static_cast<_int>(_PacketHead.PacketSize));
	}
	break;
	}
}

void UNetworkClientController::Free()
{
}
