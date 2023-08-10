#include "excelReader.h"

excelReader::excelReader(QObject* parent)
    : QObject(parent)
    , rows(0)
    , columns(0)
    , readFlag(0)
    , excel(new QAxObject("Excel.Application"))
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

bool excelReader::isRead()
{
    return readFlag==1;
}

const QVector<QVector<QString>> &excelReader::getData()
{
    return this->data;
}

int excelReader::getRows()
{
    return rows;
}

int excelReader::getColumns()
{
    return columns;
}

void excelReader::readExcel(const QString& pathOfExcel)
{

    if(pathOfExcel.isEmpty()){
        return;
    }
    QAxObject *workbooks = excel->querySubObject("WorkBooks");
    workbooks->dynamicCall("Open (const QString&)", pathOfExcel);
    QAxObject *workbook=excel->querySubObject("ActiveWorkBook");
    QAxObject *worksheets=workbook->querySubObject("Sheets");
    QAxObject *worksheet=worksheets->querySubObject("Item(int)",1);
    // read data
    QAxObject *usedRange=worksheet->querySubObject("UsedRange");
    QVariant all=usedRange->dynamicCall("value");
    const QVariantList& rowsOflist=all.toList();
    const QVariantList& columnsOfList=rowsOflist.at(0).toList();
    rows=rowsOflist.length();
    columns=columnsOfList.length();
    // rows=usedRange->querySubObject("Rows")->property("Row").toInt();
    // columns=usedRange->querySubObject("Columns")->property("Columns").toInt();
    for(int i=0;i<rows;i++){
        QVector<QString> rowData;
        const QVariantList& var=rowsOflist.at(i).toList();
        for(int j=0;j<columns;j++){
            rowData.push_back(var.at(j).toString());
        }
        data.push_back(rowData);
    }
    workbook->dynamicCall("Close()");
    // read end
    emit readed();
    readFlag=1;
}