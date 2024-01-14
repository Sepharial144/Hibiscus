#ifndef _HIBISCUS_DOCUMENT_HPP_
#define _HIBISCUS_DOCUMENT_HPP_

#include "Hibicus_definitions.hpp"
#include "Request.hpp"

#ifdef __cplusplus
extern "C" {
#endif

#include <lexbor/css/css.h>
#include <lexbor/html/html.h>
#include <lexbor/selectors/selectors.h>

#ifdef __cplusplus
} /* extern "C" */
#endif

#include <functional>
#include <stdio.h>

namespace hibiscus {

enum parserParam {
    none,
    tag,
    children,
    value
};

template <typename T>
struct Callback;

template <typename Ret, typename... Params>
struct Callback<Ret(Params...)> {
    template <typename... Args>
    static Ret callback(Args... args)
    {
        return func(args...);
    }
    static std::function<Ret(Params...)> func;
};

template <typename Ret, typename... Params>
std::function<Ret(Params...)> Callback<Ret(Params...)>::func;

typedef lxb_status_t (*callback_t)(lxb_dom_node_t*, lxb_css_selector_specificity_t*, void*);

static const char* s_pValue;

class Document {
public:
    explicit Document();
    ~Document();

    auto get() const -> const char*;
    auto select(const char* selector, const Request& request, parserParam param) -> const char*;

private:
    static auto get_tag(lxb_dom_node_t* node, lxb_css_selector_specificity_t* spec, void* ctx) -> lxb_status_t;
    static auto get_children(lxb_dom_node_t* node, lxb_css_selector_specificity_t* spec, void* ctx) -> lxb_status_t;
    static auto get_value(lxb_dom_node_t* node, lxb_css_selector_specificity_t* spec, void* ctx) -> lxb_status_t;
    static auto value(const lxb_char_t* data, size_t len, void* ctx) -> lxb_status_t;

private:
    // TODO fix static s_pValue, save value in m_value;
    //const char* m_value = nullptr;
    lxb_html_document_t* m_document = nullptr;
    lxb_dom_node_t* m_body = nullptr;
    lxb_css_parser_t* m_cssParser = nullptr;
    lxb_selectors_t* m_selectors = nullptr;
    lxb_css_selector_list_t* m_cssDocumentList = nullptr;
};

} // namespace hibiscus

#endif // !_HIBISCUS_DOCUMENT_HPP_