#pragma once
#include "RpSignalGn.hpp"
#include "waveGnPresets.hpp"
//#include "controller.hpp"
#include "Observable.hpp"


class MVC_Model: public Observable<std::pair<const int&,bool>>
{
    using p_array = std::array<float, 6>;

private:

    const char* IP_PRIMARY = "169.254.112.159"; // Master board
    const char* IP_SECONDARY = "169.254.9.76";     // Slave board
    RpSignalGn SignalGn;
    waveGnPresets GnPresets;
    Controller controller;

    int button_value;
    p_array nextPreset;
    p_array currentPreset;
    

public:
    MVC_Model() : SignalGn(IP_PRIMARY, IP_SECONDARY), controller(), GnPresets() {
        currentPreset = GnPresets.get_currentPreset();
        nextPreset = GnPresets.get_currentPreset();
    }
    ~MVC_Model(){}

    void check_newInput() {
        while (true) {
            button_value = controller.CheckControllerEvent();
            if (button_value == -1) { continue; }
            get_and_applyPreset(button_value);
        }

    }


    void get_and_applyPreset(const int& button_value) {
        std::cout << "entered" << std::endl;
        GnPresets.set_nextPreset(button_value);
        std::cout << "entered" << std::endl;
        nextPreset = GnPresets.get_nextPreset();
        std::cout << "entered" << std::endl;
        currentPreset = GnPresets.get_currentPreset();
        std::cout << "entered" << std::endl;

        for (auto& v : nextPreset)    std::cout << ' ' << v;
        std::cout << "\n";

        std::cout << "entered" << std::endl;

        for (auto& v : currentPreset) std::cout << ' ' << v;
        std::cout << "\n";

        std::cout << "entered" << std::endl;
        if (SignalGn.apply_preset_values(nextPreset, currentPreset)) {
            std::cout << "entered" << std::endl;
            GnPresets.update_currentAndPreviousPreset();
            notifyObserver(button_value);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

    // true : primary(HAT pressed ), false : secondary(button pressed)
    void notifyObserver(const int& button_value) {
        std::pair<const int&, bool> pairToSend = { button_value,Controller::isButton(button_value) ? false : true };
        Observable::notifyObserver(pairToSend);

    }







};
