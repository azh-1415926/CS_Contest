#pragma once

#include <QLabel>
#include <QMouseEvent>

class clickLabel : public QLabel {
    Q_OBJECT
    
public:
    explicit clickLabel(QWidget* parent = nullptr);
    ~clickLabel();

protected:
    void mousePressEvent(QMouseEvent* e) override;

signals:
    void clicked(clickLabel* label);

public slots:
    ;

private:
    ;
};