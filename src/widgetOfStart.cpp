#include "widgetOfStart.h"

#include "excelReader.h"
#include "settingFile.h"

#include <QFileDialog>
#include <QTimer>

widgetOfStart::widgetOfStart(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui_widgetOfStart)
    , reader(new excelReader)
    , currTypeOfQuestion(0)
    , flagOfInital(0)
{
    ui->setupUi(this);
    /* 初始化多层窗口 */
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
    /* 窗口大小调整，调用 importSetting 后会将 flagOfInital 置为 1（即只会在窗口第一次显示的时候调用） */
    if(flagOfInital==0)
    {
        /* 可选择题库有内置的 2022、2023 年题库，以及配置文件中的题库路径 */
        ui->textOfPaths->addItem(":/doc/2022.csv");
        ui->textOfPaths->addItem(":/doc/2023.csv");
        importSetting("settings.json");
        /* 选择下拉菜单中最后一个题库，并加载该题库对应的配置文件 */
        ui->textOfPaths->setCurrentIndex(ui->textOfPaths->count()-1);
        loadSetting(ui->textOfPaths->currentText());
    }
}

/* 保存当前答题信息 */
void widgetOfStart::exportSetting(const QString& fileName)
{
    /* setting 用于加载配置文件 */
    settingFile setting;
    /* 将当前文件路径、当前答题页题型下标导入 */
    setting.add("pathOfExcel",pathOfExcel);
    setting.add("currTypeOfQuestion",currTypeOfQuestion);
    /* 导入收藏页当前题目下标 */
    int currIndexOfCollection=ui->switchOfCollection->count()>0?ui->switchOfCollection->index():-1;
    setting.add("currIndexOfCollection",currIndexOfCollection);
    /*
        将当前答题页答题进度导入
        （导出数组长度与当前数组长度一致，为题型数量，存储的是对应题型的题号下标数组）
    */
    setting.add("progressOfQuestion",progressOfQuestion);
    /* progressOfCollection 数组长度为收藏总数，每一项是一个 QPair<int,int>，对应题目的题型下标、题号下标 */
    setting.add("progressOfCollection",progressOfCollection);
    /* 保存到 fileName 文件中  */
    #ifdef __ANDROID__
        /* 安卓文件路径为包的私有路径 */
        setting.save("/data/data/org.qtproject.example.CS_Contest/"+fileName);
    #else
        setting.save(fileName);
    #endif
}

/* 导入答题信息（只执行一次） */
void widgetOfStart::importSetting(const QString& fileName)
{
    settingFile setting;
    #ifdef __ANDROID__
        /* 安卓文件路径为包的私有路径 */
        setting.load("/data/data/org.qtproject.example.CS_Contest/"+fileName);
    #else
        setting.load(fileName);
    #endif
    if(setting.isLoad())
    {
        /* 若已初始化，则返回（即该函数只调用一次） */
        if(flagOfInital==1&&fileName=="settings.json")
            return;
        /* 导入题库文件路径，选择页显示题库路径 */
        pathOfExcel=setting.value("pathOfExcel").toString();
        /* 导入当前答题页题型下标 */
        currTypeOfQuestion=setting.value("currTypeOfQuestion").toInt();
        /* 导入当前收藏页题型下标 */
        int currIndexOfCollection=setting.value("currIndexOfCollection").toInt();
        /* 导入答题页答题进度 */
        QJsonArray arrayOfQuestionProcess=setting.value("progressOfQuestion").toArray();
        /* 若答题进度不为空则清空再导入 */
        if(!progressOfQuestion.isEmpty())
            progressOfQuestion.clear();
        for(int i=0;i<arrayOfQuestionProcess.count();i++)
            progressOfQuestion.push_back(arrayOfQuestionProcess.at(i).toInt());
        /* 导入收藏页题目数据 */
        QJsonArray arrayOfCollectProcess=setting.value("progressOfCollection").toArray();
        const QJsonArray& first=arrayOfCollectProcess.at(0).toArray();
        const QJsonArray& second=arrayOfCollectProcess.at(1).toArray();
        /* 若收藏进度不为空则清空再导入 */
        if(!progressOfCollection.isEmpty())
            progressOfCollection.clear();
        for(int i=0;i<first.count();i++)
            progressOfCollection.push_back(QPair<int,int>(first.at(i).toInt(),second.at(i).toInt()));
        /* 刷新收藏页题目 */
        if(currIndexOfCollection!=-1)
        {
            emit updateSumOfCollection(progressOfCollection.length());
            emit updateIndexOfCollection(currIndexOfCollection);
            showCollectionByIndex(currIndexOfCollection);
        }
        else
            resetCollection();
    }
    if(fileName=="settings.json")
    {
    /* 判定题库路径是否合法，比如路径不存在且不为 .xls 结尾的文件，便打开默认的 CSV 文件 */
        QFile path(pathOfExcel);
        QString postfix;
        if(pathOfExcel.length()>4)
            postfix=pathOfExcel.right(4);
        if(path.exists())
            if(postfix==".xls")
                ui->textOfPaths->addItem(pathOfExcel);
        /* 若为 settings 配置文件将初始化标志位置为 1（表明已被初始化） */
        flagOfInital=1;
    }
    /* 启动定时器，每隔一秒调用 exportSetting 槽函数 */
    static QTimer* timer;
    if(timer!=nullptr)
        delete timer;
    timer=new QTimer(this);
    connect(timer,&QTimer::timeout,this,[=]()
    {
        exportSetting(fileName);
    });
    timer->start(1000);
}

