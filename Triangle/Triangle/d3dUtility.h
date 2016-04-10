#pragma once
#include <d3dx9.h>
#include <string.h>
namespace d3d
{
	bool InitD3D(
		HINSTANCE hInstance,//���ڴ���
		int width, int height,//ƽ����
		bool windows,//�Ӵ�ȫ�����ߴ���
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
			Delete(t);
			t = 0;
		}
	}
}