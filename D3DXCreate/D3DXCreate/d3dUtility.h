#pragma once
#include <d3dx9.h>
#include <string.h>
namespace d3d
{
	bool InitD3D(
		HINSTANCE hInstance,//窗口代号
		int width, int height,//平面宽高
		bool windows,//视窗全屏或者窗口
		D3DDEVTYPE deviceType,//HAL or REF
		IDirect3DDevice9** device);//输出，返回创建的设备
	int EnterMsgLoop(
		bool(*ptr_display)(float timeDelta));//函数指针
	LRESULT CALLBACK WndProc(
		HWND hwnd,
		UINT msg,
		WPARAM wParam,
		LPARAM lParam);
	template<class T> void Release(T t)
	{
		if (t)
		{
			t->Release();
			t = 0;
		}
	}
	template<class T> void Delete(T t)
	{
		if (t)
		{
			Delete(t);
			t = 0;
		}
	}
}