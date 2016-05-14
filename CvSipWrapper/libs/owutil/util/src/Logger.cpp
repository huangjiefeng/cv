/*
 * WengoPhone, a voice over Internet phone
 * Copyright (C) 2004-2006  Wengo
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

#include <util/Logger.h>

#include <util/Path.h>
#include <util/Time.h>
#include <util/Date.h>
#include <util/String.h>

#include <iostream>
#include <sstream>

#include <stdio.h>
#include <stdarg.h>

#ifdef _MSC_VER
	#define vsnprintf _vsnprintf
#endif

#include <ctime>
#include <cassert>

static const char* OWLOGGER_PREFIX = "OWLOGGER_";
static const char* DEFAULT_LEVEL_ENVVAR = "OWLOGGER_DEFAULT";


Logger::Helper::Helper(const char* component, Level level, const char* className, const char* fileName, int line)
: _component(component), _level(level), _className(className), _fileName(fileName), _line(line) {}

void Logger::Helper::operator()(const std::string& message) {
	Logger::getInstance()->log(_component, _level, _className, message, _fileName, _line);
}

void Logger::Helper::operator()(const char* format, ...) {
	char buffer[1024];
	va_list ap;
	va_start(ap, format);
	vsnprintf(buffer, sizeof(buffer), format, ap);
	va_end(ap);
	Logger::getInstance()->log(_component, _level, _className, buffer, _fileName, _line);
}


Logger* Logger::getInstance() {
	static Logger instance;
	return &instance;
}

Logger::Logger() {
	_logFileState = LogFileState_Unset;
}

Logger::~Logger() {
	flush();
	_file.close();
}

void Logger::setLogFileName(const std::string& name) {
	if (name.empty()) {
		_logFileState = LogFileState_Disabled;
	} else {
		_logFileState = LogFileState_Defined;
		_logFileName = name;
	}
}

std::string Logger::getLogFileName() const {
	return _logFileName;
}

void Logger::debug(const std::string & component, const std::string & className, const std::string & message) {
	log(component, Debug, className, message);
}

void Logger::info(const std::string & component, const std::string & className, const std::string & message) {
	log(component, Info, className, message);
}

void Logger::warn(const std::string & component, const std::string & className, const std::string & message) {
	log(component, Warn, className, message);
}

void Logger::error(const std::string & component, const std::string & className, const std::string & message) {
	log(component, Error, className, message);
}

void Logger::fatal(const std::string & component, const std::string & className, const std::string & message, const char* filename, int line) {
	log(component, Fatal, className, message, filename, line);
	flush();
	assert(NULL && "fatal error");
}

static Logger::Level levelFromEnvironmentVariable(const std::string& variable, Logger::Level defaultValue) {
	char* ptr = getenv(variable.c_str());
	if (!ptr) {
		return defaultValue;
	}

	int intLevel = atoi(ptr);
	if (intLevel < Logger::Debug || intLevel > Logger::Fatal) {
		return defaultValue;
	}

	return Logger::Level(intLevel);
}

static Logger::Level minLevelForComponent(const std::string & component) {
	static Logger::Level defaultMinLevel;
	static bool defaultMinLevelInitialized = false;
	static std::map<std::string, Logger::Level> levelMap;

	if (!defaultMinLevelInitialized) {
		defaultMinLevel = levelFromEnvironmentVariable(DEFAULT_LEVEL_ENVVAR, Logger::Info);
		defaultMinLevelInitialized = true;
	}

	std::map<std::string, Logger::Level>::const_iterator it = levelMap.find(component);

	if (it == levelMap.end()) {
		std::string envName = OWLOGGER_PREFIX + String(component).toUpperCase();
		Logger::Level level = levelFromEnvironmentVariable(envName, defaultMinLevel);

		levelMap[component] = level;
		return level;
	}
	return it->second;
}

Logger::Helper Logger::getHelper(const char* component, Logger::Level level, const char* className, const char* fileName, int line) {
	return Logger::Helper(component, level, className, fileName, line);
}

void Logger::log(const std::string & component, Level level, const std::string & className, const std::string & message, const char* filename, int line) {
	boost::mutex::scoped_lock scopedLock(_mutex);

	std::string levelString;

	Level minLevel = minLevelForComponent(component);
	if (level < minLevel) {
		return;
	}

	switch (level) {
	case Debug:
		levelString = "debug";
		break;
	case Info:
		levelString = "info";
		break;
	case Warn:
		levelString = "warn";
		break;
	case Error:
		levelString = "error";
		break;
	default:
		levelString = "fatal";
		break;
	}

	std::string tmp = "(" + levelString + ") " + Time().toString() + " [" + component + "]";
	if (filename) {
		tmp += " " + std::string(filename) + ":" + String::fromNumber(line);
	}
	tmp += " " + className +  ": " + message;

	if (_logFileState == LogFileState_Unset) {
		std::cerr << "*** Logger::log() called before Logger::setLogFileName() ***" << std::endl;
	} else if (_logFileState == LogFileState_Defined) {
		_file.open(_logFileName.c_str());
		_file << "Log file=" << _logFileName << std::endl;
		_file << "Date=" << Date().toString() << std::endl;
		_file << "Time=" << Time().toString() << std::endl << std::endl;
		_logFileState = LogFileState_Created;
	}

	if (_logFileState == LogFileState_Created) {
		_file << tmp << std::endl;
	}
	std::cerr << tmp << std::endl;

#ifdef LOGGER_ENABLE_EVENT
	messageAddedEvent(tmp);
#endif
}

void Logger::flush() {
	boost::mutex::scoped_lock scopedLock(_mutex);

	_file.flush();
}
