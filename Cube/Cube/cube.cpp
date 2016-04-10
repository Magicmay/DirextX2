#include "d3dUtility.h"

IDirect3DDevice9* Device = 0;
const int Width = 640;
const int Height = 480;

IDirect3DVertexBuffer9* VB = 0;
IDirect3DIndexBuffer9* IB = 0;

struct Vertex 
{
	Vertex(){}
	Vertex(float x, float y, float z)
	{
		_x = x; _y = y; _z = z;
	}
	float _x, _y, _z;
	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ;
//框架
bool Setup()
{
	//创建顶点和索引缓存
	Device->CreateVertexBuffer(
		8 * sizeof(Vertex),//缓存分配的字节数
		D3DUSAGE_WRITEONLY,//缓存附加属性
		Vertex::FVF,//顶点类中的灵活顶点格式
		D3DPOOL_MANAGED,//缓存的内存池，agp，显存，系统，d3d自管理的内存
		&VB,//创建顶点缓存的指针
		0);//不使用
	Device->CreateIndexBuffer(
		36 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,//指定索引的大小，这里是16位
		D3DPOOL_MANAGED,
		&IB,//索引缓存的指针
		0);

	//填充缓存和立方体结构
	Vertex* vertices;
	//缓存开始锁定位置是偏移量，锁定的字节数，缓存起始指针，锁定方式
	VB->Lock(0, 0, (void**)&vertices, 0);

	vertices[0] = Vertex(-1.0f, -1.0f, -1.0f);
	vertices[1] = Vertex(-1.0f, 1.0f, -1.0f);
	vertices[2] = Vertex(1.0f, 1.0f, -1.0f);
	vertices[3] = Vertex(1.0f, -1.0f, -1.0f);
	vertices[4] = Vertex(-1.0f, -1.0f, 1.0f);
	vertices[5] = Vertex(-1.0f, 1.0f, 1.0f);
	vertices[6] = Vertex(1.0f, 1.0f, 1.0f);
	vertices[7] = Vertex(1.0f, -1.0f, 1.0f);

	VB->Unlock();

	WORD* indices = 0;
	IB->Lock(0, 0, (void**)&indices, 0);
	// front side
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;
	// back side
	indices[6] = 4; indices[7] = 6; indices[8] = 5;
	indices[9] = 4; indices[10] = 7; indices[11] = 6;
	// left side
	indices[12] = 4; indices[13] = 5; indices[14] = 1;
	indices[15] = 4; indices[16] = 1; indices[17] = 0;
	// right side
	indices[18] = 3; indices[19] = 2; indices[20] = 6;
	indices[21] = 3; indices[22] = 6; indices[23] = 7;
	// top
	indices[24] = 1; indices[25] = 5; indices[26] = 6;
	indices[27] = 1; indices[28] = 6; indices[29] = 2;
	// bottom
	indices[30] = 4; indices[31] = 0; indices[32] = 3;
	indices[33] = 4; indices[34] = 3; indices[35] = 7;
	IB->Unlock();
	//位置和摄像机位置
	D3DXVECTOR3 position(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX V;
	//返回观察点，摄像机在世界坐标的位置，世界坐标被观察点，“向上”的方向量
	D3DXMatrixLookAtLH(&V, &position, &target, &up);
	//观察坐标系（取景变换）
	Device->SetTransform(D3DTS_VIEW, &V);
	//建立投影矩阵
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,//返回投影矩阵
		D3DX_PI*0.5f,//观察点垂直领域的弧度（fovY)
		(float)Width / (float)Height,//方向比例
		1.0f,//近截面距离
		1000.0f);//远截面距离
	Device->SetTransform(D3DTS_PROJECTION, &proj);
	//转换到线框模式
	Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	return true;
}
void Cleanup()
{
	d3d::Release<IDirect3DVertexBuffer9*>(VB);
	d3d::Release<IDirect3DIndexBuffer9*>(IB);
}
bool Display(float timeDelta)
{
	if (Device)
	{
		//旋转立方体
		D3DXMATRIX Rx, Ry;
		//在X轴旋转45度
		D3DXMatrixRotationX(&Rx, 3.14f / 4.0f);
		// incremement y-rotation angle each frame
		static float y = 0.0f;
		D3DXMatrixRotationY(&Ry, y);
		y += timeDelta;
		// reset angle to zero when angle reaches 2*PI
		if (y>=6.28f)
		{
			y = 0.0f;
		}
		// combine x- and y-axis rotation transformations.
		D3DXMATRIX p = Rx*Ry;
		Device->SetTransform(D3DTS_WORLD, &p);
		//画场景
		//平面矩形数组，删除的矩形数组，删除的平面，颜色，Z深缓存，模板缓存
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		Device->BeginScene();
		//标志数据流输入源，不使用多个故为0，顶点缓存指针，偏移量，每个对象大小
		Device->SetStreamSource(0, VB, 0, sizeof(Vertex));//指定数据流输入源
		Device->SetIndices(IB);//设置索引缓存
		Device->SetFVF(Vertex::FVF);//设置顶点格式
		//画立方体
		//图元类型，索引增加的一个基数，最小索引值，顶点总数，顶点缓存中标识索引的起始点元素的指针，图元总数
		Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

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
