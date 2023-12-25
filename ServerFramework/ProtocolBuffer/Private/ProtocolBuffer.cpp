#include <string>
#include "ProtocolBuffer.h"
#include <thread>

void PROTOFUNC::MakeCsLogin(const std::string& _strName, CS_LOGIN& _login)
{
	_login.set_user_name(_strName);
}

PROTOFUNC::PROTOCOL_MEMORY_LEAK_REMOVER::~PROTOCOL_MEMORY_LEAK_REMOVER()
{
	google::protobuf::ShutdownProtobufLibrary();
}