# Code Dungeon

안녕하세요. 박태현입니다.

Code Dungeon은 2023년 12월부터 2024년 7월까지 세 명이 함께 개발한 졸업 프로젝트입니다. 이 폴더에는 DirectX12 클라이언트와 공통 엔진, 편집 도구가 들어 있습니다. 저는 공통 프레임워크와 서버 통신, 애니메이션·장비 부착 도구를 맡았습니다.

## 맡은 작업

- **객체 생성과 공통 기능:** C++20 Concepts로 `Create<T>`와 `CreateNative<T>`의 생성 조건을 제한하고, 등록한 원형을 복제해 객체를 만들도록 구성했습니다. 객체와 리소스 등 공통 기능은 UGameInstance의 공개 함수를 통해 사용하도록 했습니다.
- **리소스 관리:** 폴더와 파일을 트리로 관리하고 이름과 경로로 찾을 수 있도록 구성했습니다.
- **애니메이션 편집:** 전환 조건과 이벤트를 편집하고 결과를 확인하는 도구를 만들었습니다. 편집한 설정은 파일로 저장해 게임에서 사용할 수 있도록 했습니다.
- **본과 장비:** 애니메이션 채널이 사용하는 본을 기준으로 이동·회전 값을 다루고, 본의 합성 변환으로 장비의 위치와 회전을 갱신했습니다.

스마트 포인터는 교수님께서 제공해 주신 코드를 바탕으로 캐스팅과 메모리 풀 연동을 추가했습니다. 이 폴더에는 팀원들이 함께 작성한 코드도 포함되어 있습니다.

## 관련 코드와 자료

- [공통 객체와 생성 규칙](./Engine/public/UObject.h)
- [GameInstance](./Engine/public/UGameInstance.h)
- [리소스 파일과 경로 관리](./Engine/private/UFilePathManager.cpp)
- [애니메이션 이벤트 편집](./Tool/Private/TAnimControlView.cpp)
- [본 변환과 장비 부착](./Engine/private/UBoneNode.cpp)
- [클라이언트 솔루션](./CodeDungeon.sln) · [서버 코드](../ServerFramework) · [프로젝트 시연 영상](https://youtu.be/hQBBsoL_ETs)

C++20, DirectX12, ImGui, Assimp와 STL 등을 사용했습니다. Windows와 Visual Studio에서 개발했으며, 빌드에는 원본에서 사용한 외부 라이브러리와 리소스 설정을 별도로 준비해야 합니다.
