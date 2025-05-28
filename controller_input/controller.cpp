#include "controller.hpp"

std::map<int, std::string> ButtonsMap{
	{ VK_PAD_A,"A"},{VK_PAD_B,"B"},{VK_PAD_X,"X"},{VK_PAD_Y,"Y"},
	{ VK_PAD_DPAD_UP,"hat up" },
	{ VK_PAD_DPAD_DOWN,"hat down" },
	{ VK_PAD_DPAD_LEFT,"hat left" },
	{ VK_PAD_DPAD_RIGHT,"hat right" },

	{ VK_PAD_LSHOULDER,"Right bumper"},
	{ VK_PAD_RSHOULDER,"Left bumper" }

};

bool Controller::validControllerButton(const int& button_value) {
	return WithInInterval(Buttons::A, button_value, Buttons::BUMPER_LEFT) or WithInInterval(Buttons::HAT_UP, button_value, Buttons::HAT_RIGHT)
		or button_value==Buttons::SELECT or button_value==Buttons::START;
}

int Controller::buttonOrHat(const int& button_value) {
	return WithInInterval(Buttons::A, button_value, Buttons::Y) ? BUTTON : HAT;
}

Controller::Controller() :keyStroke{}, state() {

	DWORD dwResult = XInputGetState(dwControllerIndex, &state);// Variable to hold the result of XInput functions

	if (dwResult == ERROR_SUCCESS) {
		std::cout << "Controller is connected." << std::endl;
	}
	else {
		// Controller is not connected
		std::cout << "Controller  is not connected." << std::endl;
	}
}

const int Controller::CheckControllerEvent() {
	DWORD dwReserved = 0;
	DWORD KeyStroke_result = XInputGetKeystroke(dwControllerIndex, dwReserved, (PXINPUT_KEYSTROKE)&keyStroke);

	return  (keyStroke.Flags == XINPUT_KEYSTROKE_KEYDOWN && validControllerButton(keyStroke.VirtualKey)) ? keyStroke.VirtualKey : -1;
	
	

}



Controller::~Controller() {
	std::cout << "end" << std::endl;
}