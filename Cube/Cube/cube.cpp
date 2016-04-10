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
//���
bool Setup()
{
	//�����������������
	Device->CreateVertexBuffer(
		8 * sizeof(Vertex),//���������ֽ���
		D3DUSAGE_WRITEONLY,//���渽������
		Vertex::FVF,//�������е������ʽ
		D3DPOOL_MANAGED,//������ڴ�أ�agp���Դ棬ϵͳ��d3d�Թ�����ڴ�
		&VB,//�������㻺���ָ��
		0);//��ʹ��
	Device->CreateIndexBuffer(
		36 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,//ָ�������Ĵ�С��������16λ
		D3DPOOL_MANAGED,
		&IB,//���������ָ��
		0);

	//��仺���������ṹ
	Vertex* vertices;
	//���濪ʼ����λ����ƫ�������������ֽ�����������ʼָ�룬������ʽ
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
	//λ�ú������λ��
	D3DXVECTOR3 position(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX V;
	//���ع۲�㣬����������������λ�ã��������걻�۲�㣬�����ϡ��ķ�����
	D3DXMatrixLookAtLH(&V, &position, &target, &up);
	//�۲�����ϵ��ȡ���任��
	Device->SetTransform(D3DTS_VIEW, &V);
	//����ͶӰ����
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,//����ͶӰ����
		D3DX_PI*0.5f,//�۲�㴹ֱ����Ļ��ȣ�fovY)
		(float)Width / (float)Height,//�������
		1.0f,//���������
		1000.0f);//Զ�������
	Device->SetTransform(D3DTS_PROJECTION, &proj);
	//ת�����߿�ģʽ
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
		//��ת������
		D3DXMATRIX Rx, Ry;
		//��X����ת45��
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
		//������
		//ƽ��������飬ɾ���ľ������飬ɾ����ƽ�棬��ɫ��Z��棬ģ�建��
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		Device->BeginScene();
		//��־����������Դ����ʹ�ö����Ϊ0�����㻺��ָ�룬ƫ������ÿ�������С
		Device->SetStreamSource(0, VB, 0, sizeof(Vertex));//ָ������������Դ
		Device->SetIndices(IB);//������������
		Device->SetFVF(Vertex::FVF);//���ö����ʽ
		//��������
		//ͼԪ���ͣ��������ӵ�һ����������С����ֵ���������������㻺���б�ʶ��������ʼ��Ԫ�ص�ָ�룬ͼԪ����
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
