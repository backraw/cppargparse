#ifndef CPPARGPARSE_PARSER_BASE_H
#define CPPARGPARSE_PARSER_BASE_H

#include <algorithm>
#include <sstream>

#include <cppargparse/algorithm.h>
#include <cppargparse/types.h>


namespace cppargparse {
namespace parser {


/**
 * @brief Base class for the argument parser.
 */
class ArgumentParserBase
{
public:
    /**
     * @brief c'tor
     *
     * @param argc The command line argument count.
     * @param argv The command line argument array.
     * @param application_description The application description.
     */
    explicit ArgumentParserBase(int argc, char *argv[], const std::string &application_description)
        : m_cmd(types::CommandLine_t(argv, argv + argc))
        , m_cmdargs()
        , m_posargs()
        , m_application_description(application_description)
    {
    }

    /**
     * @brief d'tor
     *
     * Clean up storage for the command line and the command line argument map.
     */
    ~ArgumentParserBase()
    {
        m_cmd.clear();
        m_cmdargs.clear();
    }

    /**
     * @brief Add a positional argument which is neither a flag nor an option.
     *
     * The argument position is the command line iterator position
     * next the last command line argument's position (flag/option).
     *
     * If no flag/option argument has been stored yet, the position
     * will be the first command line iterator position.
     *
     * @param description The positional argument description.
     *
     * @return The generated command line argument.
     */
    const types::CommandLineArgument_t add_posarg(const std::string &description)
    {
        const types::CommandLineArgument_t arg {
            std::string(), std::string(), description,
            (m_cmdargs.size() > 0) ? std::next(std::prev(m_cmdargs.cend())->position) : m_cmd.cbegin()
        };

        m_posargs.emplace_back(arg);
        return arg;
    }

    /**
     * @brief Add a positional argument which is neither a flag nor an option.
     *
     * The argument position is the command line iterator position
     * next the last command line argument's position (flag/option).
     *
     * If no flag/option argument has been stored yet, the position
     * will be the first command line iterator position.
     *
     * @return The generated command line argument.
     */
    const types::CommandLineArgument_t add_posarg()
    {
        return add_posarg(std::string());
    }

    /**
     * @brief Add an argument to the command line arguments list.
     *
     * @param cmdarg The command line argument struct object.
     */
    void add_arg(const types::CommandLineArgument_t &cmdarg)
    {
        m_cmdargs.emplace_back(cmdarg);
    }

    /**
     * @brief Add an argument to the command line arguments list.
     *
     * @param id The argument ID.
     *
     * @return The generated command line argument.
     */
    const types::CommandLineArgument_t add_arg(const std::string &id)
    {
        const types::CommandLineArgument_t arg {
            id, std::string(), std::string(),
            algorithm::find_arg_position(m_cmd, id, std::string())
        };

        add_arg(arg);
        return arg;
    }

    /**
     * @brief Add an argument to the command line arguments list.
     *
     * @param id The argument ID.
     * @param id_alt The alternative argument ID.
     *
     * @return The generated command line argument.
     */
    const types::CommandLineArgument_t add_arg(const std::string &id, const std::string &id_alt)
    {
        const types::CommandLineArgument_t arg {
            id, id_alt, std::string(),
            algorithm::find_arg_position(m_cmd, id, id_alt)
        };

        add_arg(arg);
        return arg;
    }

    /**
     * @brief Add an argument to the command line arguments list.
     *
     * @param id The argument ID.
     * @param id_alt The alternative argument ID.
     * @param description The argument description.
     *
     * @return The generated command line argument.
     */
    const types::CommandLineArgument_t add_arg(const std::string &id, const std::string &id_alt, const std::string &description)
    {
        const types::CommandLineArgument_t arg {
            id, id_alt, description,
            algorithm::find_arg_position(m_cmd, id, id_alt)
        };

        add_arg(arg);
        return arg;
    }


    /**
     * @brief Add default help argument: -h, --help
     *
     * @return The generated command line argument.
     */
    const types::CommandLineArgument_t add_help()
    {
        return add_arg("-h", "--help", "Display this information");
    }


    template <typename T>
    /**
     * @brief Stub method for returning a positional argument value.
     *
     * @tparam The argument type. Must be non-abstract and have a default constructor.
     *
     * @return A new instance of T.
     */
    inline T get_positional(const types::CommandLineArgument_t &)
    {
        return T();
    }


    /**
     * @brief Return whether the command line contains an argument string.
     *
     * @param cmdarg The command line argument.
     *
     * @return Whether the command line contains an argument string.
     */
    inline bool get_flag(const types::CommandLineArgument_t &cmdarg)
    {
        return algorithm::find_arg_position(m_cmd, cmdarg.id, cmdarg.id_alt) != m_cmd.cend();
    }

    template <typename T>
    /**
     * @brief Stub method for returning an argument value.
     *
     * @tparam T The argument type. Must be non-abstract and have a default constructor.
     *
     * @return A new instance of T.
     */
    inline const T get_option(const types::CommandLineArgument_t &)
    {
        return T();
    }

    template <typename T>
    /**
     * @brief Stub method for returning an argument value.
     *
     * @tparam T The argument type. Is ignored in this case.
     *
     * @param default_value The default value.
     *
     * @return The default value.
     */
    inline const T get_option(const types::CommandLineArgument_t &, const T &default_value)
    {
        return default_value;
    }


    const size_t usage_argid_length() const
    {
        size_t argid_length = 0;

        for (const auto &cmdarg : m_cmdargs)
        {
            const size_t id_length = cmdarg.id.size() + cmdarg.id_alt.size();

            if (id_length > argid_length)
            {
                argid_length = id_length;
            }
        }

        return argid_length + 3;
    }

    const std::ostringstream usage_posargs() const
    {
        std::ostringstream usage_posargs_stream;

        return usage_posargs_stream;
    }

    const std::ostringstream usage_cmdargs() const
    {
        const size_t argid_length = usage_argid_length();


        std::ostringstream usage_cmdargs_stream;

        for (const auto &cmdarg : m_cmdargs)
        {
            usage_cmdargs_stream << ' ' << ' ' << cmdarg.id;

            if (!cmdarg.id_alt.empty())
            {
                usage_cmdargs_stream << '|' << cmdarg.id_alt;
            }

            for (size_t i = 0; i < argid_length; ++i)
            {
                usage_cmdargs_stream << ' ';
            }

            if (!cmdarg.description.empty())
            {
                usage_cmdargs_stream << cmdarg.description;
            }

            usage_cmdargs_stream << '\n';
        }

        return usage_cmdargs_stream;
    }

    const std::ostringstream usage_stream() const
    {
        std::ostringstream stream;
        stream << usage_posargs().str() << usage_cmdargs().str();

        return stream;
    }

    const std::string usage(const std::string &header) const
    {
        std::ostringstream stream;
        stream << header << '\n' << '\n' << usage_stream().str();

        return stream.str();
    }

    const std::string usage(const std::ostringstream &header) const
    {
        return usage(header.str());
    }

    const std::string usage() const
    {
        std::ostringstream stream;
        stream << m_application_description;

        if (m_cmdargs.size() > 0)
        {

        }

        return usage(stream);
    }


protected:
    /// The command line
    types::CommandLine_t m_cmd;

    /// The command line arguments
    types::CommandLineArguments_t m_cmdargs;

    /// The positional command line arguments
    types::CommandLineArguments_t m_posargs;


private:
    /// The application description
    const std::string m_application_description;
};


} // namespace parser
} // namespace cppargparse

#endif // CPPARGPARSE_PARSER_BASE_H
