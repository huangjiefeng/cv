
#pragma warning(disable:4995)

#include "StdAfx.h"
#include "ConfigMgr.h"
#include <fstream>
#include "io.h"         // for _access
#include "Util.h"
#include "Log.h"

static const CConfigMgr::ConfigParam g_configTable[] = 
{
	{CConfigMgr::CFG_X264_FPS,			 CConfigMgr::SEC_X264, "fps", "", "25"},
	{CConfigMgr::CFG_X264_DEBLOCK,		 CConfigMgr::SEC_X264, "deblock", "", "1"},
	{CConfigMgr::CFG_X264_DEBLOCK_ALPHA, CConfigMgr::SEC_X264, "deblock_alpha", "", "0"},
	{CConfigMgr::CFG_X264_DEBLOCK_BETA,  CConfigMgr::SEC_X264, "deblock_beta", "", "0"},

	{CConfigMgr::CFG_X264_RC_METHOD,	 CConfigMgr::SEC_X264_RC, "method", "", "1"},
	{CConfigMgr::CFG_X264_RC_CRF,		 CConfigMgr::SEC_X264_RC, "crf", "", "29"},
	{CConfigMgr::CFG_X264_RC_CQP,		 CConfigMgr::SEC_X264_RC, "cqp", "", "32"},
	{CConfigMgr::CFG_X264_RC_AQ,		 CConfigMgr::SEC_X264_RC, "aq", "", "1"},

};

static const CConfigMgr::SectionName g_sectionName[] = 
{
	{CConfigMgr::SEC_X264,	"x264"},
	{CConfigMgr::SEC_X264_RC,	"RC"},
	{CConfigMgr::SEC_X264_ANALYSE,	"Analyse"},
};

std::string CConfigMgr::s_strConfigFile;


CConfigMgr::CConfigMgr(void)
{
}

CConfigMgr::~CConfigMgr(void)
{
}


BOOL CConfigMgr::GetValue( const ConfigNames cfg, const ValueType type, void * pValue )
{
	if (NULL == pValue)
	{
		return FALSE;
	}

	if (s_strConfigFile.empty() && FALSE == Init())
	{
		return FALSE;
	}

	const ConfigParam * param = GetParam(cfg);
	if (NULL == param)
	{
		return FALSE;
	}

	const char * szSec = GetSectionName(param->section);
	if (NULL == szSec)
	{
		return FALSE;
	}

	switch (type)
	{
	case TYPE_STRING:
		{
			char value[1024];
			DWORD len = GetPrivateProfileString(szSec, param->key, param->default, value, sizeof(value), s_strConfigFile.c_str());
			*((std::string *)pValue) = value;
		}
		break;

	case TYPE_INT:
		{
			int value = GetPrivateProfileInt(szSec, param->key, atoi(param->default), s_strConfigFile.c_str());
			*((int *)pValue) = value;
		}
		break;

	case TYPE_FLOAT:
		{
			char value[1024];
			DWORD len = GetPrivateProfileString(szSec, param->key, param->default, value, sizeof(value), s_strConfigFile.c_str());
			float fv = (float)atof(value);
			*((float *)pValue) = fv;
		}
		break;

	default:
		return FALSE;
	}

	return TRUE;

}

const CConfigMgr::ConfigParam * CConfigMgr::GetParam( const ConfigNames config )
{
	const int len = sizeof(g_configTable) / sizeof(ConfigParam);
	for (int i = 0; len > i; ++i)
	{
		if (g_configTable[i].config == config)
		{
			return &g_configTable[i];
		}
	}

	return NULL;

}

const char * CConfigMgr::GetSectionName( const Section sec )
{
	const int len = sizeof(g_sectionName) / sizeof(SectionName);
	for (int i = 0; len > i; ++i)
	{
		if (g_sectionName[i].sec == sec)
		{
			return g_sectionName[i].name;
		}
	}

	return NULL;

}

BOOL CConfigMgr::Init()
{
	if (!s_strConfigFile.empty())
	{
		return TRUE;
	}

	char szPath[1024] = {0};
	getApplicationDirPath(szPath);

	s_strConfigFile = szPath;
	s_strConfigFile += "\\cfg\\CvX264.ini";

	if (_access(s_strConfigFile.c_str(), 0) == -1)
	{
		s_strConfigFile = "c:\\CvX264.ini";
		if (_access(s_strConfigFile.c_str(), 0) == -1)
		{
			LOG_INFO("No config file found, using default settings.");
			return FALSE;
		}
	}

	LOG_INFO("Found config in %s", s_strConfigFile.c_str());

	return TRUE;

}

