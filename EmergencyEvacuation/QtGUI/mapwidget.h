#pragma once

#include <QWidget>

class MapWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MapWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};
