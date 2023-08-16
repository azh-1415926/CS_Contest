# About CS_Contest
试图编写一个用于导入题库，并用于刷题练习的 Qt 程序。

该程序应当具备以下功能：
1. 具有相对完整的界面。
2. 可导入《江西省信息技术知识大赛题库》，用于刷题练习。
3. 可边刷题边存储刷题进度信息。
4. 存储错题信息，并反复练习。

**主界面**

经过初步设计的界面如下

<img src="https://github.com/azh-1415926/CS_Contest/blob/main/image/CS_Contest_01.png " alt="CS_Contest_01.png" width="50%" height="50%">

调整部分比例，且能自动等比例缩放，最终定稿为

<img src="https://github.com/azh-1415926/CS_Contest/blob/main/image/CS_Contest.png" alt="CS_Contest.png" width="50%" height="50%">

**答题窗口**

最初的答题窗口示例图中,有三个按钮，用于切换其他子窗口

目前展示的是答题页窗口，鼠标悬浮在选项上方会出现蓝色选框，且点击蓝色选框内的文字也能直接选择该选项

<img src="https://github.com/azh-1415926/CS_Contest/blob/main/image/StartWindow_QuestionPage_01.png" alt="StartWindow_QuestionPage_01.png" width="50%" height="50%">

接下来是选择页窗口，选择题库导入路径，并列举所有题型

<img src="https://github.com/azh-1415926/CS_Contest/blob/main/image/StartWindow_SelectPage_01.png" alt="StartWindow_SelectPage_01.png" width="50%" height="50%">

一开始没有做收藏页窗口，后面模仿答题页窗口复刻了一份，最后定稿为

这是未导入题库时的答题窗口
<table>
<tr>
<td><img src="https://github.com/azh-1415926/CS_Contest/blob/main/image/StartWindow_QuestionPage_NoQuestion.png" alt="StartWindow_QuestionPage_NoQuestion.png" border=0></td>
<td><img src="https://github.com/azh-1415926/CS_Contest/blob/main/image/StartWindow_SelectPage_NoLoading.png" alt="StartWindow_SelectPage_NoLoading.png" border=0></td>
<td><img src="https://github.com/azh-1415926/CS_Contest/blob/main/image/StartWindow_CollectionPage_NoQuestion.png" alt="StartWindow_CollectionPage_NoQuestion.png" border=0></td>
</tr>
</table>
在选择页导入题库时显示进度条

<img src="https://github.com/azh-1415926/CS_Contest/blob/main/image/StartWindow_SelectPage_Loading.png" alt="StartWindow_SelectPage_Loading.png" width="50%" height="50%">

导入题库时的答题界面
<table>
<tr>
<td><img src="https://github.com/azh-1415926/CS_Contest/blob/main/image/StartWindow_QuestionPage.png" alt="StartWindow_QuestionPage.png" border=0></td>
<td><img src="https://github.com/azh-1415926/CS_Contest/blob/main/image/StartWindow_SelectPage.png" alt="StartWindow_SelectPage.png" border=0></td>
<td><img src="https://github.com/azh-1415926/CS_Contest/blob/main/image/StartWindow_CollectionPage.png" alt="StartWindow_CollectionPage.png" border=0></td>
</tr>
</table>
新增功能

1. 新增导入和自动存储部分答题进度功能
2. 增加收藏功能

    * 当在答题页(取消收藏/收藏)某个题目时，会立即(取消/赋予)收藏状态，并在收藏页中(删除/添加)该题目
        * 若当前收藏的题目的数量刚好为 1，则立即显示该题目
        * 若当前取消收藏的题目刚好为收藏页当前展示的最后一题，则删除最后一题，更新收藏页题目为当前的最后一题
        * 若当前取消收藏后的收藏数量为 0，则重置收藏页
    * 当在收藏页(取消收藏/收藏)某个题目时，不会立即(取消/赋予)收藏状态，而是将当前题目下标置为(？/当前题目号)
        * 若收藏页的题目题号切换且当前题目下标为？，删除切换前的题目，并更新收藏页题目总数
        * 若当前题目总数为 1，在取消收藏状态的时候立即将该题目从收藏页中删除，并重置收藏页
        * 若当前题目总数为 0，重置收藏页
3. 当选择错误选项时会标注出错误选项，答完题最后都会显示正确选项
4. 答题结束后再次点击正确选项会跳转到下一题