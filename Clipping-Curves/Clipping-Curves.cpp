#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <math.h>
//#include <bits/stdc++.h>

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
	HWND hwnd;               /* This is the handle for our window */
	MSG messages;            /* Here messages to the application are saved */
	WNDCLASSEX wincl;        /* Data structure for the windowclass */

	/* The Window structure */
	wincl.hInstance = hThisInstance;
	wincl.lpszClassName = szClassName;
	wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
	wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
	wincl.cbSize = sizeof(WNDCLASSEX);

	/* Use default icon and mouse-pointer */
	wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL;                 /* No menu */
	wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
	wincl.cbWndExtra = 0;                      /* structure or the window instance */
	/* Use Windows's default colour as the background of the window */
	wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

	/* Register the window class, and if it fails quit the program */
	if (!RegisterClassEx(&wincl))
		return 0;

	/* The class is registered, let's create the program*/
	hwnd = CreateWindowEx(
		0,                   /* Extended possibilites for variation */
		szClassName,         /* Classname */
		_T("Code::Blocks Template Windows App"),       /* Title Text */
		WS_OVERLAPPEDWINDOW, /* default window */
		CW_USEDEFAULT,       /* Windows decides the position */
		CW_USEDEFAULT,       /* where the window ends up on the screen */
		544,                 /* The programs width */
		375,                 /* and height in pixels */
		HWND_DESKTOP,        /* The window is a child-window to desktop */
		NULL,                /* No menu */
		hThisInstance,       /* Program Instance handler */
		NULL                 /* No Window Creation data */
	);

	/* Make the window visible on the screen */
	ShowWindow(hwnd, nCmdShow);

	/* Run the message loop. It will run until GetMessage() returns 0 */
	while (GetMessage(&messages, NULL, 0, 0))
	{
		/* Translate virtual-key messages into character messages */
		TranslateMessage(&messages);
		/* Send message to WindowProcedure */
		DispatchMessage(&messages);
	}

	/* The program return-value is 0 - The value that PostQuitMessage() gave */
	return messages.wParam;
}
void swap(int& x1, int& y1, int& x2, int& y2)
{
	int tmp = x1;
	x1 = x2;
	x2 = tmp;
	tmp = y1;
	y1 = y2;
	y2 = tmp;
}
int Round(double x)
{
	return (int)(x + 0.5);
}

void Draw8Points(HDC hdc, int xc, int yc, int x, int y)
{
	SetPixel(hdc, xc + x, yc + y, RGB(0, 0, 0));
	SetPixel(hdc, xc - x, yc + y, RGB(0, 0, 0));
	SetPixel(hdc, xc - x, yc - y, RGB(0, 0, 0));
	SetPixel(hdc, xc + x, yc - y, RGB(0, 0, 0));
	SetPixel(hdc, xc + y, yc + x, RGB(0, 0, 0));
	SetPixel(hdc, xc - y, yc + x, RGB(0, 0, 0));
	SetPixel(hdc, xc - y, yc - x, RGB(0, 0, 0));
	SetPixel(hdc, xc + y, yc - x, RGB(0, 0, 0));
}
void DrawCircle(HDC hdc, int xc, int yc, int r)
{
	double theta = 1.0 / r;
	double cd = cos(theta);
	double sd = sin(theta);
	double x = r;
	double y = 0;
	Draw8Points(hdc, xc, yc, Round(x), Round(y));
	while (x > y) {
		double x1 = x * cd - y * sd;
		y = x * sd + y * cd;
		x = x1;
		Draw8Points(hdc, xc, yc, Round(x), Round(y));
	}
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	static int x[3], y[3];
	static int cnt = 0;
	int R0, R1;
	switch (message)
	{
	case WM_LBUTTONDOWN:

		x[cnt] = LOWORD(lParam);
		y[cnt++] = HIWORD(lParam);
		break;
	case WM_RBUTTONDOWN:
		x[cnt] = LOWORD(lParam);
		y[cnt++] = HIWORD(lParam);
		if (cnt == 3) {
			R0 = Round(sqrt(1.0 * (x[0] - x[2]) * (x[0] - x[2]) + 1.0 * (y[0] - y[2]) * (y[0] - y[2])));//radius of the first circle using law distance of 2 points
			R1 = Round(sqrt(1.0 * (x[1] - x[2]) * (x[1] - x[2]) + 1.0 * (y[1] - y[2]) * (y[1] - y[2])));//radius of the second circle using law distance of 2 points
			hdc = GetDC(hwnd);
			DrawCircle(hdc, x[0], y[0], R0);
			DrawCircle(hdc, x[1], y[1], R1);
			ReleaseDC(hwnd, hdc);
			cnt = 0;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
		break;
	default:                      /* for messages that we don't deal with */
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}
