#include"scpi.hpp"
#include <array>
#include <iostream>
#include "waveGnPresets.hpp"
int main() {
	std::array<float, 3> a = { 1,2,3.5 };
	//for (float& x : a) { std::cout << (float)x << std::endl; }
	//std::cout << "yolo0" << std::endl;
	//scpi s("127.0.0.1");
	//std::cout << "yolo1" << std::endl;
	//Controller j;
	//std::cout << "yolo2" << std::endl;
	//std::string stringToSend = "";
	//std::cout << "yolo3" << std::endl;
	
	//while (true) {

	//	 stringToSend= std::to_string( j.CheckControllerEvent());
	//	if (stringToSend!= "-1") {
	//		auto response =s.txrx_txt(stringToSend);
	//		std::cout << response << std::endl;

	//	}
	//}
	//waveGnPresets p;
	//int button_value;
	//while (true) {
	//	button_value = j.CheckControllerEvent();

	//	if (button_value != -1) {
	//		auto preset = p.get_preset(button_value);
	//		for (float& x : preset) {
	//			std::cout << " " << x << " ";
	//		}
	//		std::cout<< std::endl;

	//	}
	//}


		 



}
