#include "d3dUtility.h"

IDirect3DDevice9* Device = 0;
bool Setup()
{
	return true;
}
void Cleanup()
{

}
bool Display(float timeDelta)
{
	if (Device)
	{
		// Instruct the device to set each pixel on the back buffer black -
		// D3DCLEAR_TARGET: 0x00000000 (black) - and to set each pixel on
		// the depth buffer to a value of 1.0 - D3DCLEAR_ZBUFFER: 1.0f.
		//ƽ��������飬ɾ���ľ������飬ɾ����ƽ�棬��ɫ��Z��棬ģ�建��
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			0x0000000, 1.0f, 0);
		// Swap the back and front buffers.
		//�ύ��ǰ̨
		Device->Present(0, 0, 0, 0);
	}
	return true;
}
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			::DestroyWindow(hwnd);
		}
		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	if (!d3d::InitD3D(hinstance, 640, 480, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, TEXT("InitD3D()-FAILED"), 0, 0);
		return 0;
	}
	if (!Setup())
	{
		::MessageBox(0, TEXT("Setup()-FAILED"), 0, 0);
		return 0;
	}
	d3d::EnterMsgLoop(Display);
	Cleanup();
	Device->Release();
	return 0;
}