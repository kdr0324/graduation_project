// Taekwon.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Source/Taekwon/Taekwon.h"

#include "../Source/Amazon/Memory.h"

#include "../Source/Common/Bitmap.h"
#include "../Source/Amazon/ImageConvert.h"

#define BMP_MODE 0

int imageSaveMode = 1;

int main()
{
	CommonMemoryAllocate();
	TaekwonMemoryAllocate();

	CFileFind m_ffFileFinder;
	BOOL m_bWorking;
	CString m_strImagePath;
	CString m_strImageNoExtension;
	//IMG_INFO *m_imOpenedBitmap;
	FILE *fp_result;

#if 1
#if !BMP_MODE
	m_bWorking = m_ffFileFinder.FindFile("../Images/Taekwon/*.raw");
#else
	m_bWorking = m_ffFileFinder.FindFile("../Images/Taekwon/*.bmp");
#endif
	fp_result = fopen("../Images/Taekwon.csv", "wt");
	fprintf(fp_result, "이미지 이름,공격 여부,로봇 위치 X,로봇 위치 Y,리턴 값");
	CreateDirectory("../Images/Taekwon/result", NULL);
	while (m_bWorking) {
		m_bWorking = m_ffFileFinder.FindNextFile();
		if (m_ffFileFinder.IsDirectory() || m_ffFileFinder.IsDots()) {
			continue;
		}

		m_strImagePath = m_ffFileFinder.GetFilePath();
		m_strImageNoExtension = m_ffFileFinder.GetRoot() + "result/" + m_ffFileFinder.GetFileTitle();
#if !BMP_MODE
		ReleaseImageInfo(&original);
		original = OpenRawRGB565(m_strImagePath);
#else
		IMG_INFO *image = OpenBitmap(m_strImagePath);
		RGB888toRGB565(image, original);
#endif

		fprintf(fp_result, "\n%s", m_ffFileFinder.GetFileTitle().GetString());
		FindRobot(fp_result, m_strImageNoExtension.GetString());
#if BMP_MODE
		ReleaseImageInfo(&image);
#endif
	}
	fclose(fp_result);
#endif

	m_bWorking = m_ffFileFinder.FindFile("../Images/Closed/*.raw");
	fp_result = fopen("../Images/Closed.csv", "wt");
	fprintf(fp_result, "이미지 이름,공격 여부,로봇 위치 X,로봇 위치 Y,리턴 값");
	CreateDirectory("../Images/Closed/result", NULL);
	while (m_bWorking) {
		m_bWorking = m_ffFileFinder.FindNextFile();
		if (m_ffFileFinder.IsDirectory() || m_ffFileFinder.IsDots()) {
			continue;
		}

		m_strImagePath = m_ffFileFinder.GetFilePath();
		m_strImageNoExtension = m_ffFileFinder.GetRoot() + "result/" + m_ffFileFinder.GetFileTitle();
		ReleaseImageInfo(&original);
		original = OpenRawRGB565(m_strImagePath);

		fprintf(fp_result, "\n%s", m_ffFileFinder.GetFileTitle().GetString());
		IsClosed(m_strImageNoExtension.GetString());
	}
	fclose(fp_result);

	CommonMemoryRelease();
	TaekwonMemoryRelease();

    return 0;
}

