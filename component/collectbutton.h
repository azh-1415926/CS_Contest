#pragma once

#include <QPushButton>

class collectButton : public QPushButton {
    Q_OBJECT
    
public:
    explicit collectButton(QWidget* parent = nullptr,const QString& collect="⭐",const QString& uncollect="☆");
    ~collectButton();
    bool isCollect();

protected:
    void mousePressEvent(QMouseEvent* e) override;

signals:
    void collected();
    void uncollected();

public slots:
    void setCollect();
    void setUncollect();

private:
    QString textOfCollect;
    QString textOfUncollect;
};