#ifndef _PTH_FRAMEWORK_TOOL_PUBLIC_TPROTOMAKER_H
#define _PTH_FRAMEWORK_TOOL_PUBLIC_TPROTOMAKER_H

BEGIN(Engine)
class UGameInstance;
class UDevice;
class UCommand;
class USkyBox;
END

BEGIN(Tool)


class TProtoMaker {
public:
	static HRESULT CreateProtoData(CSHPTRREF<UGameInstance> _spGameInstance,
		CSHPTRREF<UDevice> _spDevice, CSHPTRREF<UCommand> _spCommand);
};

END


#endif // #ifndef _PTH_FRAMEWORK_TOOL_PUBLIC_TPROTOMAKER_H
