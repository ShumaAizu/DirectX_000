//=============================================================================
//
//	シューティングゲームアルファ版 [main.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "player.h"
#include "input.h"
#include "sound.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "ranking.h"
#include "fade.h"
#include "option.h"
#include <cstdio>
#include <crtdbg.h>


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME			"WindowClass"							// ウインドウクラスの名前
#define WINDOW_NAME			"シューティングゲームアルファ版"		// ウインドウの名前

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawDebug(void);
void DrawControl(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3D9 g_pD3D = NULL;					// Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;		// Direct3Dデバイスへのポインタ
MODE g_mode = MODE_TITLE;					// モード情報
LPD3DXFONT g_pFont = NULL;					// フォントへのポインタ
int g_nCountFPS = 0;						// FPSカウンタ

//=============================================================================
// メイン関数
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);			// メモリリーク検知用のフラグ
#endif // _DEBUG

	WNDCLASSEX wcex =									// ウインドウクラスの構造体
	{
		sizeof(WNDCLASSEX),								// WNDCLASSEXのメモリサイズ
		CS_CLASSDC,										// ウインドウのスタイル
		WindowProc,										// ウインドウプロシージャ
		0,												// 0にする(通常は使用しない)
		0,												// 0にする(通常は使用しない)
		hInstance,										// インスタンスハンドル
		LoadIcon(NULL, IDI_APPLICATION),				// タスクバーのアイコン
		LoadCursor(NULL, IDC_ARROW),					// マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),						// クライアント領域の背景色
		NULL,											// メニューバー
		CLASS_NAME,										// ウインドウクラスの名前
		LoadIcon(NULL, IDI_APPLICATION)					// ファイルのアイコン
	};

	HWND hWnd;				// ウインドウハンドル(識別子)
	MSG msg;				// メッセージを格納する変数
	DWORD dwCurrentTime;	// 現在時刻
	DWORD dwExecLastTime;	// 最後に処理した時刻
	DWORD dwFrameCount;		// フレームカウント
	DWORD dwFPSLastTime;	// 最後にFPSを計測した時刻

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };	// 画面サイズの構造体

	// ウインドウクラスの登録
	RegisterClassEx(&wcex);

	// クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// ウインドウを生成
	hWnd = CreateWindowEx(0,							// 拡張ウインドウスタイル
						  CLASS_NAME,					// ウインドウクラスの名前
						  WINDOW_NAME,					// ウインドウの名前
						  WS_OVERLAPPEDWINDOW,			// ウインドウスタイル
						  CW_USEDEFAULT,				// ウインドウの左上X座標
						  CW_USEDEFAULT,				// ウインドウの左上Y座標
						  (rect.right - rect.left),		// ウインドウの幅
						  (rect.bottom - rect.top),		// ウインドウの高さ
						  NULL,							// 親ウインドウのハンドル
						  NULL,							// メニューハンドルまたは子ウインドウID
						  hInstance,					// インスタンスハンドル
						  NULL);						// ウインドウ作成データ

	// 初期化処理
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{// 初期化処理が失敗した場合
		return - 1;
	}

	// 分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;						// 初期化する
	dwExecLastTime = timeGetTime();			// 現在時刻を取得(保存)

	dwFrameCount = 0;						// 初期化する
	dwFPSLastTime = timeGetTime();			// 現在時刻を取得(保存)

	// ウインドウを表示
	ShowWindow(hWnd, nCmdShow);				// ウインドウの表示状態を設定
	UpdateWindow(hWnd);						// クライアント領域を更新

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{// windowsの処理
			if (msg.message == WM_QUIT)
			{// WM_QUITメッセージを受け取ったらループを抜ける
				break;
			}
			else
			{
			// メッセージの設定
			TranslateMessage(&msg);				// 仮想メッセージを文字メッセージへ変換
			DispatchMessage(&msg);				// ウインドウプロシージャへメッセージを送出
			}
		}
		else
		{// DirectXの処理
			dwCurrentTime = timeGetTime();		// 現在時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5秒経過
				// FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;	// FPSを計測した時間を保存
				dwFrameCount = 0;				// フレームカウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 60分の1秒経過
				dwExecLastTime = dwCurrentTime;	// 処理開始の時刻[現在時刻]を保存

				// 更新処理
				Update();

				// 描画処理
				Draw();

				dwFrameCount++;					// フレームカウントを加算
			}
		}
	}

	// 終了処理
	Uninit();

	// ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//===============================================================================
