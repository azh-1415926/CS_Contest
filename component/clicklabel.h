#pragma once
#include <QLabel>
#include <QMouseEvent>

class clickLabel : public QLabel
{
    Q_OBJECT

    private:
        ;

    public:
        explicit clickLabel(QWidget* parent = nullptr);
        ~clickLabel();

    protected:
        void mousePressEvent(QMouseEvent* e) override;

    public slots:
        ;

    signals:
        void clicked(const clickLabel*);

    private:
        ;
};