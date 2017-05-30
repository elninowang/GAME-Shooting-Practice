// Shooting.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Shooting.h"
#include "ddraw.h"
#include "mmsystem.h"
#include "Class.h"
#include "Var.h"
#include "DirectX.h"
#include "Draw.h"
#include "Function.h"
#include "Frenquency.h"
#define MAX_LOADSTRING 100

// ȫ�ֱ�����
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������
BOOL notDone=FALSE;

// �˴���ģ���а����ĺ�����ǰ��������
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;
	//
	HBITMAP hBitmap;
	BITMAP bitmap;

	//hBitmap=(HBITMAP)LoadBitmap(NULL,file_name);
	hBitmap=(HBITMAP) LoadImage(hInstance,"BackGround1.bmp",IMAGE_BITMAP,0,0,
								LR_DEFAULTSIZE|LR_LOADFROMFILE);
	if( !hBitmap)
	{
		MessageBox(NULL,TEXT("NO Bitmap!"),TEXT("WRONG"),NULL);
		return NULL;
	}
	GetObject(hBitmap,sizeof(BITMAP),&bitmap);
	cxSurface=bitmap.bmWidth;
	cySurface=bitmap.bmHeight;
	DeleteObject( hBitmap );

	//������ܼ�ʱ��
	LONGLONG cur_time;
	DWORD time_count;
	LONGLONG perf_cnt;
	BOOL pert_flag=FALSE;
	LONGLONG next_time=0;
	LONGLONG last_time=0;
	LONGLONG begin_time=0;
	double time_elapsed;
	double time_scale;

	if( QueryPerformanceFrequency( (LARGE_INTEGER *) &perf_cnt ) )
	{
		pert_flag = TRUE;
		time_count = perf_cnt/1000;
		QueryPerformanceCounter( (LARGE_INTEGER *) &next_time );
		time_scale = 1.0/perf_cnt;
	}
	else
	{
		next_time = timeGetTime();
		time_scale = 0.001;
		time_count = 1;
	}
	iBeginTime = last_time = next_time;
	

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SHOOTING, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ����
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_SHOOTING);

	// ����Ϣѭ����
	while( notDone == TRUE )
	{
		if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE )) 
		{
			if ( msg.message == WM_QUIT ) 
				notDone = FALSE;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{			
			if( pert_flag )
			{
				QueryPerformanceCounter( (LARGE_INTEGER *) &cur_time );
			}
			else 
				cur_time = timeGetTime();
			
			if( cur_time>next_time )
			{
				iRealTime = (cur_time-iBeginTime )*1000*time_scale;
				time_elapsed = (cur_time-last_time)*time_scale;
				last_time = cur_time;
				if( TimeFunction() )
					pert_flag?QueryPerformanceCounter( (LARGE_INTEGER *) &iBeginTime ):iBeginTime=timeGetTime(); 
				next_time = cur_time + time_count;
			}

		}

	}
	return (int) msg.wParam;
}



//
//  ������MyRegisterClass()
//
//  Ŀ�ģ�ע�ᴰ���ࡣ
//
//  ע�ͣ�
//
//    ����ϣ��������ӵ� Windows 95 ��
//    ��RegisterClassEx������֮ǰ�˴����� Win32 ϵͳ����ʱ��
//    ����Ҫ�˺��������÷������ô˺���
//    ʮ����Ҫ������Ӧ�ó���Ϳ��Ի�ù�����
//   ����ʽ��ȷ�ġ�Сͼ�ꡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= 0;//CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_SHOOTING);
	wcex.hCursor		= (HCURSOR)LoadImage(hInstance, (LPCTSTR)IDC_CURSOR1, IMAGE_CURSOR, 0 ,0, LR_DEFAULTSIZE  );
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_SHOOTING;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   ������InitInstance(HANDLE, int)
//
//   Ŀ�ģ�����ʵ�����������������
//
//   ע�ͣ�
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
      100, 200, cxSurface+8, cySurface+46, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ������WndProc(HWND, unsigned, WORD, LONG)
//
//  Ŀ�ģ����������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;

	switch (message) 
	{
	case WM_CREATE:
		notDone = true;
		if( !InitDirectX(hWnd ) )
		{
			MessageBox(hWnd, TEXT("DirectX�ӿ�ʧ��"), TEXT("ERROR"), NULL);
			CleanUp();
			SendMessage( hWnd, WM_DESTROY, 0, 0 );
		}
		Init();
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// �����˵�ѡ��
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_SIZE:
		cxClient = LOWORD( lParam );
		cyClient = HIWORD( lParam );
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SetMapMode(hdc,MM_TEXT);
		// TODO: �ڴ���������ͼ����...
		DrawDirectX( hWnd );
		EndPaint(hWnd, &ps);
		break;
	
	case WM_LBUTTONDOWN:
		if( !Check() )
			iClick++;
		break;

	case WM_MOUSEMOVE:
		xMouse = LOWORD( lParam );
		yMouse = HIWORD( lParam );
		//DrawDirectX(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}
LRESULT CALLBACK Winner(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	char str[MAX_LOADSTRING];
	switch (message)
	{
	case WM_INITDIALOG:
		wsprintf(str,"%d ����",iRealTime);
		SetDlgItemText(hDlg, IDC_TIME, str);
		SetDlgItemInt(hDlg, IDC_SPEED, MAXV, TRUE);
		SetDlgItemInt(hDlg, IDC_VIRTUAL, iClick, TRUE);
		wsprintf(str,"%d %%",NO*100/(NO+iClick) );
		SetDlgItemText(hDlg, IDC_HITTING, str );
		return TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK )
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}