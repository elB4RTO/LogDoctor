#ifndef VECOPS_H
#define VECOPS_H

#include <string>
#include <vector>

class VecOps
{
public:
    VecOps();

    static bool contains( std::vector<std::string> list, std::string flag );
};

#endif // VECOPS_H
