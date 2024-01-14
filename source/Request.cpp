#include "Request.hpp"

namespace hibiscus {

Request::Request()
{
}

Request::~Request()
{
}

auto Request::text() const -> const lxb_char_t*
{
    return (const lxb_char_t*)m_html;
}

size_t Request::size() const
{
    return std::strlen(m_html) - 1;
}

} // namespace hibiscus