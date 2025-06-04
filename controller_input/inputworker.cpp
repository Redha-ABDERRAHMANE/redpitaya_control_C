#include "InputWorker.h"
#include "MVC_Controller.hpp" // Include your controller header

InputWorker::InputWorker(MVC_Controller* controller, QObject *parent)
    : QObject(parent), mvc_controller(controller), shouldStop(false)
{
}

void InputWorker::stopWorker()
{
    shouldStop = true;
}

void InputWorker::process()
{
    while (!shouldStop) {
        // Replace this with your actual input checking logic
        mvc_controller->check_newInput();
        // if (mvc_controller && mvc_controller->check_newInput()) {
        //     emit inputReceived();
        // }

        // Small delay to prevent excessive CPU usage
        QThread::msleep(10); // 10ms delay
    }

    emit finished();
}
