#include <iostream>

#include "enum_experiment.h"

int main()
{
    Result<int, char> ok_result{10};
    Result<int, std::string> err_result{"Error result"};

    auto value = match(ok_result, 
    [](auto&& k)
    {
        std::cout << "Got OK(int): ";
        return std::get<static_cast<unsigned>(Index::Ok)>(k);
    }, 
    [](auto&& k)
    {
        std::cout << "Got Err: " << std::get<static_cast<unsigned>(Index::Err)>(k) << std::endl;
        return -1;
    });

    std::cout << value << std::endl;

    if_let(err_result, Index::Err, [](auto&& k) {
        std::cout << "Got Err: " << std::get<static_cast<unsigned>(Index::Err)>(k) << std::endl; }
    );

    return 0;
}
