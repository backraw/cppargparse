#ifndef CPPARGPARSE_TYPE_TRAITS_STRING_H
#define CPPARGPARSE_TYPE_TRAITS_STRING_H

#include <algorithm>
#include <sstream>
#include <string>

#include <cppargparse/globals.h>
#include <cppargparse/parser/errors.h>

#include "stub.h"


namespace cppargparse::types {


template <>
/**
 * @brief The type_trait struct for the std::string type.
 */
struct type_trait<std::string>
{
    /**
     * @brief Try to parse and return the std::string value for an argument. Throw a #parser::ParserException on failure.
     *
     * @param key_it The argument key iterator.
     * @param default_value The default std::string value.
     *
     * @return The parsed std::string value for an argument on success or throw a #parser::ParserException on failure.
     */
    static const std::string parse(const ArgumentList_t::const_iterator &key_it)
    {
        auto value_it = std::next(key_it);

        if (value_it == g_args.cend())
        {
            throw parser::errors::ParserError(error_message(value_it));
        }

        return *value_it;
    }

    /**
     * @brief Generate an error message for a value that's not a std::string.
     *
     * @param value_it The argument value iterator.
     *
     * @return An error message for a value that's not a std::string.
     */
    static const char *error_message(const ArgumentList_t::const_iterator &key_it)
    {
        std::ostringstream message;
        message << "Couldn't find a value for argument: " << *key_it;

        return message.str().c_str();
    }
};


#endif // CPPARGPARSE_TYPE_TRAITS_STRING_H

} // namespace cppargparse::types
