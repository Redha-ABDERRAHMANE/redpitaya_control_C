// InputWorker.h
#ifndef INPUTWORKER_H
#define INPUTWORKER_H

#include <QThread>
#include <QObject>

class MVC_Controller; // Forward declaration

class InputWorker : public QObject
{
    Q_OBJECT

public:
    explicit InputWorker(MVC_Controller* controller, QObject *parent = nullptr);
    void stopWorker();

public slots:
    void process();

signals:
    void finished();
    void inputReceived(); // Signal to emit when input is detected

private:
    MVC_Controller* mvc_controller;
    bool shouldStop;
};

#endif // INPUTWORKER_H
