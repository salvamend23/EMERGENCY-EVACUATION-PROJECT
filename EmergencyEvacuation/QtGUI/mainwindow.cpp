#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mapwidget.h"
#include <QVBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    updateStatus("Ready");
    
    // initialize pathfinder connections between hallways
    pathfinder.addconnections(101, 102, 20.0);
    pathfinder.addconnections(101, 104, 25.0);
    pathfinder.addconnections(102, 103, 20.0);
    pathfinder.addconnections(103, 104, 25.0);

    // connect rooms to their hallways
    for (int roomId = 1; roomId <= 12; roomId++)
    {
        int hallwayId;

        if (roomId <= 4)
            hallwayId = 101;
        else if (roomId <= 6)
            hallwayId = 102;
        else if (roomId <= 10)
            hallwayId = 103;
        else
            hallwayId = 104;

        pathfinder.addconnections(roomId, hallwayId, 10.0);
    }

    // connect exits to hallways
    pathfinder.addconnections(101, 201, 3.0);
    pathfinder.addconnections(103, 202, 3.0);
    ui->hallwayComboBox->setItemData(0, 101);
    ui->hallwayComboBox->setItemData(1, 102);
    ui->hallwayComboBox->setItemData(2, 103);
    ui->hallwayComboBox->setItemData(3, 104);

    ui->exitComboBox->setItemData(0, 201);
    ui->exitComboBox->setItemData(1, 202);

    connect(ui->findRouteButton, &QPushButton::clicked,
            this, &MainWindow::onFindRouteClicked);

    connect(ui->blockHallwayButton, &QPushButton::clicked,
            this, &MainWindow::onBlockHallwayClicked);

    connect(ui->blockExitButton, &QPushButton::clicked,
            this, &MainWindow::onBlockExitClicked);

    connect(ui->compareExitsButton, &QPushButton::clicked,
            this, &MainWindow::onCompareExitsClicked);

    mapWidget = new MapWidget(building, ui->mapWidget);
    mapWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *mapLayout = new QVBoxLayout(ui->mapWidget);
    mapLayout->setContentsMargins(0, 0, 0, 0);
    mapLayout->addWidget(mapWidget);
}

void MainWindow::updateStatus(const QString& message)
{
    ui->statusLabel->setText("Status: " + message);
}

void MainWindow::onFindRouteClicked()
{
    int selectedRoomId = ui->findRouteButton_2->currentIndex() + 1;

    mapWidget->setSelectedRoom(selectedRoomId);

    std::set<int> blockedExitIds;
    std::set<int> blockedHallwayIds;

    // Check blocked exits
    if (mapWidget->isExitBlocked(201))
        blockedExitIds.insert(201);

    if (mapWidget->isExitBlocked(202))
        blockedExitIds.insert(202);

    // Check blocked hallways
    for (int hallwayId = 101; hallwayId <= 104; hallwayId++)
    {
        if (mapWidget->isHallwayBlocked(hallwayId))
            blockedHallwayIds.insert(hallwayId);
    }

    path_results results =
        pathfinder.calculate_distances(
            selectedRoomId,
            -1,
            -1,
            blockedExitIds,
            blockedHallwayIds
        );
    if (!results.found)
    {
        mapWidget->setRoute({});
        updateStatus("No Available Route");
        QMessageBox::warning(
            this,
            "No Route Available",
            "No evacuation route is available from this room."
        );
        return;
    }
    
    mapWidget->setRoute(results.route);
    updateStatus("Route Found");
    QString routeText;

    for (size_t i = 0; i < results.route.size(); i++)
    {
        routeText += QString::number(results.route[i]);

        if (i < results.route.size() - 1)
            routeText += " -> ";
    }

    QString message =
        "Nearest Exit: Exit " + QString::number(results.exitid - 200)
        + "\nRoute: " + routeText
        + "\nTotal Distance: "
        + QString::number(results.total_distance) + " meters";

    QMessageBox::information(
        this,
        "Evacuation Route",
        message
    );
}

void MainWindow::onBlockHallwayClicked()
{
    int selectedHallwayId =
        ui->hallwayComboBox->currentData().toInt();

    bool currentlyBlocked =
        mapWidget->isHallwayBlocked(selectedHallwayId);

    mapWidget->setHallwayBlocked(selectedHallwayId, !currentlyBlocked);
    if (currentlyBlocked)
        updateStatus("Hallway Unblocked");
    else
        updateStatus("Hallway Blocked");

    QString message;

    if (currentlyBlocked)
    {
        message = "Hallway "
                  + QString::number(selectedHallwayId - 100)
                  + " has been unblocked.";
    }
    else
    {
        message = "Hallway "
                  + QString::number(selectedHallwayId - 100)
                  + " has been blocked.";
    }

    QMessageBox::information(
        this,
        "Hallway Status",
        message
        );
}

void MainWindow::onBlockExitClicked()
{
    int selectedExitId = ui->exitComboBox->currentData().toInt();

    bool currentlyBlocked = mapWidget->isExitBlocked(selectedExitId);

    mapWidget->setExitBlocked(selectedExitId, !currentlyBlocked);
    if (currentlyBlocked)
        updateStatus("Exit Unblocked");
    else
        updateStatus("Exit Blocked");
    QString message;

    if (currentlyBlocked)
    {
        message = "Exit " + QString::number(selectedExitId - 200)
        + " has been unblocked.";
    }
    else
    {
        message = "Exit " + QString::number(selectedExitId - 200)
        + " has been blocked.";
    }

    QMessageBox::information(this, "Exit Status", message);
}

void MainWindow::onCompareExitsClicked()
{
    int selectedRoomId = ui->findRouteButton_2->currentIndex() + 1;

    std::set<int> blockedExitIds;
    std::set<int> blockedHallwayIds;

    // Check blocked exits
    if (mapWidget->isExitBlocked(201))
        blockedExitIds.insert(201);

    if (mapWidget->isExitBlocked(202))
        blockedExitIds.insert(202);

    // Check blocked hallways
    for (int hallwayId = 101; hallwayId <= 104; hallwayId++)
    {
        if (mapWidget->isHallwayBlocked(hallwayId))
            blockedHallwayIds.insert(hallwayId);
    }

    path_results exit1 =
        pathfinder.calculate_distances(
            selectedRoomId, -1, 201,
            blockedExitIds, blockedHallwayIds
        );

    path_results exit2 =
        pathfinder.calculate_distances(
            selectedRoomId, -1, 202,
            blockedExitIds, blockedHallwayIds
        );

    QString message;

    if (exit1.found)
        message += "Exit 1: " + QString::number(exit1.total_distance) + " meters\n";
    else
        message += "Exit 1: Unavailable\n";

    if (exit2.found)
        message += "Exit 2: " + QString::number(exit2.total_distance) + " meters\n";
    else
        message += "Exit 2: Unavailable\n";

    updateStatus("Exits Compared");
    QMessageBox::information(
        this,
        "Compare Exits",
        message
    );
}

MainWindow::~MainWindow()
{
    delete ui;
}
