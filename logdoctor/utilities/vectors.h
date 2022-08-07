#ifndef VECOPS_H
#define VECOPS_H

#include <string>
#include <vector>

class VecOps
{
public:
    VecOps();

    static const bool contains( const std::vector<int>& list, const int& flag );
    static const bool contains( const std::vector<std::string>& list, const std::string& flag );
};

#endif // VECOPS_H
