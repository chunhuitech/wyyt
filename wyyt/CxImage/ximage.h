/*
 * File:	ximage.h
 * Purpose:	General Purpose Image Class 
 */
/*
  --------------------------------------------------------------------------------

	COPYRIGHT NOTICE, DISCLAIMER, and LICENSE:

	CxImage version 6.0.0 02/Feb/2008

	CxImage : Copyright (C) 2001 - 2008, Davide Pizzolato

	Original CImage and CImageIterator implementation are:
	Copyright (C) 1995, Alejandro Aguilar Sierra (asierra(at)servidor(dot)unam(dot)mx)

	Covered code is provided under this license on an "as is" basis, without warranty
	of any kind, either expressed or implied, including, without limitation, warranties
	that the covered code is free of defects, merchantable, fit for a particular purpose
	or non-infringing. The entire risk as to the quality and performance of the covered
	code is with you. Should any covered code prove defective in any respect, you (not
	the initial developer or any other contributor) assume the cost of any necessary
	servicing, repair or correction. This disclaimer of warranty constitutes an essential
	part of this license. No use of any covered code is authorized hereunder except under
	this disclaimer.

	Permission is hereby granted to use, copy, modify, and distribute this
	source code, or portions hereof, for any purpose, including commercial applications,
	freely and without fee, subject to the following restrictions: 

	1. The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgment in the product documentation would be
	appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.

	3. This notice may not be removed or altered from any source distribution.

  --------------------------------------------------------------------------------

	Other information about CxImage, and the latest version, can be found at the
	CxImage home page: http://www.xdp.it/cximage/

  --------------------------------------------------------------------------------
 */
#if !defined(__CXIMAGE_H)
#define __CXIMAGE_H

#if _MSC_VER > 1000
#pragma once
#endif 

/////////////////////////////////////////////////////////////////////////////
#include "xfile.h"
#include "xiofile.h"
#include "xmemfile.h"
#include "ximadef.h"	//<vho> adjust some #define

/* see "ximacfg.h" for CxImage configuration options */

