#pragma once

/* 程序标题 */
const QString titleOfProgram("知识竞赛答题");

/* 全部题库，最后一个将为当前显示的题库 */
const QStringList pathOfquestionBanks=
{
    ":/doc/2022.csv",
    ":/doc/2023.csv"
};

/* 列出题型所代表字符，用其下标代表该类题型 */
const char charOfType[]=
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
const QString stringOfType[]=
{
    "计算机应用基础","数据结构","数据库原理","网络","软件工程","操作系统",
    "多媒体技术","硬件系统","移动互联网应用","数据表示和计算","离散数学","知识产权",
    "C","C++","Java","JavaScript","C#","Python"
};

/* sumOfType 为题型总数 */
const int sumOfType=sizeof(charOfType)/sizeof(char);

/* 题目文本、选项ABCD、答案所对应单元格的下标 */
const int indexOfProblemText=2;
const int indexOfOptionA=3;
const int indexOfOptionB=4;
const int indexOfOptionC=5;
const int indexOfOptionD=6;
const int indexOfAnswer=7;