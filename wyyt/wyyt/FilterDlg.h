#pragma once
#include "EditDelayed.h"

class CTitleMenu;
class CFilterDlg{// X: [FI] - [FilterItem]
public:
	CFilterDlg():filter(0){};

	void CreateFilterMenu(CTitleMenu&CatMenu) const;
	CString GetFilterLabel() const;

	virtual void	UpdateFilterLabel()=0;
	//CEditDelayed	m_ctlFilter;
	//CEditDelayed/*CEdit*/ m_ctlFilter_Record;
	size_t			filter;
};