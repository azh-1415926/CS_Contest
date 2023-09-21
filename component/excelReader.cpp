#include "excelReader.h"

#include <QProgressDialog>
#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <QRegularExpressionValidator>
#include <QDebug>

excelReader::excelReader(QObject* parent)
    : QObject(parent)
    , rows(0), columns(0)
    , readFlag(0), reloadFlag(0)
    #ifdef _WIN32
    , excel(new QAxObject("Excel.Application"))
    #endif
{
    /* 不显示 excel 进程的窗口 */
    #ifdef _WIN32
    excel->dynamicCall("SetVisible (bool Visible)","false");
    /* 忽略所有警告退出（比如不保存退出）*/
    excel->setProperty("DisplayAlerts", false);
    #endif
}

excelReader::~excelReader()
{
    #ifdef _WIN32
    /* 退出 excel 进程，并释放内存 */
    excel->dynamicCall("Quit(void)");
    delete excel;
    #endif
}

/* 读取指定路径的 excel 文件 */
void excelReader::readExcel(const QString& pathOfExcel)
{
    #ifdef _WIN32
    /* 路径为空立即返回 */
    if(pathOfExcel.isEmpty())
        return;
    /* 显示进度条对话框，标题为 Loading Excel file，文本为 loading */
    QProgressDialog process;
    process.setValue(0);
    process.setWindowTitle("Loading Excel file");
    process.setLabelText("loading");
    /* 设置为模态，数值范围为 0-100 */
    process.setModal(true);
    /*
        半模态
        process.setWindowModality(Qt::WindowModal);
    */
    process.setRange(0,100);
    /*
        reloadFlag 是导入新文件的标记，为 1 代表导入了新文件
        在导入新文件时会被置为 1，第一次导入文件除外
        我们用 path 记录上一次导入文件的路径，导入文件便会先与 path 作比较。若不一样会被置为 1，path 为空则除外
    */
    reloadFlag=(path.isEmpty()||path==pathOfExcel)?(0):(1);
    /* 若当前导入的路径与上一次路径一致，则没必要再读取一次，直接返回 */
    if(path==pathOfExcel)
        return;
    /* 更新 path 为当前导入的 excel 文件路径 */
    path=pathOfExcel;
    /* workbooks 获取 excel 进程中工作簿集合，进度置为 5% */
    QAxObject *workbooks = excel->querySubObject("WorkBooks");
    process.setValue(5);
    /* workbooks 打开新的工作簿，路径为导入文件的路径，Qt5 这样打开中文路径的文件会崩溃 */
    workbooks->dynamicCall("Open (const QString&)", pathOfExcel);
    /* workbook 指向当前活跃的工作簿，进度置为 10% */
    QAxObject *workbook=excel->querySubObject("ActiveWorkBook");
    process.setValue(10);
    /* worksheets 指向活跃工作簿的工作表集合，进度置为 20% */
    QAxObject *worksheets=workbook->querySubObject("Sheets");
    process.setValue(20);
    /* 我们只有一个工作表，故选择第一个工作表，用 worksheet 接收，进度置为 30% */
    QAxObject *worksheet=worksheets->querySubObject("Item(int)",1);
    process.setValue(30);
    /* 读取该工作表中所有数据 */
    QAxObject *usedRange=worksheet->querySubObject("UsedRange");
    /* all 接收所有数据，进度置为 40% */
    QVariant all=usedRange->dynamicCall("value");
    process.setValue(40);
    /* rowsOflist 存储所有行数据，columnsOfList 存储第 0 行的数据，用于获取长度 */
    const QVariantList& rowsOflist=all.toList();
    const QVariantList& columnsOfList=rowsOflist[0].toList();
    /* 获取行数和列数 */
    rows=rowsOflist.length();
    columns=columnsOfList.length();
    /*
        获取 行数和列数，但这样效率比较低
        rows=usedRange->querySubObject("Rows")->property("Row").toInt();
        columns=usedRange->querySubObject("Columns")->property("Columns").toInt();
    */
    /* 若之前读过数据，那么 data 不为空，则需要清空 */
    if(!data.isEmpty())
        data.clear();
    /* 读取所有数据并存进 data 数组 */
    for(int i=0;i<rows;i++)
    {
        /* 存储第 i 行的行数据 */
        QVector<QString> columnData;
        /* var 指向第 i 行的列数据 */
        const QVariantList& var=rowsOflist[i].toList();
        for(int j=0;j<columns;j++)
            /* 将字符串格式的列数据存入数组 */
            columnData.push_back(var[j].toString());
        /* 将第 i 行的列数据数组存入 data 数组，更新进度条 */
        data.push_back(columnData);
        process.setValue(39+60/rows*i);
    }
    /* 关闭当前的工作表 */
    workbook->dynamicCall("Close()");
    /* 将读取标记置为 1，表明已读取数据，将进度置为 100%。发送 readed 信号，告诉其他窗口数据读取完毕 */
    readFlag=1;
    process.setValue(100);
    #ifdef QT_DEBUG
    importCSV("2022.csv");
    importTestCSV();
    #endif
    emit readed();
    if(reloadFlag==1)
        emit reload();
    #endif
}

