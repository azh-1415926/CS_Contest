#include "widgetOfStart.h"

#include <QRect>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTimer>

widgetOfStart::widgetOfStart(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui_widgetOfStart)
    , reader(new excelReader)
    , currTypeOfQuestion(0)
    , flagOfInital(0)
{
    ui->setupUi(this);
    initalStackWindow();
    initalQuestionPage();
    initalSelectionPage();
    initalCollectionPage();
}

widgetOfStart::~widgetOfStart()
{
    delete ui;
    /* 释放读写模块 */
    delete reader;
}

void widgetOfStart::resizeEvent(QResizeEvent *)
{
    /* 窗口大小调整，调用 loadSetting()，调用过程中会将 flagOfInital 置为 1（即只会在窗口第一次显示的时候调用） */
    if(flagOfInital==0)
        importSetting();
}

/* 保存当前答题信息 */
void widgetOfStart::exportSetting()
{
    /* 若导入文件路径为空，不保存配置，直接返回 */
    if(pathOfExcel.isEmpty())
        return;
    /* 
        json 作为 QJson 对象的根节点
        arrayOfQuestionProcess 数组存储答题页答题进度信息
        arrayOfCollectProcess 数组存储收藏页答题进度信息
        doc 存储 json 文件对象
    */
    QJsonObject json;
    QJsonArray arrayOfQuestionProcess;
    QJsonArray arrayOfCollectProcess;
    QJsonDocument doc;
    /* 默认配置文件路径为当前目录下的 settings.json 文件 */
    QFile file("./settings.json");
    /* 将当前文件路径、当前答题页题型下标导入 */
    json.insert("pathOfExcel",pathOfExcel);
    json.insert("currTypeOfQuestion",currTypeOfQuestion);
    /*
        将当前答题页答题进度导入
        （导出数组长度与当前数组长度一致，为题型数量，存储的是对应题型的题号下标数组）
    */
    for(int i=0;i<progressOfQuestion.length();i++)
        arrayOfQuestionProcess.push_back(progressOfQuestion[i]);
    json.insert("progressOfQuestion",arrayOfQuestionProcess);
    /*
        将当前收藏页答题进度导入
        （导出数组长度与当前数组长度不一致，导出长度为 2，存储着对应题目的题型下标数组、题号下标数组）
        first 暂存题型下标
        second 暂存题目下标
    */
    QJsonArray first;
    QJsonArray second;
    /* progressOfCollection 数组长度为收藏总数，每一项是一个 QPair<int,int>，对应题目的题型下标、题号下标 */
    for(int i=0;i<progressOfCollection.length();i++)
    {
        first.push_back(progressOfCollection[i].first);
        second.push_back(progressOfCollection[i].second);
    }
    arrayOfCollectProcess.push_back(first);
    arrayOfCollectProcess.push_back(second);
    json.insert("progressOfCollection",arrayOfCollectProcess);
    /* 保存到 settings.json 文件中（只写、截断保存）  */
    if(file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        QTextStream stream(&file);
        #if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
            stream.setCodec("utf-8");
        #elif (QT_VERSION < QT_VERSION_CHECK(6,0,0))
            stream.setEncoding(QStringConverter::Utf8);
        #endif
        doc.setObject(json);
        stream<<doc.toJson();
        file.close();
    }
    else
        QMessageBox::warning(nullptr,"error","save \"settings.json\" is failed!");
}

