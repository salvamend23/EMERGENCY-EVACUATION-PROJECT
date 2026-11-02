#include "mapwidget.h"

#include <QPainter>
#include <QPaintEvent>
#include <QPolygon>
#include <QRect>
#include <QString>
#include <QColor>
#include <QtGlobal>
#include <QPoint>
#include <QPen>
#include <QBrush>

MapWidget::MapWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(500, 400);
}

void MapWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Scale the map to fit the available area.
    const qreal scaleX = width() / 780.0;
    const qreal scaleY = height() / 580.0;
    const qreal scale = qMin(scaleX, scaleY);

    const qreal offsetX = (width() - 780.0 * scale) / 2.0;
    const qreal offsetY = (height() - 580.0 * scale) / 2.0;

    painter.fillRect(rect(), Qt::white);
    painter.translate(offsetX, offsetY);
    painter.scale(scale, scale);

    const QPen wallPen(Qt::black, 2);
    const QBrush roomBrush(QColor(220, 235, 250));
    const QBrush hallwayBrush(QColor(245, 245, 245));

    auto drawRoom = [&](int x, int y, int w, int h, int number)
    {
        painter.setPen(wallPen);
        painter.setBrush(roomBrush);
        painter.drawRect(x, y, w, h);

        painter.setPen(Qt::black);
        painter.drawText(
            QRect(x, y, w, h),
            Qt::AlignCenter,
            QString("Room %1").arg(number)
            );
    };

    auto drawHallway = [&](int x, int y, int w, int h,
                           const QString &label)
    {
        painter.setPen(wallPen);
        painter.setBrush(hallwayBrush);
        painter.drawRect(x, y, w, h);

        painter.setPen(Qt::black);
        painter.drawText(
            QRect(x, y, w, h),
            Qt::AlignCenter,
            label
            );
    };

    // Draw a tree with a trunk and leafy canopy
    auto drawTree = [&](int x, int y, int size)
    {
        // Trunk
        painter.setBrush(QColor(120, 78, 45));
        painter.drawRect(x + size / 2 - 4, y + size / 2, 8, size / 2);

        // Main canopy
        painter.setBrush(QColor(45, 115, 55));
        painter.drawEllipse(x, y, size, size);

        // Lighter foliage
        painter.setBrush(QColor(75, 145, 65));
        painter.drawEllipse(x + size / 6, y + size / 8,
                            size / 2, size / 2);
        painter.drawEllipse(x + size / 3, y + size / 3,
                            size / 2, size / 2);
    };

    // Central garden background
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(205, 230, 185));
    painter.drawRect(230, 190, 330, 200);

    // Draw a leafy tree
    auto drawGardenTree = [&](int x, int y, int size)
    {
        // Trunk
        painter.setBrush(QColor(115, 75, 45));
        painter.drawRect(x + size / 2 - 3, y + size / 2, 6, size / 2);

        // Layered foliage
        painter.setBrush(QColor(42, 105, 48));
        painter.drawEllipse(x, y, size, size);
        painter.setBrush(QColor(65, 135, 58));
        painter.drawEllipse(x + size / 5, y + size / 8,
                            size / 2, size / 2);
        painter.drawEllipse(x + size / 3, y + size / 3,
                            size / 2, size / 2);
    };

    // Small leafy plants
    auto drawPlant = [&](int x, int y)
    {
        painter.setBrush(QColor(45, 125, 55));
        painter.drawEllipse(x, y, 12, 8);
        painter.drawEllipse(x + 5, y - 5, 10, 8);
        painter.drawEllipse(x + 9, y + 2, 10, 8);
    };

    // Colorful flowers
    auto drawFlower = [&](int x, int y, QColor color)
    {
        painter.setBrush(QColor(65, 115, 45));
        painter.drawLine(x + 4, y + 4, x + 4, y + 13);

        painter.setPen(Qt::NoPen);
        painter.setBrush(color);
        painter.drawEllipse(x, y, 5, 5);
        painter.drawEllipse(x + 6, y, 5, 5);
        painter.drawEllipse(x + 3, y - 4, 5, 5);
        painter.drawEllipse(x + 3, y + 3, 5, 5);

        painter.setBrush(QColor(245, 205, 65));
        painter.drawEllipse(x + 3, y + 1, 4, 4);
    };

    // Irregularly placed trees around the garden
    drawGardenTree(242, 205, 32);
    drawGardenTree(493, 208, 36);
    drawGardenTree(250, 325, 30);
    drawGardenTree(500, 330, 32);

    // A few shrubs and leafy plants
    drawPlant(290, 215);
    drawPlant(455, 225);
    drawPlant(285, 315);
    drawPlant(460, 320);
    drawPlant(330, 350);
    drawPlant(440, 355);

    // Flower beds
    drawFlower(300, 250, QColor(235, 105, 145));
    drawFlower(325, 275, QColor(245, 210, 75));
    drawFlower(465, 270, QColor(170, 115, 220));
    drawFlower(475, 295, QColor(235, 105, 145));
    drawFlower(350, 350, QColor(245, 210, 75));
    drawFlower(420, 350, QColor(170, 115, 220));

    // Circular fountain in the center
    painter.setPen(QPen(QColor(120, 140, 150), 2));
    painter.setBrush(QColor(150, 205, 220));
    painter.drawEllipse(365, 255, 65, 65);

    // Fountain rim
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(370, 260, 55, 55);

    // Water in the middle
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(80, 160, 205));
    painter.drawEllipse(382, 272, 31, 31);

    // Fountain centerpiece
    painter.setBrush(QColor(225, 235, 240));
    painter.drawEllipse(391, 278, 13, 13);
    painter.drawLine(397, 278, 397, 267);
    // Trees around the edges of the courtyard
    drawGardenTree(238, 200, 28);
    drawGardenTree(505, 205, 32);
    drawGardenTree(242, 342, 30);
    drawGardenTree(510, 340, 27);

    // Natural groups of shrubs
    drawPlant(280, 218);
    drawPlant(300, 232);
    drawPlant(465, 218);
    drawPlant(480, 235);

    drawPlant(280, 315);
    drawPlant(300, 330);
    drawPlant(465, 320);
    drawPlant(485, 310);

    // Flower patches around the fountain
    drawFlower(315, 240, QColor(240, 100, 145));
    drawFlower(340, 220, QColor(255, 210, 70));
    drawFlower(460, 250, QColor(170, 115, 220));
    drawFlower(480, 275, QColor(255, 145, 75));

    drawFlower(320, 305, QColor(255, 210, 70));
    drawFlower(350, 345, QColor(240, 100, 145));
    drawFlower(445, 340, QColor(255, 210, 70));
    drawFlower(475, 320, QColor(170, 115, 220));

    // Fountain basin
    painter.setPen(QPen(QColor(115, 135, 145), 2));
    painter.setBrush(QColor(190, 215, 220));
    painter.drawEllipse(370, 260, 60, 60);

    // Water
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(75, 165, 210));
    painter.drawEllipse(377, 267, 46, 46);

    // Fountain center
    painter.setBrush(QColor(225, 240, 245));
    painter.drawEllipse(393, 282, 14, 14);

    // Water ripples
    painter.setPen(QPen(QColor(210, 240, 250), 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(384, 274, 32, 32);


    // ================================================
    // HALLWAYS
    // ================================================

    drawHallway(160, 125, 470, 65, "Hallway 1");
    drawHallway(160, 190, 70, 200, "Hallway 4");
    drawHallway(560, 190, 70, 200, "Hallway 2");
    drawHallway(160, 390, 470, 65, "Hallway 3");

    // ================================================
    // ROOMS
    // ================================================

    // Room 1: L-shaped upper-left room
    painter.setPen(wallPen);
    painter.setBrush(roomBrush);

    QPolygon room1;
    room1 << QPoint(60, 25) //
          << QPoint(230, 25)
          << QPoint(230, 125)
          << QPoint(160, 125)
          << QPoint(160, 190)
          << QPoint(60, 190);

    painter.drawPolygon(room1);
    painter.setPen(Qt::black);
    painter.drawText(
        QRect(60, 25, 170, 100),
        Qt::AlignCenter,
        "Room 1"
        );

    // Rooms 2 and 3
    drawRoom(230, 25, 160, 100, 2);
    drawRoom(390, 25, 170, 100, 3);

    // Room 4: L-shaped upper-right room
    painter.setPen(wallPen);
    painter.setBrush(roomBrush);

    QPolygon room4;
    room4 << QPoint(560, 25)
          << QPoint(730, 25)
          << QPoint(730, 190)
          << QPoint(630, 190)
          << QPoint(630, 125)
          << QPoint(560, 125);

    painter.drawPolygon(room4);
    painter.setPen(Qt::black);
    painter.drawText(
        QRect(560, 25, 170, 100),
        Qt::AlignCenter,
        "Room 4"
        );

    // Rooms 12 and 11: left side
    drawRoom(60, 190, 100, 100, 12);
    drawRoom(60, 290, 100, 100, 11);

    // Rooms 5 and 6: right side
    drawRoom(630, 190, 100, 100, 5);
    drawRoom(630, 290, 100, 100, 6);

    // Room 10: L-shaped lower-left room
    painter.setPen(wallPen);
    painter.setBrush(roomBrush);

    QPolygon room10;
    room10 << QPoint(60, 390)
           << QPoint(160, 390)
           << QPoint(160, 455)
           << QPoint(230, 455)
           << QPoint(230, 555)
           << QPoint(60, 555);

    painter.drawPolygon(room10);
    painter.setPen(Qt::black);
    painter.drawText(
        QRect(60, 455, 160, 100),
        Qt::AlignCenter,
        "Room 10"
        );

    // Rooms 9 and 8: bottom row
    drawRoom(230, 455, 160, 100, 9);
    drawRoom(390, 455, 170, 100, 8);

    // Room 7: L-shaped lower-right room
    painter.setPen(wallPen);
    painter.setBrush(roomBrush);

    QPolygon room7;
    room7 << QPoint(630, 390)
          << QPoint(730, 390)
          << QPoint(730, 555)
          << QPoint(560, 555)
          << QPoint(560, 455)
          << QPoint(630, 455);

    painter.drawPolygon(room7);
    painter.setPen(Qt::black);
    painter.drawText(
        QRect(560, 455, 170, 100),
        Qt::AlignCenter,
        "Room 7"
        );
}