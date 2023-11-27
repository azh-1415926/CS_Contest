#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class collectButton;
class clickOptions;

class switchQuestion : public QWidget {
    Q_OBJECT

    private:
        int indexOfQuestion;
        int sumOfQuestion;
        QPushButton* forwardBtn;
        QPushButton* nextBtn;
        collectButton* collectBtn;
        QLabel* textOfTag;
        QLabel* textOfIndex;
        QLabel* textOfSum;
    public:
        explicit switchQuestion(QWidget* parent = nullptr);
        ~switchQuestion();
        /* 返回当前组件的标题 */
        inline QString title() const { return this->textOfTag->text(); }
        /* 返回当前问题的下标 */
        inline int index() const { return this->indexOfQuestion; }
        /* 返回当前问题下标对应标签上的字符串文本 */
        inline QString stringOfIndex() const { return this->textOfIndex->text(); }
        /* 返回当前问题的总数 */
        inline int count() const { return this->sumOfQuestion; }

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
        void lastIndex(int i);

    private:
        void initalQuestion();
};