/* 导入答题信息（只执行一次） */
void widgetOfStart::importSetting()
{
    /* 若已初始化，则返回（即该函数只调用一次） */
    if(flagOfInital==1)
        return;
    /* json 节点接收转化的 QJson 对象，doc 用于存储导入的 json 文件对象 */
    QJsonObject json;
    QJsonDocument doc;
    QFile file("./settings.json");
    /* 只读、转换行尾结束符为本地格式 */
    if(file.open(QIODevice::ReadOnly|QFile::Text))
    {
        /*
            error 用于接收解析错误
            stream 用于读取文件
            str 存储字符串格式的 json 文件
        */
        QJsonParseError error;
        QTextStream stream(&file);
        QString str=stream.readAll();
        file.close();
        /* 将 json 字符串解析为 QJson 文件对象，并存储错误信息 */
        doc=QJsonDocument::fromJson(str.toUtf8(),&error);
        if(error.error!=QJsonParseError::NoError&&!doc.isNull())
        {
            QMessageBox::warning(nullptr,"json parse error","json 格式错误!");
            return;
        }
        /* json 接收 QJson 文件对象里的 QJson 根节点 */
        json=doc.object();
        /* 导入题库文件路径，选择页显示题库路径 */
        pathOfExcel=json.value("pathOfExcel").toString();
        ui->textOfPath->setText(pathOfExcel);
        /* 导入当前答题页题型下标 */
        currTypeOfQuestion=json.value("currTypeOfQuestion").toInt();
        /* 导入答题页答题进度 */
        QJsonArray arrayOfQuestionProcess=json.value("progressOfQuestion").toArray();
        for(int i=0;i<arrayOfQuestionProcess.count();i++)
            progressOfQuestion.push_back(arrayOfQuestionProcess[i].toInt());
        /* 导入收藏页题目数据 */
        QJsonArray arrayOfCollectProcess=json.value("progressOfCollection").toArray();
        const QJsonArray& first=arrayOfCollectProcess[0].toArray();
        const QJsonArray& second=arrayOfCollectProcess[1].toArray();
        for(int i=0;i<first.count();i++)
            progressOfCollection.push_back(QPair<int,int>(first[i].toInt(),second[i].toInt()));
        /*
            更新 switchOfCollection 的题目总数
            读取配置文件中导入的题库文件路径
        */
        emit updateSumOfCollection(progressOfCollection.length());
        emit loadExcel(pathOfExcel);
    }
    /* 启动定时器，每隔一秒调用 saveSetting() 槽函数，将初始化标志位置为 1（表明已被初始化） */
    QTimer* timer=new QTimer(this);
    connect(timer,&QTimer::timeout,this,&widgetOfStart::exportSetting);
    timer->start(1000);
    flagOfInital=1;
}

