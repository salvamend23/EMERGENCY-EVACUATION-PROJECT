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

    connect(ui->findRouteButton, &QPushButton::clicked,
            this, &MainWindow::onFindRouteClicked);

    MapWidget *map = new MapWidget(ui->mapWidget);

    map->setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Expanding
        );

    QVBoxLayout *mapLayout = new QVBoxLayout(ui->mapWidget);
    mapLayout->setContentsMargins(0, 0, 0, 0);
    mapLayout->addWidget(map);
}

void MainWindow::onFindRouteClicked()
{
    QString selectedRoom = ui->findRouteButton_2->currentText();

    QMessageBox::information(
        this,
        "Selected Room",
        "You selected: " + selectedRoom
        );
}

void MainWindow::onBlockHallwayClicked()
{
}

void MainWindow::onCompareExitsClicked()
{
}

MainWindow::~MainWindow()
{
    delete ui;
}
