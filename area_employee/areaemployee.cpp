﻿#include "areaemployee.h"
#include "ui_areaemployee.h"

AreaEmployee::AreaEmployee(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AreaEmployee)
{
    ui->setupUi(this);
    InitWidget();
}

AreaEmployee::~AreaEmployee()
{
    delete ui;
}

void AreaEmployee::InitWidget()
{
    QString HasSignPersonNum = ReadDataFromMysql::instance()->HasSignPersonNum();
    QString NoSignPersonNum = ReadDataFromMysql::instance()->NoSignPersonNum();
    QString OperatingWorkNum = ReadDataFromMysql::instance()->OperatingWorkNum();
    QString NoStartWorkNum = ReadDataFromMysql::instance()->NoStartWorkNum();

    ui->workSumBtn->setText(QStringLiteral("作业人员总数:%1").arg(QString::number(HasSignPersonNum.toInt() + NoSignPersonNum.toInt())));
    ui->workSumBtn->setFont(QFont("微软雅黑",12,QFont::Bold));
    ui->workSumBtn->setStyleSheet("QPushButton{color:rgb(255,255,255);border:1px solid ;border-color:white;"\
                                  "border-radius:5px;}");
    ui->SignPerSumBtn->setText(QStringLiteral("已经签到人员数:%1").arg(HasSignPersonNum));
    ui->SignPerSumBtn->setFont(QFont("微软雅黑",12,QFont::Bold));
    ui->SignPerSumBtn->setStyleSheet("QPushButton{color:rgb(255,255,255);border:1px solid ;border-color:white;"\
                                     "border-radius:5px;}");

    ui->NoSignPerSumBtn->setText(QStringLiteral("未签到人员数:%1").arg(NoSignPersonNum));
    ui->NoSignPerSumBtn->setFont(QFont("微软雅黑",12,QFont::Bold));
    ui->NoSignPerSumBtn->setStyleSheet("QPushButton{color:rgb(255,0,0);border:1px solid ;border-color:white;"\
                                       "border-radius:5px;}");

    ui->startWorkBtn->setText(QStringLiteral("正在进行作业(%1)").arg(OperatingWorkNum));
    ui->startWorkBtn->setFont(QFont("微软雅黑",12,QFont::Bold));
    ui->startWorkBtn->setStyleSheet("QPushButton{color:rgb(255,255,255);border:1px solid ;border-color:white;"\
                                    "border-radius:5px;}");

    ui->NostartWorkBtn->setText(QStringLiteral("未开始作业(%1)").arg(NoStartWorkNum));
    ui->NostartWorkBtn->setFont(QFont("微软雅黑",12,QFont::Bold));
    ui->NostartWorkBtn->setStyleSheet("QPushButton{color:rgb(255,0,0);border:1px solid ;border-color:white;"\
                                      "border-radius:5px;}");

    /*************************************************************************/
    QStringList header;
    header << QStringLiteral("作业区域名称") << QStringLiteral("作业名称") << QStringLiteral("人员姓名") << QStringLiteral("所在班组") << QStringLiteral("作业开始时间") ;
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  //禁止编辑表格
    ui->tableWidget->verticalHeader()->setVisible(false);  //隐藏垂直表头
    ui->tableWidget->setAlternatingRowColors(true);  //表格背景色交替显示
    ui->tableWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  //禁止编辑表格
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //去掉水平滚动条
    ui->tableWidget->setFocusPolicy(Qt::NoFocus); //设置点击表格无虚边框
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置点击表格为选中一行
    ui->tableWidget->setShowGrid(true);                         //去掉格子

    int row = ui->tableWidget->rowCount();
    qDebug()<<"row ="<<row;
    for(int i=0;i<row;++i)
    {
        ui->tableWidget->setRowHeight(i,30);
    }

    ui->tableWidget->horizontalHeader()->setFixedHeight(40);
    ui->tableWidget->setColumnWidth(0,260);
    ui->tableWidget->setColumnWidth(1,500);
    ui->tableWidget->setColumnWidth(2,150);
    ui->tableWidget->setColumnWidth(3,150);
    ui->tableWidget->setColumnWidth(4,285);
}

void AreaEmployee::UpdateTableInfo(QList<Area_person> list)
{
    Area_person areadata;
    ui->tableWidget->setRowCount(list.count());
    ui->label->setFont(QFont("微软雅黑",12,QFont::Bold));
    ui->label->setStyleSheet("QLabel({color:rgb(255,255,255))");
    ui->label->setText(QStringLiteral("共%1条").arg(QString::number(list.count())));
    for(int i = 0;i < list.count(); i++)
    {
        areadata=list.at(i);

        ui->tableWidget->setItem(i,0,new QTableWidgetItem(areadata.areaname));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(areadata.workname));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(areadata.employeename));
        ui->tableWidget->setItem(i,3,new QTableWidgetItem(areadata.deptname));
        ui->tableWidget->setItem(i,4,new QTableWidgetItem(areadata.starttime));

        //显示字体局中对齐
        ui->tableWidget->item(i,0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(i,1)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(i,2)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(i,3)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(i,4)->setTextAlignment(Qt::AlignCenter);

        //qDebug()<<areadata.areaname;
    }

}
