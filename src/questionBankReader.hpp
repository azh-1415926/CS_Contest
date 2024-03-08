#ifndef QUESTION_BANK_READER
#define QUESTION_BANK_READER

#include <excelReader.h>

#include <QFile>
#include <QTextStream>
#include <QDebug>

class questionBankReader : public excelReader
{
    private:

    public:
        questionBankReader(excelReader* parent=nullptr): excelReader(parent) {  }
        ~questionBankReader() {  }

    public slots:
        void readCSV(const QString& pathOfCSV);
        void importCSV(const QString& pathOfCSV);

    private:
        #ifdef QT_DEBUG
            void importTestCSV();
        #endif
        
};

/* 读取指定路径的 CSV 文件 */
void questionBankReader::readCSV(const QString &pathOfCSV)
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
        // importTestCSV();
        #endif
        file.close();
        readFlag=1;
        emit readed();
        if(reloadFlag==1)
            emit reload();
    }
}

void questionBankReader::importCSV(const QString &pathOfCSV)
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
void questionBankReader::importTestCSV()
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

#endif /* QUESTION_BANK_READER */