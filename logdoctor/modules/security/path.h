#ifndef LOGDOCTOR__MODULES__SECURITY__PATH_H
#define LOGDOCTOR__MODULES__SECURITY__PATH_H


#include <QString>

#include <expected>
#include <filesystem>
#include <vector>


using path_t = std::filesystem::path;


struct InvalidPath;


inline QString toQString(const path_t& path)
{
    #if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
        return QString( path.native().c_str() );
    #else
        return QString( path.c_str() );
    #endif
}


//! PathHandler
/*!
    Handles a path, enforcing checks when access to the resource is requested
*/
class PathHandler final
{
    path_t path;
    std::vector<path_t> components;

public:
    PathHandler() = default;
    PathHandler( PathHandler&& ) = default;
    PathHandler( const PathHandler& ) = default;
    PathHandler& operator=( PathHandler&& ) = default;
    PathHandler& operator=( const PathHandler& ) = default;

    PathHandler( path_t&& path_ );
    PathHandler( const std::string& path_ );
    PathHandler( std::string&& path_ );

    PathHandler operator/( const std::string& path_ ) const;

    //! Convenience function that returns the path as string, without checks
    std::string toString() const;

    //! Returns the path if it is valid
    /*!
        Checks every component of the path to determine whether it is valid.
        For a path to be valid, all of its components must exist on the filesystem
        and they must follow the security configuration about the symlinks.
    */
    std::expected<path_t, InvalidPath> getPath() const;

    //! Convenience function that just returns the path, skipping all the checks
    const path_t& getPathUnchecked() const;

    //! Convenience function that just returns the parent path, skipping all the checks
    path_t getParentUnchecked() const;
};


struct InvalidPath final
{
    enum class Reason {
        NotExists,
        IsSymlink,
    };

    const path_t full_path;
    const path_t invalid_component;
    const Reason reason;

    InvalidPath( const path_t& p, const path_t& c, const Reason r )
    : full_path{p}, invalid_component{c}, reason{r} {}

    bool isReasonExistence() const
    { return reason == Reason::NotExists; }

    bool isReasonSymlink() const
    { return reason == Reason::IsSymlink; }

    void showDialogMessage() const;
};


#endif // LOGDOCTOR__MODULES__SECURITY__PATH_H
