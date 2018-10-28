#include "helper.h"


namespace ootzwin
{


std::string ErrorCodeToString(const DWORD dwErrorCode)
{
    LPSTR lpMsgBuf = nullptr;
    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dwErrorCode,

        /* TODO : MAKELANGID is deprecated */
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),

        /* caution : function parameter type is LPSTR 
        * but you must use LPSTR* type for argument 
        */
        (LPSTR)&lpMsgBuf, 

        0,  NULL);

    std::string str;
    str += std::to_string(dwErrorCode);
    str += " - ";
    str += lpMsgBuf;

    LocalFree(lpMsgBuf);

    return str;
}


} // namespace ootzwin