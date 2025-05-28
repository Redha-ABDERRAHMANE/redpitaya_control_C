#pragma once
#include "waveGnPresets.hpp"
#include "scpi.hpp"
#include <array>
#include <thread>
#include <chrono>
#include <ctime>
#include <cmath>
#include <vector>
#include "RedpitayaCard.hpp"

#define SOURCE_1 1
#define SOURCE_2 2

#define DELAY 0.3f
#define STEPS 20
#define lessOrEqualValue(value,x) ((value)<=(x))


#define check_ValueBounds(value,x,y) ((x)<=(value) && (value)<=(y))

class RpSignalGn
{
	using p_array = std::array<float,6>;
private:
	
	//waveGnPresets presetFactory;
	RedpitayaCards rp_boards;



public:
	
	RpSignalGn(const char* primaryBoardIP, const char* secondaryBoardIP) :
		rp_boards(primaryBoardIP, secondaryBoardIP, 5){





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


	void ramp_up_and_down(const int& card, int source, float current_value, float target_value, const std::string& V_P = "VOLT") {
		float step_size = (target_value - current_value) / STEPS;
		const std::string command = "SOUR" + std::to_string(source) + ":" + V_P + " ";
		float new_value = current_value;

		if (V_P == "PHAS") {
			current_value = (int)current_value;
			target_value = (int)target_value;
		}


		
		for (volatile int i = 0; i < STEPS; i++) {
			
			new_value += step_size;

			rp_boards.send_txt(card, command + std::to_string(new_value));
	
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
		}

		std::cout << command + std::to_string(target_value) << std::endl;
		rp_boards.send_txt(card, command + std::to_string(target_value));
	}

	void detect_ramp_up_or_down(const int& card, const float& target_value, const float& current_value,const int& source, const std::string& V_P = "VOLT"){
		if (target_value != current_value){
			ramp_up_and_down(card, source, current_value, target_value, V_P);
		}

	}
	//TODO COMPLETE THIS METHOD
	bool apply_preset_values(p_array& nextPreset, const p_array& currentPreset){
		if (nextPreset == currentPreset) {
			std::cout << "same preset:" << std::endl;
			std::cout << "preset:" << std::endl;
			for (auto& x : nextPreset){
				std::cout << x << " ";
			}
		


			std::cout << "\n previous preset:" << std::endl;
			for (auto& x : currentPreset) {
				std::cout << x << " ";
			}
			std::cout<<std::endl;

			return false; }

		std::vector<std::thread> threadVector;
		threadVector.reserve(8);

		threadVector.emplace_back(&RpSignalGn::detect_ramp_up_or_down,this, PRIMARY_BOARD, nextPreset[2], currentPreset[2], SOURCE_1, "PHAS");
		threadVector.emplace_back(&RpSignalGn::detect_ramp_up_or_down,this, PRIMARY_BOARD, nextPreset[2], currentPreset[2], SOURCE_2, "PHAS");
		threadVector.emplace_back(&RpSignalGn::detect_ramp_up_or_down,this, SECONDARY_BOARD, nextPreset[5], currentPreset[5], SOURCE_1, "PHAS");
		threadVector.emplace_back(&RpSignalGn::detect_ramp_up_or_down,this, SECONDARY_BOARD, nextPreset[5], currentPreset[5], SOURCE_2, "PHAS");

		threadVector.emplace_back(&RpSignalGn::detect_ramp_up_or_down,this, PRIMARY_BOARD, nextPreset[0], currentPreset[0], SOURCE_1, "VOLT");
		threadVector.emplace_back(&RpSignalGn::detect_ramp_up_or_down,this, PRIMARY_BOARD, nextPreset[1], currentPreset[1], SOURCE_2, "VOLT");
		threadVector.emplace_back(&RpSignalGn::detect_ramp_up_or_down,this, SECONDARY_BOARD, nextPreset[3], currentPreset[3], SOURCE_1, "VOLT");
		threadVector.emplace_back(&RpSignalGn::detect_ramp_up_or_down,this, SECONDARY_BOARD, nextPreset[4], currentPreset[4], SOURCE_2, "VOLT");


		for (std::thread& t : threadVector) { t.join(); }
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		return true;



		//


	
	
	
	}


		







};
