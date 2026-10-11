#pragma once

class Hallway {
private:
    int locationAId;
    int locationBId;
    double distance;
    bool blocked;

public:
    Hallway(int firstLocationId, int secondLocationId, double hallwayDistance)
        : locationAId(firstLocationId),
        locationBId(secondLocationId),
        distance(hallwayDistance),
        blocked(false) {
    }

    int getLocationAId() const {
        return locationAId;
    }

    int getLocationBId() const {
        return locationBId;
    }

    double getDistance() const {
        return distance;
    }

    bool isBlocked() const {
        return blocked;
    }

    void setBlocked(bool isBlocked) {
        blocked = isBlocked;
    }
};