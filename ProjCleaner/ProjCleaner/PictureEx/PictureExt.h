#pragma once


// CPictureExt

class CPictureExt : public CStatic
{
	DECLARE_DYNAMIC(CPictureExt)

public:
	CPictureExt();
	virtual ~CPictureExt();

protected:
	DECLARE_MESSAGE_MAP()
public:
	BOOL LoadPicture(UINT nResource, LPCTSTR lpszResType)
	{return LoadPicture(MAKEINTRESOURCE(nResource), lpszResType);}
	BOOL LoadPictureFromFile(LPCTSTR lpszFileName);
	BOOL LoadPicture(LPCTSTR lpszResource, LPCTSTR lpszResType);
	BOOL IsValid(){ return m_pPic != NULL; }
	CSize GetSize(){return m_size;}
	void Draw();
	void Draw(CDC* pDC, LPCRECT lprcDest, LPCRECT lprcSrc); 
	void Draw(CDC* pDC, int xDest,int yDest, int cxDest, int cyDest, int xSrc ,int ySrc ,int cxSrc ,int cySrc); 
	void Release();
protected:
	IPicture *m_pPic;
	OLE_XSIZE_HIMETRIC _w_him;
	OLE_YSIZE_HIMETRIC _h_him;
	CSize m_size;
protected:
	void CalcSize();
};


