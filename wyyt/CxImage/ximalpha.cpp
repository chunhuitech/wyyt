// xImalpha.cpp : Alpha channel functions
/* 07/08/2001 v1.00 - Davide Pizzolato - www.xdp.it
 * CxImage version 6.0.0 02/Feb/2008
 */

#include "ximage.h"

#if CXIMAGE_SUPPORT_ALPHA

////////////////////////////////////////////////////////////////////////////////
/**
 * Checks if the image has a valid alpha channel.
 */
bool CxImage::AlphaIsValid()
{
	return pAlpha!=0;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * Allocates an empty (opaque) alpha channel.
 */
bool CxImage::AlphaCreate()
{
	if (pAlpha==NULL) {
		pAlpha = (BYTE*)malloc(head.biWidth * head.biHeight);
		if (pAlpha) memset(pAlpha,255,head.biWidth * head.biHeight);
	}
	return (pAlpha!=0);
}
////////////////////////////////////////////////////////////////////////////////
void CxImage::AlphaDelete()
{
	if (pAlpha) { free(pAlpha); pAlpha=0; }
}
////////////////////////////////////////////////////////////////////////////////
void CxImage::AlphaInvert()
{
	if (pAlpha) {
		BYTE *iSrc=pAlpha;
		long n=head.biHeight*head.biWidth;
		for(long i=0; i < n; i++){
			*iSrc=(BYTE)~(*(iSrc));
			iSrc++;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Imports an existing alpa channel from another image with the same width and height.
 */
bool CxImage::AlphaCopy(CxImage &from)
{
	if (from.pAlpha == NULL || head.biWidth != from.head.biWidth || head.biHeight != from.head.biHeight) return false;
	if (pAlpha==NULL) pAlpha = (BYTE*)malloc(head.biWidth * head.biHeight);
	if (pAlpha==NULL) return false;
	memcpy(pAlpha,from.pAlpha,head.biWidth * head.biHeight);
	info.nAlphaMax=from.info.nAlphaMax;
	return true;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Sets the alpha level for a single pixel 
 */
void CxImage::AlphaSet(const long x,const long y,const BYTE level)
{
	if (pAlpha && IsInside(x,y)) pAlpha[x+y*head.biWidth]=level;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Gets the alpha level for a single pixel 
 */
BYTE CxImage::AlphaGet(const long x,const long y)
{
	if (pAlpha && IsInside(x,y)) return pAlpha[x+y*head.biWidth];
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Returns pointer to alpha data for pixel (x,y).
 *
 * \author ***bd*** 2.2004
 */
BYTE* CxImage::AlphaGetPointer(const long x,const long y)
{
	if (pAlpha && IsInside(x,y)) return pAlpha+x+y*head.biWidth;
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Get alpha value without boundscheck (a bit faster). Pixel must be inside the image.
 *
 * \author ***bd*** 2.2004
 */
BYTE CxImage::BlindAlphaGet(const long x,const long y)
{
#ifdef _DEBUG
	if (!IsInside(x,y) || (pAlpha==0))
  #if CXIMAGE_SUPPORT_EXCEPTION_HANDLING
		throw 0;
  #else
		return 0;
  #endif
#endif
	return pAlpha[x+y*head.biWidth];
}
////////////////////////////////////////////////////////////////////////////////
bool CxImage::AlphaFlip()
{
	if (!pAlpha) return false;

	BYTE *buff = (BYTE*)malloc(head.biWidth);
	if (!buff) return false;

	BYTE *iSrc,*iDst;
	iSrc = pAlpha + (head.biHeight-1)*head.biWidth;
	iDst = pAlpha;
	for (long i=0; i<(head.biHeight/2); ++i)
	{
		memcpy(buff, iSrc, head.biWidth);
		memcpy(iSrc, iDst, head.biWidth);
		memcpy(iDst, buff, head.biWidth);
		iSrc-=head.biWidth;
		iDst+=head.biWidth;
	}

	free(buff);

	return true;
}
#endif //CXIMAGE_SUPPORT_ALPHA
