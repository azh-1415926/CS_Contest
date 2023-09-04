#pragma once

#include <QGroupBox>
#include <QEvent>
#include <QRadioButton>

#include "clicklabel.h"

class clickOptions : public QGroupBox
{
    Q_OBJECT

    private:
        int numOfOptions;
        int answerOfOptions;
        int hoverOption;
        int checkedOption;
        QList<QRadioButton*> buttons;
        QList<clickLabel*> labels;
        QRect* hoverBox;
        QRect* correctBox;
        QRect* incorrectBox; 

    public:
        explicit clickOptions(QWidget* parent = nullptr);
        ~clickOptions();
        /* 返回答案选项的下标 */
        inline int getAnswer() const { return this->answerOfOptions; };

    protected:
        bool eventFilter(QObject *obj, QEvent *e) override;

    public slots:
        void setTextOfOption(int i,const QString& text);
        void setAnswer(int i);
        void displayAnswer(bool state);
        void resetOption();

    signals:
        void selectOption(int);

    private:
        void initalOptions();
        void initalEvent();
        void paintBox(QWidget* widget,QRect* box);
        void freeBoxes();
        QRect* setOptionOfBox(int i,QRect* rect=nullptr);
};