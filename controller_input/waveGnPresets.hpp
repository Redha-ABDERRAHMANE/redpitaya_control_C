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
	
	/////////////////////////
	std::map<int, p_array> dictionary_bumperHatPreset{
		{ Buttons::BUMPER_LEFT,  { AMPLITUDE_0,	AMPLITUDE_0	,   PHASE_0,	AMPLITUDE_0	,   AMPLITUDE_0,   PHASE_0   } },
		{ Buttons::BUMPER_RIGHT, { 0.11f,			0.11f	,	PHASE_0,	0.15f		,         0.15f,          PHASE_0 } },
		{ Buttons::HAT_UP,       { 0.11f,			0.11f	,          85,   0.285f,          AMPLITUDE_0,   PHASE_0} },
		{ Buttons::HAT_DOWN,     { 0.11f,			0.11f	,         85	, AMPLITUDE_0,          0.32f,   PHASE_0 } },
		{ Buttons::HAT_RIGHT,    { 0.22f,		AMPLITUDE_0	,			85,   0.15f,         0.15f,          175 } },
		{ Buttons::HAT_LEFT,     {AMPLITUDE_0,   0.21f,         85,   0.15f,         0.15f,          175 } }
	};
	std::map<int,pair_p_array> dictionary_buttonPreset{
		{ Buttons::X, {
			{ 0.11f, 0.11f, 85,0.285f, AMPLITUDE_0, 175 },  // up‐X
			{ 0.11f, 0.11f, 85,   AMPLITUDE_0, 0.32f, 175 }   // down‐X
		  }
		},
		{ Buttons::B, {
			{ 0.11f,0.11f,  85, 0.285f, AMPLITUDE_0,PHASE_0},  // up‐B
			{ 0.11f,0.11f	, 85, AMPLITUDE_0,0.32f,PHASE_0 }   // down‐B
		  }
		},
		{ Buttons::Y, {
			{ 0.22f, AMPLITUDE_0, 85,0.15f, 0.15f, PHASE_0 },  // left‐Y
			{ AMPLITUDE_0, 0.21f, 85,   0.15f, 0.15f, PHASE_0 }   // right‐Y

		  }
		},
		{ Buttons::A, {
			{ AMPLITUDE_0,0.21f	,85, 0.15f,  0.15f, 175 }, // left-A
			{0.22f,  AMPLITUDE_0,  85, 0.15f, 0.15f, 175 },   // right‐A


		  }
		}
	};
	p_array currentPreset = {};
	p_array previousPresetUsed = {};

public:
	waveGnPresets(const float& a=1.0f, const float& b=1.0f, const float& c = 1.0f, const float& d = 1.0f){





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


		if (currentPreset == hat_up_preset || currentPreset == hat_down_preset || previousPresetUsed == hat_up_preset || previousPresetUsed == hat_down_preset) {
			return check_Equals(button_value, Buttons::B, Buttons::X);
		}
		if (currentPreset== hat_left_preset || currentPreset== hat_right_preset || previousPresetUsed == hat_left_preset || previousPresetUsed == hat_right_preset) {
			return check_Equals(button_value, Buttons::A, Buttons::Y);
		}
		return -1;
	}

	void update_currentAndPreviousPreset(const p_array& currentPreset, const p_array& nextPreset) {
		set_previousPresetUsed(currentPreset);
		set_currentPreset(nextPreset);
	}




};

