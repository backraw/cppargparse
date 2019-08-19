#ifndef CPPARGPARSE_ARGUMENTS_NUMERICAL_H
#define CPPARGPARSE_ARGUMENTS_NUMERICAL_H

#include <functional>
#include <sstream>
#include <string>

#include <cppargparse/errors.h>
#include <cppargparse/types.h>


namespace cppargparse {


template <typename T>
struct numerical_argument
{
    /**
     * @brief Try to convert a command line argument value to a numerical value.
     *
     * @param cmd The command line.
     * @param position The command line argument iterator.
     * @param numerical_converter The function used to convert the cmdarg into a numerical value.
     * @param type_string The numerical type's string representation.
     *
     * @return The numerical value of the command line argument.
     * @throws #cppargparse::errors::CommandLineOptionError if the conversion was unsuccessful.
     */
    static T convert(
            const types::CommandLine_t &cmd,
            const types::CommandLinePosition_t &position,
            const std::function<T(const std::string &)> &numerical_converter,
            const std::string &type_string)
    {
        if (position == cmd.cend())
        {
            throw errors::CommandLineOptionError(error_message(std::prev(position), type_string));
        }

        try
        {
            return numerical_converter(*position);
        }

        catch (std::invalid_argument const &)
        {
            throw errors::CommandLineOptionError(error_message(position, type_string));
        }
    }

    /**
     * @brief Generate an error message for a value that's not a numerical value.
     *
     * @param position The command line argument iterator position.
     * @param type_string The numerical type's string representation.
     *
     * @return An error message for a value that's not convertible to the specified numerical type.
     */
    static const std::string error_message(const types::CommandLinePosition_t &position, const std::string &type_string)
    {
        std::ostringstream message;
        message << "Couldn't convert '" << *position << "' to type <" << type_string << ">.";
        std::string x = message.str();

        return message.str();
    }
};


} // namespace cppargparse::numerical_argument

#endif // CPPARGPARSE_ARGUMENTS_NUMERICAL_H
