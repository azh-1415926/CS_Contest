#include "excelReader.h"



excelReader::excelReader()
    : excel(new QAxObject("Excel.Application"))
{
    // not show the window
    excel->dynamicCall("SetVisible (bool Visible)","false");
    // // not show warnning
    // excel->setProperty("DisplayAlerts", false);
}

excelReader::~excelReader()
{
    excel->dynamicCall("Quit(void)");
    delete excel;
}

bool excelReader::readExcel(const QString &pathOfExcel)
{

    if(pathOfExcel.isEmpty())
        return false;
    QAxObject *workbooks = excel->querySubObject("WorkBooks");
    workbooks->dynamicCall("Open (const QString&)", pathOfExcel);
    QAxObject *workbook=excel->querySubObject("ActiveWorkBook");
    QAxObject *worksheets=workbook->querySubObject("Sheets");
    QAxObject *worksheet=worksheets->querySubObject("Item(int)",1);
    // read data
    QAxObject *usedRange=worksheet->querySubObject("UsedRange");
    QVariant all=usedRange->dynamicCall("value");
    rows=usedRange->querySubObject("Rows")->property("Row").toInt();
    columns=usedRange->querySubObject("Columns")->property("Columns").toInt();
    foreach(QVariant varOfRows,all.toList()){
        QVector<QString> rowData;
        foreach(QVariant var,varOfRows.toList()){
            rowData.push_back(var.toString());
        }
        data.push_back(rowData);
    }
    workbook->dynamicCall("Close()");
    return true;
}

const QVector<QVector<QString>> &excelReader::getData()
{
    return this->data;
}
