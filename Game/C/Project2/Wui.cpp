#include<Windows.h>
#include"White.h"

#define CELL_PIXEL			100
#define COLOR_WHITE          RGB(255,255,255)
#define COLOR_BLACK          RGB(0,0,0)
#define COLOR_BOUNDARY		 RGB(139, 134, 130)
#define COLOR_TEXT			 RGB(173,216,230)

#define INIT_TIMER_ELAPSE	1000//游戏初始速度
#define SPEEDUP_RATIO 0.7//升级后提高速度比例
#define MAX_X		9// 游戏界面大小
#define MAX_Y		5
#define NOTE_X      450
#define NOTE_Y      200
#define NOTE_WIDTH  450
#define NOTE_HIGH   300
// 全局变量
HINSTANCE hinst; /// HINSTANCE是用来表示程序运行实例的句柄，某些API函数会使用到这个变量。
RECT rectBoundary;

// 函数声明

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
//LONG NoteCreate(HWND hwnd);



int WINAPI WinMain(
	HINSTANCE hinstance, // 程序实例句柄，在程序运行，进程创建后，由操作系统向应用程序传入
	HINSTANCE hPrevInstance, // 父进程的程序实例句柄
	LPSTR lpCmdLine,  // 命令行参数，地位和作用类似C语言main函数参数argc和argv,但是没有按空格进行切分
	int nCmdShow)   // 用于指明窗口是否需要显示的参数。
{


	WNDCLASS wc;
	// 窗口句柄，hwnd变量是主窗口的句柄，这个程序中只用到了一个窗口。
	HWND hwnd;

	MSG msg;
	int fGotMessage;

	hinst = hinstance;

	// 窗口类的样式，这里设置的样式表示窗口在大小变化是需要重绘
	wc.style = CS_HREDRAW | CS_VREDRAW;
	// 一个函数指针，这个函数用来处理窗口消息。 详见 MainWndProc函数的注释。
	wc.lpfnWndProc = MainWndProc;
	// no extra class memory 
	wc.cbClsExtra = 0;
	// no extra window memory
	wc.cbWndExtra = 0;
	// handle to instance 
	wc.hInstance = hinstance;
	// hIcon成员用来指定窗口的图标
	// 这里直接使用LoadIcon函数加载了一个系统预定义的图标，开发人员可也可以自己创建图标。
	wc.hIcon = LoadIcon(NULL,///加载图标
		IDI_APPLICATION);
	// Cursor是鼠标光标，这里是设定了鼠标光标的样式。
	// 直接使用LoadCursor API函数载入了一个系统预定义的光标样式，还有IDC_CROSS,IDC_HAND等样式 
	wc.hCursor = LoadCursor(NULL,
		IDC_CROSS);///光标IDC_HAND;
				   // GetStockObject的功能是加载一个系统预定义（在栈中）的GDI对象，
				   // 这里加载的是一个白色的画刷，有关画刷和GDI对象，详见GDI说明。
	wc.hbrBackground = (HBRUSH)GetStockObject(
		WHITE_BRUSH);
	// 窗口的菜单的资源名。
	wc.lpszMenuName = "MainMenu";
	// 给窗口类起一个名字，在创建窗口时需要这个名字。
	wc.lpszClassName = "MainWClass";

	// Register the window class. 

	if (!RegisterClass(&wc))///
	{
		// 窗口注册失败，消息框提示，并退出。
		MessageBox(NULL, "创建窗口class失败", "错误！", MB_ICONERROR | MB_OK);
		return -1;
	}
	// 窗口注册成功，继续运行。


	// Create the main window. 

	hwnd = CreateWindow(
		"MainWClass",			// 窗口类名，必须是已经注册了的窗口类
		"HeiBaiKuai Game!!",		// title-bar string 
		WS_OVERLAPPEDWINDOW,	// 窗口的style，这个表示为top-level window 
		CW_USEDEFAULT,			// 窗口水平位置default horizontal POINT 
		CW_USEDEFAULT,			// 窗口垂直位置default vertical POINT 
		CW_USEDEFAULT,			// 窗口宽度 default width 
		CW_USEDEFAULT,			// 窗口高度 default height 
		(HWND)NULL,				// 父窗口句柄 no owner window 
		(HMENU)NULL,			// 窗口菜单的句柄 use class menu 
		hinstance,				// 应用程序实例句柄 handle to application instance 
		(LPVOID)NULL);			// 指向附加数据的指针 no window-creation data 

	if (!hwnd)
	{
		// 窗口创建失败，消息框提示，并退出。
		MessageBox(NULL, "创建窗口失败", "错误！", MB_ICONERROR | MB_OK);
		return -1;
	}

	// 窗口创建成功，继续运行。
	//
	// 显示窗口，WinMain函数的nCmdShow参数在这里发挥作用，一般都设置为SW_SHOW
	ShowWindow(hwnd, nCmdShow);

	// 刷新窗口，向窗口发送一个WM_PAINT消息，使得窗口进行重绘。
	UpdateWindow(hwnd);
	//Sleep(5000);


	// 以下进入消息循环。获取消息--翻译消息--分配消息（由窗口的消息处理函数来处理消息）
	while ((fGotMessage = GetMessage(&msg, (HWND)NULL, 0, 0)) != 0
		&& fGotMessage != -1)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);//调用MainWndProc
	}
	return msg.wParam;
}
void GamePaint(HWND hwnd)
{

	HPEN hpen;
	HDC hdc, hdcmem, hdcBitmapSrc;
	HBITMAP hbmMem, hBitmap;
	BITMAP bmp;

	HPEN hPenBoundary;
	HPEN hOldPen;

	HBRUSH hbrushWhite;
	HBRUSH hBrushBlack;
	HBRUSH hOldBrush;

	HFONT hFont, hOldFont;

	RECT rect;

	POINT pBlack;
	POINT lpWhite;

	int i, j;

	GetClientRect(hwnd, &rect);

	hdc = GetDC(hwnd);
	hdcmem = CreateCompatibleDC(hdc);
	hbmMem = CreateCompatibleBitmap(hdc,
		rect.right - rect.left, rect.bottom - rect.top);

	SelectObject(hdcmem, hbmMem);


	// 创建需要用到的PEN和BRUSH
	hbrushWhite = CreateSolidBrush(COLOR_WHITE); // RGB颜色，实心BRUSH
	hpen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));  // PEN， PS_NULL表示不可见
	hBrushBlack = CreateSolidBrush(COLOR_BLACK);
	hPenBoundary = CreatePen(0, 3, COLOR_BOUNDARY);//边框像素


												   /*******************************************************************************
												   * #############  画背景  ################
												   *
												   *******************************************************************************/
	FillRect(hdcmem, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

	/*******************************************************************************
	* #############  画白块 ################
	*
	*******************************************************************************/

	// 将画图需要用的PEN和BRUSH选择到DC中
	hOldBrush = (HBRUSH)SelectObject(hdcmem, hbrushWhite);
	hOldPen = (HPEN)SelectObject(hdcmem, hpen);

	for (i = 0; i < COL; i++)
	{
		for (j = 0; j < ROW; j++)
		{
			if (*(GetK() + i*ROW + j) == 1)
			{
				lpWhite = GetPosit(i, j);
				Rectangle(hdcmem,
					lpWhite.x * CELL_PIXEL + rectBoundary.left,
					lpWhite.y * CELL_PIXEL + rectBoundary.top,
					(lpWhite.x + 1)*CELL_PIXEL + rectBoundary.left,
					(lpWhite.y + 1)*CELL_PIXEL + rectBoundary.top);

			}
		}
	}

	/*******************************************************************************
	* #############  画黑块 ################
	*
	*******************************************************************************/

	SelectObject(hdcmem, hBrushBlack);
	for (i = 0; i < COL; i++)
	{
		for (j = 0; j < ROW; j++)
		{
			if (*(GetK() + i*ROW + j) == 0)
			{
				pBlack = GetPosit(i, j);
				Rectangle(hdcmem,
					pBlack.x * CELL_PIXEL + rectBoundary.left,
					pBlack.y * CELL_PIXEL + rectBoundary.top,
					(pBlack.x + 1)*CELL_PIXEL + rectBoundary.left,
					(pBlack.y + 1)*CELL_PIXEL + rectBoundary.top);

			}
		}
	}



	/*******************************************************************************
	* #############  画墙  ################
	*
	*******************************************************************************/

	SelectObject(hdcmem, hPenBoundary);

	// 将PEN移动到需要绘制的方框的左上角
	MoveToEx(hdcmem, rectBoundary.left, rectBoundary.top, NULL);
	// 画了一个方框。演示LineTo函数
	LineTo(hdcmem, rectBoundary.left, rectBoundary.bottom);
	LineTo(hdcmem, rectBoundary.right, rectBoundary.bottom);
	LineTo(hdcmem, rectBoundary.right, rectBoundary.top);
	LineTo(hdcmem, rectBoundary.left, rectBoundary.top);

	/*******************************************************************************
	* #############  写一行字  ################
	*
	*******************************************************************************/

	// 创建了一个字体对象
	hFont = CreateFont(48, 0, 0, 0, FW_DONTCARE, 0, 1, 0, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Consolas"));

	// 将这个FONT对象放入DC中
	if (hOldFont = (HFONT)SelectObject(hdcmem, hFont))
	{
		CHAR szSourceInfo[1024];
		wsprintf(szSourceInfo, "Sorce %d  Level %d", GetScore(), GetLevel());
		// 设置输出颜色
		SetTextColor(hdcmem, COLOR_TEXT);
		// 输出字符串。
		TextOut(hdcmem, rectBoundary.left + 3, rectBoundary.bottom + 3,
			szSourceInfo, lstrlen(szSourceInfo));
		// 输出完成，将原来的字体对象放回DC中
		SelectObject(hdcmem, hOldFont);
	}

	//音符
	hdcBitmapSrc = CreateCompatibleDC(hdc);
	SelectObject(hdcBitmapSrc, getNote1(hwnd));
	GetObject(getNote1(hwnd), sizeof(BITMAP), &bmp);

	StretchBlt(hdcmem,
		NOTE_X, NOTE_Y,
		NOTE_WIDTH, NOTE_HIGH,
		hdcBitmapSrc,
		0, 0, bmp.bmWidth, bmp.bmHeight,
		SRCCOPY);

	// 在内存DC中画完，一次输出的窗口DC上。
	BitBlt(hdc,
		0, 0, rect.right - rect.left, rect.bottom - rect.top,
		hdcmem, 0, 0, SRCCOPY);

	/*******************************************************************************
	* #############  回收和释放资源  ################
	*
	*******************************************************************************/
	// 回收资源
	// DeleteObject 释放GDI对象
	DeleteObject(hbmMem);
	DeleteObject(hdcmem);
	DeleteObject(hdcBitmapSrc);
	DeleteObject(hbrushWhite);
	DeleteObject(hBrushBlack);
	DeleteObject(hpen);
	DeleteObject(hPenBoundary);
	DeleteObject(hFont);
	/*******************************************************************************
	* #############  ReleaseDC 函数  ################
	* 释放占用的DC等系统资源。
	*******************************************************************************/
	ReleaseDC(hwnd, hdc);
}




void ReSizeGameWnd(HWND hwnd)
{
	POINT ptLeftTop;		// 左上角
	POINT ptRightBottom;	// 右下角
	RECT rectWindow;
	PGAME_COORD pCoordBoundary = GetBoundary();

	// 设置游戏边界
	rectBoundary.left = 10;
	rectBoundary.top = 10;
	rectBoundary.right = 10 + CELL_PIXEL*(pCoordBoundary->x + 1);
	rectBoundary.bottom = 10 + CELL_PIXEL*(pCoordBoundary->y + 1);

	// 计算上下左右角的位置
	ptLeftTop.x = rectBoundary.left;
	ptLeftTop.y = rectBoundary.top;
	ptRightBottom.x = rectBoundary.right;
	ptRightBottom.y = rectBoundary.bottom;
	ClientToScreen(hwnd, &ptLeftTop);
	ClientToScreen(hwnd, &ptRightBottom);

	GetWindowRect(hwnd, &rectWindow);
	// 计算好了，设置窗口大小。
	MoveWindow(hwnd,
		rectWindow.left,
		rectWindow.top,
		ptLeftTop.x - rectWindow.left + ptRightBottom.x - rectWindow.left, // 保存边界和左右两边边框相等。
		rectBoundary.bottom + 120, //给积分信息留出显示空间。
		TRUE);
}
LONG CALLBACK MainWndProc(
	HWND hwnd, //
	UINT msg, // 消息
	WPARAM wParam, // 消息参数，不同的消息有不同的意义，详见MSDN中每个消息的文档
	LPARAM lParam) // 消息参数，不同的消息有不同的意义，详见MSDN中每个消息的文档
{
	// 注意，是switch-case, 每次这个函数被调用，只会落入到一个case中。
	switch (msg)
	{
		// 当窗口被创建时，收到的第一个消息就是WM_CREATE，
		// 一般收到这个消息处理过程中，可以用来进行一些初始化的工作
	case WM_CREATE:
		CreateGame(hwnd,
			INIT_TIMER_ELAPSE,
			SPEEDUP_RATIO,
			MAX_X, MAX_Y);
		ReSizeGameWnd(hwnd);


		//Sleep(1000);
		break;

		// 当系统认为窗口上的GDI对象应该被重绘时，会向窗口发送一个WM_PAINT消息。
		// 当然应用程序也可以通过调用 UpateWindow来主动向窗口发送一个WM_PAINT消息。
		// 所有使用GDI在窗口上绘制图形的程序都 “必须” 写在这里。
		// 如果不是在WM_PAINT消息的处理过程中绘制GDI图形，那么在窗口刷新时就会被新被抹除和覆盖
	case WM_PAINT:

		GamePaint(hwnd);

		break;

	case WM_KEYDOWN:

		OnKeyDown(wParam, hwnd);
		GamePaint(hwnd);
		break;

	case WM_LBUTTONDOWN:
		//OnTimer(hwnd, wParam);
		OnTimer(hwnd);
		GamePaint(hwnd);
		break;

	case WM_TIMER:

		OnTimer(hwnd);
		GamePaint(hwnd);
		break;

	case WM_DESTROY:
		ExitProcess(0);
		break;

	default:
		break;
	}
	return DefWindowProc(hwnd,
		msg,
		wParam,
		lParam);
}