#pragma once
#include <QPushButton>

class collectButton : public QPushButton
{
    Q_OBJECT

    private:
        QString textOfCollect;
        QString textOfUncollect;

    public:
        explicit collectButton(QWidget* parent = nullptr,const QString& collect="⭐",const QString& uncollect="☆");
        ~collectButton();
        /* 返回是否被收藏，被收藏返回 true */
        inline bool isCollect() const { return this->text()==textOfCollect; };

    protected:
        void mousePressEvent(QMouseEvent* e) override;

    public slots:
        void setCollect();
        void setUncollect();
        void setCollectNoSignal();
        void setUncollectNoSignal();

    signals:
        void collected();
        void uncollected();

    private:
        ;
};