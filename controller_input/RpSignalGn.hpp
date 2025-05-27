#pragma once
#include "waveGnPresets.hpp"
#include "scpi.hpp"
#include <array>
#include <thread>
#include <chrono>
#include <ctime>
#include <cmath>
#define SOURCE_1 1
#define SOURCE_2 2

#define DELAY 0.3f
#define STEPS 20
#define lessOrEqualValue(value,x) ((value)<=(x))


#define check_ValueBounds(value,x,y) ((x)<=(value) && (value)<=(y))

class RpSignalGn
{
private:
	Controller controller;
	scpi rp_primary;
	scpi rp_secondary;
	waveGnPresets presetFactory;



public:
	RpSignalGn(const char* primaryBoardIP, const char* secondaryBoardIP) :
		rp_primary(primaryBoardIP), rp_secondary(secondaryBoardIP), controller(), presetFactory() {





	}


	bool check_presetVoltageValueBounds(const std::array<float, 4>& presetArray)const {
		std::array<int, 4> indexes = { 0,1,2,3 };
		if (presetArray.size() == 6) {
			indexes = { 0,1,3,4 };
		}

		for (const float& amplitude : presetArray) {
			if (!check_ValueBounds(amplitude, AMPLITUDE_0, AMPLITUDE_MAX)) { return false; }
		}
		return true;
	}


	void ramp_up_and_down(scpi& rp_board, int source, float current_value, float target_value, bool ramp_up, const std::string& V_P = "VOLT") {

		float step_size = (target_value - current_value) / STEPS;
		const std::string command = "SOUR" + std::to_string(source) + ":" + V_P + " ";
		float new_value = current_value;


		if (!ramp_up) { step_size = -step_size; }
		for (int i = 0; i < STEPS; ++i) {
			new_value += step_size;
			rp_board.tx_txt(command + std::to_string(new_value));
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}


		rp_board.tx_txt(command + std::to_string(target_value));
	}

	void detect_ramp_up_or_down(scpi& rp_board, const float& target_value, const float& current_value,const int& source, const std::string& V_P = "VOLT"){
		if (target_value > current_value) {
			ramp_up_and_down(rp_board, source, current_value, target_value, true, V_P);
		}
		else if (target_value < current_value) {
			ramp_up_and_down(rp_board, source, current_value, target_value, false, V_P);

		}
	}
	//TODO COMPLETE THIS METHOD
	void apply_preset_values(std::array<float,6>){
		//


	
	
	
	}
		







};
