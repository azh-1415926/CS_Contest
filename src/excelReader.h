#pragma once

#include <QObject>
#include <QAxObject>

class excelReader : public QObject {
    Q_OBJECT

    public:
        explicit excelReader(QObject* parent=nullptr);
        ~excelReader();
        bool isRead();
        bool isReload();
        const QVector<QVector<QString>>& getData();
        int getRows();
        int getColumns();

    public slots:
        void readExcel(const QString& pathOfExcel);

    signals:
        void readed();

    private:
        int rows;
        int columns;
        int readFlag;
        int reloadFlag;
        QString path;
        QAxObject* excel;
        QVector<QVector<QString>> data;
};