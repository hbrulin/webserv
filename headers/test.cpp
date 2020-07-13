#include "head_req.hpp"
#include <string>

int main()
{
    std::cout << "lolo" << std::endl;
    Head_req head;
    std::cout << "lolo" << std::endl;
    std::string s = head.get_meta();
    //char *str = head.get_meta();
    std::cout << "ailleurs" << s << std::endl;
    std::cout << "char: " << s.c_str() << std::endl;
}