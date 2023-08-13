#include "excelReader.h"
#include <QProgressDialog>

excelReader::excelReader(QObject* parent)
    : QObject(parent)
    , rows(0)
    , columns(0)
    , readFlag(0)
    , reloadFlag(0)
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

bool excelReader::isReload()
{
    return reloadFlag==1;
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
    QProgressDialog process;
    process.setValue(0);
    process.setWindowTitle("Loading Excel file");
    process.setLabelText("loading");
    process.setModal(true);
    process.setRange(0,100);
    reloadFlag=(path.isEmpty()||path==pathOfExcel)?(0):(1);
    path=pathOfExcel;
    QAxObject *workbooks = excel->querySubObject("WorkBooks");
    process.setValue(5);
    workbooks->dynamicCall("Open (const QString&)", pathOfExcel);
    QAxObject *workbook=excel->querySubObject("ActiveWorkBook");
    process.setValue(10);
    QAxObject *worksheets=workbook->querySubObject("Sheets");
    process.setValue(20);
    QAxObject *worksheet=worksheets->querySubObject("Item(int)",1);
    process.setValue(30);
    // read data
    QAxObject *usedRange=worksheet->querySubObject("UsedRange");
    QVariant all=usedRange->dynamicCall("value");
    process.setValue(40);
    const QVariantList& rowsOflist=all.toList();
    const QVariantList& columnsOfList=rowsOflist.at(0).toList();
    rows=rowsOflist.length();
    columns=columnsOfList.length();
    // rows=usedRange->querySubObject("Rows")->property("Row").toInt();
    // columns=usedRange->querySubObject("Columns")->property("Columns").toInt();
    if(!data.isEmpty())
        data.clear();
    for(int i=0;i<rows;i++){
        QVector<QString> rowData;
        const QVariantList& var=rowsOflist.at(i).toList();
        for(int j=0;j<columns;j++){
            rowData.push_back(var.at(j).toString());
        }
        data.push_back(rowData);
        process.setValue(39+60/rows*i);
    }
    workbook->dynamicCall("Close()");
    // read end
    readFlag=1;
    emit readed();
    process.setValue(100);
}