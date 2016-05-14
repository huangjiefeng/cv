/*
 * WengoPhone, a voice over Internet phone
 * Copyright (C) 2004-2007  Wengo
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <util/Path.h>

#include <util/File.h>
#include <util/String.h>
#include <util/Logger.h>

#if defined(OS_MACOSX)
	#include <CoreFoundation/CoreFoundation.h>
#elif defined(OS_WINDOWS)
	#include <windows.h>
#elif defined(OS_LINUX)
	#include <stdio.h>
	#include <sys/types.h>
	#include <unistd.h>
#endif

#if !defined(OS_WINDOWS)
	static const int MAX_PATH = 256;
#endif

using namespace std;

string Path::getApplicationDirPath() {
	string result;

#if defined(OS_WINDOWS)

	char moduleName[MAX_PATH];
	GetModuleFileNameA(NULL, moduleName, sizeof(moduleName));

	File file(moduleName);
	result = file.getPath();
	result += File::getPathSeparator();

#elif defined(OS_MACOSX)

	CFBundleRef mainBundle = CFBundleGetMainBundle();
	if (mainBundle) {
		CFURLRef execUrl = CFBundleCopyExecutableURL(mainBundle);
		CFURLRef url = CFURLCreateCopyDeletingLastPathComponent(kCFAllocatorDefault, execUrl);

		char applicationPath[MAX_PATH];

		if (CFURLGetFileSystemRepresentation(url, true, (UInt8 *) applicationPath, sizeof(applicationPath))) {
			result = (string(applicationPath) + File::getPathSeparator());
		}

		CFRelease(execUrl);
		CFRelease(url);
	}

#elif defined(OS_LINUX)

	char procname[MAX_PATH];
	memset(procname, 0, sizeof(procname));
	pid_t pid = getpid();
	if (snprintf(procname, sizeof(procname), "/proc/%i/exe", pid) < 0) {
		return String::null;
	}

	char buff[MAX_PATH];
	memset(buff, 0, sizeof(buff));
	int ret = readlink(procname, buff, sizeof(buff));

	if (ret == -1 || (unsigned int) ret >= sizeof(buff)) {
		return String::null;
	}

	buff[ret] = 0;
	string mstr(buff);
	File f(mstr);
	result = f.getPath() + File::getPathSeparator();

#endif

	return result;
}

string Path::getConfigurationDirPath() {
	string result;

#if defined(OS_WINDOWS)

	char *appData = getenv("APPDATA");
	if(appData){
		// remove the last separator to appData
		String tmpAppData = String(appData);
		if ((!tmpAppData.endsWith("\\")) || (!tmpAppData.endsWith("/"))) {
			tmpAppData = tmpAppData + File::getPathSeparator();
		}
		////

		result = tmpAppData;
	} else {
		result = getHomeDirPath() + File::convertPathSeparators("Application Data/");	
	}

#elif defined(OS_MACOSX)

	result = getHomeDirPath() + File::convertPathSeparators("Library/Application Support/");

#elif defined(OS_POSIX)

	result = getHomeDirPath();

#endif

	return result;
}

std::string Path::getPathSeparator() {
	return File::getPathSeparator();
}


string Path::getHomeDirPath() {
	string result;

#if defined(OS_WINDOWS)
	char * homeDrive = getenv("HOMEDRIVE");
	char * homeDir = getenv("HOMEPATH");
	if (homeDrive && homeDir) {

		// remove the last separator to homeDrive
		String tmpHomeDrive = String(homeDrive);
		if ((tmpHomeDrive.endsWith("\\")) || (tmpHomeDrive.endsWith("/"))) {
			tmpHomeDrive.substr(0, tmpHomeDrive.size() - 2);
		}
		////

		// add the leading separator to homeDir
		String tmpHomeDir = String(homeDir);
		if ((!tmpHomeDir.beginsWith("\\")) || (!tmpHomeDir.beginsWith("/"))) {
			tmpHomeDir = File::getPathSeparator() + tmpHomeDir;
		}
		////

		result = tmpHomeDrive + tmpHomeDir;
	}

#elif defined(OS_POSIX)
	char * homeDir = getenv("HOME");
	if (homeDir) {
		result = homeDir;
	}
#endif
	result += File::getPathSeparator();

	return result;
}

#ifdef OS_MACOSX

std::string Path::getApplicationBundlePath() {
	std::string result;
	CFBundleRef mainBundle = CFBundleGetMainBundle();

	if (mainBundle) {
		CFURLRef url = CFBundleCopyBundleURL(mainBundle);
		char bundlePath[MAX_PATH];

		if (CFURLGetFileSystemRepresentation(url, true, (UInt8 *) bundlePath, sizeof(bundlePath))) {
			result = (std::string(bundlePath) + File::getPathSeparator());
		}

		CFRelease(url);
	}

	return result;
}

std::string Path::getApplicationPrivateFrameworksDirPath() {
	std::string result;
	CFBundleRef mainBundle = CFBundleGetMainBundle();

	if (mainBundle) {
		CFURLRef url = CFBundleCopyPrivateFrameworksURL(mainBundle);
		char frameworkPath[MAX_PATH];

		if (CFURLGetFileSystemRepresentation(url, true, (UInt8 *) frameworkPath, sizeof(frameworkPath))) {
			result = (std::string(frameworkPath) + File::getPathSeparator());
		}

		CFRelease(url);
	}

	return result;
}

std::string Path::getApplicationResourcesDirPath() {
	std::string result;
	CFBundleRef mainBundle = CFBundleGetMainBundle();

	if (mainBundle) {
		CFURLRef url = CFBundleCopyResourcesDirectoryURL(mainBundle);
		char resPath[MAX_PATH];

		if (CFURLGetFileSystemRepresentation(url, true, (UInt8 *) resPath, sizeof(resPath))) {
			result = (std::string(resPath) + File::getPathSeparator());
		}

		CFRelease(url);
	}

	return result;
}


#endif
