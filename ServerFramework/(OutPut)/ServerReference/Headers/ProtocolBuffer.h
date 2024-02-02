
#include "Protocol.pb.h"
#include "Tag.pb.h"

namespace PROTOFUNC
{
	CS_LOGIN CreateCsLoginPacket(std::string _strName);
	CS_LOGOUT CreateCsLogOutPacket(long long _id);

	SC_CHECKLOGIN CreateScCheckLogin(long long _id);
	SC_REMOVE_OBJECT CreateScRemoveObject(long long _id);

	class PROTOCOL_MEMORY_LEAK_REMOVER
	{
	public:
		PROTOCOL_MEMORY_LEAK_REMOVER(){}
		~PROTOCOL_MEMORY_LEAK_REMOVER();
	};
}

static PROTOFUNC::PROTOCOL_MEMORY_LEAK_REMOVER MemoryRemover;