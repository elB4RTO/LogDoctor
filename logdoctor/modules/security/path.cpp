
#include "path.h"

#include "globals/security.h"

#include "modules/exceptions.h"
#include "modules/dialogs.h"


using reason_t = InvalidPath::Reason;
using components_t = std::vector<path_t>;
using invalid_t = std::optional<std::tuple<path_t,reason_t>>;


namespace /*private*/
{

path_t makeAbsolute( path_t&& path )
{
    return std::filesystem::absolute( path );
}

components_t getComponents( const path_t& path )
{
    path_t path_slice;
    components_t components;
    for ( const auto& component : path ) {
        path_slice /= component;
        components.push_back( path_slice );
    }
    return components;
}

invalid_t checkIsValid( const components_t& components )
{
    const bool symlink_denied{ ! GlobalConfigs::Security::follow_symlinks };

    for ( const path_t& component : components ) {
        if ( ! std::filesystem::exists( component ) ) {
            return std::make_tuple( component, reason_t::NotExists );
        } else if ( symlink_denied && std::filesystem::is_symlink( component ) ) {
            return std::make_tuple( component, reason_t::IsSymlink );
        }
    }
    return std::nullopt;
}

} // namespace (private)


PathHandler::PathHandler( path_t&& path_ )
    : path{std::move(path_)}, components{getComponents(this->path)}
{
}

PathHandler::PathHandler( const std::string& path_ )
    : path{makeAbsolute(path_)}, components{getComponents(this->path)}
{
}

PathHandler::PathHandler( std::string&& path_ )
    : path{makeAbsolute(std::move(path_))}, components{getComponents(this->path)}
{
}

PathHandler PathHandler::operator/( const std::string& path_ ) const
{
    return PathHandler( this->path / path_ );
}

std::string PathHandler::toString() const
{
    return this->path.string();
}

std::expected<path_t, InvalidPath> PathHandler::getPath() const
{
    const invalid_t invalid_reason{ checkIsValid( this->components ) };
    if ( invalid_reason.has_value() ) {
        const auto [c,r]{ invalid_reason.value() };
        return std::unexpected(InvalidPath( this->path, c, r ));
    }
    return this->path;
}

const path_t& PathHandler::getPathUnchecked() const
{
    return this->path;
}

path_t PathHandler::getParentUnchecked() const
{
    return this->path.parent_path();
}


void InvalidPath::showDialogMessage() const
{
    const QString fp( toQString( this->full_path ) );
    const QString ic( toQString( this->invalid_component ) );
    switch ( this->reason ) {
        case Reason::NotExists:
            DialogSec::errPathNotExists( fp, ic );
            break;
        case Reason::IsSymlink:
            DialogSec::errPathHasSymlink( fp, ic );
            break;
    }
}
