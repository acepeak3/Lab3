#include <Windows.h>
#include <string>
#include <vector>

int y = 0;

struct Point
{
	int x_, y_;
};

std::vector<Point> points(20);

LRESULT CALLBACK WindowProcedure(
	HWND aWindow,
	UINT aMessage,
	WPARAM aWParameter,
	LPARAM aLParameter)
{
	switch (aMessage)
	{
	case WM_CREATE:
	{
		for (auto point = points.begin(); point != points.end(); point++)
		{
			point->x_ = rand() % 700 + 50,
				point->y_ = rand() % 700 + 50;
		}
		return 0;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT paintStructure;
		HDC deviceContext = BeginPaint(aWindow, &paintStructure);

		EndPaint(aWindow, &paintStructure);
	}
	case WM_TIMER:
	{
		HDC deviceContext = GetDC(aWindow);

		RECT clientRect;
		GetClientRect(aWindow, &clientRect);
		FillRect(deviceContext, &clientRect, (HBRUSH)GetStockObject(WHITE_BRUSH));

		SelectObject(deviceContext, GetStockObject(BLACK_PEN));

		POINT previousPoint;
		MoveToEx(deviceContext, 0, y, &previousPoint);
		LineTo(deviceContext, 800, y);

		HPEN
			bluePen = CreatePen(PS_SOLID, 1, RGB(0, 0, 250)),
			redPen = CreatePen(PS_SOLID, 1, RGB(250, 0, 0));

		HBRUSH redBrush = CreateSolidBrush(RGB(250, 0, 0));

		SelectObject(deviceContext, bluePen);

		for (auto point = points.begin(); point != points.end(); point++)
		{
			const int
				pointX = point->x_,
				pointY = point->y_;

			if (pointY == y)
			{
				SelectObject(deviceContext, redPen);
				SelectObject(deviceContext, redBrush);

				Ellipse(deviceContext, pointX - 3, pointY - 3, pointX + 3, pointY + 3);

				SelectObject(deviceContext, bluePen);
				SelectObject(deviceContext, GetStockObject(WHITE_BRUSH));
			}
			else
			{
				if (pointY < y)/* && (y - pointY < 400)*/
				{
					const int
						p = y - pointY,
						nodeY = pointY + p / 2;

					for (int xx = 0; xx < 200; xx++)
					{
						int yy = xx * xx / (2 * p);

						if (yy < 200)
						{
							if (pointX - xx >= 0)
							{
								SetPixel(deviceContext, pointX - xx, nodeY - yy, RGB(0, 255, 0));
							}

							if (pointX + xx < 800)
							{
								SetPixel(deviceContext, pointX + xx, nodeY - yy, RGB(0, 255, 0));
							}
						}
					}
				}

				Ellipse(deviceContext, pointX - 2, pointY - 2, pointX + 2, pointY + 2);
			}
		}

		DeleteObject(bluePen);
		DeleteObject(redPen);
		DeleteObject(redBrush);

		ReleaseDC(aWindow, deviceContext);

		y++;
		if (y == 800) y = 0;

		return 0;
	}
	case WM_CLOSE:
	{
		DestroyWindow(aWindow);
		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(aWindow, aMessage, aWParameter, aLParameter);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE aInstance, HINSTANCE aPreviousInstance, LPSTR aCommandLine, int aShowCommand)
{
	UNREFERENCED_PARAMETER(aPreviousInstance);
	UNREFERENCED_PARAMETER(aCommandLine);

	WNDCLASSEX vWindowClass;
	vWindowClass.cbSize = sizeof(WNDCLASSEX);
	vWindowClass.style = CS_HREDRAW | CS_VREDRAW;
	vWindowClass.lpfnWndProc = WindowProcedure;
	vWindowClass.cbClsExtra = 0;
	vWindowClass.cbWndExtra = 0;
	vWindowClass.hInstance = aInstance;
	vWindowClass.hIcon = LoadIcon(aInstance, IDI_APPLICATION);
	vWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	vWindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	vWindowClass.lpszMenuName = NULL;
	vWindowClass.lpszClassName = "MainWindowClass";
	vWindowClass.hIconSm = LoadIcon(aInstance, IDI_APPLICATION);

	RegisterClassEx(&vWindowClass);

	HWND vMainWindow = CreateWindow(
		"MainWindowClass",
		"TEST APPLICATION",
		WS_OVERLAPPEDWINDOW,
		100,
		100,
		800,
		800 + 40,
		NULL,
		NULL,
		aInstance,
		NULL);

	ShowWindow(vMainWindow, aShowCommand);
	UpdateWindow(vMainWindow);

	SetTimer(vMainWindow, NULL, 20, NULL);

	MSG vMessage;
	
	while (GetMessage(&vMessage, NULL, 0, 0))
	{
		DispatchMessage(&vMessage);
	}

	return (int)vMessage.wParam;
}