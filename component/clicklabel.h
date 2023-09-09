#pragma once
#include <QLabel>

#ifndef __ANDROID__
#include <QMouseEvent>
#else
#include <QTouchEvent>
#endif

class clickLabel : public QLabel
{
    Q_OBJECT

    private:
        ;

    public:
        explicit clickLabel(QWidget* parent = nullptr);
        ~clickLabel();

    protected:
        #ifndef __ANDROID__
        void mousePressEvent(QMouseEvent* e) override;
        #else
        bool touchBeginEventProcess(QTouchEvent *event) override;
        bool touchUpdateEventProcess(QTouchEvent *event) override;
        bool touchEndEventProcess(QTouchEvent *event) override;
        #endif

    public slots:
        ;

    signals:
        void clicked(const clickLabel*);

    private:
        ;
};