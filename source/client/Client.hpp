#ifndef _HIBISCUS_CLIENT_HPP_
#define _HIBISCUS_CLIENT_HPP_

#include "Hibicus_definitions.hpp"

#include <cstdint>
#include <string>

namespace hibiscus {

class Client {
public:
    explicit Client();
    ~Client();

    void setSettings(const char* port, const char* source);
    std::string invoke();

private:
    const char* m_port;
    const char* m_source;
};

} // namespace hibiscus

#endif // !_HIBISCUS_CLIENT_HPP_