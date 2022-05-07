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
		1544,                 /* The programs width */
		1375,                 /* and height in pixels */
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


COLORREF clip(int x, int y, int xc, int yc, double r) {
	double dist = sqrt(1.0 * (xc - x) * (xc - x) + 1.0 * (yc - y) * (yc - y));
	if (dist <= r)
		return RGB(255, 0, 0);
	return RGB(0, 0, 255);
}

void Bezier(HDC hdc, int x[], int y[], double r, int xc, int yc) {
	int a1 = x[0];
	int a2 = 3*(x[1] - x[0]);
	int a3 = 3 * x[0] - 6 * x[1] + 3 * x[2];
	int a4 = -1 * x[0] + 3 * x[1] - 3 * x[2] + x[3];
	int b1 = y[0];
	int b2 = 3 * (y[1] - y[0]);
	int b3 = 3 * y[0] - 6 * y[1] + 3 * y[2];
	int b4 = -1 * y[0] + 3 * y[1] - 3 * y[2] + y[3];

	for (double t = 0; t <= 1; t += 0.0001) {
		double t2 = t * t, t3 = t2 * t;
		double x1 = a1 + a2 * t + a3 * t2 + a4 * t3;
		double y1 = b1 + b2 * t + b3 * t2 + b4 * t3;
		SetPixel(hdc, Round(x1), Round(y1), clip(Round(x1), Round(y1), xc, yc, r));
	}
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	static int x[4], y[4];
	static bool isWindow = true;
	static int xc[2], yc[2];
	static double r;
	static int cnt = 0, cnt2 = 0;
	switch (message)
	{
	case WM_LBUTTONDOWN:
		if (isWindow) {
			xc[cnt] = LOWORD(lParam);
			yc[cnt++] = HIWORD(lParam);
			if (cnt == 2) {
				hdc = GetDC(hwnd);
				r = sqrt(1.0 * (xc[0] - xc[1]) * (xc[0] - xc[1]) + 1.0 * (yc[0] - yc[1]) * (yc[0] - yc[1]));
				isWindow = false;
				cnt = 0;
				DrawCircle(hdc, xc[0], yc[0], r);
				ReleaseDC(hwnd, hdc);
			}
		}
		else {
			x[cnt] = LOWORD(lParam);
			y[cnt++] = HIWORD(lParam);
			if (cnt == 4 && cnt2 < 2) {
				cnt2++;
				cnt = 0;
				hdc = GetDC(hwnd);
				Bezier(hdc, x, y, r, xc[0], yc[0]);
				ReleaseDC(hwnd, hdc);
			}
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
