#pragma once

void FillRect(long lCanvasWidth, long lCanvasHeight, unsigned char* pCanvasData, long x, long y, long w, long h, unsigned char R, unsigned char G, unsigned char B);
void FillCircle(long lCanvasWidth, long lCanvasHeight, unsigned char* pCanvasData, long x, long y, long r, unsigned char R, unsigned char G, unsigned char B);
bool SimpleSaveBMP(long lWidth, long lHeight, unsigned char* pData, const char* szFileName);
