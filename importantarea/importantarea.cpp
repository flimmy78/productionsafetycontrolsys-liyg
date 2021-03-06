﻿#include "importantarea.h"
#include "ui_importantarea.h"

ImportantArea::ImportantArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImportantArea)
{
    ui->setupUi(this);
    InitWidget();
}

ImportantArea::~ImportantArea()
{
    delete ui;
}


void ImportantArea::InitWidget()
{
    QStringList header;
    header<<QStringLiteral("区域名称")<<QStringLiteral("安全设备数")<<QStringLiteral("生产设备数")<<QStringLiteral("作业数")<<QStringLiteral("告警数")
         <<QStringLiteral("作业人数");
    ui->tableWidget->setRowCount(15);   //设置10行6列
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setHorizontalHeaderLabels(header);

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  //禁止编辑表格
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);   //设置表格点击无焦点
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置表格选中一列
    ui->tableWidget->horizontalHeader()->setFixedHeight(40);
    ui->tableWidget->setColumnWidth(0,243);  //设置第一列的宽度
    ui->tableWidget->setColumnWidth(1,220);
    ui->tableWidget->setColumnWidth(2,220);
    ui->tableWidget->setColumnWidth(3,220);
    ui->tableWidget->setColumnWidth(4,220);
    ui->tableWidget->setColumnWidth(5,210);
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableWidget->setAlternatingRowColors(true);  //设置表格交替色显示
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);  //设置表格点击无虚边框
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置表格点击选中一行
}

void ImportantArea::UpdataTableInfo(QList<Import_Area> ImportAreaList)
{
    int i = 0;
    Import_Area Temp;

    //遍历链表
    for(i = 0; i < ImportAreaList.count(); i++)
    {
//        if(i > 9)
//        {
//            break;
//        }
        Temp = ImportAreaList.at(i);  //得到链表某一个参数值
        //往单元格添加数据显示
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(Temp.Area_Name));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(Temp.SafeDeviceNum));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(Temp.MakeDeviceNum));
        ui->tableWidget->setItem(i,3,new QTableWidgetItem(Temp.WorkNum));
        ui->tableWidget->setItem(i,4,new QTableWidgetItem(Temp.AlarmNum));
        ui->tableWidget->setItem(i,5,new QTableWidgetItem(Temp.PersonNum));
        //设置表格字体局中显示
        ui->tableWidget->item(i,0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(i,1)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(i,2)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(i,3)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(i,4)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(i,5)->setTextAlignment(Qt::AlignCenter);
    }
}

//点击表头获取到所点击单元格的内容并启动相应的视频界面
//void ImportantArea::on_tableWidget_cellClicked(int row, int column)
//{
//    Q_UNUSED(column);
//    qDebug()<<"点击表头";
//    QString Text;
//      if(row>Row_Count-1)
//      {
//        return;
//      }
//      else
//      {
//         Text=ui->tableWidget->item(row,0)->text();
//      }
//      emit View_Id(Text);

//}


