# Code Dungeon and ServerFramework

2024년 1월부터 7월까지 진행한 3인 팀 프로젝트 Code Dungeon의 클라이언트·엔진·편집 도구와 서버 코드를 모았습니다.

## 프로젝트 구성

| 폴더 | 내용 | 솔루션 |
| --- | --- | --- |
| [CodeDungeon](./CodeDungeon) | DirectX12 클라이언트, 공통 엔진, ImGui 편집 도구 | [CodeDungeon.sln](./CodeDungeon/CodeDungeon.sln) |
| [ServerFramework](./ServerFramework) | Boost.Asio 서버, 공통 세션, Protocol Buffers 메시지, 테스트 클라이언트 | [ServerFramework.sln](./ServerFramework/ServerFramework.sln) |

사용 기술: C++20, DirectX12, Boost.Asio, Protocol Buffers, ImGui, Assimp, STL.

## 박태현 담당 범위

- 공통 프레임워크의 객체 생성 규칙, 메모리 풀 연동, 리소스 파일과 경로 관리
- 공통 세션의 수신 처리와 게임 메시지 처리
- 애니메이션 이벤트 편집, 본 변환을 이용한 장비 위치와 회전 갱신

팀 프로젝트 전체 코드를 포함합니다. 위 담당 범위는 박태현의 작업이며, 스마트 포인터는 교수님이 제공한 코드를 바탕으로 캐스팅과 풀 연동을 추가했습니다.

## 주요 코드 바로 보기

- [객체 생성과 공통 객체 구조](./CodeDungeon/Engine/public/UObject.h)
- [리소스 파일과 경로 관리](./CodeDungeon/Engine/private/UFilePathManager.cpp)
- [애니메이션 이벤트 편집](./CodeDungeon/Tool/Private/TAnimControlView.cpp)
- [본 변환과 장비 부착](./CodeDungeon/Engine/private/UBoneNode.cpp)
- [공통 세션과 수신 버퍼](./ServerFramework/Core/Private/ASession.cpp)
- [게임 메시지 처리](./ServerFramework/Server/Private/CPlayerSession.cpp)

## 시연

[Code Dungeon 프로젝트 시연](https://youtu.be/hQBBsoL_ETs)

## 코드 기준과 개발 환경

원본 기준: [`364a7dd`](https://github.com/PTH9043/GraduateProject/commit/364a7ddd7c360cf60e95601ff5668e97c254d1bb). 두 프로젝트의 파일은 이 공개 커밋과 같습니다.

Windows의 Visual Studio C++ 솔루션입니다. 원본 프로젝트가 사용하는 외부 헤더·라이브러리와 리소스 설정이 필요합니다. `.gitignore`에 지정된 `Reference` 폴더 등은 공개 저장소에 포함되지 않으므로, 이 브랜치만 내려받아 빌드하려면 해당 의존성을 별도로 준비해야 합니다.

2026년 AI 지원 재현 빌드와 검증 기록은 이 브랜치의 2024년 원본 코드와 구분합니다.
