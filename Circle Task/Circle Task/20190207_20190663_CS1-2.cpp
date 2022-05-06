#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif
/**
* Rana Ihab Ahmed	20190207
* Arwa Hazem		20190663
* Task 1	group: CS-1/2
**/
#include <tchar.h>
#include <windows.h>
#include <math.h>
//#include <bits/stdc++.h>

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain(HINSTANCE hThisInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpszArgument,
	int nCmdShow)
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
void drawLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c) {
	int dx = x2 - x1;
	int dy = y2 - y1;
	if (abs(dx) >= abs(dy)) {
		if (x1 > x2)
			swap(x1, y1, x2, y2);
		int x = x1;
		double y = y1;
		double m = (double)dy / dx;
		SetPixel(hdc, x1, y1, c);
		while (true) {
			x++;
			y += m;
			if (x == x2)
				break;
			SetPixel(hdc, x, floor(y), c);
			SetPixel(hdc, x, ceil(y), c);
		}
		//SetPixel(hdc, x, Round(y), c);
	}
	else {
		if (y1 > y2)
			swap(x1, y1, x2, y2);
		double x = x1;
		int y = y1;
		double m = (double)dx / dy;
		SetPixel(hdc, x1, y1, c);
		while (true) {
			y++;
			x += m;
			if (y == y2)
				break;
			SetPixel(hdc, floor(x), y, c);
			SetPixel(hdc, ceil(x), y, c);
		}
		//SetPixel(hdc, Round(x), y, c);
	}
}
/*  This function is called by the Windows function DispatchMessage()  */
void Draw8Points(HDC hdc, int xc, int yc, int a, int b, COLORREF color, bool isColor)
{
	SetPixel(hdc, xc + a, yc + b, color);
	SetPixel(hdc, xc - a, yc + b, color);
	SetPixel(hdc, xc - a, yc - b, color);
	SetPixel(hdc, xc + a, yc - b, color);
	SetPixel(hdc, xc + b, yc + a, color);
	SetPixel(hdc, xc - b, yc + a, color);
	SetPixel(hdc, xc - b, yc - a, color);
	SetPixel(hdc, xc + b, yc - a, color);
	if (isColor) {
		drawLine(hdc, xc, yc, xc + a, yc + b, color);
		drawLine(hdc, xc, yc, xc - a, yc - b, color);
	}
}
void DrawCircle(HDC hdc, int x1, int y1, int R, COLORREF c, bool isColor)
{
	int x = R, y = 0;
	double theta = 0, dtheta = 1.0 / R;
	Draw8Points(hdc, x1, y1, x, y, c, isColor);
	while (x > y)
	{
		theta += dtheta;
		x = Round(R * cos(theta));
		y = Round(R * sin(theta));
		Draw8Points(hdc, x1, y1, x, y, c, isColor);
	}
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	static int x[3], y[3];
	static int cnt = 0;
	int R0, R1;
	switch (message)                  /* handle the messages */
	{
	case WM_LBUTTONDOWN:
		// array of x and y to store points
		//index zero for c1 , 1 for c2 and 2 for the right click
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
			DrawCircle(hdc, x[0], y[0], R0, RGB(0, 0, 0), true);
			DrawCircle(hdc, x[1], y[1], R1, RGB(0, 0, 0), false);
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
