
// finalProject.h: archivo de encabezado principal para la aplicación PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "incluir 'pch.h' antes de incluir este archivo para PCH"
#endif

#include "resource.h"		// Símbolos principales


// CfinalProjectApp:
// Consulte finalProject.cpp para obtener información sobre la implementación de esta clase
//

class CfinalProjectApp : public CWinApp
{
public:
	CfinalProjectApp();

// Reemplazos
public:
	virtual BOOL InitInstance();

// Implementación

	DECLARE_MESSAGE_MAP()
};

extern CfinalProjectApp theApp;
