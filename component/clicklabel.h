#pragma once

#include <QLabel>
#include <QMouseEvent>

class clickLabel : public QLabel {
    Q_OBJECT
    
public:
    explicit clickLabel(QWidget* parent = nullptr);
    ~clickLabel();

signals:
    void clicked(clickLabel* label);

public slots:
    ;

protected:
    void mousePressEvent(QMouseEvent* e) override;

private:
    ;
};