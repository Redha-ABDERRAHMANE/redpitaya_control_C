#include "view.h"
#include "ui_view.h"
#include <QVBoxLayout>
#include <QGroupBox>
#include <QTimer>
#include <QIcon>
#include <future>
View::View(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::View)
{
    ui->setupUi(this);

    std::cout << "entered"<<std::endl;
    setFixedSize(1920, 1080);
    QHBoxLayout* mainLayout = new QHBoxLayout(ui->centralwidget);
    mediaCaptureSession = new QMediaCaptureSession();
    currentCam = new QCamera();
    comboBox = new QComboBox();
    videoWidget = new QVideoWidget();

    // Main horizontal layout (splits left and right)
    // Left side vertical layout
    QVBoxLayout* leftLayout = new QVBoxLayout();

    // Camera group box (top left)
    QGroupBox* cameraGroupBox = new QGroupBox("Camera", this);
    QVBoxLayout* verticalCameraLayout = new QVBoxLayout();
    verticalCameraLayout->addWidget(videoWidget, 3);
    verticalCameraLayout->addWidget(comboBox, 1);
    cameraGroupBox->setLayout(verticalCameraLayout);

    // Controller group box (bottom left)
    QGroupBox* controllerGroupBox = new QGroupBox("Controller", this);
    QHBoxLayout* horizontalControllerLayout = new QHBoxLayout();

    // Controller image (left side)
    controllerImageLabel = new QLabel(this);
    loadImage(-1);

    // Direction buttons group (right side)
    QGroupBox* directionGroupBox = new QGroupBox("Directions", this);
    QGridLayout* directionGridLayout = new QGridLayout();

    set_Direction_buttons(directionGridLayout);
    directionGroupBox->setLayout(directionGridLayout);

    // Add both to horizontal layout
    horizontalControllerLayout->addWidget(controllerImageLabel, 2);
    horizontalControllerLayout->addWidget(directionGroupBox, 1);

    controllerGroupBox->setLayout(horizontalControllerLayout);

    // Info group box (right side)
    QGroupBox* infoGroupBox = new QGroupBox("Info", this);
    QVBoxLayout* infoLayout = new QVBoxLayout();
    infoGroupBox->setLayout(infoLayout);

    // Assemble left side (camera on top, controller below)
    leftLayout->addWidget(cameraGroupBox, 2);
    leftLayout->addWidget(controllerGroupBox, 1);

    // Add left side and right side to main layout
    mainLayout->addLayout(leftLayout, 2);
    mainLayout->addWidget(infoGroupBox, 1);

    getCameras();
    connect(comboBox, &QComboBox::currentIndexChanged, this, &View::selectCam);

    // Initialize MVC_Controller
    mvc_controller = new MVC_Controller();
    mvc_controller->set_Observer(*this);

    // Set up worker thread for input checking
    inputThread = new QThread(this);
    inputWorker = new InputWorker(mvc_controller);
    inputWorker->moveToThread(inputThread);

    // Connect signals
    connect(inputThread, &QThread::started, inputWorker, &InputWorker::process);
    connect(inputWorker, &InputWorker::finished, inputThread, &QThread::quit);
    connect(inputWorker, &InputWorker::finished, inputWorker, &InputWorker::deleteLater);
    connect(inputThread, &QThread::finished, inputThread, &QThread::deleteLater);

    // Connect input received signal to your handler
    connect(inputWorker, &InputWorker::inputReceived, this, &View::handleInputReceived);

    // Start the thread
    inputThread->start();
}


View::~View()
{
    if (inputWorker) {
        inputWorker->stopWorker();
    }

    if (inputThread && inputThread->isRunning()) {
        inputThread->quit();
        inputThread->wait(3000); // Wait up to 3 seconds for thread to finish
    }

    delete ui;
    delete mvc_controller;
}

void View::getCameras(){
    comboBox->addItem("<None>");
    cameras = QMediaDevices::videoInputs();
    for(const QCameraDevice& camera:std::as_const(cameras)){
        comboBox->addItem(camera.description());

    }
}

void View::update_DirectionButton_clicked(const int& directionIndex)
{
    qDebug()<<"direction index"<<directionIndex;
    mvc_controller->apply_userButtonInput(std::make_pair(1,directionIndex));
    (void)directionIndex;

}

void View::selectCam(){
    if(currentCam->isActive()){
        currentCam->stop();
    }
    for( const QCameraDevice& camera: std::as_const(cameras)){
        if(camera.description()==comboBox->currentText()){

            if(!currentCam || currentCam->cameraDevice() != camera){

                if(currentCam){
                    delete currentCam;
                }

                currentCam = new QCamera(camera, this);
            }

            currentCam->setCameraDevice(camera);

            mediaCaptureSession->setCamera(currentCam);
            mediaCaptureSession->setVideoOutput(videoWidget);

            currentCam->start();

            qDebug()<<"selected cam: "<<camera.description();
            return ;

        }
    }

}

void View::loadImage(const int &button_value = 22528)
{
    bool success = pixmap.load(controllerImagePath + dictionary_controllerButtonsPath.at(button_value));
    if (success) {
        controllerImageLabel->setPixmap(pixmap);
        controllerImageLabel->update();
        controllerImageLabel->repaint();
    }
    if(button_value != -1) {
        QTimer::singleShot(300, this, [this]() {
            loadImage(-1);
        });
    }

}


void View::set_Direction_buttons(QGridLayout*& directionGridLayout){


    uint8_t row;
    uint8_t col;
    uint8_t size= array_directionButtons.size();

    for(uint8_t index = 0; index !=size; index++) {
        array_directionButtons[index] = new QPushButton("&Test", this);
        connect(array_directionButtons[index], &QPushButton::clicked, this, [this, index]() {
            update_DirectionButton_clicked(index+1);
        });
    }




    for(int i=0 ; i!=9;i++){
        array_directionButtons[i]->setIcon(QIcon(arrowImagePath+ array_arrowPath[i]));
        array_directionButtons[i]->setIconSize(QSize(32,32));
        array_directionButtons[i]->setText("");
        row = i / 2;  // 2 buttons per row
        col = i % 2;  // Alternate columns
        directionGridLayout->addWidget(array_directionButtons[i], row, col);


    }
}
void View::update(const std::pair<const int&, bool>& info) {
    std::cout << "received";
}


void View::handleInputReceived()
{
    // Handle the input on the main thread
    // This is where you update your UI based on the controller input
    // For example:
    // updateControllerDisplay();
    // updateDirectionButtons();
}
