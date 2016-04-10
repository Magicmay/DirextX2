#include "d3dUtility.h"
bool d3d::InitD3D(HINSTANCE hInstance, int width, int height, bool windowed, D3DDEVTYPE deviceType, IDirect3DDevice9** device)
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)d3d::WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = TEXT("Direct3D9App");
	if (!RegisterClass(&wc))
	{
		::MessageBox(0, TEXT("RegisterClass() - FAILED"), 0, 0);
		return false;
	}

	HWND hwnd = 0;
	hwnd = ::CreateWindow(TEXT("Direct3D9App"), 
		TEXT("Direct3D9App"),
		WS_EX_TOPMOST,
		0, 0, width, height,
		0, 0, hInstance, 0/*extra*/);
	if (!hwnd)
	{
		::MessageBox(0, TEXT("CreateWindow() - FAILED"), 0, 0);
		return false;
	}
	::ShowWindow(hwnd, SW_SHOW);
	::UpdateWindow(hwnd);
	//һ�������豸ָ��
	HRESULT hr = 0;
	IDirect3D9* d3d9 = 0;
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d9)
	{
		::MessageBox(0, TEXT("Direct3DCreate9() - FAILED"), 0, 0);
		return false;
	}
	//��������豸���ԣ�Ӳ����������������
	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);
	int vp = 0;
	if (caps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	//��������豸�ṹ
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth = width;//��̨�������Ŀ�
	d3dpp.BackBufferHeight = height;//��̨�������ĸ�
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;//��̨�������ظ�ʽ
	d3dpp.BackBufferCount = 1;//��̨�������
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;//���ز�������
	d3dpp.MultiSampleQuality = 0;//���ز�������ˮƽ
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;//ҳ���û�ģʽ
	d3dpp.hDeviceWindow = hwnd;//�豸��صĴ��ھ��
	d3dpp.Windowed = windowed;//����ģʽ����ȫ��
	d3dpp.EnableAutoDepthStencil = true;//��Ȼ����ģ�建��
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;//���ģ�����ظ�ʽ
	d3dpp.Flags = 0;//��������
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;//ˢ��Ƶ��
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;//�����ύ
	//�ġ������ӿ�
	hr = d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,//�����Կ�
		deviceType,//�豸����HAL��REF
		hwnd,//���ھ��
		vp,//���õĶ������㷽ʽ
		&d3dpp,//�����豸�ṹʵ��
		device);//���ؽ��
	if (FAILED(hr))
	{
		// try again using a 16-bit depth buffer
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

		hr = d3d9->CreateDevice(
			D3DADAPTER_DEFAULT,
			deviceType,
			hwnd,
			vp,
			&d3dpp,
			device);

		if (FAILED(hr))
		{
			d3d9->Release(); // done with d3d9 object
			::MessageBox(0, TEXT("CreateDevice() - FAILED"), 0, 0);
			return false;
		}
	}
	d3d9->Release();
	return true;
}
int d3d::EnterMsgLoop(bool(*ptr_display)(float timeDelta))
{
	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));
	static float lastTime = (float)timeGetTime();
	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg,0,0,0,PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			float currTime = (float)timeGetTime();
			float timeDelta = (currTime - lastTime)*0.001f;
			ptr_display(timeDelta);
			lastTime = currTime;
		}
	}
	return msg.wParam;
}