/* 处理导入的题库数据 */
void widgetOfStart::handleData()
{
    /* 列出题型所代表字符，用其下标代表该类题型 */
    static char charOfType[]=
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
    static QString stringOfType[]=
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
            if(reader->getData().at(i).at(1)==charOfType[j])
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
        questionType[i].second[0]=questionType.at(i).second.length()-1;
        ui->tableOfQuestionType->setItem(i,0,new QTableWidgetItem(questionType.at(i).first));
        ui->tableOfQuestionType->setItem(i,1,new QTableWidgetItem(QString(charOfType[i])));
        ui->tableOfQuestionType->setItem(i,2,new QTableWidgetItem(QString::number(questionType.at(i).second.at(0))));
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
    int currIndexOfQuestion=progressOfQuestion.at(currTypeOfQuestion);
    /* 更新 switchOfQuestion 的题目总数和当前题号，并显示该题目 */
    emit updateSumOfQuestion(questionType.at(currTypeOfQuestion).second.at(0));
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
    int index=questionType.at(currTypeOfQuestion).second.at(i+1);
    progressOfQuestion[currTypeOfQuestion]=i;
    /* 题目的内容，位于第 2 列的单元格 */
    ui->textOfQuestion->setText(QString::number(i+1)+". "+data.at(index).at(2));
    /* 题目的选项，位于第 3、4、5、6 列的单元格 */
    emit updateTextOfQuestion(0,data.at(index).at(3));
    emit updateTextOfQuestion(1,data.at(index).at(4));
    emit updateTextOfQuestion(2,data.at(index).at(5));
    emit updateTextOfQuestion(3,data.at(index).at(6));
    /* 题目的答案，位于第 7 列的单元格，转为大写，并将其转化为 0、1、2、3 的下标，更新 optionsOfQuestion 模块的答案*/
    QString answer=data.at(index).at(7).trimmed().toUpper();
    emit updateAnswerOfQuestion(answer.at(0).unicode()-'A');
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
    int row=progressOfCollection.at(i).first;
    int column=progressOfCollection.at(i).second;
    int index=questionType.at(row).second.at(column+1);
     /* 题目的内容，上面已做解释，就不重复赘述了 */
    ui->textOfCollection->setText(QString::number(i+1)+". "+data.at(index).at(2));
    /* 题目的选项 */
    emit updateTextOfCollection(0,data.at(index).at(3));
    emit updateTextOfCollection(1,data.at(index).at(4));
    emit updateTextOfCollection(2,data.at(index).at(5));
    emit updateTextOfCollection(3,data.at(index).at(6));
    /* 题目的答案 */
    QString answer=data.at(index).at(7).trimmed().toUpper();
    emit updateAnswerOfCollection(answer.at(0).unicode()-'A');
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
        if(ui->textOfPaths->findText(pathOfExcel)==-1)
            ui->textOfPaths->addItem(pathOfExcel);
        ui->textOfPaths->setCurrentIndex(ui->textOfPaths->findText(pathOfExcel));
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
        int currIndexOfQuestion=progressOfQuestion.at(currTypeOfQuestion);
        /* 若题号为 0 且点击次数为 0，则将上个选项置为 -1 */
        if(n==0&&currIndexOfQuestion==0)
            preOption=-1;
        /*
            若当前选项和上一次选择的选项不一样，需要重置 n 为 1
            或当前选项不等于答案，需要重置 n 为 1，直到选择了正确答案才置为 0
            
            这样会导致一种情况，若你答完上一道题，你又返回上一道题，选择正确答案会直接跳转到下一题，暂不处理这个问题
        */
        n=preOption!=currIndexOfQuestion||option!=ui->optionsOfQuestion->getAnswer()?1:n-1;
        preOption=currIndexOfQuestion;
        /* 跳转到下一题 */
        if(n==0)
        {
            if(currIndexOfQuestion+1<questionType.at(currTypeOfQuestion).second.at(0))
            {
                emit updateIndexOfQuestion(currIndexOfQuestion+1);
                showQuestionByIndex(currIndexOfQuestion+1);
            }
            else
                ui->optionsOfQuestion->displayAnswer(false);
            preOption=-1;
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
        int currIndexOfQuestion=progressOfQuestion.at(currTypeOfQuestion);
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
        int currIndexOfQuestion=progressOfQuestion.at(currTypeOfQuestion);
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
    /* 除 win 平台以外隐藏掉导入组件 */
    #ifndef _WIN32
    ui->inputButton->hide();
    #endif
    /* 当 inputButton 按钮被点击，则调用 getPath() 获取文件路径，若获取成功将添加路径 */
    connect(ui->inputButton,&QRadioButton::clicked,this,&widgetOfStart::getPath);
    /* 下拉菜单显示的题库路径变化，自动切换题库 */
    connect(ui->textOfPaths,&QComboBox::currentTextChanged,this,[=](const QString& path)
    {
        if(flagOfInital==0)
            return;
        if(path.endsWith(".xls"))
            emit loadExcel(path);
        else if(path.endsWith(".csv"))
        {
            emit loadCSV(path);
            loadSetting(path);
            /* 刷新答题页题目 */
            ui->questionType->setCurrentIndex(currTypeOfQuestion);
            setQuestionType(currTypeOfQuestion);
        }
    });
    /* 接收到 loadExcel 信号，触发 reader 的 readExcel 槽函数 */
    connect(this,&widgetOfStart::loadExcel,reader,&excelReader::readExcel);
    /* 接收到 loadCSV 信号，触发 reader 的 readCSV 槽函数 */
    connect(this,&widgetOfStart::loadCSV,reader,&excelReader::readCSV);
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
        static int preOption;
        static int n;
        int currIndexOfCollection=ui->switchOfCollection->index();
        if(n==0&&currIndexOfCollection==0)
            preOption=-1;
        n=preOption!=currIndexOfCollection||option!=ui->optionsOfCollection->getAnswer()?1:n-1;
        preOption=currIndexOfCollection;
        if(n==0)
        {
            if(preOption+1<progressOfCollection.length())
            {
                emit updateIndexOfCollection(preOption+1);
                showCollectionByIndex(preOption+1);
            }
            else
                ui->optionsOfCollection->displayAnswer(false);
            preOption=-1;
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
            int row=progressOfCollection.at(i).first;
            int column=progressOfCollection.at(i).second;
            #if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
                progressOfCollection.removeAt(i);
            #elif (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
                progressOfCollection.remove(i);
            #endif
            /* 若此时被取消的收藏项刚好也是当前答题页的题目，那么把答题页的收藏按钮状态置为未收藏 */
            if(currTypeOfQuestion==row&&progressOfQuestion.at(currTypeOfQuestion)==column)
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
                int row=progressOfCollection.at(0).first;
                int column=progressOfCollection.at(0).second;
                #if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
                    progressOfCollection.removeAt(0);
                #elif (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
                    progressOfCollection.remove(0);
                #endif
                if(currTypeOfQuestion==row&&progressOfQuestion.at(currTypeOfQuestion)==column)
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
    /* 初始化收藏页的同时重置收藏页 */
    resetCollection();
}

void widgetOfStart::loadSetting(const QString &path)
{
    QString fileName=path.right(path.length()-1-path.lastIndexOf("/"));
    QString name=fileName.left(fileName.indexOf("."));
    importSetting(name+".json");
}

/* 重置收藏页 */
void widgetOfStart::resetCollection()
{
    /* 重置文本 */
    ui->textOfCollection->setText("暂无收藏");
    emit updateTextOfCollection(0,"Option A");
    emit updateTextOfCollection(1,"Option B");
    emit updateTextOfCollection(2,"Option C");
    emit updateTextOfCollection(3,"Option D");
    /* 重置答案、答案选框 */
    emit updateAnswerOfCollection(-1);
    ui->optionsOfCollection->displayAnswer(false);
    ui->switchOfCollection->setCollect(true);
    /* 重置收藏题目总数、下标 */
    emit updateSumOfCollection(0);
    emit updateIndexOfCollection(0);
}