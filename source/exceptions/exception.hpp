#ifndef _HIBISCUS_EXCEPTION_HPP_
#define _HIBISCUS_EXCEPTION_HPP_

#include <cstdint>
#include <exception>
#include <string>

/*
 * Exception wrapper
 */
void throw_exception_on(const bool is_error, const char* error);

/*
 * Exception
 */
class exception : public std::exception {
public:
    explicit exception(const char* error);
    explicit exception(const char* error, int32_t status_code);
    virtual ~exception() noexcept;
    virtual const char* what() const noexcept;

    int32_t errorCode() const;

protected:
    std::string m_error;
    int32_t m_errorCode;
};

#endif // !_HIBISCUS_EXCEPTION_HPP_