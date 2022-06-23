#include "io.h"

IOutils::IOutils()
{
    
}


auto IOutils::readFile( std::string path ) -> std::string
{
    
    constexpr std::size_t read_size = std::size_t(4096);
    std::ifstream file = std::ifstream(path.data());
    
    file.exceptions(std::ios_base::badbit);
    
    std::string content = std::string();
    std::string buf = std::string(read_size, '\0');
    while (file.read(& buf[0], read_size)) {
        content.append(buf, 0, file.gcount());
    }
    content.append(buf, 0, file.gcount());
    file.close();
    return content;
}
