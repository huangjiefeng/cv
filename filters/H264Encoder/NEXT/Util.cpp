
#include "StdAfx.h"
#include "Util.h"
#include <Windows.h>

int getApplicationDirPath(char * applicationDirPath) 
{
	char szAppPath[MAX_PATH]      = "";
	//char szAppDirectory[MAX_PATH] = "";

	if(!applicationDirPath) 
		return -1;

	if(!::GetModuleFileName(0, szAppPath, sizeof(szAppPath) - 1))
	{
		return -1;
	}

	// Extract directory
	strncpy(applicationDirPath, szAppPath, strrchr(szAppPath, '\\') - szAppPath);
	applicationDirPath[strlen(applicationDirPath)] = '\0';

	return 0;
}

BOOL bIdInit = FALSE;
int getEncID()
{
	if (!bIdInit)
	{
		srand(GetCurrentProcessId());
		bIdInit = TRUE;
	}
	return rand() % 10000;
}
