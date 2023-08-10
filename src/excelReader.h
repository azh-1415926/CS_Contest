#pragma once

#include <QObject>
#include <QAxObject>

class excelReader {
    public:
        excelReader();
        ~excelReader();
        bool readExcel(const QString& pathOfExcel);
        const QVector<QVector<QString>>& getData();

    private:
        int rows;
        int columns;
        QAxObject* excel;
        QVector<QVector<QString>> data;
};