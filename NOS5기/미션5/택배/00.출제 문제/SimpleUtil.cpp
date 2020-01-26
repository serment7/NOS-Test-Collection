#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include "SimpleUtil.h"

// bmp로 저장할 raw 이미지 버퍼에 주어진 색깔의 사각형을 채우는 API
// lCanvasWidth : bmp 의 너비
// lCanvasHeight : bmp 의 높이
// pCanvasData : bmp 로 출력할 raw 이미지 버퍼. r, g, b 순서로 컬러를 저장
// x : 채울 사각형의 왼쪽끝 위치 (left)
// y : 채울 사각형의 위쪽끝 위치 (top)
// w : 채울 사각형의 너비
// h : 채울 사각형의 높이
// R : 채울 사각형의 Red 색 [0,255]
// G : 채울 사각형의 Green 색 [0,255]
// B : 채울 사각형의 Blue 색 [0,255]
void FillRect(long lCanvasWidth, long lCanvasHeight, unsigned char* pCanvasData, long x, long y, long w, long h, unsigned char R, unsigned char G, unsigned char B)
{
	long lChannel = 3;

	assert(w >= 1 && x >= 0 && x < lCanvasWidth && x+w <= lCanvasWidth);
	assert(h >= 1 && y >= 0 && y < lCanvasHeight && y+h <= lCanvasHeight);
	for (int k = y; k < y+h; k++)
	for (int i = x; i < x+w; i++)
	{
		pCanvasData[((k * lCanvasWidth) + i) * lChannel + 0] = R;
		pCanvasData[((k * lCanvasWidth) + i) * lChannel + 1] = G;
		pCanvasData[((k * lCanvasWidth) + i) * lChannel + 2] = B;
	}
}

// bmp로 저장할 raw 이미지 버퍼에 주어진 색깔의 원(Circle)을 채우는 API
// lCanvasWidth : bmp 의 너비
// lCanvasHeight : bmp 의 높이
// pCanvasData : bmp 로 출력할 raw 이미지 버퍼. r, g, b 순서로 컬러를 저장
// x : 채울 원의 중심 위치 (수평축)
// y : 채울 원의 중심 위치 (수직축)
// r : 채울 원의 반지름
// R : 채울 사각형의 Red 색 [0,255]
// G : 채울 사각형의 Green 색 [0,255]
// B : 채울 사각형의 Blue 색 [0,255]
void FillCircle(long lCanvasWidth, long lCanvasHeight, unsigned char* pCanvasData, long x, long y, long r, unsigned char R, unsigned char G, unsigned char B)
{
	long lChannel = 3;

	assert(r > 1);
	assert(x-r >= 0 && x+r < lCanvasWidth);
	assert(y-r >= 0 && y+r < lCanvasHeight);
	for (int k = y-r; k < y+r; k++)
	for (int i = x-r; i < x+r; i++)
	{
		if (((k-y)*(k-y) + (i-x)*(i-x)) > r*r) continue;

		pCanvasData[((k * lCanvasWidth) + i) * lChannel + 0] = R;
		pCanvasData[((k * lCanvasWidth) + i) * lChannel + 1] = G;
		pCanvasData[((k * lCanvasWidth) + i) * lChannel + 2] = B;
	}
}

// 24bit & no-palette & no-compression 형태의 간단한 bmp 를 출력하는 API
// =====================================================================
// lWidth : bmp 의 너비
// lHeight : bmp 의 높이
// pData : bmp 로 출력할 raw 이미지 버퍼. r, g, b 순서로 컬러를 저장
// szFileName : 출력할 파일 이름
bool SimpleSaveBMP(long lWidth, long lHeight, unsigned char* pData, const char* szFileName)
{
	if (sizeof(long) != 4)
		return false;
	if (szFileName == 0 || szFileName[0] == 0 || pData == 0)
		return false;
	if (lWidth <= 0 || lHeight <= 0)
		return false;

	FILE *pFileOut = fopen(szFileName, "wb");
	if (pFileOut == 0) return false;

	long lChannel = 3;
	long lPixelNum = lWidth*lHeight;
	long lDataSize = lPixelNum*lChannel;
	long lHeaderSize = 54;
	long lFileSize = lDataSize + lHeaderSize;
	long lDataStart = lHeaderSize;
	long lHeaderInfoSize = 40;
	long lZero = 0, lPlane = 1, lBitDepth = 24;
	long lCompression = 0; // no compression
	long lPPMX = 120, lPPMY = 120; // pixel per meter

	// Header
	fwrite("BM", 2, 1, pFileOut);
	fwrite(&lFileSize, 4, 1, pFileOut);
	fwrite(&lZero, 4, 1, pFileOut);
	fwrite(&lDataStart, 4, 1, pFileOut);
	fwrite(&lHeaderInfoSize, 4, 1, pFileOut);
	fwrite(&lWidth, 4, 1, pFileOut);
	fwrite(&lHeight, 4, 1, pFileOut);
	fwrite(&lPlane, 2, 1, pFileOut);
	fwrite(&lBitDepth, 2, 1, pFileOut);
	fwrite(&lCompression, 4, 1, pFileOut);
	fwrite(&lDataSize, 4, 1, pFileOut);
	fwrite(&lPPMX, 4, 1, pFileOut);
	fwrite(&lPPMY, 4, 1, pFileOut);
	fwrite(&lZero, 4, 2, pFileOut);

	// Data
	long lRowSize = lWidth*lChannel;
	unsigned char* pTemp = pData;
	pTemp += (lRowSize*lHeight) - lRowSize;

	for (long lRow = 0; lRow < lHeight; lRow++, pTemp -= (lRowSize*2))
	for (long lCol = 0; lCol < lWidth; lCol++) 
	{
		unsigned char cRGB[3];
		cRGB[2] = *pTemp++; // b
		cRGB[1] = *pTemp++; // g
		cRGB[0] = *pTemp++; // r
		fwrite(cRGB, 3, 1, pFileOut);
	}
	fclose(pFileOut);
	return true;
}

#if 0
// API 사용 예제
int main()
{
	long WIDTH = 512;
	long HEIGHT = 512;
	long lChannel = 3;//RGB
	long lSize = WIDTH*HEIGHT*lChannel*sizeof(unsigned char);
	unsigned char* pData = (unsigned char*)malloc(lSize);
	memset(pData, 0, lSize);

	// 십자가
	FillRect(WIDTH, HEIGHT, pData, 0, 256-32, 512, 64, 255, 0, 0); // 빨간 가로
	FillRect(WIDTH, HEIGHT, pData, 256-32, 0, 64, 512, 255, 255, 0); // 노란 세로
	// 왼쪽위 원
	FillCircle(WIDTH, HEIGHT, pData, 112, 112, 110, 0, 255, 0); // 초록원
	
	SimpleSaveBMP(WIDTH, HEIGHT, pData, "sample.bmp");

	free(pData);
	return EXIT_SUCCESS;
}
#endif