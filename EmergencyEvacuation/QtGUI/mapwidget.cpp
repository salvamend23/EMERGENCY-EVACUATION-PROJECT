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
#include <QPainterPath>

MapWidget::MapWidget(Building& building, QWidget *parent)
    : QWidget(parent), building(building)
{
    setMinimumSize(500, 400);
}

void MapWidget::setHallwayBlocked(int hallwayId, bool blocked)
{
    if (blocked)
    {
        blockedHallways.insert(hallwayId);
    }
    else
    {
        blockedHallways.erase(hallwayId);
    }

    update();
}

bool MapWidget::isHallwayBlocked(int hallwayId) const
{
    return blockedHallways.count(hallwayId) > 0;
}

void MapWidget::setExitBlocked(int exitId, bool blocked)
{
    if (blocked)
    {
        blockedExits.insert(exitId);
    }
    else
    {
        blockedExits.erase(exitId);
    }

    update();
}

bool MapWidget::isExitBlocked(int exitId) const
{
    return blockedExits.count(exitId) > 0;
}

void MapWidget::setSelectedRoom(int roomId)
{
    selectedRoomId = roomId;
    update();
}

void MapWidget::setRoute(const std::vector<int>& route)
{
    currentRoute = route;
    update();
}

void drawPerson(QPainter& painter, int x, int y, QColor shirtColor)
{
    // Head
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(235, 190, 150));
    painter.drawEllipse(x + 6, y, 14, 14);

    // Body
    painter.setBrush(shirtColor);
    painter.drawRoundedRect(x + 4, y + 15, 18, 25, 5, 5);

    // Legs
    painter.setPen(QPen(Qt::black, 2));
    painter.drawLine(x + 9, y + 40, x + 7, y + 52);
    painter.drawLine(x + 17, y + 40, x + 19, y + 52);
}

void MapWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // scale the map to fit the available area
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
    const QBrush selectedRoomBrush(QColor(255, 235, 150));
    const QBrush hallwayBrush(QColor(245, 245, 245));

    auto drawRoom = [&](int x, int y, int w, int h, int number)
    {
        painter.setPen(wallPen);

        if (selectedRoomId == number)
        {
            painter.setBrush(selectedRoomBrush);
        }
        else
        {
            painter.setBrush(roomBrush);
        }

        painter.drawRect(x, y, w, h);

        painter.setPen(Qt::black);
        painter.drawText(
            QRect(x, y, w, h),
            Qt::AlignCenter,
            QString("Room %1").arg(number)
            );
    };

    auto drawHallway = [&](int x, int y, int w, int h,
        const QString& label, int hallwayId)
        {
            bool isBlocked = blockedHallways.count(hallwayId) > 0;

            painter.setPen(wallPen);

            if (isBlocked)
                painter.setBrush(QColor(230, 100, 100));
            else
                painter.setBrush(hallwayBrush);

            painter.drawRect(x, y, w, h);

            if (isBlocked)
            {

                // Draw a more visible fire
                painter.setPen(Qt::NoPen);

                // Outer flame
                QPolygon fire;
                fire << QPoint(x + w / 2, y + h / 2 + 18)
                    << QPoint(x + w / 2 - 13, y + h / 2 + 5)
                    << QPoint(x + w / 2 - 10, y + h / 2 - 8)
                    << QPoint(x + w / 2 - 4, y + h / 2 - 2)
                    << QPoint(x + w / 2, y + h / 2 - 22)
                    << QPoint(x + w / 2 + 5, y + h / 2 - 5)
                    << QPoint(x + w / 2 + 12, y + h / 2 - 12)
                    << QPoint(x + w / 2 + 13, y + h / 2 + 5);

                painter.setBrush(QColor(230, 55, 10));
                painter.drawPolygon(fire);

                // Middle flame
                QPolygon innerFire;
                innerFire << QPoint(x + w / 2, y + h / 2 + 12)
                    << QPoint(x + w / 2 - 7, y + h / 2 + 2)
                    << QPoint(x + w / 2, y + h / 2 - 10)
                    << QPoint(x + w / 2 + 7, y + h / 2 + 2);

                painter.setBrush(QColor(255, 170, 20));
                painter.drawPolygon(innerFire);

                // Bright center
                painter.setBrush(QColor(255, 240, 120));
                painter.drawEllipse(
                    QPoint(x + w / 2, y + h / 2 + 5), 3, 5
                );
            }

            // Always display the hallway name
            painter.setPen(Qt::black);
            painter.drawText(
                QRect(x, y, w, h),
                Qt::AlignCenter,
                label
            );
        };


    // tree with a trunk and leafy canopy
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

    // central garden background
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(205, 230, 185));
    painter.drawRect(230, 190, 330, 200);

    // leafy tree
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

    // leafy plants
    auto drawPlant = [&](int x, int y)
    {
        painter.setBrush(QColor(45, 125, 55));
        painter.drawEllipse(x, y, 12, 8);
        painter.drawEllipse(x + 5, y - 5, 10, 8);
        painter.drawEllipse(x + 9, y + 2, 10, 8);
    };

    // flowers
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

    // irregularly placed trees around the garden
    drawGardenTree(242, 205, 32);
    drawGardenTree(493, 208, 36);
    drawGardenTree(250, 325, 30);
    drawGardenTree(500, 330, 32);

    // shrubs and leafy plants
    drawPlant(290, 215);
    drawPlant(455, 225);
    drawPlant(285, 315);
    drawPlant(460, 320);
    drawPlant(330, 350);
    drawPlant(440, 355);

    // flower beds
    drawFlower(300, 250, QColor(235, 105, 145));
    drawFlower(325, 275, QColor(245, 210, 75));
    drawFlower(465, 270, QColor(170, 115, 220));
    drawFlower(475, 295, QColor(235, 105, 145));
    drawFlower(350, 350, QColor(245, 210, 75));
    drawFlower(420, 350, QColor(170, 115, 220));

    // fountain in the center
    painter.setPen(QPen(QColor(120, 140, 150), 2));
    painter.setBrush(QColor(150, 205, 220));
    painter.drawEllipse(365, 255, 65, 65);

    // fountain rim
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(370, 260, 55, 55);

    // water in the middle
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(80, 160, 205));
    painter.drawEllipse(382, 272, 31, 31);

    // fountain centerpiece
    painter.setBrush(QColor(225, 235, 240));
    painter.drawEllipse(391, 278, 13, 13);
    painter.drawLine(397, 278, 397, 267);

    // trees around the edges of the courtyard
    drawGardenTree(238, 200, 28);
    drawGardenTree(505, 205, 32);
    drawGardenTree(242, 342, 30);
    drawGardenTree(510, 340, 27);

    // shrubs
    drawPlant(280, 218);
    drawPlant(300, 232);
    drawPlant(465, 218);
    drawPlant(480, 235);

    drawPlant(280, 315);
    drawPlant(300, 330);
    drawPlant(465, 320);
    drawPlant(485, 310);

    // flower patches around the fountain
    drawFlower(315, 240, QColor(240, 100, 145));
    drawFlower(340, 220, QColor(255, 210, 70));
    drawFlower(460, 250, QColor(170, 115, 220));
    drawFlower(480, 275, QColor(255, 145, 75));

    drawFlower(320, 305, QColor(255, 210, 70));
    drawFlower(350, 345, QColor(240, 100, 145));
    drawFlower(445, 340, QColor(255, 210, 70));
    drawFlower(475, 320, QColor(170, 115, 220));

    // fountain basin
    painter.setPen(QPen(QColor(115, 135, 145), 2));
    painter.setBrush(QColor(190, 215, 220));
    painter.drawEllipse(370, 260, 60, 60);

    // water
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(75, 165, 210));
    painter.drawEllipse(377, 267, 46, 46);

    // fountain center
    painter.setBrush(QColor(225, 240, 245));
    painter.drawEllipse(393, 282, 14, 14);

    // water ripples
    painter.setPen(QPen(QColor(210, 240, 250), 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(384, 274, 32, 32);


    // HALLWAYS
    drawHallway(160, 125, 470, 65, "Hallway 1", 101);
    drawHallway(160, 190, 70, 200, "Hallway 4", 104);
    drawHallway(560, 190, 70, 200, "Hallway 2", 102);
    drawHallway(160, 390, 470, 65, "Hallway 3", 103);

    // ROOMS

    // room 1: L-shaped upper-left room
    painter.setPen(wallPen);

    if (selectedRoomId == 1)
    {
        painter.setBrush(selectedRoomBrush);
    }
    else
    {
        painter.setBrush(roomBrush);
    }

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

    // rooms 2 and 3
    drawRoom(230, 25, 160, 100, 2);
    drawRoom(390, 25, 170, 100, 3);

    // room 4: L-shaped upper-right room
    painter.setPen(wallPen);

    if (selectedRoomId == 4)
    {
        painter.setBrush(selectedRoomBrush);
    }
    else
    {
        painter.setBrush(roomBrush);
    }

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

    // rooms 12 and 11: left side
    drawRoom(60, 190, 100, 100, 12);
    drawRoom(60, 290, 100, 100, 11);

    // rooms 5 and 6: right side
    drawRoom(630, 190, 100, 100, 5);
    drawRoom(630, 290, 100, 100, 6);

    // room 10: L-shaped lower-left room
    painter.setPen(wallPen);
    if (selectedRoomId == 10)
    {
        painter.setBrush(selectedRoomBrush);
    }
    else
    {
        painter.setBrush(roomBrush);
    }

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

    // rooms 9 and 8: bottom row
    drawRoom(230, 455, 160, 100, 9);
    drawRoom(390, 455, 170, 100, 8);

    // room 7: L-shaped lower-right room
    painter.setPen(wallPen);
    if (selectedRoomId == 7)
    {
        painter.setBrush(selectedRoomBrush);
    }
    else
    {
        painter.setBrush(roomBrush);
    }

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

    // EXITS
    QPen exitPen(QColor(200, 35, 35), 2);

    // exit 1
    painter.setPen(exitPen);
    painter.setBrush(isExitBlocked(201)
                         ? QBrush(QColor(255, 180, 180))
                         : QBrush(Qt::white));

    painter.drawRoundedRect(370, 114, 54, 22, 4, 4);
    painter.setPen(QColor(200, 35, 35));
    painter.drawText(QRect(370, 114, 54, 22),
                     Qt::AlignCenter, "EXIT 1");

    if (isExitBlocked(201))
    {
        painter.setPen(Qt::NoPen);

        // Outer flame
        QPolygon fire;
        fire << QPoint(397, 112)
            << QPoint(389, 119)
            << QPoint(391, 108)
            << QPoint(397, 100)
            << QPoint(403, 108)
            << QPoint(405, 119);

        painter.setBrush(QColor(230, 55, 10));
        painter.drawPolygon(fire);

        // Inner flame
        QPolygon innerFire;
        innerFire << QPoint(397, 115)
            << QPoint(392, 110)
            << QPoint(397, 104)
            << QPoint(402, 110);

        painter.setBrush(QColor(255, 170, 20));
        painter.drawPolygon(innerFire);
    }

    // exit 2
    painter.setPen(exitPen);
    painter.setBrush(isExitBlocked(202)
                         ? QBrush(QColor(255, 180, 180))
                         : QBrush(Qt::white));

    painter.drawRoundedRect(370, 444, 54, 22, 4, 4);
    painter.setPen(QColor(200, 35, 35));
    painter.drawText(QRect(370, 444, 54, 22),
                     Qt::AlignCenter, "EXIT 2");
    if (isExitBlocked(202))
    {
        painter.setPen(Qt::NoPen);

        // Outer flame
        QPolygon fire;
        fire << QPoint(397, 442)
            << QPoint(389, 449)
            << QPoint(391, 438)
            << QPoint(397, 430)
            << QPoint(403, 438)
            << QPoint(405, 449);

        painter.setBrush(QColor(230, 55, 10));
        painter.drawPolygon(fire);

        // Inner flame
        QPolygon innerFire;
        innerFire << QPoint(397, 445)
            << QPoint(392, 440)
            << QPoint(397, 434)
            << QPoint(402, 440);

        painter.setBrush(QColor(255, 170, 20));
        painter.drawPolygon(innerFire);
    }

    // Draw two adults and fire in the selected room
    if (selectedRoomId != -1)
    {
        QPoint personPosition;

        switch (selectedRoomId)
        {
        case 1:  personPosition = QPoint(110, 75); break;
        case 2:  personPosition = QPoint(275, 75); break;
        case 3:  personPosition = QPoint(435, 75); break;
        case 4:  personPosition = QPoint(650, 75); break;
        case 5:  personPosition = QPoint(650, 220); break;
        case 6:  personPosition = QPoint(650, 320); break;
        case 7:  personPosition = QPoint(650, 475); break;
        case 8:  personPosition = QPoint(435, 475); break;
        case 9:  personPosition = QPoint(275, 475); break;
        case 10: personPosition = QPoint(90, 475); break;
        case 11: personPosition = QPoint(90, 310); break;
        case 12: personPosition = QPoint(90, 210); break;
        default: break;
        }

        drawPerson(painter, personPosition.x(), personPosition.y(), QColor(60, 120, 200));

        drawPerson(painter, personPosition.x() + 30, personPosition.y(), QColor(190, 90, 140));

        // Draw a more realistic fire in the selected room
        painter.setPen(Qt::NoPen);

        // Outer flame
        QPolygon fire;
        fire << QPoint(personPosition.x() + 75, personPosition.y() + 25)
            << QPoint(personPosition.x() + 62, personPosition.y() + 12)
            << QPoint(personPosition.x() + 66, personPosition.y() - 2)
            << QPoint(personPosition.x() + 72, personPosition.y() + 5)
            << QPoint(personPosition.x() + 75, personPosition.y() - 12)
            << QPoint(personPosition.x() + 82, personPosition.y() + 4)
            << QPoint(personPosition.x() + 88, personPosition.y() - 2)
            << QPoint(personPosition.x() + 88, personPosition.y() + 12);

        painter.setBrush(QColor(230, 55, 10));
        painter.drawPolygon(fire);

        // Middle flame
        QPolygon innerFire;
        innerFire << QPoint(personPosition.x() + 75, personPosition.y() + 20)
            << QPoint(personPosition.x() + 68, personPosition.y() + 10)
            << QPoint(personPosition.x() + 75, personPosition.y() - 2)
            << QPoint(personPosition.x() + 82, personPosition.y() + 10);

        painter.setBrush(QColor(255, 170, 20));
        painter.drawPolygon(innerFire);

        // Bright center
        painter.setBrush(QColor(255, 240, 120));
        painter.drawEllipse(
            QPoint(personPosition.x() + 75, personPosition.y() + 12), 3, 5
        );
    }

    // Draw the calculated evacuation route
    if (!currentRoute.empty())
    {
        QPainterPath routePath;
        bool firstPoint = true;
        int previousLocationId = -1;

        for (int locationId : currentRoute)
        {
            QPoint point;

            switch (locationId)
            {
            case 201: point = QPoint(397, 125); break; // Exit 1
            case 202: point = QPoint(397, 445); break; // Exit 2

            case 101: point = QPoint(395, 155); break; // Hallway 1
            case 102: point = QPoint(595, 290); break; // Hallway 2
            case 103: point = QPoint(395, 420); break; // Hallway 3
            case 104: point = QPoint(195, 290); break; // Hallway 4

            case 1: point = QPoint(145, 155); break;
            case 2: point = QPoint(310, 125); break;
            case 3: point = QPoint(475, 125); break;
            case 4: point = QPoint(645, 155); break;
            case 5: point = QPoint(645, 240); break;
            case 6: point = QPoint(645, 340); break;
            case 7: point = QPoint(645, 490); break;
            case 8: point = QPoint(475, 490); break;
            case 9: point = QPoint(310, 490); break;
            case 10: point = QPoint(145, 490); break;
            case 11: point = QPoint(110, 340); break;
            case 12: point = QPoint(110, 240); break;

            default: continue;
            }

            if (firstPoint)
            {
                routePath.moveTo(point);
                firstPoint = false;
            }
            else
            {
                // Avoid crossing the central garden between hallways
                if (previousLocationId == 102 && locationId == 101)
                {
                    routePath.lineTo(QPoint(595, 155));
                    routePath.lineTo(QPoint(395, 155));
                }
                else if (previousLocationId == 101 && locationId == 102)
                {
                    routePath.lineTo(QPoint(395, 155));
                    routePath.lineTo(QPoint(595, 155));
                }
                else if (previousLocationId == 104 && locationId == 103)
                {
                    routePath.lineTo(QPoint(195, 290));
                    routePath.lineTo(QPoint(195, 420));
                }
                else if (previousLocationId == 103 && locationId == 104)
                {
                    routePath.lineTo(QPoint(195, 420));
                    routePath.lineTo(QPoint(195, 290));
                }
                else if (previousLocationId == 101 && locationId == 104)
                {
                    routePath.lineTo(QPoint(195, 155));
                    routePath.lineTo(QPoint(195, 290));
                }
                else if (previousLocationId == 104 && locationId == 101)
                {
                    routePath.lineTo(QPoint(195, 290));
                    routePath.lineTo(QPoint(195, 155));
                }
                else if (previousLocationId == 102 && locationId == 103)
                {
                    routePath.lineTo(QPoint(595, 290));
                    routePath.lineTo(QPoint(595, 420));
                }
                else if (previousLocationId == 103 && locationId == 102)
                {
                    routePath.lineTo(QPoint(595, 420));
                    routePath.lineTo(QPoint(595, 290));
                }
                else
                {
                    routePath.lineTo(point);
                }
            }

            previousLocationId = locationId;
        }

        QPen routePen(QColor(0, 170, 70), 7);
        routePen.setCapStyle(Qt::RoundCap);
        routePen.setJoinStyle(Qt::RoundJoin);

        painter.setPen(routePen);
        painter.setBrush(Qt::NoBrush);
        painter.drawPath(routePath);
    }
}
