#ifndef _d3dUtilityH_
#define _d3dUtilityH_
#include <d3dx9.h>
#include <string>
//#pragma comment(lib, "d3d9.lib")
//#pragma comment(lib, "d3dx9.lib")
namespace d3d
{
	bool InitD3D(
		HINSTANCE hInstance,//���ڴ���
		int width, int height,//ƽ����
		bool windowed,//�Ӵ�ȫ�����ߴ���
		D3DDEVTYPE deviceType,//HAL or REF
		IDirect3DDevice9** device);//��������ش������豸
	int EnterMsgLoop(
		bool(*ptr_display)(float timeDelta));//����ָ��
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
			delete(t);
			t = 0;
		}
	}
	const D3DXCOLOR WHITE(D3DCOLOR_XRGB(255, 255, 255));
	const D3DXCOLOR BLACK(D3DCOLOR_XRGB(0, 0, 0));
	const D3DXCOLOR RED(D3DCOLOR_XRGB(255, 0, 0));
	const D3DXCOLOR GREEN(D3DCOLOR_XRGB(0, 255, 0));
	const D3DXCOLOR BLUE(D3DCOLOR_XRGB(0, 0, 255));
	const D3DXCOLOR YELLOW(D3DCOLOR_XRGB(255, 255, 0));
	const D3DXCOLOR CYAN(D3DCOLOR_XRGB(0, 255, 255));
	const D3DXCOLOR MAGENTA(D3DCOLOR_XRGB(255, 0, 255));
}
#endif