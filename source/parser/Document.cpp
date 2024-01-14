
#include "Document.hpp"
#include "Request.hpp"
#include "logger/logger.hpp"

namespace hibiscus {

auto callback(const lxb_char_t* data, size_t len, void* ctx) -> lxb_status_t;
auto get_tag(lxb_dom_node_t* node, lxb_css_selector_specificity_t* spec, void* ctx) -> lxb_status_t;
auto get_children(lxb_dom_node_t* node, lxb_css_selector_specificity_t* spec, void* ctx) -> lxb_status_t;
auto get_value(lxb_dom_node_t* node, lxb_css_selector_specificity_t* spec, void* ctx) -> lxb_status_t;

Document::Document()
{
}

Document::~Document()
{
    /* Destroy Documents object. */
    (void)lxb_selectors_destroy(m_selectors, true);

    /* Destroy resources for CSS Parser. */
    (void)lxb_css_parser_destroy(m_cssParser, true);

    /* Destroy all Document List memory. */
    lxb_css_selector_list_destroy_memory(m_cssDocumentList);

    /* Destroy HTML Document. */
    lxb_html_document_destroy(m_document);
}

auto Document::get() const -> const char*
{
    return m_value;
}

const char* Document::select(const char* selector, const Request& request, parserParam param)
{
    lxb_status_t status;
    /*
        const lxb_char_t html[] = "<table>"
                                  "<tbody>"
                                  "<tr>"
                                  "<td header=\"choose-this-header\">0.7</td>"
                                  "<td header=\"ignore-this-header\">1.3</td>"
                                  "<td header=\"ignore-this-header\">5.4</td>"
                                  "</tr>"
                                  "</tbody>"
                                  "</table>";
    */

    static const lxb_char_t slctrs[] = "td[header='choose-this-header']";

    m_document = lxb_html_document_create();
    if (m_document == nullptr) {
        logger::error("Document: could not create html document!");
    }

    // status = lxb_html_document_parse(m_document, html, sizeof(html) - 1);
    status = lxb_html_document_parse(m_document, request.text(), request.size());
    if (status != LXB_STATUS_OK) {
        logger::error("Document: parse document filed!");
    }

    /* Create CSS parser. */
    m_cssParser = lxb_css_parser_create();
    status = lxb_css_parser_init(m_cssParser, NULL);
    if (status != LXB_STATUS_OK) {
        logger::error("Document: css parser init failed!");
    }

    /* Documents. */

    m_selectors = lxb_selectors_create();
    status = lxb_selectors_init(m_selectors);
    if (status != LXB_STATUS_OK) {
        logger::error("Document: selector init failed!");
    }

    m_cssDocumentList = lxb_css_selectors_parse(m_cssParser, slctrs, sizeof(slctrs) - 1);
    if (m_cssParser->status != LXB_STATUS_OK) {
        logger::error("Document: css selector parse failed!");
    }

    /* Find DOM/HTML nodes by selectors. */

    lxb_dom_node_t* body;
    body = lxb_dom_interface_node(lxb_html_document_body_element(m_document));
    if (body == nullptr) {
        logger::error("Document: dom interface node failed!");
    }

    // TODO: create status exception for every function
    //    lxb_html_serialize_deep_cb(body, callback, NULL);
    if (param == parserParam::tag) {
        status = lxb_selectors_find(m_selectors, body, m_cssDocumentList, (lxb_selectors_cb_f)get_tag, nullptr);
    }

    if (param == parserParam::children) {
        status = lxb_selectors_find(m_selectors, body, m_cssDocumentList, (lxb_selectors_cb_f)get_children, nullptr);
    }

    if (param == parserParam::value) {
        status = lxb_selectors_find(m_selectors, body, m_cssDocumentList, (lxb_selectors_cb_f)get_value, nullptr);
    }

    if (status != LXB_STATUS_OK) {
        logger::error("Document: selector find failed!");
    }

    return s_pValue;
}

auto Document::value(const lxb_char_t* data, size_t len, void* ctx) -> lxb_status_t
{
    printf("%.*s", (int)len, (const char*)data);
    s_pValue = (const char*)data;
    return LXB_STATUS_OK;
}

auto Document::get_tag(lxb_dom_node_t* node, lxb_css_selector_specificity_t* spec, void* ctx) -> lxb_status_t
{
    (void)lxb_html_serialize_cb(node, value, NULL);
    return LXB_STATUS_OK;
}

auto Document::get_children(lxb_dom_node_t* node, lxb_css_selector_specificity_t* spec, void* ctx) -> lxb_status_t
{
    (void)lxb_html_serialize_tree_cb(node, value, nullptr);
    return LXB_STATUS_OK;
}

auto Document::get_value(lxb_dom_node_t* node, lxb_css_selector_specificity_t* spec, void* ctx) -> lxb_status_t
{
    (void)lxb_html_serialize_deep_cb(node, value, nullptr);
    return LXB_STATUS_OK;
}

} // namespace hibiscus