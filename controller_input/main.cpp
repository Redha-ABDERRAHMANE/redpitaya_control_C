#include"scpi.hpp"
#include <array>
#include <iostream>
#include "waveGnPresets.hpp"
#include "RpSignalGn.hpp"
#include "controller.hpp"
int main() {
	const char* IP_PRIMARY = "169.254.112.159"; //Master board : rp - f0cafe.local
	const char* IP_SECONDARY = "169.254.9.76"; //slave board 
	std::array<float, 3> a = { 1,2,3.5 };
	//for (float& x : a) { std::cout << (float)x << std::endl; }
	//std::cout << "yolo0" << std::endl;
	//scpi s("169.254.112.159");
	//std::cout << "yolo1" << std::endl;
	Controller j;
	//std::cout << "yolo2" << std::endl;
	/*std::string stringToSend = "";*/
	//std::cout << "yolo3" << std::endl;
	//RpSignalGn rp(IP_PRIMARY, IP_SECONDARY);

	//rp.ramp_up_and_down(rp.rp_boards.rp_primary, 1, 1, 0.5, false);

	//while (true) {

	//	 
	//	if (Buttons::A== j.CheckControllerEvent()) {
	//		stringToSend = "SOUR1:VOLT 0";
	//		auto response =s.tx_txt(stringToSend);
	//		std::this_thread::sleep_for(std::chrono::milliseconds(400));
	//		stringToSend = "SOUR1:VOLT?";
	//		std::cout << "response" << s.txrx_txt(stringToSend) <<std::endl;

	//	}
	//}
	
	RpSignalGn SignalGn("169.254.112.159", "169.254.9.76");
	waveGnPresets p;
	int button_value;
	while (true) {
		button_value = j.CheckControllerEvent();

		if (button_value != -1) {
			p.update_currentPreviousPreset(button_value);
			auto preset = p.get_currentPreset();
			auto previousPreset = p.get_previousPresetUsed();
	



			SignalGn.apply_preset_values(preset,previousPreset);


		}
	}

	








	std::this_thread::sleep_for(std::chrono::milliseconds(500));

		 



}
