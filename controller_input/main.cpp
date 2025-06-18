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
    RpSignalGn SignalGn(IP_PRIMARY, IP_SECONDARY);
    waveGnPresets p;

    int button_value;
    p_array nextPreset;
    p_array currentPreset;
    float a, b, c, d, e, f; // for scanf
    int userfrequency;

    while (true) {
        button_value = j.CheckControllerEvent();
        switch (button_value) {

            case Buttons::SELECT: {
                // custom full‐preset entry
                std::cout << " custom preset:\n";
                scanf_s(" %f,%f,%f,%f,%f,%f", &a, &b, &c, &d, &e, &f); 
                nextPreset = { a, b, c, d, e, f };
                currentPreset = p.get_currentPreset();
                


                break;
            }
            case Buttons::TRIGGER_LEFT: {
                std::cout << " custom frequency:\n";
                do { scanf_s(" %d", &userfrequency); } while (!WithInInterval(0, userfrequency, 100));
                SignalGn.apply_frequency_values(userfrequency);
                button_value = -1;
                break;
            }

            case Buttons::START: {
                // custom phase only
                std::cout << " custom phase:\n";
                do { scanf_s(" %f,%f", &a, &b); } while (!WithInInterval(0, a, 180) or !WithInInterval(0, b, 180));
                nextPreset = p.get_currentPreset();
                nextPreset[2] = a;  // primary phase
                nextPreset[5] = b;  // secondary phase
                currentPreset = p.get_currentPreset();


                break;   
            }

            case -1:
                
                break;

            default: {
                
                nextPreset = p.get_preset(button_value);
                currentPreset = p.get_currentPreset();
                break;
            }
        }
        if (button_value != -1) {
            // Debug print
            for (auto& v : nextPreset)    std::cout << ' ' << v;
            std::cout << "\n";
            for (auto& v : currentPreset) std::cout << ' ' << v;
            std::cout << "\n";

            if (SignalGn.apply_preset_values(nextPreset, currentPreset)) {
                p.set_previousPresetUsed(currentPreset);
                p.set_currentPreset(nextPreset);


            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        
        std::cin.ignore(1000, '\n');
    }
}
