#include <array>
#include <iostream>
#include "MVC_Model.hpp"





int main() {
    MVC_Model model;
    char input = '\0';
    std::map<char, int> m{
        {'u',22544},
        {'d',22545},
        {'l',22546},
        {'r',22547}
    };


    do {
        /*std::cout << "enter l or r or u or d" << std::endl;
        scanf_s(" %c", &input, 1);
        std::cout << m.at(input) << std::endl;
        model.get_and_applyPreset(m.at(input));*/

        model.check_newInput();

    } while (true);
}
