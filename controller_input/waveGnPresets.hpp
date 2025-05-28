#pragma once 
#include<map>
#include<algorithm>
#include <utility>
#include <array>
#include "controller.hpp"

#define AMPLITUDE_0 0.0f
#define AMPLITUDE_MAX 1.0f

#define FREQUENCY_0 0
#define FREQUENCY_MAX 1000



#define PHASE_0		0
#define PHASE_50	50
#define PHASE_90	90
#define PHASE_140	140
#define PHASE_180	180



//#define SOURCE_1 1
//#define SOURCE_2 2

//#define DELAY 0.3f
//#define STEPS 20
#define check_Equals(val, x, y) ((val) == (x) || (val) == (y))


class waveGnPresets
{
using p_array = std::array<float, 6>;
using pair_p_array = std::pair<p_array, p_array>;

private:
	std::map<int, p_array> dictionary_bumperHatPreset;
	std::map<int,pair_p_array> dictionary_buttonPreset;
	p_array currentPreset = {};
	p_array previousPresetUsed = {};

public:
	waveGnPresets(const float& a=1.0f, const float& b=1.0f, const float& c = 1.0f, const float& d = 1.0f){





		dictionary_bumperHatPreset[Buttons::BUMPER_LEFT] = { AMPLITUDE_0, AMPLITUDE_0 ,PHASE_0 ,AMPLITUDE_0 ,AMPLITUDE_0 ,PHASE_0 };
		dictionary_bumperHatPreset[Buttons::BUMPER_RIGHT] = { AMPLITUDE_MAX,AMPLITUDE_MAX,PHASE_0,AMPLITUDE_MAX,AMPLITUDE_MAX,PHASE_50 };

		dictionary_bumperHatPreset[Buttons::HAT_UP] = { a,a,PHASE_0,c,AMPLITUDE_0,PHASE_50 };
		dictionary_bumperHatPreset[Buttons::HAT_DOWN] = { a,a,PHASE_140,c,AMPLITUDE_0,PHASE_50 };

		dictionary_bumperHatPreset[Buttons::HAT_LEFT] = { AMPLITUDE_0,d,PHASE_0,b,b,PHASE_50 };
		dictionary_bumperHatPreset[Buttons::HAT_RIGHT] = { d,AMPLITUDE_0,PHASE_0,b,b,PHASE_50 };

		//TODO 
		/*
		Make sure to put a pointer for the shared presets between the two dictionnaries to avoid unnecessary copies
		*/

		// Button combinations - each button has 2 presets (up/down or left/right variants)
		dictionary_buttonPreset[Buttons::X] = {
			{ a,a,PHASE_140,AMPLITUDE_0,c,PHASE_50 },  // preset_hat_up_button_x
			{ a,a,PHASE_0,AMPLITUDE_0,c,PHASE_50 }     // preset_hat_down_button_x  
		};

		dictionary_buttonPreset[Buttons::B] = {
			{ a,a,PHASE_0,c,AMPLITUDE_0,PHASE_50 },    // preset_hat_up_button_b
			{ a,a,PHASE_140,c,AMPLITUDE_0,PHASE_50 }   // preset_hat_down_button_b
		};

		dictionary_buttonPreset[Buttons::Y] = {
			{ d,AMPLITUDE_0,PHASE_140,b,b,PHASE_50 },  // preset_hat_left_button_y
			{ AMPLITUDE_0,d,PHASE_140,b,b,PHASE_50 }   // preset_hat_right_button_y
		};

		dictionary_buttonPreset[Buttons::A] = {
			{ AMPLITUDE_0,d,PHASE_0,b,b,PHASE_50 },    // preset_hat_left_button_a
			{ d,AMPLITUDE_0,PHASE_0,b,b,PHASE_50 }     // preset_hat_right_button_a
		};

		currentPreset = get_preset(Buttons::BUMPER_LEFT);
		previousPresetUsed = currentPreset;
	}

	const p_array& get_currentPreset()const {
		return currentPreset;
	}
	const p_array& get_previousPresetUsed()const {
		return previousPresetUsed;
	}
	void set_previousPresetUsed(const p_array preset) {
		previousPresetUsed = preset;
	}

	void set_currentPreset(const int& button_value) {
		currentPreset = get_preset(button_value);
	}
	void set_currentPreset(const p_array preset) {
		currentPreset = preset;
	}






	const p_array& get_preset(const int& button_value){

		if (Controller::isButton(button_value) && check_lastPresetCoherence(button_value)==-1) { return currentPreset; }

		static const p_array& hat_up_preset = dictionary_bumperHatPreset.at(Buttons::HAT_UP);
		static const p_array& hat_left_preset = dictionary_bumperHatPreset.at(Buttons::HAT_LEFT);
		bool isUp_or_left = (currentPreset == hat_up_preset) || (currentPreset == hat_left_preset);


		if (WithInInterval(Buttons::A, button_value, Buttons::Y)) {
			const std::pair<p_array, p_array>& preset_pair = dictionary_buttonPreset.at(button_value);
			return isUp_or_left ? preset_pair.first : preset_pair.second;

		}
		return dictionary_bumperHatPreset.at(button_value);

	}

	int check_lastPresetCoherence(const int& button_value) const {
		static const p_array& hat_up_preset = dictionary_bumperHatPreset.at(Buttons::HAT_UP);
		static const p_array& hat_down_preset = dictionary_bumperHatPreset.at(Buttons::HAT_DOWN);
		static const p_array& hat_left_preset = dictionary_bumperHatPreset.at(Buttons::HAT_LEFT);
		static const p_array& hat_right_preset = dictionary_bumperHatPreset.at(Buttons::HAT_RIGHT);


		if (currentPreset == hat_up_preset || currentPreset == hat_down_preset) {
			return check_Equals(button_value, Buttons::B, Buttons::X);
		}
		if (currentPreset== hat_left_preset || currentPreset== hat_right_preset) {
			return check_Equals(button_value, Buttons::A, Buttons::Y);
		}
		return -1;
	}




};