/* 处理导入的题库数据 */
void widgetOfStart::handleData()
{
    /* 列出题型所代表字符，用其下标代表该类题型 */
    char charOfType[]=
    {
        /* 计算机应用基础 数据结构 数据库原理 */
        'C','J','K',
        /* 网络 软件工程 操作系统 */
        'W','R','Z',
        /* 多媒体技术 硬件系统 移动互联网应用 */
        'D','Y','H',
        /* 数据表示和计算 离散数学 知识产权*/
        'S','L','Q',
        /* 1-6 C,C++,Java,JavaScript,C#,Python */
        '1','2','3','4','5','6'
    };
    /* 列出题型字符对应的题型名称 */
    QString stringOfType[]=
    {
        "计算机应用基础","数据结构","数据库原理","网络","软件工程","操作系统",
        "多媒体技术","硬件系统","移动互联网应用","数据表示和计算","离散数学","知识产权",
        "C","C++","Java","JavaScript","C#","Python"
    };
    /* sumOfType 为题型总数 */
    int sumOfType=sizeof(charOfType)/sizeof(char);
    /*
        questionType 数组存储着 QPair<QString,QList<int>>
        其中 questionType[i].first 是题型下标为 i 的题型名称
        其中 questionType[i].second[0] 是题型下标为 i 的题号总数
        其中 questionType[i].second[1-n] 是题型下标为 i 的题号
    */
    if(!questionType.isEmpty())
    {
        /* 若题型数组不为空，说明可能导入过其他题库，需要重新统计对应题型的题目总数（即将重置题型名称、题号总数置为 0） */
        for(int i=0;i<sumOfType;i++){
            questionType[i].first=stringOfType[i];
            questionType[i].second.clear();
            questionType[i].second.push_back(0);
        }
    }
    else
    {
        /* 插入数量为 sumOfType 的题型名称和题号数组，题号数组首位为题号总数，初始化为 0 */
        for(int i=0;i<sumOfType;i++)
        {
            QList<int> num;
            num.push_back(0);
            questionType.push_back(QPair<QString,QList<int>>(stringOfType[i],num));
        }
    }
    /* 选择页的展示，行数为题型总数，列数为 3，对应题型名称、对应题型字符、对应题型的题号总数 */
    ui->tableOfQuestionType->setColumnCount(3);
    ui->tableOfQuestionType->setRowCount(sumOfType);
    /* 设置选择页的展示为不可编辑 */
    ui->tableOfQuestionType->setEditTriggers(QAbstractItemView::NoEditTriggers);
    /* 统计读取到的所有题目数据，并统计对应题型的题号数量 */
    for(int i=0;i<reader->getData().length();i++)
    {
        for(int j=0;j<sumOfType;j++)
        {
            /* 若题型字符符合，则将题目真实的行下标存储到数组 */
            if(reader->getData()[i][1]==charOfType[j])
            {
                questionType[j].second.push_back(i);
                break;
            }
        }
    }
    /* 当重新读取题库的时候清空所有答题进度 */
    if(reader->isReload()||progressOfQuestion.isEmpty())
    {
        progressOfQuestion.clear();
        if(reader->isReload())
            progressOfCollection.clear();
        /* 答题页答题进度默认为第一题 */
        for(int i=0;i<sumOfType;i++)
            progressOfQuestion.push_back(0);
    }
    /* 展示所有题型名称、所代表字符、题号总数 */
    for(int i=0;i<sumOfType;i++)
    {
        /* 更新每种题型的题号总数（剔除第一位，首位存储长度） */
        questionType[i].second[0]=questionType[i].second.length()-1;
        ui->tableOfQuestionType->setItem(i,0,new QTableWidgetItem(questionType[i].first));
        ui->tableOfQuestionType->setItem(i,1,new QTableWidgetItem(QString(charOfType[i])));
        ui->tableOfQuestionType->setItem(i,2,new QTableWidgetItem(QString::number(questionType[i].second[0])));
    }
    /* 重设下拉菜单，将默认选中的选项置为从配置文件中读取到的 currTypeOfQuestion */
    int currType=currTypeOfQuestion;
    if(reader->isReload())
        ui->questionType->clear();
    if(ui->questionType->count()==0)
        for(int i=0;i<sumOfType;i++)
            ui->questionType->addItem(stringOfType[i]);
    ui->questionType->setCurrentIndex(currType);
    /* 处理完数据发送 ready 信号 */
    emit ready();
}

/* 更新题型 */
void widgetOfStart::setQuestionType(int i)
{
    if(i<0||i>=progressOfQuestion.length())
        return;
    currTypeOfQuestion=i;
    /* 从进度数组中取出对应题型的题号下标 */
    int currIndexOfQuestion=progressOfQuestion[currTypeOfQuestion];
    /* 更新 switchOfQuestion 的题目总数和当前题号，并显示该题目 */
    emit updateSumOfQuestion(questionType[currTypeOfQuestion].second[0]);
    emit updateIndexOfQuestion(currIndexOfQuestion);
    showQuestionByIndex(currIndexOfQuestion);
}

/* 显示答题页题目信息 */
void widgetOfStart::showQuestionByIndex(int i)
{
    /* 若未读取文件则立即返回 */
    if(!reader->isRead())
        return;
    /* 重置 optionsOfQuestion 的状态（清除正确选框、当前选项） */
    ui->optionsOfQuestion->resetOption();
    /*
        所读取的数据位于 reader 模块中
        题号从 1开始，0 位置为没有题目，故题号下标需要加一
        并将当前显示题目的题号下标存入进度数组
    */
    const QVector<QVector<QString>>& data=reader->getData();
    /* 从 questionType 数组中取出真实题号 */
    int index=questionType[currTypeOfQuestion].second[i+1];
    progressOfQuestion[currTypeOfQuestion]=i;
    /* 题目的内容，位于第 2 列的单元格 */
    ui->textOfQuestion->setText(QString::number(i+1)+". "+data[index][2]);
    /* 题目的选项，位于第 3、4、5、6 列的单元格 */
    emit updateTextOfQuestion(0,data[index][3]);
    emit updateTextOfQuestion(1,data[index][4]);
    emit updateTextOfQuestion(2,data[index][5]);
    emit updateTextOfQuestion(3,data[index][6]);
    /* 题目的答案，位于第 7 列的单元格，为 A、B、C、D，将其转化为 0、1、2、3，更新 optionsOfQuestion 模块的答案*/
    emit updateAnswerOfQuestion(data[index][7][0].unicode()-'A');
    /* 将该题目在收藏页的答题进度中查找，查找失败 status 的值为 -1 */
    int status=progressOfCollection.indexOf(QPair<int,int>(currTypeOfQuestion,i));
    /* 若该题在收藏列表中，将该题标记为收藏，即发送 isColllect(true) 信号 */
    emit isCollect(status!=-1);
}