/////////////////////////////////////////////////////////////////////////////
// CxImage formats enumerator
enum ENUM_CXIMAGE_FORMATS{
CXIMAGE_FORMAT_UNKNOWN = 0,
#if CXIMAGE_SUPPORT_BMP
CXIMAGE_FORMAT_BMP = 1,
#endif
#if CXIMAGE_SUPPORT_PNG
CXIMAGE_FORMAT_PNG = 2,
#endif
CMAX_IMAGE_FORMATS = CXIMAGE_SUPPORT_BMP + CXIMAGE_SUPPORT_PNG + 1
};
/////////////////////////////////////////////////////////////////////////////
// CxImage class
/////////////////////////////////////////////////////////////////////////////
class DLL_EXP CxImage
{
//extensible information collector
typedef struct tagCxImageInfo {
	DWORD	dwEffWidth;			///< DWORD aligned scan line width
	BYTE*	pImage;				///< THE IMAGE BITS
	CxImage* pGhost;			///< if this is a ghost, pGhost points to the body
	CxImage* pParent;			///< if this is a layer, pParent points to the body
	DWORD	dwType;				///< original image format
	char	szLastError[256];	///< debugging
	long	nProgress;			///< monitor
	long	nEscape;			///< escape
	long	nBkgndIndex;		///< used for GIF, PNG, MNG
	RGBQUAD nBkgndColor;		///< used for RGB transparency
	float	fQuality;			///< used for JPEG, JPEG2000 (0.0f ... 100.0f)
	BYTE	nJpegScale;			///< used for JPEG [ignacio]
	long	nFrame;				///< used for TIF, GIF, MNG : actual frame
	long	nNumFrames;			///< used for TIF, GIF, MNG : total number of frames
	DWORD	dwFrameDelay;		///< used for GIF, MNG
	long	xDPI;				///< horizontal resolution
	long	yDPI;				///< vertical resolution
	RECT	rSelectionBox;		///< bounding rectangle
	BYTE	nAlphaMax;			///< max opacity (fade)
	bool	bAlphaPaletteEnabled; ///< true if alpha values in the palette are enabled.
	bool	bEnabled;			///< enables the painting functions
	long	xOffset;
	long	yOffset;
	DWORD	dwCodecOpt[CMAX_IMAGE_FORMATS];	///< for GIF, TIF : 0=def.1=unc,2=fax3,3=fax4,4=pack,5=jpg
	RGBQUAD last_c;				///< for GetNearestIndex optimization
	BYTE	last_c_index;
	bool	last_c_isvalid;
	long	nNumLayers;
	DWORD	dwFlags;			///< 0x??00000 = reserved, 0x00??0000 = blend mode, 0x0000???? = layer id - user flags
	BYTE	dispmeth;
	bool	bGetAllFrames;
	bool	bLittleEndianHost;

} CXIMAGEINFO;

public:
	//public structures
struct rgb_color { BYTE r,g,b; };

/** \addtogroup Constructors */ //@{
	CxImage(DWORD imagetype = 0);
	CxImage(DWORD dwWidth, DWORD dwHeight, DWORD wBpp, DWORD imagetype = 0);
	CxImage(BYTE * buffer, DWORD size, DWORD imagetype);
	virtual ~CxImage() { DestroyFrames(); Destroy(); };
//@}

/** \addtogroup Initialization */ //@{
	void*	Create(DWORD dwWidth, DWORD dwHeight, DWORD wBpp, DWORD imagetype = 0);
	bool	Destroy();
	bool	DestroyFrames();
	void	Clear(BYTE bval=0);
	void	Copy(const CxImage &src, bool copypixels = true, bool copyselection = true, bool copyalpha = true);
	bool	Transfer(CxImage &from, bool bTransferFrames = true);

//@}

/** \addtogroup Attributes */ //@{
	long	GetSize();
	BYTE*	GetBits(DWORD row = 0);
	void*	GetDIB() const;
	DWORD	GetHeight() const;
	DWORD	GetWidth() const;
	DWORD	GetEffWidth() const;
	DWORD	GetNumColors() const;
	WORD	GetBpp() const;
	DWORD	GetType() const;
	const char*	GetLastError();
	void	SetXDPI(long dpi);
	void	SetYDPI(long dpi);
	DWORD	GetClrImportant() const;
	void	SetClrImportant(DWORD ncolors = 0);
	long	GetTransIndex() const;
	RGBQUAD	GetTransColor();
	void	SetTransIndex(long idx);
	DWORD	GetCodecOption(DWORD imagetype = 0);
	bool	SetCodecOption(DWORD opt, DWORD imagetype = 0);
	static DWORD GetTypeIndexFromId(const DWORD id);
//@}

/** \addtogroup Palette
 * These functions have no effects on RGB images and in this case the returned value is always 0.
 * @{ */
	bool	IsGrayScale();
	bool	IsIndexed() const;
	DWORD	GetPaletteSize();
	RGBQUAD* GetPalette() const;
	RGBQUAD GetPaletteColor(BYTE idx);
	bool	GetPaletteColor(BYTE i, BYTE* r, BYTE* g, BYTE* b);
	BYTE	GetNearestIndex(RGBQUAD c);
	void	SetGrayPalette();
	void	SetPalette(RGBQUAD* pPal,DWORD nColors=256);
	void	SetPalette(rgb_color *rgb,DWORD nColors=256);
	void	SetPaletteColor(BYTE idx, BYTE r, BYTE g, BYTE b, BYTE alpha=0);
	void	SetPaletteColor(BYTE idx, RGBQUAD c);
	void	SwapIndex(BYTE idx1, BYTE idx2);
	void	SetStdPalette();
//@}

/** \addtogroup Pixel */ //@{
	bool	IsInside(long x, long y);
	RGBQUAD GetPixelColor(long x,long y, bool bGetAlpha = true);
	BYTE	GetPixelIndex(long x,long y);
	void	SetPixelColor(long x,long y,RGBQUAD c, bool bSetAlpha = false);
	void	SetPixelColor(long x,long y,COLORREF cr);
	void	SetPixelIndex(long x,long y,BYTE i);
//@}

protected:
/** \addtogroup Protected */ //@{
	BYTE BlindGetPixelIndex(const long x,const long y);
	RGBQUAD BlindGetPixelColor(const long x,const long y, bool bGetAlpha = true);
	void *BlindGetPixelPointer(const long x,const  long y);
	void BlindSetPixelColor(long x,long y,RGBQUAD c, bool bSetAlpha = false);
	void BlindSetPixelIndex(long x,long y,BYTE i);
//@}

public:

#if CXIMAGE_SUPPORT_INTERPOLATION
/** \addtogroup Interpolation */ //@{
	//overflow methods:
	enum OverflowMethod {
		OM_BACKGROUND=2,
		OM_REPEAT=5
	};
	void OverflowCoordinates(long  &x, long &y, OverflowMethod const ofMethod);
	RGBQUAD GetPixelColorWithOverflow(long x, long y, OverflowMethod const ofMethod=OM_BACKGROUND, RGBQUAD* const rplColor=0);
	//interpolation methods:
	enum InterpolationMethod {
		IM_BILINEAR		=2
	};
	RGBQUAD GetPixelColorInterpolated(float x,float y, InterpolationMethod const inMethod=IM_BILINEAR, OverflowMethod const ofMethod=OM_BACKGROUND, RGBQUAD* const rplColor=0);
	RGBQUAD GetAreaColorInterpolated(float const xc, float const yc, float const w, float const h, InterpolationMethod const inMethod, OverflowMethod const ofMethod=OM_BACKGROUND, RGBQUAD* const rplColor=0);
//@}

protected:
/** \addtogroup Protected */ //@{
	void  AddAveragingCont(RGBQUAD const &color, float const surf, float &rr, float &gg, float &bb, float &aa);
//@}
#endif //CXIMAGE_SUPPORT_INTERPOLATION
	
/** \addtogroup Painting */ //@{
#if CXIMAGE_SUPPORT_WINDOWS
public:
	HBITMAP MakeBitmap(HDC hdc = NULL, bool bTransparency = false);
	bool	CreateFromHBITMAP(HBITMAP hbmp, HPALETTE hpal=0);	//Windows resource
	long	Draw(HDC hdc, long x=0, long y=0, long cx = -1, long cy = -1, RECT* pClipRect = 0, bool bSmooth = false);
	long	DrawString(HDC hdc, long x, long y, const TCHAR* text, RGBQUAD color, const TCHAR* font, long lSize=0, long lWeight=400, BYTE bItalic=0, BYTE bUnderline=0, bool bSetAlpha=false);
#endif //CXIMAGE_SUPPORT_WINDOWS
//@}

