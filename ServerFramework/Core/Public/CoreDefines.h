#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_COREDEFINES_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_COREDEFINES_H

#pragma warning (disable : 4251)
#pragma warning (disable : 6031)
#pragma warning (disable : 4150)


#define _CRT_SECURE_NO_WARNINGS 0

#define _WIN32_WINDOWS 0x0601
#define BOOST_ASIO_WINDOWS
#define  _WINSOCK_DEPRECATED_NO_WARNINGS

#define USE_DEBUG 0

#ifdef _DEBUG
#pragma comment (lib, "CusLibDebug")
#else
#pragma comment (lib, "CusLib")
#endif

#include <iostream>
#include <atomic>
#include <boost/asio.hpp>
#include <oneapi/tbb/concurrent_hash_map.h>
#include <oneapi/tbb/concurrent_priority_queue.h>
#include <oneapi/tbb/concurrent_queue.h>
#include <oneapi/tbb/concurrent_unordered_map.h>
#include <oneapi/tbb/concurrent_unordered_set.h>
#include <oneapi/tbb/concurrent_vector.h>
#include <oneapi/tbb/concurrent_set.h>
#include <oneapi/tbb/mutex.h>
#include <mutex>
#include <thread>
#include <string>
#include <array>
#include <concepts>
#include <chrono>
#include "CusMath/CusMath.h"
#include<concepts>

#include "SmartPointer.h"

#include "CoreConcept.h"
#include "CoreTypedef.h"
#include "CorePacket.h"
#include "CoreEnum.h"
#include "CoreFunction.h"
#include "CoreMacro.h"
#include "CoreTls.h"

#include "Lock.h"


namespace Asio = boost::asio;

namespace Core{ }


#endif 