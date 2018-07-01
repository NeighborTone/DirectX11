#include "Engine.h"
#include <memory>
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 

std::string Engine::title = "";
int Engine::width = 0;
int Engine::height = 0;
bool Engine::isFullScreen = false;
Engine::Engine(std::string WindowTitle, int width, int height, bool isFullScreen)
{
	title = WindowTitle;
	this->width = width;
	this->height = height;
	this->isFullScreen = isFullScreen;
	in.CreateInput(GetWindow().GetHwnd());
	pad.CreatePadInput(GetWindow().GetHwnd());
	mouse.CreateMousenput(GetWindow().GetHwnd());
	GetWindow().SetFullScreen(this->isFullScreen);
	COMInitialize();
}

Engine::~Engine()
{

}

void Engine::COMInitialize()
{
	static bool isInitialized = false;

	if (!isInitialized)
	{
		//COMの初期化
		/*CoinitializeExの第一引数は使われていないので0（NULL）を指定する。
		第二引数は、初期化フラグを指定し、COINIT列挙値の値を使用する。ほとんどの場合、COINIT_MULTITHREADEDかCOINIT_APARTMENTTHREADEDのどちらかを指定する。
		COINIT_MULTITHREADEDにした場合、現在のスレッドのアパートメント属性として、MTA（マルチスレッドアパートメント）に属するように設定する。
		COINIT_APARTMENTTHREADEDにした場合、STA（シングルスレッドアパートメント）に属するように設定する（そして、CoInitializeを使用すると、STA固定となる）
		マルチスレッドアパートメントに設定すると、そのスレッドは、COMコンポーネントとのやり取りで発生するすべてのスレッド同期作業を、自分で行うと宣言したことになる。
		シングルスレッドアパートメントに設定すると、そのスレッドは、COMコンポーネントとのやり取りで発生するすべてのスレッド同期作業を、COMが背後で面倒見てほしいと宣言したことになる。*/
		CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		// メモリリーク検出
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		//_CrtSetBreakAlloc(37335);
		isInitialized = true;
	}
}

bool Engine::Run()
{
	KeyBoard::UpDate();
	Mouse::UpDate();
	DInput::UpDate();
	XInput::UpDate();
	GetDirect3D().UpDate();
	GetFps().UpDate();
	return GetWindow().UpDate();
}

HWND Engine::GetWindowHandle()
{
	return GetWindow().GetHwnd();
}

DirectX::XMINT2 Engine::GetWindowSize()
{
	return GetWindow().GetSize();
}

ID3D11Device& Engine::GetDXDevice3D()
{
	return GetDirect3D().GetDevice3D();
}

ID3D11DeviceContext& Engine::GetDXContext3D()
{
	return GetDirect3D().GetContext3D();
}

ID2D1Device& Engine::GetDXDevice2D()
{
	return GetDirect3D().GetDevice2D();
}

ID2D1DeviceContext& Engine::GetDXContext2D()
{
	return GetDirect3D().GetContext2D();
}

IDXGISwapChain& Engine::GetDXSwapChain()
{
	return GetDirect3D().GetSwapChain();
}

IWICImagingFactory& Engine::GetTextureFactory()
{
	return GetDirect3D().GetTextureFactory();
}

IDWriteFactory& Engine::GetTextFactory()
{
	return  GetDirect3D().GetTextFactory();
}

System& Engine::GetWindow()
{
	static std::unique_ptr<System>window(new System(title, width, height));
	return *window.get();
}

Direct3D& Engine::GetDirect3D()
{
	static std::unique_ptr<Direct3D> direct3D(new Direct3D(isFullScreen));
	return *direct3D.get();
}

FPS& Engine::GetFps()
{
	static std::unique_ptr<FPS> fps(new FPS());
	return *fps.get();
}

Physics& Engine::GetPhysics()
{
	static std::unique_ptr<Physics> physics(new Physics());
	return *physics.get();
}