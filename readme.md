# About CS_Contest
试图编写一个用于导入题库，并用于刷题练习的 Qt 程序。

该程序应当具备以下功能：
1. 具有相对完整的界面。
2. 可导入《江西省信息技术知识大赛题库》，用于刷题练习。
3. 可边刷题边存储刷题进度信息。
4. 存储错题信息，并反复练习。

经过初步设计的界面如下

![CS_Contest_01](https://github.com/azh-1415926/CS_Contest/blob/main/image/CS_Contest_01.png)

调整部分比例，且能自动等比例缩放

最终定稿为

![CS_Contest](https://github.com/azh-1415926/CS_Contest/blob/main/image/CS_Contest.png)

开始窗口示例图中,有三个按钮，用于切换其他子窗口

目前展示的是答题页窗口，鼠标悬浮在选项上方会出现蓝色选框，且点击蓝色选框内的文字也能直接选择该选项

![StartWindow_QuestionPage_01](https://github.com/azh-1415926/CS_Contest/blob/main/image/StartWindow_QuestionPage_01.png)

接下来是选择页窗口，选择题库导入路径，并列举所有题型

![StartWindow_SelectPage_01](https://github.com/azh-1415926/CS_Contest/blob/main/image/StartWindow_SelectPage_01.png)