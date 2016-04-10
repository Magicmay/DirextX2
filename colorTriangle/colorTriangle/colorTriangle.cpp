#include "d3dUtility.h"
IDirect3DDevice9* Device = 0;

const int Width = 640;
const int Height = 480;

D3DXMATRIX WorldMatrix;

IDirect3DVertexBuffer9* Triangle = 0;

struct ColorVertex 
{
	ColorVertex(){}
	ColorVertex(float x, float y, float z, D3DCOLOR c)
	{
		_x = x;
		_y = y;
		_z = z;
		_color = c;
	}
	float _x, _y, _z;
	D3DCOLOR _color;
	static const DWORD FVF;
};
const DWORD ColorVertex::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

bool Setup()
{
	Device->CreateVertexBuffer(
		3 * sizeof(ColorVertex),//缓存分配的字节数
		D3DUSAGE_WRITEONLY,//缓存附加属性
		ColorVertex::FVF,//顶点类中的灵活顶点格式
		D3DPOOL_MANAGED,//缓存的内存池，agp，显存，系统，d3d自管理的内存
		&Triangle,//创建顶点缓存的指针
		0);//不使用

	ColorVertex* v;
	//缓存开始锁定位置是偏移量，锁定的字节数，缓存起始指针，锁定方式
	Triangle->Lock(0, 0, (void**)&v, 0);

	v[0] = ColorVertex(-1.0f, 0.0f, 2.0f, D3DCOLOR_XRGB(255, 0, 0));
	v[1] = ColorVertex(0.0f, 1.0f, 2.0f, D3DCOLOR_XRGB(0, 255, 0));
	v[2] = ColorVertex(1.0f, 0.0f, 2.0f, D3DCOLOR_XRGB(0, 0, 255));

	Triangle->Unlock();

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,//返回投影矩阵
		D3DX_PI*0.5f,//观察点垂直领域的弧度（fovY)
		(float)Width / (float)Height,//方向比例
		1.0f,//近截面距离
		1000.0f);//远截面距离
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	Device->SetRenderState(D3DRS_LIGHTING, false);
	return true;
}
void Cleanup()
{
	d3d::Release<IDirect3DVertexBuffer9*>(Triangle);
}
bool Display(float timeDelta)
{
	if (Device)
	{
		//平面矩形数组，删除的矩形数组，删除的平面，颜色，Z深缓存，模板缓存
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		Device->BeginScene();

		Device->SetFVF(ColorVertex::FVF);
		//标志数据流输入源，不使用多个故为0，顶点缓存指针，偏移量，每个对象大小
		Device->SetStreamSource(0, Triangle, 0, sizeof(ColorVertex));

		D3DXMatrixTranslation(&WorldMatrix, -1.25f, 0.0f, 0.0f);
		Device->SetTransform(D3DTS_WORLD, &WorldMatrix);

		Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

		D3DXMatrixTranslation(&WorldMatrix, 1.25f, 0.0f, 0.0f);
		Device->SetTransform(D3DTS_WORLD, &WorldMatrix);

		Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
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
		if (wParam == VK_ESCAPE)
			::DestroyWindow(hwnd);
		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE prevInstance,
	PSTR cmdLine,
	int showCmd)
{
	if (!d3d::InitD3D(hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, TEXT("InitD3D() - FAILED"), 0, 0);
		return 0;
	}

	if (!Setup())
	{
		::MessageBox(0, TEXT("Setup() - FAILED"), 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop(Display);

	Cleanup();

	Device->Release();

	return 0;
}