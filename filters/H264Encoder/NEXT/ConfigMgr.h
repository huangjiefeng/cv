#pragma once

#include <string>

class CConfigMgr
{
public:

	enum ConfigNames
	{
		CFG_X264_FPS,
		CFG_X264_DEBLOCK,
		CFG_X264_DEBLOCK_ALPHA,
		CFG_X264_DEBLOCK_BETA,

		CFG_X264_RC_METHOD,
		CFG_X264_RC_CQP,
		CFG_X264_RC_CRF,
		CFG_X264_RC_AQ,
	};

	enum Section
	{
		SEC_X264,
		SEC_X264_RC,
		SEC_X264_ANALYSE,
	};

	enum ValueType
	{
		TYPE_INT,
		TYPE_FLOAT,
		TYPE_STRING,
	};

	struct ConfigParam
	{
		ConfigNames		config;
		Section			section;
		char *			key;
		char *			value;
		char *			default;
	};

	struct SectionName
	{
		Section			sec;
		char *			name;
	};

private:

	static std::string s_strConfigFile;

	CConfigMgr(void);

	static BOOL Init();

	static const ConfigParam *	GetParam(const ConfigNames config);
	static const char *			GetSectionName(const Section sec);

public:

	~CConfigMgr(void);

	static BOOL GetValue(const ConfigNames cfg, const ValueType type, void * pValue);

};
