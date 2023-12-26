
#include "Protocol.pb.h"
#include "Tag.pb.h"

namespace PROTOFUNC
{
	void MakeCsLogin(const std::string& _strName, CS_LOGIN& _login);

	class PROTOCOL_MEMORY_LEAK_REMOVER
	{
	public:
		PROTOCOL_MEMORY_LEAK_REMOVER(){}
		~PROTOCOL_MEMORY_LEAK_REMOVER();
	};
}

static PROTOFUNC::PROTOCOL_MEMORY_LEAK_REMOVER MemoryRemover;