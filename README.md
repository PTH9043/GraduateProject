# Code Dungeon 및 ServerFramework

안녕하세요. 박태현입니다.

졸업 프로젝트로 개발한 Code Dungeon의 클라이언트와 서버 코드를 정리했습니다. 2024년 1월부터 7월까지 세 명이 함께 개발했고, 저는 공통 프레임워크와 서버 통신, 애니메이션 편집 도구를 맡았습니다.

## 프로젝트 구성

| 폴더 | 내용 | 솔루션 |
| --- | --- | --- |
| [CodeDungeon](./CodeDungeon) | DirectX12 클라이언트와 공통 엔진, ImGui 편집 도구 | [CodeDungeon.sln](./CodeDungeon/CodeDungeon.sln) |
| [ServerFramework](./ServerFramework) | Boost.Asio 서버와 공통 세션, Protocol Buffers 메시지, 테스트 클라이언트 | [ServerFramework.sln](./ServerFramework/ServerFramework.sln) |

C++20, DirectX12, Boost.Asio, Protocol Buffers, ImGui, Assimp와 STL을 사용했습니다.

## 제가 맡은 작업

- 객체 생성 규칙을 정하고 메모리 풀을 객체와 STL 컨테이너의 할당에 연결했습니다.
- 리소스의 폴더와 파일을 관리하고 이름과 경로로 찾는 기능을 만들었습니다.
- 공통 세션의 수신 처리와 플레이어 메시지 처리를 구현했습니다. 패킷을 처리한 뒤 남은 데이터를 옮기는 위치 계산이 잘못되어 데이터가 누락되는 문제도 수정했습니다.
- 애니메이션 이벤트를 편집하고 확인하는 도구를 만들었습니다. 실제 애니메이션 채널에서 사용하는 본을 기준으로 이동·회전 값을 추출하고, 본의 합성 변환을 이용해 장비 위치와 회전을 갱신했습니다.

스마트 포인터는 교수님께서 제공해 주신 코드를 바탕으로 캐스팅과 메모리 풀 연동 기능을 추가했습니다. 두 폴더에는 팀이 함께 개발한 코드도 포함되어 있으며, 위 목록은 제가 담당한 작업입니다.

## 관련 코드

- [객체 생성과 공통 객체 구조](./CodeDungeon/Engine/public/UObject.h)
- [리소스 파일과 경로 관리](./CodeDungeon/Engine/private/UFilePathManager.cpp)
- [애니메이션 이벤트 편집](./CodeDungeon/Tool/Private/TAnimControlView.cpp)
- [본 변환과 장비 부착](./CodeDungeon/Engine/private/UBoneNode.cpp)
- [공통 세션과 수신 버퍼](./ServerFramework/Core/Private/ASession.cpp)
- [플레이어 메시지 처리](./ServerFramework/Server/Private/CPlayerSession.cpp)

## 시연 영상

[Code Dungeon 프로젝트 시연 보기](https://youtu.be/hQBBsoL_ETs)

## 개발 환경과 코드 기준

Windows와 Visual Studio에서 개발한 C++ 프로젝트입니다. 두 프로젝트의 코드는 2024년 공개 원본인 [`364a7dd`](https://github.com/PTH9043/GraduateProject/commit/364a7ddd7c360cf60e95601ff5668e97c254d1bb)와 같습니다.

빌드하려면 원본 프로젝트에서 사용한 외부 헤더·라이브러리와 리소스 설정이 필요합니다. `.gitignore`에 지정된 `Reference` 폴더 등은 저장소에 포함되어 있지 않아 별도로 준비해야 합니다.

2026년에는 Claude·Codex의 도움을 받아 별도 재현 빌드에서 통신을 추가로 검증했습니다. 이력서의 추가 검증 기록은 그 작업의 결과이며, 이 브랜치에 있는 2024년 원본 코드에서 수행한 검사와는 구분했습니다.

읽어 주셔서 감사합니다.
