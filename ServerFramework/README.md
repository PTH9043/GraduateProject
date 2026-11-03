# ServerFramework

안녕하세요. 박태현입니다.

이 폴더에는 세 명이 함께 개발한 졸업 프로젝트 Code Dungeon의 서버 코드가 들어 있습니다. 2023년 12월부터 2024년 7월까지 개발했으며, 저는 공통 프레임워크와 서버 통신, 애니메이션·장비 부착 도구를 맡았습니다.

## 맡은 작업

- **서버 통신:** Boost.Asio와 Protocol Buffers를 사용해 비동기 통신을 구성했습니다. ASession에서 공통 수신을 처리하고 CPlayerSession에서 게임 메시지를 처리하도록 나누었습니다.
- **패킷 처리:** 헤더의 메시지 종류와 크기를 확인해 완성된 패킷을 처리하고, 데이터가 나누어 도착하면 남은 바이트를 수신 버퍼에 보관해 다음 데이터와 함께 처리했습니다.
- **수신 오류 수정:** 패킷 처리 후 남은 데이터를 옮기는 위치 계산의 오류로 데이터가 누락되는 문제를 찾아 수정했습니다.
- **메모리 관리:** 크기별 메모리 풀에서 반환된 블록을 재사용하고, STL 컨테이너의 할당에도 풀을 사용할 수 있도록 했습니다.

서버 폴더에는 팀원들이 함께 작성한 코드도 포함되어 있습니다.

## 관련 코드와 자료

- [공통 세션과 수신 버퍼](./Core/Private/ASession.cpp)
- [플레이어 메시지 처리](./Server/Private/CPlayerSession.cpp)
- [메모리 풀](./Core/Public/Memory.h) · [STL 할당기](./Core/Public/Allocator.h)
- [서버 솔루션](./ServerFramework.sln) · [클라이언트 코드](../CodeDungeon) · [프로젝트 시연 영상](https://youtu.be/hQBBsoL_ETs)

C++20, Boost.Asio와 Protocol Buffers 등을 사용했습니다. Windows와 Visual Studio에서 개발했으며, 빌드에는 원본에서 사용한 외부 라이브러리와 리소스 설정을 별도로 준비해야 합니다.

2026년에는 Claude·Codex의 도움을 받아 별도 재현 빌드에서 통신을 추가로 검증했습니다. 이는 2024년 원본 코드에서 당시 수행한 검사와 구분되는 작업입니다.
