#include <string>
#include "ProtocolBuffer.h"
#include <thread>


PROTOFUNC::PROTOCOL_MEMORY_LEAK_REMOVER::~PROTOCOL_MEMORY_LEAK_REMOVER()
{
	google::protobuf::ShutdownProtobufLibrary();
}

CS_LOGIN PROTOFUNC::CreateCsLoginPacket(std::string _strName)
{
	CS_LOGIN login;
	login.set_user_name(_strName);
	return login;
}

CS_LOGOUT PROTOFUNC::CreateCsLogOutPacket(long long _id)
{
	CS_LOGOUT logout;
	logout.set_id(_id);
	return logout;
}

SC_CHECKLOGIN PROTOFUNC::CreateScCheckLogin(long long _id)
{
	SC_CHECKLOGIN checkin;
	checkin.set_id(_id);
	return checkin;
}

SC_REMOVE_OBJECT PROTOFUNC::CreateScRemoveObject(long long _id)
{
	SC_REMOVE_OBJECT remoeObj;
	remoeObj.set_id(_id);
	return remoeObj;
}