/* 读取指定路径的 CSV 文件 */
void excelReader::readCSV(const QString &pathOfCSV)
{
    /* 路径为空立即返回 */
    if(pathOfCSV.isEmpty())
        return;
    reloadFlag=(path.isEmpty()||path==pathOfCSV)?(0):(1);
    /* 若当前导入的路径与上一次路径一致，则没必要再读取一次，直接返回 */
    if(path==pathOfCSV)
        return;
    /* 更新 path 为当前导入的 excel 文件路径 */
    path=pathOfCSV;
    QFile file(pathOfCSV);
    /* 只读、转换行尾结束符为本地格式 */
    if(file.open(QIODevice::ReadOnly|QFile::Text))
    {
        /* 若之前读过数据，那么 data 不为空，则需要清空 */
        if(!data.isEmpty())
            data.clear();
        QTextStream stream(&file);
        QString line;
        #if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
            stream.setCodec("utf-8");
        #endif
        /* 读取所有行，并插入到 data 中 */
        while(!stream.atEnd())
        {
            line=stream.readLine();
            QRegularExpression reg("^\\d{1,4}AZH{1}[C,D,H,J,K,L,W,Y,Z,Q,R,S,1-6]{1}AZH+[\\s\\S]*");
            QRegularExpressionValidator v(reg,0);
            int pos=0;
            QValidator::State result=v.validate(line,pos);
            QVector<QString> vec=line.split("AZH").toVector();
            if(result!=QValidator::State::Acceptable)
            {
                if(data.isEmpty())
                    data.push_back(vec);
                else
                {
                    QVector<QString>& temp=data[data.length()-1];
                    if(!temp.isEmpty())
                    {
                        temp[temp.length()-1].append(vec.constFirst());
                        vec.removeFirst();
                    }
                    if(!vec.isEmpty())
                        temp.append(vec);
                }
            }
            else
                data.push_back(vec);
        }
        rows=data.length();
        columns=data[0].length();
        #ifdef QT_DEBUG
        importTestCSV();
        #endif
        file.close();
        readFlag=1;
        emit readed();
        if(reloadFlag==1)
            emit reload();
    }
}

void excelReader::importCSV(const QString &pathOfCSV)
{
    /* 路径为空、文件未读取立即返回 */
    if(pathOfCSV.isEmpty()||!isRead()||!pathOfCSV.endsWith(".csv"))
        return;
    QFile file(pathOfCSV);
    if (file.open(QIODevice::WriteOnly | QFile::Text))
    {
        QTextStream text(&file);
        #if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
            text.setCodec("utf-8");
        #endif
        for(int i=0;i<rows;i++)
        {
            for(int j=0;j<columns;j++)
                text<<data[i][j]<<(j!=columns-1?"AZH":"");
            text <<"\n";
        }
        file.close();
    }
}

#ifdef QT_DEBUG
/* 将数据处理错误的数据导出到 test.csv */
void excelReader::importTestCSV()
{
    QFile testFile("test.csv");
    if (testFile.open(QIODevice::WriteOnly | QFile::Text))
    {
        QTextStream text(&testFile);
        #if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
            text.setCodec("utf-8");
        #endif
        for(int i=0;i<data.length();i++)
        {
            int option=data[i].length()!=9?data[i][7].trimmed()[0].toUpper().unicode()-'A':1;
            if(option<0||option>=4||data[i].length()!=9)
            {
                for(int j=0;j<data[i].length();j++)
                    text<<data[i][j]<<(j!=data[i].length()-1?"AZH":"");
                text <<"\n";
                if(option<0||option>=4)
                    qDebug()<<"index of "<<i<<" 's question answer error,is "<<option;
                else    
                    qDebug()<<"index of "<<i<<" 's question length error,is "<<data[i].length();
            }
        }
        testFile.close();
    }
}
#endif