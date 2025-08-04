#include "scpi.hpp"
#include <array>
#include <iostream>
#include "waveGnPresets.hpp"
#include "RpSignalGn.hpp"
#include "controller.hpp"

int main() {

    typedef std::array<float, 9> preset_array_t;
    typedef std::pair<preset_array_t, preset_array_t> pair_p_array_t;
    typedef std::pair<std::map<int, preset_array_t>&, std::map<int, pair_p_array_t>&> pair_dimension_dictionnary_t;

    const char* IP_PRIMARY = "127.0.0.1"; // Master board
    const char* IP_SECONDARY = "169.254.9.76";     // Slave board
    
    Controller j;
    std::array<const char*, SLAVE_BOARDS> arraySlaveBoardIPs = { "127.0.0.1", "127.0.0.1" };
    RpSignalGn signalGn(IP_PRIMARY, arraySlaveBoardIPs);
    waveGnPresets p;

    PressedButton button_value;
    preset_array_t nextPreset;
    preset_array_t currentPreset = { 0,0,0,0,0,0,0,0,0 };
    float a, b, c, d, e, f,g,h,i; // for scanf
    int userfrequency;

    while (true) {
        button_value = j.CheckControllerEvent();
        switch (button_value.button) {

            case Buttons::SELECT: {
                // custom full‐preset entry
                std::cout << " custom preset:\n";
                

                do {
                    int result =scanf_s(" %f,%f,%f,%f,%f,%f,%f,%f,%f", &a, &b, &c, &d, &e, &f, &g, &h, &i);

                } while (!WithInInterval(0, a, 1) or !WithInInterval(0, b, 1) or !WithInInterval(0, c, 180) or !WithInInterval(0, d, 1) or !WithInInterval(0, e, 1) or !WithInInterval(0, f, 180) or !WithInInterval(0, g, 1)
                    or !WithInInterval(0, h, 1) or !WithInInterval(0, i, 180));
                nextPreset = { a, b, c, d, e, f,g,h,i };
   
                std::cout << "VALID PRESET" << std::endl;
                


                break;
            }
            case Buttons::TRIGGER_LEFT: {
                p.SetDimension(Buttons::TRIGGER_LEFT);
                button_value.button = -1;
                    break;

            }
            case Buttons::JOYSTICKPRESS:
                std::cout << " custom frequency:\n";
                do { scanf_s(" %d", &userfrequency); } while (!WithInInterval(0, userfrequency, 1000));
                signalGn.ApplyFrequencyValues(userfrequency);
                button_value.button = -1;
                break;
                

            case Buttons::START: {
                // custom phase only
                std::cout << " custom phase:\n";
                do { scanf_s(" %f,%f", &a, &b,&c); } while (!WithInInterval(0, a, 180) or !WithInInterval(0, b, 180));
                nextPreset = p.GetCurrentPreset();
                currentPreset = p.GetCurrentPreset();
                nextPreset[2] = a;  // primary phase
                nextPreset[5] = b;  // secondary phase
                


                break;   
            }

            case -1:
                
                break;

            default: {
                
                p.SetNextPreset(button_value.button);
                nextPreset = p.GetNextPreset();
                
                break;
            }
        }
        if (button_value.button != -1) {
            // Debug print
            for (auto& v : nextPreset)    std::cout << ' ' << v;
            std::cout << "\n";
            for (auto& v : currentPreset) std::cout << ' ' << v;
            std::cout << "\n";

            if (signalGn.ApplyPresetValues(nextPreset,currentPreset)) {
                std::cout << "PRESET APPLIED" << std::endl;
                currentPreset = nextPreset;

            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        
        //std::cin.ignore(1000, '\n');
    }
}
