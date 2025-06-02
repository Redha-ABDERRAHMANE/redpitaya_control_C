#include "MVC_Model.hpp"


class MVC_Controller
{
private:

	MVC_Model model;

public:

	MVC_Controller(){}
	~MVC_Controller(){}

	void apply_userButtonInput(const int& button_value) {
		model.get_and_applyPreset(button_value);
	}



};

