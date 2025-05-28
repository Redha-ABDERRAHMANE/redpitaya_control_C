#include "scpi.hpp"
#include <array>
#include <iostream>
#include "waveGnPresets.hpp"
#include "RpSignalGn.hpp"
#include "controller.hpp"

int main() {
    using p_array = std::array<float, 6>;

    const char* IP_PRIMARY = "169.254.112.159"; // Master board
    const char* IP_SECONDARY = "169.254.9.76";     // Slave board

    Controller j;
    std::cout << "-1" << std::endl;
    RpSignalGn SignalGn(IP_PRIMARY, IP_SECONDARY);
    std::cout << "0" << std::endl;
    waveGnPresets p;

    int button_value;
    p_array nextPreset;
    p_array currentPreset;
    std::cout << "1" << std::endl;


    while (true) {
        
        button_value = j.CheckControllerEvent();
        if(button_value !=-1){
            std::cout << "2" << std::endl;
                nextPreset = p.get_preset(button_value);
            for (auto& v : nextPreset)    std::cout << ' ' << v;
            std::cout << "\n";
            for (auto& v : currentPreset) std::cout << ' ' << v;
            std::cout << "\n";

            if (SignalGn.apply_preset_values(nextPreset, currentPreset)) { 
                std::cout << "3" << std::endl;
                p.update_currentAndPreviousPreset(currentPreset, nextPreset);
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }


            
            
        }

        
        
    }
}
