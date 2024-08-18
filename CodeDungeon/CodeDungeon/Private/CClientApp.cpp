#include "ClientDefines.h"
#include "CClientApp.h"
#include "UGameInstance.h"
#include "UTimer.h"
#include "CNetworkClientController.h"
#include "CProtoMaker.h"
#include "CMainScene.h"
#include "CDataManager.h"
#include "CLogoScene.h"

class LoadIPAddres {
public:
	LoadIPAddres(const _string& _strPath) {
		std::ifstream file(_strPath); // 읽을 파일 열기
		if (!file.is_open()) {
			return;
		}
		std::ostringstream buffer; // 문자열 스트림 생성
		buffer << file.rdbuf(); // 파일 내용을 버퍼로 읽어오기

		m_strIpAddress = buffer.str().c_str();
	}
public:
	const _string& GetIPAddress() const { return m_strIpAddress; }
private:
	_string		m_strIpAddress{""};
};


CClientApp::CClientApp() :
	m_iTickCount{ 0 },
	m_hwnd{ NULL },
	m_dRenderTick{ 0.0 },
	m_spGameInstance{ nullptr },
	m_spTickTimer{ nullptr },
	m_spDeltaTimer{ nullptr },
	m_spRenderDeltaTimer{ nullptr },
	m_spRenderTimer{ nullptr },
	m_dDeltaTime{ 0 },
	TICK_DOUBLE{ 1 / 60.f },
	m_dShowTickFPS{ 0.0 },
	DELTA_TIMER{ L"DELTA_TIMER" },
	TICK_TIMER{ L"TICK_TIMER" },
	RENDER_TIMER{ L"RENDER_TIMER" },
	RENDER_DELETATIMER{ L"RENDER_DELTATIMER" },
	m_isTickThread{ true },
	m_spDataManager{nullptr},
	m_spNetworkClientController{ nullptr }
{
}

void CClientApp::Free()
{
}

HRESULT CClientApp::NativeConstruct(const HINSTANCE& _hInst, const _uint& _iCmdShow)
{
	m_hInst = _hInst;
	m_iCmdShow = _iCmdShow;
	m_spGameInstance = GET_INSTANCE(UGameInstance);

	LoadIPAddres IPAddress("..\\..\\Resource\\Ip.txt");
	m_spNetworkClientController = std::make_shared<CNetworkClientController>();
	m_spNetworkClientController->NativeConstruct(IPAddress.GetIPAddress(), TCP_PORT_NUM);
	m_spGameInstance->StartNetwork(m_spNetworkClientController);
	return S_OK;
}

void CClientApp::Render()
{
	if (FALSE == InitInstance(m_hInst, m_iCmdShow))
	{
		return;
	}

	/* Graphic Desc */
	GRAPHICDESC	stGraphicDesc;
	RECT rect;
	GetClientRect(m_hwnd, &rect);
	{
		stGraphicDesc.eWinMode = GRAPHICDESC::MODE_WIN;
		stGraphicDesc.hInst = m_hInst;
		stGraphicDesc.hWnd = m_hwnd;
		stGraphicDesc.iWinCX = static_cast<_int>(rect.right);
		stGraphicDesc.iWinCY = static_cast<_int>(rect.bottom);
		DXVALUE::g_iWindowWidth = stGraphicDesc.iWinCX;
		DXVALUE::g_iWindowHeight = stGraphicDesc.iWinCY;
	}
	OUTPUTDATA stOutputData;
	RETURN_CHECK_FAILED(m_spGameInstance->ReadyInstance(stGraphicDesc, stOutputData), ;);
	RETURN_CHECK_FAILED(m_spGameInstance->LoadFirstFolder(FIRST_RESOURCE_FOLDER), ;);
	m_spDataManager = CreateNative<CDataManager>();
	CProtoMaker::CreateProtoData(m_spGameInstance, stOutputData.wpDevice.lock(), stOutputData.wpGpuCmd.lock());
	m_spDataManager->Load_Data();

	//m_spNetworkClientController->ServerTick();
	// Register 
	//m_spGameInstance->RegisterScene(CreateConstructorNative<CLogoScene>(stOutputData.wpDevice.lock()));
	m_spGameInstance->RegisterScene(CreateConstructorNative<CMainScene>(stOutputData.wpDevice.lock()));

	m_spDeltaTimer = m_spGameInstance->CreateTimerAdd(DELTA_TIMER);
	m_spTickTimer = m_spGameInstance->CreateTimerAdd(TICK_TIMER);
	m_spRenderTimer = m_spGameInstance->CreateTimerAdd(RENDER_TIMER);
	m_spRenderDeltaTimer = m_spGameInstance->CreateTimerAdd(RENDER_DELETATIMER);
	
	MSG msg{};
	ZeroMemory(&msg, sizeof(MSG));
	while (m_isTickThread)
	{
		// if PeekMeesage
		while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				m_isTickThread = false;
				m_spNetworkClientController.reset();
				m_spGameInstance->NetworkEnd();
				m_spGameInstance.reset();
				::PostQuitMessage(0);
				return;
			}
		}
		if (m_spGameInstance->GetDIKeyDown(DIK_ESCAPE))
		{
			m_spNetworkClientController.reset();
			m_spGameInstance->NetworkEnd();
			m_spGameInstance.reset();
			::PostQuitMessage(0);
			break;
		}
		// Run Func
		m_spDeltaTimer->Tick();

		m_dDeltaTime += m_spDeltaTimer->GetDeltaTime();
		if (m_dDeltaTime >= TICK_DOUBLE)
		{
			m_spTickTimer->Tick();
			m_dDeltaTime = m_spTickTimer->GetDeltaTime();

			m_spGameInstance->AwakeTick();
			// Tick 
			m_spGameInstance->Tick(m_dDeltaTime);
			// LateTick
			m_spGameInstance->LateTick(m_dDeltaTime);
			// Render
			m_spGameInstance->RenderBegin();
			m_spGameInstance->RenderEnd();
			++m_iTickCount;
			m_dShowTickFPS += m_dDeltaTime;
			if (m_dShowTickFPS >= 1.0)
			{
				static _tchar FPS[200];
				wsprintf(FPS, TEXT("RFPS : %d"), m_iTickCount);
				SetWindowTextW(m_hwnd, FPS);
				m_iTickCount = 0;
				m_dShowTickFPS = 0.0;
			}
			m_dDeltaTime = 0.0;
		}
	}
}

void CClientApp::KeyUpdate(WPARAM _wParam)
{
	m_spGameInstance->OtherFrame(m_dDeltaTime, _wParam);
}

BOOL CClientApp::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	RECT rc = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, true);

	m_hwnd = CreateWindowW(L"CodeDungeon", L"CodeDungeons", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);

	if (!m_hwnd)
	{
		return FALSE;
	}

	ShowWindow(m_hwnd, nCmdShow);
	UpdateWindow(m_hwnd);

	return TRUE;
}