/* 显示收藏页题目信息 */
void widgetOfStart::showCollectionByIndex(int i)
{
    /* 若未读取文件则立即返回 */
    if(!reader->isRead())
        return;
    /* 重置 optionsOfCollection 的状态（清除正确选框、当前选项） */
    ui->optionsOfCollection->resetOption();
    const QVector<QVector<QString>>& data=reader->getData();
    /* 从答题进度数组中读取 QPair<int,int>，first 对应题型下标，second 对应题号下标 */
    int row=progressOfCollection[i].first;
    int column=progressOfCollection[i].second;
    int index=questionType[row].second[column+1];
     /* 题目的内容，上面已做解释，就不重复赘述了 */
    ui->textOfCollection->setText(QString::number(i+1)+". "+data[index][2]);
    /* 题目的选项 */
    emit updateTextOfCollection(0,data[index][3]);
    emit updateTextOfCollection(1,data[index][4]);
    emit updateTextOfCollection(2,data[index][5]);
    emit updateTextOfCollection(3,data[index][6]);
    /* 题目的答案 */
    emit updateAnswerOfCollection(data[index][7][0].unicode()-'A');
    /* 收藏页的收藏按钮状态默认是已收藏 */
    ui->switchOfCollection->setCollect(true);
}

/* 获取用户选择 excel 文件的路径 */
void widgetOfStart::getPath()
{
    /* 弹出选择文件的窗口，过滤出 excel 文件 */
    const QString& filepath=QFileDialog::getOpenFileName(this, QStringLiteral("select excel file"), "",QStringLiteral("Exel file(*.xls *.xlsx)"));
    /* 获取路径成功，便赋给 pathOfExcel，并更新选择页的路径显示，并发送 loadExcel(pathOfExcel) 信号读取文件 */
    if(!filepath.isEmpty())
    {
        pathOfExcel=filepath;
        ui->textOfPath->setText(pathOfExcel);
        emit loadExcel(pathOfExcel);
    }
}

/* 初始化多页窗口 */
void widgetOfStart::initalStackWindow()
{
    QStackedWidget* windows=this->ui->stackOfWindows;
    /* 三个按钮分别对应三个窗口，将按钮的点击事件和对应窗口的显示连接在一起 */
    QPushButton* buttons[]=
    {
        ui->buttonOfQuestion,
        ui->buttonOfSelect,
        ui->buttonOfCollect
    };
    QWidget* pages[]=
    {
        ui->pageOfQuestion,
        ui->pageOfSelection,
        ui->pageOfCollection
    };
    for(int i=0;i<sizeof(buttons)/sizeof(QPushButton*);i++)
    {
        connect(buttons[i],&QPushButton::clicked,this,[windows,buttons,pages,i]()
        {
            windows->setCurrentIndex(windows->indexOf(pages[i]));
        });
    }
}

