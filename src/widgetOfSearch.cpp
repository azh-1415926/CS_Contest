#include "widgetOfSearch.h"

widgetOfSearch::widgetOfSearch(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui_widgetOfSearch)
{
    ui->setupUi(this);
}

widgetOfSearch::~widgetOfSearch()
{
    delete ui;
}