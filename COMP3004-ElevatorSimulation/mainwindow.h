#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>

#include "defs.h"

#include "control_system.h"
#include "building.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //Output Log Function
    void AddToLog(std::string, int);

public slots:
    void FloorButtonHandle();
    void MovementHandle(int, int);
    bool SafetyHandle(std::string, int, int);
private:
    Ui::MainWindow *ui;

    //Console Log
    QString *consoleString;
    QWidget *consoleScroll;
    QVBoxLayout *consoleLayout;

    //Information
    //int numOfFloors;
    //int numOfCars;

    //Systems
    Control_System* cs;
    Building* building;

private slots:
    void StartSimulation();

    void UpButtonHandle();
    void DownButtonHandle();

    void OpenButtonHandle();
    void ReleaseOpenButtonHandle();
    void CloseButtonHandle();

    void PowerOutHandle();
    void FireHandle();
    void HelpHandle();
    void BlockageHandle();

    void FloorSpinHandle();
    void ElevatorSpinHandle();
    void WeightSpinHandle();
};
#endif // MAINWINDOW_H
