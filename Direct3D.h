#pragma once
#pragma warning (disable : 4005)	
#include "System.h"
#include <cstdio>
#include <forward_list>
#include <fstream>
#include <memory>
#include <vector>
#include <Windows.h>
#include <atlbase.h>
#include <d2d1_1.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <dsound.h>
#include <dwrite.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <wincodec.h>
#include <wrl.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "strmiids.lib")


class Direct3D
{

public:
	Direct3D();
	~Direct3D();
};

