#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include "SimpleUtil.h"

// bmp�� ������ raw �̹��� ���ۿ� �־��� ������ �簢���� ä��� API
// lCanvasWidth : bmp �� �ʺ�
// lCanvasHeight : bmp �� ����
// pCanvasData : bmp �� ����� raw �̹��� ����. r, g, b ������ �÷��� ����
// x : ä�� �簢���� ���ʳ� ��ġ (left)
// y : ä�� �簢���� ���ʳ� ��ġ (top)
// w : ä�� �簢���� �ʺ�
// h : ä�� �簢���� ����
// R : ä�� �簢���� Red �� [0,255]
// G : ä�� �簢���� Green �� [0,255]
// B : ä�� �簢���� Blue �� [0,255]
void FillRect(long lCanvasWidth, long lCanvasHeight, unsigned char* pCanvasData, long x, long y, long w, long h, unsigned char R, unsigned char G, unsigned char B)
{
	long lChannel = 3;
	long lMinX = x;
	long lMinY = y;

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

// 24bit & no-palette & no-compression ������ ������ bmp �� ����ϴ� API
// =====================================================================
// lWidth : bmp �� �ʺ�
// lHeight : bmp �� ����
// pData : bmp �� ����� raw �̹��� ����. r, g, b ������ �÷��� ����
// szFileName : ����� ���� �̸�
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
// API ��� ����
int main()
{
	long WIDTH = 512;
	long HEIGHT = 512;
	long lChannel = 3;//RGB
	long lSize = WIDTH*HEIGHT*lChannel*sizeof(unsigned char);
	unsigned char* pData = (unsigned char*)malloc(lSize);
	memset(pData, 0, lSize);

	// ���ڰ�
	FillRect(WIDTH, HEIGHT, pData, 0, 256-32, 512, 64, 255, 0, 0); // ���� ����
	FillRect(WIDTH, HEIGHT, pData, 256-32, 0, 64, 512, 255, 255, 0); // ��� ����

	SimpleSaveBMP(WIDTH, HEIGHT, pData, "sample.bmp");

	free(pData);
	return EXIT_SUCCESS;
}
#endif