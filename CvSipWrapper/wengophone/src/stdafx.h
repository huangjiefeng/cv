// stdafx.h : 标准系统包含文件的包含文件，
// 或是常用但不常更改的项目特定的包含文件
//

#pragma once

// TODO: 在此处引用程序要求的附加头文件
#include <boost/signal.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/visit_each.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/archive_exception.hpp>
#include <boost/serialization/map.hpp>
#include <boost/program_options.hpp>

#include <QtGui/QtGui>
#include <qtutil/SafeConnect.h>
#include <qtutil/Widget.h>

#include <util/Logger.h>
#include <util/SafeDelete.h>
#include <cutil/global.h>

#include <WengoPhoneBuildId.h>


#define  DBUS_ENABLE