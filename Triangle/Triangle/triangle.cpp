#include "d3dUtility.h"
IDirect3DDevice9* Device = 0;
const int Width = 640;
const int Height = 480;
IDirect3DVertexBuffer9* Triangle = 0;

struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z)
	{
		_x = x; _y = y; _z = z;
	}
	float _x,_y,_z;
	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ;
bool Setup()
{
	Device->CreateVertexBuffer(
		3 * sizeof(Vertex),//���������ֽ���
		D3DUSAGE_WRITEONLY,//���渽������
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&Triangle,
		0);
	Vertex* vertices;
	//���濪ʼ����λ����ƫ�������������ֽ�����������ʼָ��,������ʽ
	Triangle->Lock(0, 0, (void**)&vertices, 0);
	vertices[0] = Vertex(-1.0f, 0.0f, 2.0f);
	vertices[1] = Vertex(0.0f, 1.0f, 2.0f);
	vertices[2] = Vertex(1.0f, 0.0f, 2.0f);
	Triangle->Unlock();
	//����ͶӰ����
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI*0.5,
		(float)Width / (float)Height,
		1.0f,
		1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);
	Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	return true;
}
void Cleanup()
{
	d3d::Release<IDirect3DVertexBuffer9*>(Triangle);
}
bool display(float timeDelta)
{
	if (Device)
	{
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		Device->BeginScene();
		//��־����������Դ����ʹ�ö����Ϊ0�����㻺��ָ�룬ƫ������ÿ�������С
		Device->SetStreamSource(0, Triangle, 0, sizeof(Vertex));
		Device->SetFVF(Vertex::FVF);
		//ͼԪ����,���㻺���б�ʶ��������ʼ��Ԫ�ص�ָ�룬ͼԪ����
		Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

		Device->EndScene();
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
		if (wParam==VK_ESCAPE)
		{
			::DestroyWindow(hwnd);
		}
		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine,int showCmd)
{
	if (!d3d::InitD3D(hInstance,Width,Height,true,D3DDEVTYPE_HAL,&Device))
	{
		::MessageBox(0, TEXT("InitD3D() - FAILED"), 0, 0);
		return 0;
	}
	if (!Setup())
	{
		::MessageBox(0, TEXT("Setup() - FAILED"), 0, 0);
		return 0;
	}
	d3d::EnterMsgLoop(display);
	Cleanup();
	Device->Release();
	return 0;
}