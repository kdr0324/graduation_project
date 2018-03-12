#pragma once

#include <afxwin.h>
#include <Vfw.h>

#include "../Source/Common/Global.h"
#include "../Source/Common/imageManagement.h"

#pragma comment (lib, "vfw32.lib")

enum
{
	CAP_PROP_FRAME_WIDTH = 0,
	CAP_PROP_FRAME_HEIGHT = 1,
	CAP_PROP_FPS = 2,
	CAP_PROP_FOURCC = 3
};

class CaptureCAM_VFW
{
public:
	CaptureCAM_VFW() { init(); }
	~CaptureCAM_VFW() { close(); }

	bool open(int index);
	void close();
	double getProperty(int);
	bool setProperty(int, double);
	bool grabFrame();
	IMG_INFO* retrieveFrame(int);
	IMG_INFO* queryFrame();
	
protected:
	void init();
	void closeHIC();
	static LRESULT PASCAL frameCallback(HWND hWnd, VIDEOHDR* hdr);

	CAPDRIVERCAPS caps;
	HWND   capWnd;
	VIDEOHDR* hdr;
	DWORD  fourcc;
	int width, height;
	int widthSet, heightSet;
	HIC    hic;

public:
	IMG_INFO* frame;
};