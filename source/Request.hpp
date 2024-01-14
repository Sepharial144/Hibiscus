#ifndef _HIBISCUS_REQUEST_HPP_
#define _HIBISCUS_REQUEST_HPP_

#include "Hibicus_definitions.hpp"

#include <lexbor/css/css.h>
#include <lexbor/html/html.h>
#include <lexbor/selectors/selectors.h>

#include <cstring>

namespace hibiscus {

class Request {
private:
public:
    explicit Request();
    ~Request();
    const lxb_char_t* text() const;
    size_t size() const;

private:
    const char* m_html = "<table>"
                         "<tbody>"
                         "<tr>"
                         "<td header=\"choose-this-header\">0.7</td>"
                         "<td header=\"ignore-this-header\">1.3</td>"
                         "<td header=\"ignore-this-header\">5.4</td>"
                         "</tr>"
                         "</tbody>"
                         "</table>";
};

} // namespace hibiscus

#endif // !_HIBISCUS_REQUEST_HPP_