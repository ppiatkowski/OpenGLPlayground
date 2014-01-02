#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <sstream>

template <typename T> std::string toStr(T tmp)
{
    std::ostringstream out;
    out << tmp;
    return out.str();
}


template <typename T> T strTo(std::string tmp)
{
    T output;
    std::istringstream in(tmp);
    in >> output;
    return output;
}

#endif // _UTILS_H_
