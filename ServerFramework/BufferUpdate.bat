

// // xcopy /옵션 .복사할 파일이 들어가있는 경로      .파일들을 복사할 경로 

//xcopy /y/s	.\ProtocolBuffer\ServerProtocol\*.*						.\Reference\Headers\
//xcopy /y/s	.\ProtocolBuffer\Public\*.*										.\Reference\Headers\
//xcopy /y/s	.\ProtocolBuffer\Bin\*.lib											.\Reference\Librarys\
//xcopy /y/s	.\ProtocolBuffer\ThirdPartyLib\*.lib						.\Reference\Librarys\

xcopy /y/s	.\Core\Public\boost\*.*											.\(Output)\Headers\boost\
xcopy /y/s .\Core\Public\CorePacket.h										.\(Output)\Headers\

xcopy /y/s	.\ProtocolBuffer\ServerProtocol\*.*						.\(Output)\Headers\
xcopy /y/s	.\ProtocolBuffer\Public\*.*										.\(Output)\Headers\
xcopy /y/s	.\ProtocolBuffer\Bin\*.lib											.\(Output)\Librarys\
xcopy /y/s	.\ProtocolBuffer\ThirdPartyLib\*.lib						.\(Output)\Librarys\
