// ximainfo.cpp : main attributes
/* 03/10/2004 v1.00 - Davide Pizzolato - www.xdp.it
 * CxImage version 6.0.0 02/Feb/2008
 */

#include "ximage.h"

////////////////////////////////////////////////////////////////////////////////
/**
 * \return the color used for transparency, and/or for background color
 */
RGBQUAD	CxImage::GetTransColor()
{
	if (head.biBitCount<24 && info.nBkgndIndex>=0) return GetPaletteColor((BYTE)info.nBkgndIndex);
	return info.nBkgndColor;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Gets the index used for transparency. Returns -1 for no transparancy.
 */
long CxImage::GetTransIndex() const
{
	return info.nBkgndIndex;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Sets the index used for transparency with 1, 4 and 8 bpp images. Set to -1 to remove the effect.
 */
void CxImage::SetTransIndex(long idx)
{
	if (idx<(long)head.biClrUsed)
		info.nBkgndIndex = idx;
	else 
		info.nBkgndIndex = 0;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Returns true if the image has 256 colors or less.
 */
bool CxImage::IsIndexed() const
{
	return head.biClrUsed!=0;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Set resolution for TIFF, JPEG, PNG and BMP formats.
 */
void CxImage::SetXDPI(long dpi)
{
	if (dpi<=0) dpi = CXIMAGE_DEFAULT_DPI;
	info.xDPI = dpi;
	head.biXPelsPerMeter = (long) floor(dpi * 10000.0 / 254.0 + 0.5);
	if (pDib) ((BITMAPINFOHEADER*)pDib)->biXPelsPerMeter = head.biXPelsPerMeter;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Set resolution for TIFF, JPEG, PNG and BMP formats.
 */
void CxImage::SetYDPI(long dpi)
{
	if (dpi<=0) dpi = CXIMAGE_DEFAULT_DPI;
	info.yDPI = dpi;
	head.biYPelsPerMeter = (long) floor(dpi * 10000.0 / 254.0 + 0.5);
	if (pDib) ((BITMAPINFOHEADER*)pDib)->biYPelsPerMeter = head.biYPelsPerMeter;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * \sa SetCodecOption
 */
DWORD CxImage::GetCodecOption(DWORD imagetype)
{
	imagetype = GetTypeIndexFromId(imagetype);
	if (imagetype==0){
		imagetype = GetTypeIndexFromId(GetType());
	}
	return info.dwCodecOpt[imagetype];
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Encode option for GIF, TIF and JPG.
 * - GIF : 0 = LZW (default), 1 = none, 2 = RLE.
 * - TIF : 0 = automatic (default), or a valid compression code as defined in "tiff.h" (COMPRESSION_NONE = 1, COMPRESSION_CCITTRLE = 2, ...)
 * - JPG : valid values stored in enum CODEC_OPTION ( ENCODE_BASELINE = 0x01, ENCODE_PROGRESSIVE = 0x10, ...)
 * - RAW : valid values stored in enum CODEC_OPTION ( DECODE_QUALITY_LIN = 0x00, DECODE_QUALITY_VNG = 0x01, ...)
 *
 * \return true if everything is ok
 */
bool CxImage::SetCodecOption(DWORD opt, DWORD imagetype)
{
	imagetype = GetTypeIndexFromId(imagetype);
	if (imagetype==0){
		imagetype = GetTypeIndexFromId(GetType());
	}
	info.dwCodecOpt[imagetype] = opt;
	return true;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * \return internal hDib object..
 */
void* CxImage::GetDIB() const
{
	return pDib;
}
////////////////////////////////////////////////////////////////////////////////
DWORD CxImage::GetHeight() const
{
	return head.biHeight;
}
////////////////////////////////////////////////////////////////////////////////
DWORD CxImage::GetWidth() const
{
	return head.biWidth;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * \return DWORD aligned width of the image.
 */
DWORD CxImage::GetEffWidth() const
{
	return info.dwEffWidth;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * \return 2, 16, 256; 0 for RGB images.
 */
DWORD CxImage::GetNumColors() const
{
	return head.biClrUsed;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * \return: 1, 4, 8, 24.
 */
WORD CxImage::GetBpp() const
{
	return head.biBitCount;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * \return original image format
 * \sa ENUM_CXIMAGE_FORMATS.
 */
DWORD CxImage::GetType() const
{
	return info.dwType;
}
////////////////////////////////////////////////////////////////////////////////
DWORD CxImage::GetTypeIndexFromId(const DWORD id)
{
	DWORD n;

	n=0; if (id == CXIMAGE_FORMAT_UNKNOWN) return n;
#if CXIMAGE_SUPPORT_BMP
	n++; if (id == CXIMAGE_FORMAT_BMP) return n;
#endif
#if CXIMAGE_SUPPORT_PNG
	n++; if (id == CXIMAGE_FORMAT_PNG) return n;
#endif

	return 0;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Checks if the image is correctly initializated.
 */
bool CxImage::IsValid() const
{
	return pDib!=0;
}
////////////////////////////////////////////////////////////////////////////////
short CxImage::ntohs(const short word)
{
	if (info.bLittleEndianHost) return word;
	return ( (word & 0xff) << 8 ) | ( (word >> 8) & 0xff );
}
////////////////////////////////////////////////////////////////////////////////
long CxImage::ntohl(const long dword)
{
	if (info.bLittleEndianHost) return dword;
	return  ((dword & 0xff) << 24 ) | ((dword & 0xff00) << 8 ) |
			((dword >> 8) & 0xff00) | ((dword >> 24) & 0xff);
}
////////////////////////////////////////////////////////////////////////////////
void CxImage::bihtoh(BITMAPINFOHEADER* bih)
{
	bih->biSize = ntohl(bih->biSize);
	bih->biWidth = ntohl(bih->biWidth);
	bih->biHeight = ntohl(bih->biHeight);
	bih->biPlanes = ntohs(bih->biPlanes);
	bih->biBitCount = ntohs(bih->biBitCount);
	bih->biCompression = ntohl(bih->biCompression);
	bih->biSizeImage = ntohl(bih->biSizeImage);
	bih->biXPelsPerMeter = ntohl(bih->biXPelsPerMeter);
	bih->biYPelsPerMeter = ntohl(bih->biYPelsPerMeter);
	bih->biClrUsed = ntohl(bih->biClrUsed);
	bih->biClrImportant = ntohl(bih->biClrImportant);
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Returns the last reported error.
 */
const char* CxImage::GetLastError()
{
	return info.szLastError;
}
