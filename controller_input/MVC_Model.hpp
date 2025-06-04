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

    
    p_array nextPreset;
    p_array currentPreset;
    

public:
    MVC_Model() : SignalGn(IP_PRIMARY, IP_SECONDARY),GnPresets(),controller() {
        currentPreset = GnPresets.get_currentPreset();
        nextPreset = GnPresets.get_currentPreset();
    }
    ~MVC_Model(){}




    void get_and_applyPreset(const int& button_value) {
        GnPresets.set_nextPreset(button_value);

        nextPreset = GnPresets.get_nextPreset();

        currentPreset = GnPresets.get_currentPreset();

        for (auto& v : nextPreset)    std::cout << ' ' << v;
        std::cout << "\n";


        for (auto& v : currentPreset) std::cout << ' ' << v;
        std::cout << "\n";

        if (SignalGn.apply_preset_values(nextPreset, currentPreset)) {

            GnPresets.update_currentAndPreviousPreset();
            std::pair<const int&, bool> pairToSend = { button_value,Controller::isButton(button_value) ? false : true };
            notifyObserver(pairToSend);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

    //// true : primary(HAT pressed ), false : secondary(button pressed)
    //void notifyController(const int& button_value) {
    //    std::pair<const int&, bool> pairToSend = { button_value,Controller::isButton(button_value) ? false : true };
    //    Observable::notifyObserver(pairToSend);

    //}







};