/* 初始化答题页 */
void widgetOfStart::initalQuestionPage()
{
    /* 实现点击正确选项跳转到下一题  */
    connect(ui->optionsOfQuestion,&clickOptions::selectOption,this,[=](int option)
    {
        if(!reader->isRead())
            return;
        /*
            optionsOfQuestion 返回当前被选择的选项
            option 为接收到的选项下标
            preOption 为上一次接收到的选项
            n 为答题结束后，点击正确答案可跳转到下一题的点击次数，我们设为 1
        */
        static int preOption;
        static int n;
        /* 获取当前题号 currIndexOfQuestion */
        int currIndexOfQuestion=progressOfQuestion[currTypeOfQuestion];
        /*
            若当前选项和上一次选择的选项不一样，需要重置 n 为 1
            或当前选项不等于答案，需要重置 n 为 1，直到选择了正确答案才置为 0
            
            这样会导致一种情况，若你答完上一道题，你又返回上一道题，选择正确答案会直接跳转到下一题，暂不处理这个问题
        */
        n=preOption!=currIndexOfQuestion||option!=ui->optionsOfQuestion->getAnswer()?1:n-1;
        preOption=currIndexOfQuestion;
        /* 跳转到下一题 */
        if(n<=0)
        {
            if(currIndexOfQuestion+1<questionType[currTypeOfQuestion].second[0])
            {
                emit updateIndexOfQuestion(currIndexOfQuestion+1);
                showQuestionByIndex(currIndexOfQuestion+1);
            }
            return;
        }
        /* 展示正确答案、当前选择的错误答案（若没有选择错误答案便只显示正确答案） */
        ui->optionsOfQuestion->displayAnswer(true);
    });
    /* 更新答题页题目选项的文本内容，绑定到当前类的 updateTextOfQuestion 信号上 */
    connect(this,&widgetOfStart::updateTextOfQuestion,ui->optionsOfQuestion,&clickOptions::setTextOfOption);
    /* 更新答题页答案选项，绑定到当前类的 updateAnswerOfQuestion 信号上 */
    connect(this,&widgetOfStart::updateAnswerOfQuestion,ui->optionsOfQuestion,&clickOptions::setAnswer);
    /* 更新答题页题号总数，绑定到当前类的 updateSumOfQuestion 信号上 */
    connect(this,&widgetOfStart::updateSumOfQuestion,ui->switchOfQuestion,&switchQuestion::setSum);
    /* 更新答题页当前题号下标，绑定到当前类的 updateIndexOfQuestion 信号上 */
    connect(this,&widgetOfStart::updateIndexOfQuestion,ui->switchOfQuestion,&switchQuestion::setIndex);
    /* 更新答题页当前题号收藏按钮的状态，绑定到当前类的 isCollect 信号上 */
    connect(this,&widgetOfStart::isCollect,ui->switchOfQuestion,&switchQuestion::setCollect);
    ui->switchOfQuestion->setTitle("答题页");
    /* 当 switchOfQuestion 中题号变化，显示该题号的题目信息 */
    connect(ui->switchOfQuestion,&switchQuestion::changeQuestion,this,&widgetOfStart::showQuestionByIndex);
    /* 当 switchOfQuestion 中题号被收藏，则... */
    connect(ui->switchOfQuestion,&switchQuestion::collectQuestion,this,[=]()
    {
        if(!reader->isRead())
            return;
        /* 获取当前题号下标，并在收藏列表中查找 */
        int currIndexOfQuestion=progressOfQuestion[currTypeOfQuestion];
        int index=progressOfCollection.indexOf(QPair<int,int>(currTypeOfQuestion,currIndexOfQuestion));
        /* 查找不到才将该题号收藏 */
        if(index==-1)
        {
            progressOfCollection.push_back(QPair<int,int>(currTypeOfQuestion,currIndexOfQuestion));
            /* 更新收藏总数，若此时收藏总数为 1，自动显示该仅有的收藏 */
            emit updateSumOfCollection(progressOfCollection.length());
            if(progressOfCollection.length()==1)
            {
                emit updateIndexOfCollection(0);
                showCollectionByIndex(0);
            }
        }
    });
    /* 当 switchOfQuestion 中题号被取消收藏，则... */
    connect(ui->switchOfQuestion,&switchQuestion::uncollectQuestion,this,[=]()
    {
        if(!reader->isRead())
            return;
        /* 获取当前题号下标，并在收藏列表中查找 */
        int currIndexOfQuestion=progressOfQuestion[currTypeOfQuestion];
        int index=progressOfCollection.indexOf(QPair<int,int>(currTypeOfQuestion,currIndexOfQuestion));
        /* 查找到才将该题号取消收藏 */
        if(index!=-1)
        {
            #if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
                progressOfCollection.removeAt(index);
            #elif (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
                progressOfCollection.remove(index);
            #endif
            /* 删除该收藏项，并获取当前收藏项下标 */
            int currIndexOfCollection=ui->switchOfCollection->index();
            /* 若此时收藏列表为空，重置收藏页 */
            if(progressOfCollection.isEmpty())
            {
                resetCollection();
            }
            /* 若此时收藏项下标大于等于收藏总数，即当前收藏项刚好为删除的收藏项，将当前收藏项下标-1 */
            else if(currIndexOfCollection>=progressOfCollection.length())
            {
                showCollectionByIndex(progressOfCollection.length()-1);
                emit updateIndexOfCollection(progressOfCollection.length()-1);
            }
            /* 更新收藏总数 */
            emit updateSumOfCollection(progressOfCollection.length());
        }
    });
}

