#include "d3dUtility.h"
IDirect3DDevice9* Device = 0;
const int Width = 640;
const int Height = 480;

ID3DXMesh* Objects[5] = { 0,0,0,0,0 };

D3DXMATRIX ObjWorldMatrices[5];
bool Setup()
{
	//与网格关联的设备，接收网格指针，LPD3DXBUFFER*
	D3DXCreateTeapot(Device, &Objects[0], 0);
	//盒子
	D3DXCreateBox(
		Device,
		2.0f,//width
		2.0f,//height
		2.0f,//depth
		&Objects[1],
		0);
	//圆筒,圆柱体
	D3DXCreateCylinder(
		Device,
		1.0f,// radius at negative z end
		1.0f,// radius at positive z end
		3.0f,// length of cylinder
		10,// slices
		10,// stacks
		&Objects[2],
		0);
	//圆环
	D3DXCreateTorus(
		Device,
		1.0f,// inner radius
		3.0f,// outer radius
		10,// sides
		10,// rings
		&Objects[3],
		0);
	//球体
	D3DXCreateSphere(
		Device,
		1.0f,// radius
		10,// slices
		10,// stacks
		&Objects[4],
		0);

	//贮存建立用在世界坐标的矩阵对应的物品
	D3DXMatrixTranslation(&ObjWorldMatrices[0], 0.0f, 0.0f, 0.0f);
	D3DXMatrixTranslation(&ObjWorldMatrices[1], -5.0f, 0.0f, 5.0f);
	D3DXMatrixTranslation(&ObjWorldMatrices[2], 5.0f, 0.0f, 5.0f);
	D3DXMatrixTranslation(&ObjWorldMatrices[3], -5.0f, 0.0f, -5.0f);
	D3DXMatrixTranslation(&ObjWorldMatrices[4], 5.0f, 0.0f, -5.0f);

	//投影矩阵
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI*0.5f,
		(float)Width / (float)Height,
		1.0f,
		1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	return true;
}
void Cleanup()
{
	for(int i=0;i<5;i++)
		d3d::Release<ID3DXMesh*>(Objects[i]);
}
bool Display(float timeDelta)
{
	if (Device)
	{
		//摄像机动画
		static float angle = (3.0f*D3DX_PI) / 2.0f;
		static float cameraHeight = 0.0f;
		static float cameraHeightDirection = 5.0f;

		D3DXVECTOR3 position(cosf(angle)*10.0f, cameraHeight, cameraHeightDirection);
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		
		D3DXMATRIX v;
		D3DXMatrixLookAtLH(&v, &position, &target, &up);
		Device->SetTransform(D3DTS_VIEW, &v);

		angle += timeDelta;
		if (angle >= 6.28f)
			angle = 0.0f;
		cameraHeight += cameraHeightDirection*timeDelta;
		if (cameraHeight >= 10.0f)
			cameraHeightDirection = -5.0f;
		if (cameraHeight <= -10.0f)
			cameraHeightDirection = 5.0f;

		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		Device->BeginScene();

		for (int i=0;i<5;i++)
		{
			//设置目标的世界矩阵
			Device->SetTransform(D3DTS_WORLD, &ObjWorldMatrices[i]);
			//画出目标
			Objects[i]->DrawSubset(0);
		}

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
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int showCmd)
{
	if (!d3d::InitD3D(hInstance, Width, Height, true, D3DDEVTYPE_HAL, &Device))
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