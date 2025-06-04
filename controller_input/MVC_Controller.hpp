#include "MVC_Model.hpp"
#include <QDebug>
class MVC_Controller
{
private:

	MVC_Model model;
	Controller controller;
	

public:
	void apply_userButtonInput(const std::pair<int,int> pair) {
		std::cout << "button: " << pair.first << " " << pair.second << std::endl;
		//model.get_and_applyPreset(button_value);
	}

	void check_newInput() {
		int button_value;
        qDebug()<<"entered";

		while (true) {
			button_value = controller.CheckControllerEvent();
			if (button_value == -1) { continue; }
			model.get_and_applyPreset(button_value);
		}

	}

	
	void set_Observer(Observer<std::pair<const int&, bool>>& observer) {
		model.setObserver(observer);
	}
};



