#pragma once

#ifdef _TEST
static int REQUEST_ID = 0;

class Request
{
public:
    Request() : m_id(++REQUEST_ID)
    {
#if _DEBUG_LVL > 2
        std::cout << "Request created " << m_id << "\n";
#endif 
    }

    ~Request()
    {
#if _DEBUG_LVL > 2
        std::cout << "Request deleted " << m_id << "\n";
#endif
    }

    Request(const Request&) = default;
    Request& operator=(const Request&) = default;
    Request(Request&&) = default;
    Request& operator=(Request&&) = default;

private:
    const int m_id;
};
#else
#include "External.h"
#endif
