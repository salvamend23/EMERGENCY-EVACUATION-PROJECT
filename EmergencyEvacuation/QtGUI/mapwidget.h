#pragma once
#include <QWidget>
#include "Building.h"
#include <set>

class MapWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MapWidget(Building& building, QWidget *parent = nullptr);
    void setHallwayBlocked(int hallwayId, bool blocked);
    bool isHallwayBlocked(int hallwayId) const;
    void setSelectedRoom(int roomId);
    void setRoute(const std::vector<int>& route);
    void setExitBlocked(int exitId, bool blocked);
    bool isExitBlocked(int exitId) const;
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Building& building;
    std::set<int> blockedHallways;
    std::set<int> blockedExits;
    int selectedRoomId = -1;
    std::vector<int> currentRoute;
};
