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
	std::map<int, p_array> dictionary_bumperHatPreset{
	  { Buttons::BUMPER_LEFT,  { AMPLITUDE_0,   AMPLITUDE_0,   PHASE_0,   AMPLITUDE_0,   AMPLITUDE_0,   PHASE_0   } },
	  { Buttons::BUMPER_RIGHT, { 0.4f,          0.2f,          PHASE_0,   0.35f,         0.2f,          PHASE_50 } },
	  { Buttons::HAT_UP,       { 0.4f,          0.2f,          PHASE_0,   0.5f,          AMPLITUDE_0,   PHASE_50 } },
	  { Buttons::HAT_DOWN,     { 0.4f,          0.2f,          PHASE_140, 0.5f,          AMPLITUDE_0,   PHASE_50 } },
	  { Buttons::HAT_RIGHT,    { 0.75f,         AMPLITUDE_0,   PHASE_0,   0.35f,         0.2f,          PHASE_50 } },
	  { Buttons::HAT_LEFT,     { AMPLITUDE_0,   0.35f,         PHASE_0,   0.35f,         0.2f,          PHASE_50 } }
	};
	// Button combinations - each button has 2 presets (up/down or left/right variants)
	std::map<int, pair_p_array> dictionary_buttonPreset{
		{ Buttons::X, {
			{ 0.4f, 0.2f, PHASE_140, AMPLITUDE_0, 0.35f, PHASE_50 },  // up‐X
			{ 0.4f, 0.2f, PHASE_0,   AMPLITUDE_0, 0.35f, PHASE_50 }   // down‐X
		  }
		},
		{ Buttons::B, {
			{ 0.4f, 0.2f, PHASE_0,   0.5f,AMPLITUDE_0, PHASE_50 },  // up‐B
			{ 0.4f, 0.2f, PHASE_140, 0.5f,AMPLITUDE_0, PHASE_50 }   // down‐B
		  }
		},
		{ Buttons::Y, {
			{ AMPLITUDE_0, 0.35f, PHASE_140, 0.35f, 0.2f, PHASE_50 },   // right‐Y
			{ 0.75f,      AMPLITUDE_0, PHASE_140, 0.35f, 0.2f, PHASE_50 }  // left‐Y
		  }
		},
		{ Buttons::A, {
			{ 0.75f,      AMPLITUDE_0, PHASE_0, 0.35f, 0.2f, PHASE_50 },  // right‐A
			{ AMPLITUDE_0, 0.35f,      PHASE_0, 0.35f, 0.2f, PHASE_50 }   // left‐A
		  }
		}
	};
	p_array currentPreset = dictionary_bumperHatPreset.at(Buttons::BUMPER_LEFT);
	p_array previousPresetUsed = dictionary_bumperHatPreset.at(Buttons::BUMPER_LEFT);
	p_array nextPreset = {};


public:
	waveGnPresets(){}

	const p_array& get_currentPreset()const {
		return currentPreset;
	}
	const p_array& get_previousPresetUsed()const {
		return previousPresetUsed;
	}
	const p_array& get_nextPreset()const {
		return nextPreset;
	}
	void set_previousPresetUsed(const p_array preset) {
		previousPresetUsed = preset;
	}

	//void set_currentPreset(const int& button_value) {
	//	currentPreset = get_preset(button_value);
	//}
	void set_currentPreset(const p_array preset) {
		currentPreset = preset;
	}


	void set_nextPreset(const int& button_value){


		static const p_array& hat_up_preset = dictionary_bumperHatPreset.at(Buttons::HAT_UP);
		static const p_array& hat_left_preset = dictionary_bumperHatPreset.at(Buttons::HAT_LEFT);

		if (Controller::isButton(button_value)) {
			bool isUp_or_left = (currentPreset == hat_up_preset) || (currentPreset == hat_left_preset);
			const std::pair<p_array, p_array>& preset_pair = dictionary_buttonPreset.at(button_value);
			nextPreset = isUp_or_left ? preset_pair.first : preset_pair.second;
			return;

		}
		nextPreset= dictionary_bumperHatPreset.at(button_value);

	}



	void update_currentAndPreviousPreset() {
		set_previousPresetUsed(currentPreset);
		set_currentPreset(nextPreset);
	}




};

