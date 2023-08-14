#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include "collectbutton.h"

class switchQuestion : public QWidget {
    Q_OBJECT
    
public:
    explicit switchQuestion(QWidget* parent = nullptr);
    ~switchQuestion();

protected:
    ;

public slots:
    void setTitle(const QString& title);
    void setIndex(int i);
    void setSum(int n);
    void setTextOfIndex(const QString& str);
    void setTextOfSum(const QString& str);
    void setCollect(bool status);

signals:
    void collectQuestion();
    void uncollectQuestion();
    void changeQuestion(int i);

private:
    int index;
    int sum;
    QPushButton* forwardBtn;
    QPushButton* nextBtn;
    collectButton* collectBtn;
    QLabel* textOfTag;
    QLabel* textOfIndex;
    QLabel* textOfSum;
    void inital();
};