// xImaCodec.cpp : Encode Decode functions
/* 07/08/2001 v1.00 - Davide Pizzolato - www.xdp.it
 * CxImage version 6.0.0 02/Feb/2008
 */

#include "ximage.h"

#if CXIMAGE_SUPPORT_PNG
#include "ximapng.h"
#endif

#if CXIMAGE_SUPPORT_BMP
#include "ximabmp.h"
#endif
////////////////////////////////////////////////////////////////////////////////
#if CXIMAGE_SUPPORT_ENCODE
////////////////////////////////////////////////////////////////////////////////
bool CxImage::EncodeSafeCheck(CxFile *hFile)
{
	if (hFile==NULL) {
		strcpy(info.szLastError,CXIMAGE_ERR_NOFILE);
		return true;
	}

	if (pDib==NULL){
		strcpy(info.szLastError,CXIMAGE_ERR_NOIMAGE);
		return true;
	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Saves to memory buffer the image in a specific format.
 * \param buffer: output memory buffer pointer. Must be NULL,
 * the function allocates and fill the memory,
 * the application must free the buffer, see also FreeMemory().
 * \param size: output memory buffer size.
 * \param imagetype: file format, see ENUM_CXIMAGE_FORMATS
 * \return true if everything is ok
 */
bool CxImage::Encode(BYTE * &buffer, long &size, DWORD imagetype)
{
	if (buffer!=NULL){
		strcpy(info.szLastError,"the buffer must be empty");
		return false;
	}
	CxMemFile file;
	file.Open();
	if(Encode(&file,imagetype)){
		buffer=file.GetBuffer();
		size=file.Size();
		return true;
	}
	return false;
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Saves to disk the image in a specific format.
 * \param hFile: file handle (CxMemFile or CxIOFile), with write access.
 * \param imagetype: file format, see ENUM_CXIMAGE_FORMATS
 * \return true if everything is ok
 * \sa ENUM_CXIMAGE_FORMATS
 */
bool CxImage::Encode(CxFile *hFile, DWORD imagetype)
{

#if CXIMAGE_SUPPORT_BMP

	if (imagetype==CXIMAGE_FORMAT_BMP){
		CxImageBMP newima;
		newima.Ghost(this);
		if (newima.Encode(hFile)){
			return true;
		} else {
			strcpy(info.szLastError,newima.GetLastError());
			return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_PNG
	if (imagetype==CXIMAGE_FORMAT_PNG){
		CxImagePNG newima;
		newima.Ghost(this);
		if (newima.Encode(hFile)){
			return true;
		} else {
			strcpy(info.szLastError,newima.GetLastError());
			return false;
		}
	}
#endif

	strcpy(info.szLastError,"Encode: Unknown format");
	return false;
}
////////////////////////////////////////////////////////////////////////////////
#endif //CXIMAGE_SUPPORT_ENCODE
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
#if CXIMAGE_SUPPORT_DECODE
////////////////////////////////////////////////////////////////////////////////
/**
 * Constructor from memory buffer, see Decode()
 * \param buffer: memory buffer
 * \param size: size of buffer
 * \param imagetype: file format, see ENUM_CXIMAGE_FORMATS
 */
CxImage::CxImage(BYTE * buffer, DWORD size, DWORD imagetype)
{
	Startup(imagetype);
	CxMemFile stream(buffer,size);
	Decode(&stream,imagetype);
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Loads an image from memory buffer
 * \param buffer: memory buffer
 * \param size: size of buffer
 * \param imagetype: file format, see ENUM_CXIMAGE_FORMATS
 * \return true if everything is ok
 */
bool CxImage::Decode(BYTE * buffer, DWORD size, DWORD imagetype)
{
	CxMemFile file(buffer,size);
	return Decode(&file,imagetype);
}
////////////////////////////////////////////////////////////////////////////////
/**
 * Loads an image from CxFile object
 * \param hFile: file handle (CxMemFile or CxIOFile), with read access.
 * \param imagetype: file format, see ENUM_CXIMAGE_FORMATS
 * \return true if everything is ok
 * \sa ENUM_CXIMAGE_FORMATS
 */
bool CxImage::Decode(CxFile *hFile, DWORD imagetype)
{
	if (hFile == NULL){
		strcpy(info.szLastError,CXIMAGE_ERR_NOFILE);
		return false;
	}

	if (imagetype==CXIMAGE_FORMAT_UNKNOWN){
		DWORD pos = hFile->Tell();
#if CXIMAGE_SUPPORT_BMP
		{ CxImageBMP newima; newima.CopyInfo(*this); if (newima.Decode(hFile)) { Transfer(newima); return true; } else hFile->Seek(pos,SEEK_SET); }
#endif
#if CXIMAGE_SUPPORT_PNG
		{ CxImagePNG newima; newima.CopyInfo(*this); if (newima.Decode(hFile)) { Transfer(newima); return true; } else hFile->Seek(pos,SEEK_SET); }
#endif
	}

#if CXIMAGE_SUPPORT_BMP
	if (imagetype==CXIMAGE_FORMAT_BMP){
		CxImageBMP newima;
		newima.CopyInfo(*this);
		if (newima.Decode(hFile)){
			Transfer(newima);
			return true;
		} else {
			strcpy(info.szLastError,newima.GetLastError());
			return false;
		}
	}
#endif
#if CXIMAGE_SUPPORT_PNG
	if (imagetype==CXIMAGE_FORMAT_PNG){
		CxImagePNG newima;
		newima.CopyInfo(*this);
		if (newima.Decode(hFile)){
			Transfer(newima);
			return true;
		} else {
			strcpy(info.szLastError,newima.GetLastError());
			return false;
		}
	}
#endif

	strcpy(info.szLastError,"Decode: Unknown or wrong format");
	return false;
}
////////////////////////////////////////////////////////////////////////////////
#endif //CXIMAGE_SUPPORT_DECODE
////////////////////////////////////////////////////////////////////////////////