/* 初始化选择页 */
void widgetOfStart::initalSelectionPage()
{
    /* 当 inputButton 按钮被点击，则调用 getPath() 获取文件路径，若获取成功将发送 loadExcel 信号 */
    connect(ui->inputButton,&QRadioButton::clicked,this,&widgetOfStart::getPath);
    /* 接收到 loadExcel 信号，触发 reader 的 readExcel 槽函数 */
    connect(this,&widgetOfStart::loadExcel,reader,&excelReader::readExcel);
    /* 接收 reader 读取完数据发送的 readed 信号，调用 handleData 槽函数处理这些数据 */
    connect(reader,&excelReader::readed,this,&widgetOfStart::handleData);
    /* 接收 handleData 处理完数据发送的 ready 信号，若有收藏项则显示 */
    connect(this,&widgetOfStart::ready,this,[=]()
    {
        ui->questionType->setCurrentIndex(currTypeOfQuestion);
        if(ui->switchOfCollection->index()<progressOfCollection.length())
        {
            showCollectionByIndex(ui->switchOfCollection->index());
            emit updateIndexOfCollection(ui->switchOfCollection->index());
        }
    });
    /* 下拉菜单显示的题型变化，自动调用 setQuestionType 调整当前题型 */
    #if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
        void(QComboBox::*indexChanged)(int)=&QComboBox::currentIndexChanged;
        connect(ui->questionType,indexChanged,this,&widgetOfStart::setQuestionType);
    #elif (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
        connect(ui->questionType,&QComboBox::currentIndexChanged,this,&widgetOfStart::setQuestionType);
    #endif
}

