#pragma once

#include <QGroupBox>
#include <QEvent>
#include <QRadioButton>
#include "clicklabel.h"

class clickOptions : public QGroupBox {
    Q_OBJECT
    
public:
    explicit clickOptions(QWidget* parent = nullptr);
    ~clickOptions();

protected:
    bool eventFilter(QObject *obj, QEvent *e) override;

public slots:
    void setTextOfOption(int i,const QString& text);

signals:
    ;

private:
    int numOfOptions;
    QList<QRadioButton*> buttons;
    QList<clickLabel*> labels;
    QRect* hoverBox;
    QRect* correctBox;
    QRect* incorrectBox;
    void initalOptions();
    void initalEvent();
    void paintBox(QWidget* widget,QRect* box);
};