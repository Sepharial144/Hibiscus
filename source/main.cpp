#include <iostream>

#include "FileObserver.hpp"
#include "Request.hpp"
#include "logger/logger.hpp"
#include "parser/Document.hpp"

#ifdef __cplusplus
extern "C" {
#endif

#include <lexbor/css/css.h>
#include <lexbor/html/html.h>
#include <lexbor/selectors/selectors.h>

lxb_status_t
callback(const lxb_char_t* data, size_t len, void* ctx)
{
    printf("%.*s", (int)len, (const char*)data);

    return LXB_STATUS_OK;
}

lxb_status_t
find_callback(lxb_dom_node_t* node, lxb_css_selector_specificity_t* spec,
    void* ctx)
{
    printf("Tag:\n");

    /* Print only <td> tag. */
    (void)lxb_html_serialize_cb(node, callback, NULL);

    printf("\n\nTag with children:\n");

    /* Print <td> element and all children in <td>. */
    (void)lxb_html_serialize_tree_cb(node, callback, NULL);

    printf("\n\nChildren:\n");

    /* Print children in <td>. */
    (void)lxb_html_serialize_deep_cb(node, callback, NULL);

    /* Use lxb_html_serialize_*_str(...) for buffer. */

    return LXB_STATUS_OK;
}

#ifdef __cplusplus
} /* extern "C" */
#endif

int main(int argc, char* argv[])
{
    logger::info("Hibiscus start application ...");
    // FileObserver FileObserver { "/workspaces/Hibiscus/data", 5000 };
    // FileObserver.start();

    hibiscus::Request request;
    hibiscus::Document document;
    const char* value = document.select("td[header='choose-this-header']", request, hibiscus::parserParam::value);
    std::cout << value << std::endl;

    logger::info("Hibiscus start application ... complete");

    return 0;
}