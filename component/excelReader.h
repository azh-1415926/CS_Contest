#pragma once

#include <QObject>
#include <QAxObject>

class excelReader : public QObject {
    Q_OBJECT

    public:
        explicit excelReader(QObject* parent=nullptr);
        ~excelReader();
        bool isRead() const;
        bool isReload() const;
        const QVector<QVector<QString>>& getData() const;
        int getRows() const;
        int getColumns() const;

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