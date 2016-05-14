#pragma once

#define PERFORMANCE_PROCESS_SERVICE_NAME	"com.dcampus.coolview.process"
#define PERFORMANCE_PROCESS_OBJECT_PATH		"/com/dcampus/coolview/process"


QString __GetCvPerfProcessServiceName( const QString processName)
{
	return QString(PERFORMANCE_PROCESS_SERVICE_NAME) + "." + processName;
}

QString __GetCvPerfProcessObjectPath( const QString processName)
{
	return QString(PERFORMANCE_PROCESS_OBJECT_PATH) + "/" + processName;
}

#define  CV_SIPWRAPPER_PROC_NAME		"CvSipWrapper"