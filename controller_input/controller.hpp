#pragma once
#include <Windows.h>
#include <mmsystem.h>
#include <Xinput.h>
#include <iostream>
#include <map>
#include <array>
#include <algorithm>

#pragma comment(lib, "xinput.lib") // or "xinput.lib"
// Add linker XInput.lib

#define WithInInterval(a, b, c) ((a) <= (b) && (b) <= (c))
#define BUTTON	0
#define HAT		1

extern std::map<int, std::string> ButtonsMap;

enum Buttons {
	A = VK_PAD_A, B = VK_PAD_B, X = VK_PAD_X, Y = VK_PAD_Y,

	HAT_UP = VK_PAD_DPAD_UP, HAT_DOWN = VK_PAD_DPAD_DOWN, HAT_LEFT = VK_PAD_DPAD_LEFT, HAT_RIGHT = VK_PAD_DPAD_RIGHT,

	BUMPER_RIGHT = VK_PAD_RSHOULDER, BUMPER_LEFT = VK_PAD_LSHOULDER,

	SELECT= VK_PAD_START, START= VK_PAD_BACK,
	TRIGGER_LEFT = VK_PAD_LTRIGGER

};

// add to compilation option : -lwinmm

class Controller
{

private:
	DWORD dwControllerIndex = 0;

	_XINPUT_KEYSTROKE keyStroke; // Structure to hold key stroke information

	XINPUT_STATE state; // Structure to hold the state of the controller

	bool validControllerButton(const int& button_value);

	// To Use only after checkButtonValue is verified
	// 0: button    | 1 : hat
	int buttonOrHat(const int& button_value);

public:
	Controller();
	const int CheckControllerEvent();
	static bool isButton(const int& button_value) {
		return WithInInterval(Buttons::A, button_value, Buttons::Y);

	}
	~Controller();

};