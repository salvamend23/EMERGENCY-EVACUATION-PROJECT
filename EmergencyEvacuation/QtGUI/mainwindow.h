#pragma once
#include <QMainWindow>
#include "Building.h"
#include "mapwidget.h"
#include "Pathfinder.h"
#include "Room.h"
#include "Hallway.h"
#include "Exit.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onFindRouteClicked();
    void onBlockHallwayClicked();
    void onCompareExitsClicked();
    void onBlockExitClicked();

private:
    Ui::MainWindow *ui;
    Building building;
    MapWidget *mapWidget;
    Pathfinder pathfinder;
    void updateStatus(const QString& message);
};