/* 初始化收藏页 */
void widgetOfStart::initalCollectionPage()
{
    /* 实现点击正确选项跳转到下一题  */
    connect(ui->optionsOfCollection,&clickOptions::selectOption,this,[=](int option)
    {
        /* 在初始化答题窗口时已使用过该代码，不做解释 */
        if(!reader->isRead())
            return;
        static int index;
        static int n;
        int currIndexOfCollection=ui->switchOfCollection->index();
        n=index!=currIndexOfCollection||option!=ui->optionsOfCollection->getAnswer()?1:n-1;
        index=currIndexOfCollection;
        if(n==0)
        {
            if(index+1<progressOfCollection.length())
            {
                emit updateIndexOfCollection(index+1);
                showCollectionByIndex(index+1);
            }
            else
                ui->optionsOfCollection->displayAnswer(false);
            return;
        }
        ui->optionsOfCollection->displayAnswer(true);
    });
    /* 更新答题页题目选项的文本内容，绑定到当前类的 updateTextOfQuestion 信号上 */
    connect(this,&widgetOfStart::updateTextOfCollection,ui->optionsOfCollection,&clickOptions::setTextOfOption);
    /* 更新收藏页答案选项，绑定到当前类的 updateAnswerOfCollection 信号上 */
    connect(this,&widgetOfStart::updateAnswerOfCollection,ui->optionsOfCollection,&clickOptions::setAnswer);
    /* 更新收藏页题号总数，绑定到当前类的 updateSumOfCollection 信号上 */
    connect(this,&widgetOfStart::updateSumOfCollection,ui->switchOfCollection,&switchQuestion::setSum);
    /* 更新答题页当前题号下标（int）、文本（QString），绑定到当前类的 updateIndexOfCollection 信号上 */
    void(widgetOfStart::*updateIndexByInt)(int) =&widgetOfStart::updateIndexOfCollection;
    void(widgetOfStart::*updateIndexByString)(const QString&) =&widgetOfStart::updateIndexOfCollection;
    connect(this,updateIndexByInt,ui->switchOfCollection,&switchQuestion::setIndex);
    connect(this,updateIndexByString,ui->switchOfCollection,&switchQuestion::setTextOfIndex);
    ui->switchOfCollection->setTitle("收藏页");
    /* 当 switchOfCollection 题号改变时发送 lastIndex(option)，为未改变前的题号 */
    connect(ui->switchOfCollection,&switchQuestion::lastIndex,this,[=](int i)
    {
        /* 当前下标文本设置为 ？，代表应取消该题号应被取消收藏 */
        if(ui->switchOfCollection->stringOfIndex()=="?")
        {
            /* 当题号变动再执行取消收藏操作，而当前题号已发生变动，于是对未改变前的题号执行取消收藏操作 */
            int row=progressOfCollection[i].first;
            int column=progressOfCollection[i].second;
            #if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
                progressOfCollection.removeAt(i);
            #elif (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
                progressOfCollection.remove(i);
            #endif
            /* 若此时被取消的收藏项刚好也是当前答题页的题目，那么把答题页的收藏按钮状态置为未收藏 */
            if(currTypeOfQuestion==row&&progressOfQuestion[currTypeOfQuestion]==column)
                ui->switchOfQuestion->setCollect(false);
            /* 获取当前收藏页的当前题号，若未改变前的题号小于当前题号，需要更新当前题号下标=当前题号下标-1、收藏总数 */
            int currIndexOfCollection=ui->switchOfCollection->index();
            if(i<currIndexOfCollection)
                currIndexOfCollection--;
            emit updateIndexOfCollection(currIndexOfCollection);
            emit updateSumOfCollection(progressOfCollection.length());
        }
    });
    /* 当 switchOfCollection 题号改变时，显示当前收藏项的题目信息 */
    connect(ui->switchOfCollection,&switchQuestion::changeQuestion,this,&widgetOfStart::showCollectionByIndex);
    /* 当 switchOfCollection 题号被取消收藏时，则将题号下标文本设置为 ？ */
    connect(ui->switchOfCollection,&switchQuestion::uncollectQuestion,this,[=]()
    {
        /* 收藏总数小于等于 1 */
        if(progressOfCollection.length()<=1)
        {
            /* 若为 1，则说明是最后一个收藏项，直接取消收藏该题目 */
            if(progressOfCollection.length()==1)
            {
                int row=progressOfCollection[0].first;
                int column=progressOfCollection[0].second;
                #if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
                    progressOfCollection.removeAt(0);
                #elif (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
                    progressOfCollection.remove(0);
                #endif
                if(currTypeOfQuestion==row&&progressOfQuestion[currTypeOfQuestion]==column)
                    ui->switchOfQuestion->setCollect(false);
            }
            /* 此时收藏总数为 0，重置收藏页 */
            resetCollection();
            return;
        }
        /* 暂不取消收藏，而是将当前题号下标的文本置为 ？，等待题号变化的时候自动将其取消收藏 */
        emit updateIndexOfCollection("?");
    });
    /* 当 switchOfCollection 题号被重新收藏时，则更新下标 */
    connect(ui->switchOfCollection,&switchQuestion::collectQuestion,this,[=]()
    {
        emit updateIndexOfCollection(ui->switchOfCollection->index());
    });
    resetCollection();
}

/* 重置收藏页 */
void widgetOfStart::resetCollection()
{
    ui->textOfCollection->setText("暂无收藏");
    emit updateTextOfCollection(0,"Option A");
    emit updateTextOfCollection(1,"Option B");
    emit updateTextOfCollection(2,"Option C");
    emit updateTextOfCollection(3,"Option D");
    ui->switchOfCollection->setCollect(true);
    emit updateSumOfCollection(0);
    emit updateIndexOfCollection(0);
}