	// file operations
#if CXIMAGE_SUPPORT_DECODE
/** \addtogroup Decode */ //@{
	bool Decode(CxFile * hFile, DWORD imagetype);
	bool Decode(BYTE * buffer, DWORD size, DWORD imagetype);
//@}
#endif //CXIMAGE_SUPPORT_DECODE

#if CXIMAGE_SUPPORT_ENCODE
protected:
/** \addtogroup Protected */ //@{
	bool EncodeSafeCheck(CxFile *hFile);
//@}

public:
/** \addtogroup Encode */ //@{
	bool Encode(CxFile * hFile, DWORD imagetype);
	bool Encode(BYTE * &buffer, long &size, DWORD imagetype);
//@}
#endif //CXIMAGE_SUPPORT_ENCODE

/** \addtogroup Attributes */ //@{
	//misc.
	bool IsValid() const;
//@}

#if CXIMAGE_SUPPORT_BASICTRANSFORMATIONS
/** \addtogroup BasicTransformations */ //@{
	bool GrayScale();
	bool Flip(bool bFlipSelection = false, bool bFlipAlpha = true);
	bool RotateLeft(CxImage* iDst = NULL);
	bool RotateRight(CxImage* iDst = NULL);
//@}
#endif //CXIMAGE_SUPPORT_BASICTRANSFORMATIONS

#if CXIMAGE_SUPPORT_TRANSFORMATION
/** \addtogroup Transformations */ //@{
	// image operations
	bool Rotate2(float angle, CxImage *iDst = NULL, InterpolationMethod inMethod=IM_BILINEAR,
                OverflowMethod ofMethod=OM_BACKGROUND, RGBQUAD *replColor=0,
                bool const optimizeRightAngles=true, bool const bKeepOriginalSize=false);
	bool Rotate180(CxImage* iDst = NULL);
	bool Resample(long newx, long newy, int mode = 1, CxImage* iDst = NULL);
	bool DecreaseBpp(DWORD nbit, bool errordiffusion, RGBQUAD* ppal = 0, DWORD clrimportant = 0);

//@}
#endif //CXIMAGE_SUPPORT_TRANSFORMATION

//#if CXIMAGE_SUPPORT_DSP
/** \addtogroup DSP */ //@{
	bool Jitter(long radius=2);
//@}

//#endif //CXIMAGE_SUPPORT_DSP
	static RGBQUAD RGBtoRGBQUAD(COLORREF cr);
protected:
/** \addtogroup Protected */ //@{
#if CXIMAGE_SUPPORT_ALPHA
/** \addtogroup Alpha */ //@{
	bool AlphaCreate();
	void AlphaDelete();
	void AlphaInvert();
	bool AlphaFlip();
	bool AlphaCopy(CxImage &from);
	void AlphaSet(const long x,const long y,const BYTE level);
	BYTE AlphaGet(const long x,const long y);
	bool AlphaIsValid();
	BYTE* AlphaGetPointer(const long x = 0,const long y = 0);
//@}

protected:
/** \addtogroup Protected */ //@{
	BYTE BlindAlphaGet(const long x,const long y);
//@}
#endif //CXIMAGE_SUPPORT_ALPHA
	void Startup(DWORD imagetype = 0);
	void CopyInfo(const CxImage &src);
	void Ghost(const CxImage *src);
	void RGBtoBGR(BYTE *buffer, int length);
	void Bitfield2RGB(BYTE *src, DWORD redmask, DWORD greenmask, DWORD bluemask, BYTE bpp);
	short ntohs(const short word);
	long ntohl(const long dword);
	void bihtoh(BITMAPINFOHEADER* bih);

	void*				pDib; //contains the header, the palette, the pixels
    BITMAPINFOHEADER    head; //standard header
	CXIMAGEINFO			info; //extended information
	BYTE*				pSelection;	//selected region
	BYTE*				pAlpha; //alpha channel
	CxImage**			ppLayers; //generic layers
	CxImage**			ppFrames;
//@}
};

////////////////////////////////////////////////////////////////////////////
#endif // !defined(__CXIMAGE_H)
