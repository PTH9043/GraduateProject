#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_COREDEFINES_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_COREDEFINES_H

#pragma warning (disable : 4251)
#pragma warning (disable : 6031)
#pragma warning (disable : 4150)


#define _CRT_SECURE_NO_WARNINGS 0

#define USE_DEBUG 0

/*
@ Date: 2023-12-30
@ Writer: 박태현
@ Explain
- 현재 실행되는 코드의 상황이 Window OS인지 Linux OS인지 정의한다.
*/
#define WINDOW_OS
// #define LINUX_OS

#ifdef WINDOW_OS

#define _WIN32_WINDOWS 0x6001
#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#endif

#ifdef _DEBUG
#pragma comment (lib, "PthMathD")
#else
#pragma comment (lib, "PthMath")
#endif

#include <iostream>
#include <atomic>
#include <boost/asio.hpp>
#include <oneapi/tbb/concurrent_hash_map.h>
#include <oneapi/tbb/concurrent_priority_queue.h>
#include <oneapi/tbb/concurrent_queue.h>
#include <oneapi/tbb/concurrent_unordered_map.h>
#include <oneapi/tbb/concurrent_unordered_set.h>
#include <oneapi/tbb/concurrent_map.h>
#include <oneapi/tbb/concurrent_vector.h>
#include <oneapi/tbb/concurrent_set.h>
#include <oneapi/tbb/mutex.h>
#include <mutex>
#include <thread>
#include <string>
#include <array>
#include <concepts>
#include <chrono>
#include "PthMath/PthMath.h"
#include<concepts>

#include "CoreConcept.h"
#include "CoreTypedef.h"
#include "SmartPointer.h"

#include "CoreFunction.h"
#include "CorePacket.h"
#include "CoreEnum.h"

#include "CoreMacro.h"
#include "CoreTls.h"

#include "UCoreGrobal.h"
#include "Lock.h"
#include "LockFree.h"


namespace Asio = boost::asio;

namespace Core{ }


#endif 