// ウインドウプロシージャ
//===============================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	int nID;

	switch (uMsg)
	{
	case WM_DESTROY:
		// WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:				// キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:				// [ESC]が押された
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
			if (nID == IDYES)
			{
				// ウインドウを破棄する
				DestroyWindow(hWnd);
			}
			break;
		}
		break;

	case WM_CLOSE:								// 閉じるボタン押下のメッセージ
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
		if (nID == IDYES)
		{
			// ウインドウを破棄する
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;							// 0を返さないと終了してしまう
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//===============================================================================
// 初期化処理
//===============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	// プレゼンテーションパラメータ

	// DirectXオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}
	
	// 現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;							// ゲーム画面サイズ(幅)

	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							// ゲーム画面サイズ(高さ)

	d3dpp.BackBufferFormat = d3ddm.Format;							// バックバッファの形成

	d3dpp.BackBufferCount = 1;										// バックバッファの数

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						// ダブルバッファの切り替え(映像信号に同期)

	d3dpp.EnableAutoDepthStencil = TRUE;							// デブスバッファとステンシルバッファを作成

	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						// デブスバッファとして16bitを使う

	d3dpp.Windowed = bWindow;										// ウインドウモード

	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート

	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル

	// Direct3Dデバイスの生成
	// 描画処理と頂点処理をハードウェアで行う
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		// 頂点処理のみCPUで行う
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			// 両方CPUで行う
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// カリングの設定
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				//
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// アルファブレンドの設定
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	//

	// サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャの拡縮補間の設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャの繰り返し設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// デバッグ表示用フォントの生成
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
				   OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
			   	   "Terminal", &g_pFont);

	// 乱数シードセット
	srand((unsigned)timeGetTime());
	
	// キーボードの初期化処理
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}
	// ジョイパッドの初期化処理
	if(FAILED(InitJoypad()))
	{
		return E_FAIL;
	}

	// サウンドの初期化処理
	InitSound(hWnd);

	// モードの設定
	InitFade(g_mode);


	return S_OK;
}
//===============================================================================
// 終了処理
//===============================================================================
void Uninit(void)
{
	// キーボードの終了処理
	UninitKeyboard();

	// ジョイパッドの終了処理
	UninitJoypad();

	// タイトル画面の終了処理
	UninitTitle();

	// ゲーム画面の終了処理
	UninitGame();

	// リザルト画面の終了処理
	UninitResult();

	// ランキング画面の終了処理
	UninitRanking();

	// フェードの終了処理
	UninitFade();

	// サウンドを止める
	StopSound();

	// サウンドの終了処理
	UninitSound();

	// デバッグ表示用フォントの破棄
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	// Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//===============================================================================
// 更新処理
//===============================================================================
void Update(void)
{
	// 各種オブジェクト更新処理
	UpdateKeyboard();

	// ジョイパッドの更新処理
	UpdateJoypad();

	switch (g_mode)
	{
		// タイトルモード
	case MODE_TITLE:
		UpdateTitle();
		break;

		// ゲームモード
	case MODE_GAME:
		UpdateGame();
		break;

		// リザルトモード
	case MODE_RESULT:
		UpdateResult();
		break;

		// ランキングモード
	case MODE_RANKING:
		UpdateRanking();
		break;
	}

	// フェードの更新処理
	UpdateFade();
}

//===============================================================================
// 描画処理
//===============================================================================
void Draw(void)
{
	// 画面クリア(バックバッファとZバッファのクリア)
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// 描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{// 描画開始が成功した場合
		// 各種オブジェクトの描画処理

		switch (g_mode)
		{
			// タイトルモード
		case MODE_TITLE:
			DrawTitle();
			break;

			// ゲームモード
		case MODE_GAME:
			DrawGame();
			break;

			// リザルトモード
		case MODE_RESULT:
			DrawResult();
			break;

			// ランキングモード
		case MODE_RANKING:
			DrawRanking();
			break;
		}

		// フェードの描画処理
		DrawFade();

		// デバッグ表示
#ifdef _DEBUG
		DrawDebug();
#endif

		// 操作方法表示
		DrawControl();

		// 描画終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//==================================================================================
//	デバイスの取得
//==================================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//==================================================================================
//	モードの設定
//==================================================================================
void SetMode(MODE mode)
{
	// 現在のモードの終了処理
	switch (g_mode)
	{
		// タイトルモード
	case MODE_TITLE:
		UninitTitle();
		break;

		// ゲームモード
	case MODE_GAME:
		UninitGame();
		break;

		// リザルトモード
	case MODE_RESULT:
		UninitResult();
		break;
	}

	// 新しいモードの初期化処理
	switch (mode)
	{
		// タイトルモード
	case MODE_TITLE:
		InitTitle();
		break;

		// ゲームモード
	case MODE_GAME:
		InitGame();
		break;

		// リザルトモード
	case MODE_RESULT:
		InitResult();
		break;

		// ランキングモード
	case MODE_RANKING:
		InitRanking();
		break;
	}

	// モードの更新
	g_mode = mode;
}

//==================================================================================
//	モードの取得
//==================================================================================
MODE GetMode(void)
{
	return g_mode;		// 現在のモードを返す
}

//==================================================================================
//	デバッグ表示
//==================================================================================
void DrawDebug(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	RECT rect1 = { 0, 80, SCREEN_WIDTH, SCREEN_HEIGHT };

	char aStr[256] = {};		// 画面に表示する文字列
	char aStr1[256] = {};		// 画面に表示する文字列

	XINPUT_STATE* pjoystate = GetJoypadState();
	Player* pPlayer = GetPlayer();
	Option* pOption = GetOption();

	// 文字列を代入
	wsprintf(&aStr[0], "FPS:%d\nLX:%d\nLY:%d", g_nCountFPS, pjoystate->Gamepad.sThumbLX, pjoystate->Gamepad.sThumbLY);		// FPS表示
	sprintf(&aStr1[0], "OpitonPos = { %.2f,%.2f,%.2f }", pOption->pos.x, pOption->pos.y, pOption->rot.z);

	// テキストを描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aStr1[0], -1, &rect1, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//==================================================================================
//	操作方法表示
//==================================================================================
void DrawControl(void)
{
	RECT rect = { 0, 600, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256] = {};		// 画面に表示する文字列

	wsprintf(&aStr[0], "Lスティック WASD : 移動\nA SPACEKEY : ショット\nX F1 : オプション生成\nLB/RB : オプション回転");

	// テキストを描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}