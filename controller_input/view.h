#ifndef VIEW_H
#define VIEW_H
#include <QMainWindow>
#include <QCamera>
#include <QCameraDevice>        // Changed: was QCameraDevices
#include <QMediaDevices>
#include <QVideoWidget>
#include <QMediaCaptureSession>
#include <QComboBox>
#include <QLabel>
#include <Qpixmap>
#include <QString>
#include <QPushButton>
#include <unordered_map>
#include <array>
#include <QGridLayout>
#include "MVC_Controller.hpp"
#include "controller.hpp"
#include "observer.hpp"
#include "inputworker.h"
#define withinInterval(a,b,c) (((a)<=(b)) && ((b)<=(c)))
QT_BEGIN_NAMESPACE
namespace Ui {
class View;
}
QT_END_NAMESPACE

class View : public QMainWindow, public Observer<std::pair<const int&, bool>>
{
    Q_OBJECT
public:
    View(QWidget *parent = nullptr);
    ~View();
    
    QList<QCameraDevice> cameras;
    QComboBox* comboBox;
    QVideoWidget* videoWidget;
    QCamera* currentCam;
    QMediaCaptureSession* mediaCaptureSession;
    QLabel* controllerImageLabel;
    QPixmap pixmap;
    QPushButton* testButton;
    MVC_Controller* mvc_controller;




private slots:
    void selectCam();
    void loadImage(const int& button_value);
    void handleInputReceived();


private:

    Ui::View *ui;
    void getCameras();

    QThread* inputThread;
    InputWorker* inputWorker;

    void update_DirectionButton_clicked(const int& directionIndex);


    const QString arrowImagePath= "C:/Users/Redha/Documents/cnrs/redpitaya_GUI/Images/arrows_images/arrow";
    const QString controllerImagePath = "C:/Users/Redha/Documents/cnrs/redpitaya_GUI/Images/controller_images/controller";
    const std::array<QString,9> array_arrowPath= {
        "_up_right.png","_up_left.png",
        "_down_left.png","_down_right.png",
        "_right_down.png","_right_up.png",
        "_left_down.png","_left_up.png",
        "_circle.png"
    };
    const std::unordered_map<int,QString> dictionary_controllerButtonsPath = {
        {-1    , ".png"},
        {Buttons::A,"_button_A.png"},
        {Buttons::B,"_button_B.png"},
        {Buttons::X,"_button_X.png"},
        {Buttons::Y,"_button_Y.png"},

        {Buttons::HAT_UP,"_hat_up.png"},
        {Buttons::HAT_DOWN,"_hat_down.png"},
        {Buttons::HAT_LEFT,"_hat_left.png"},
        {Buttons::HAT_RIGHT,"_hat_right.png"},

        {Buttons::BUMPER_LEFT,"_LB.png"},
        {Buttons::BUMPER_RIGHT,"_RB.png"}



    };


    //Template for array_directionButtons = {up-left, up-right,down-left, down-right, right-up , right-down, left-up  , left-down}

    std::array<QPushButton*,9> array_directionButtons;



    void set_Direction_buttons(QGridLayout*& directionGridLayout);

    virtual void update(const std::pair<const int&, bool>& info);
};
#endif // VIEW_H
