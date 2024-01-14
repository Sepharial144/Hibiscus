#include "Client.hpp"

namespace hibiscus {

Client::Client()
{
}

Client::~Client()
{
}

void Client::setSettings(const char* port, const char* source)
{
    m_port = port;
    m_source = source;
}

std::string Client::invoke()
{
    return std::string { "invoke" };
}

} // namespace hibiscus