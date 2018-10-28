#pragma once


#include <string>


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


namespace ootzwin
{


std::string ErrorCodeToString(const DWORD dwErrorCode);


} // ootzwin