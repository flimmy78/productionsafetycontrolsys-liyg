﻿#include "twoticketexemonitor.h"
#include "ui_twoticketexemonitor.h"
#include <QDesktopWidget>
#include <QRect>
#include <QApplication>
#include <QHeaderView>


TwoTicketExeMonitor::TwoTicketExeMonitor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TwoTicketExeMonitor)
{
    ui->setupUi(this);

    InitTwoTicketWidget();
 }

TwoTicketExeMonitor::~TwoTicketExeMonitor()
{
    delete ui;
}

/************************************************************************
  Description：初始化表格数据和控件显示内容
  @param：
  Output:
  @Return:
  @author:
************************************************************************/
void TwoTicketExeMonitor::InitTwoTicketWidget()
{
    listTableWidget.append(ui->tableWidget);
    listTableWidget.append(ui->tableWidget_2);

    ReadDataFromMysql::instance()->InquireTwoTicketSum(data);  //操作票查询
    workTicketData = ReadDataFromMysql::instance()->getWorkTicketInfoNum();
    QStringList WidgetText;
    WidgetText << QStringLiteral("热控第一种工作票") << QStringLiteral("热控第二种工作票") << QStringLiteral("热力机械第一种工作票") \
               << QStringLiteral("热力机械第二种工作票") << QStringLiteral("电气第一种工作票") << QStringLiteral( "电气第二种工作票");
    QGridLayout *Layout = new QGridLayout(ui->frame_6);
    Layout->setMargin(0);

    for(int i = 0; i < WidgetText.size(); ++i)
    {
       QTableWidget *tableWidget = new QTableWidget();
       tableWidget->setMinimumHeight(150);
       listTableWidget.append(tableWidget);
       Layout->addWidget(tableWidget,i / 3, i % 3);
    }

    for(int i = 0; i < listTableWidget.size(); ++i)
    {
        listTableWidget.at(i)->horizontalHeader()->setVisible(false);
        listTableWidget.at(i)->verticalHeader()->setVisible(false);
        listTableWidget.at(i)->setRowCount(3);
        listTableWidget.at(i)->setColumnCount(3);
        listTableWidget.at(i)->setSpan(0,0,1,3);
        listTableWidget.at(i)->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        if(i >= 2)           //因为listTableWidget前2个包含了工作内容
        {
            QTableWidgetItem *tableWidgetItemHead = new QTableWidgetItem(WidgetText.at(i - 2));
            tableWidgetItemHead->setBackgroundColor(QColor(0, 85, 255));                      //背景色
            tableWidgetItemHead->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            listTableWidget.at(i)->setItem(0,0,tableWidgetItemHead);
        }


        if(i==6)
        {
            QTableWidgetItem *item=new QTableWidgetItem(workTicketData.job_count);
            item->setTextAlignment(Qt::AlignCenter);

            QTableWidgetItem *item1=new QTableWidgetItem(workTicketData.doing_work);
            item1->setTextAlignment(Qt::AlignCenter);

            QTableWidgetItem *item2=new QTableWidgetItem(workTicketData.alarmticket_num);
            item2->setTextAlignment(Qt::AlignCenter);

            listTableWidget.at(i)->setItem(2,0,item);
            listTableWidget.at(i)->setItem(2,1,item1);
            listTableWidget.at(i)->setItem(2,2,item2);
            connect(listTableWidget.at(i),SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(ClickedTableWidget(QTableWidgetItem*)));
        }
        else if(i >= 2)
        {
            QTableWidgetItem *item=new QTableWidgetItem("0");
            item->setTextAlignment(Qt::AlignCenter);

            QTableWidgetItem *item1=new QTableWidgetItem("0");
            item1->setTextAlignment(Qt::AlignCenter);

            QTableWidgetItem *item2=new QTableWidgetItem("0");
            item2->setTextAlignment(Qt::AlignCenter);

            listTableWidget.at(i)->setItem(2,0,item);
            listTableWidget.at(i)->setItem(2,1,item1);
            listTableWidget.at(i)->setItem(2,2,item2);
        }
    }

    ui->pushButton_work->clicked(true);
    ui->stackedWidget->setCurrentWidget(ui->page);
}

void TwoTicketExeMonitor::InitTwoTicketData()
{
    ui->pushButton_work->clicked(true);
}

/************************************************************************
  Description：工作票按钮单击响应事件
  @param：
  Output:
  @Return:
  @author:
************************************************************************/
void TwoTicketExeMonitor::on_pushButton_work_clicked()
{
    workTicketData = ReadDataFromMysql::instance()->getWorkTicketInfoNum();
    ui->stackedWidget->setCurrentWidget(ui->page);
    ui->pushButton_operation->setChecked(false);
    ui->pushButton_operation->setStyleSheet("QPushButton{background-color:rgb(255,255,255,0);font:'微软雅黑' 14px;color:#ffffff;}");
    ui->pushButton_work->setChecked(true);
    ui->pushButton_work->setStyleSheet("QPushButton{background-color:rgb(77,176,247);font:'微软雅黑' 14px;color:#ffffff}");
    QStringList ItemText;
    ItemText << QStringLiteral("作业数") << QStringLiteral("正在作业") << QStringLiteral("告警票数");
    for(int i = 0; i < ItemText.size(); ++i)
    {
        for(int j = 2; j < listTableWidget.size(); j++)
        {
            QTableWidgetItem *tableWidgetItem = new QTableWidgetItem(ItemText.at(i));
            tableWidgetItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            listTableWidget.at(j)->setItem(1,i,tableWidgetItem);
        }
    }

    ui->label_worknum->setText(workTicketData.job_count);
    ui->label_workingnum->setText(workTicketData.doing_work);
    ui->label_nostartnum->setText("0");
    ui->label_haveendnum->setText("0");
    ui->label_allalarmnum->setText(workTicketData.alarmticket_num);
}


/************************************************************************
  Description：操作票按钮单击事件响应
  @param：
  Output:
  @Return:
  @author:
************************************************************************/
void TwoTicketExeMonitor::on_pushButton_operation_clicked()
{
    ReadDataFromMysql::instance()->InquireTwoTicketSum(data);  //操作票查询
    ui->pushButton_work->setChecked(false);
    ui->pushButton_work->setStyleSheet("QPushButton{background-color:rgb(255,255,255,0);font:'微软雅黑' 14px;color:#ffffff}");
    ui->pushButton_operation->setChecked(true);
    ui->pushButton_operation->setStyleSheet("QPushButton{background-color:rgb(77,176,247);font:'微软雅黑' 14px;color:#ffffff}");

    QTableWidgetItem *tableWidgetItemHead1 = new QTableWidgetItem(QStringLiteral("电气倒闸操作票"));
    tableWidgetItemHead1->setBackgroundColor(QColor(0, 85, 255));                      //背景色
    tableWidgetItemHead1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidget->setItem(0,0,tableWidgetItemHead1);

    QTableWidgetItem *tableWidgetItemHead2 = new QTableWidgetItem(QStringLiteral("热力机械操作票"));
    tableWidgetItemHead2->setBackgroundColor(QColor(0, 85, 255));                       //背景色
    tableWidgetItemHead2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);   //设置字体对齐方式
    ui->tableWidget_2->setItem(0,0,tableWidgetItemHead2);    //添加到0行0列

    //操作票表格显示
    QStringList ItemText;
    ItemText << QStringLiteral("作业数") << QStringLiteral("正在作业") << QStringLiteral("告警票数");
    for(int i = 0; i < ItemText.size(); ++i)
    {
        QTableWidgetItem *tableWidgetItem = new QTableWidgetItem(ItemText.at(i));
        tableWidgetItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableWidget->setItem(1,i,tableWidgetItem);

        QTableWidgetItem *tableWidgetItem1 = new QTableWidgetItem(ItemText.at(i));
        tableWidgetItem1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableWidget_2->setItem(1,i,tableWidgetItem1);
    }

    QTableWidgetItem *item=new QTableWidgetItem(data.job_count);
    item->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item1=new QTableWidgetItem(data.doing_work);
    item1->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *item2=new QTableWidgetItem(data.alarmticket_num);
    item2->setTextAlignment(Qt::AlignCenter);

    ui->tableWidget->setItem(2,0,item);
    ui->tableWidget->setItem(2,1,item1);
    ui->tableWidget->setItem(2,2,item2);

    QTableWidgetItem *item4=new QTableWidgetItem("0");
    item4->setTextAlignment(Qt::AlignHCenter);

    QTableWidgetItem *item5=new QTableWidgetItem("0");
    item5->setTextAlignment(Qt::AlignHCenter);

    QTableWidgetItem *item6=new QTableWidgetItem("0");
    item6->setTextAlignment(Qt::AlignHCenter);

    ui->tableWidget_2->setItem(2,0,item4);
    ui->tableWidget_2->setItem(2,1,item5);
    ui->tableWidget_2->setItem(2,2,item6);

    ui->stackedWidget->setCurrentWidget(ui->page_1);
    ui->label_worknum->setText(data.job_count);
    ui->label_workingnum->setText(data.doing_work);
    ui->label_nostartnum->setText("0");
    ui->label_haveendnum->setText("0");
    ui->label_allalarmnum->setText(data.alarmticket_num);
    connect(ui->tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(ClickedOperationWidget(QTableWidgetItem*)));
}


/************************************************************************
  Description：工作票表格单击事件响应发送一个信号
  @param：
  Output:
  @Return:
  @author:
************************************************************************/
void TwoTicketExeMonitor::ClickedTableWidget(QTableWidgetItem *item)
{
     QString tickettype="0";
    //只能点击0行0列
    if(0==item->row()&& 0==item->column())
    {
     //    qDebug()<<QStringLiteral("点击工作票操作")<<item->text()<<item->row()<<item->column();
         emit Signal_ticketMtepMonitor(tickettype);
    }
    else
    {
        QMessageBox::information(NULL,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("请点击表头"));
        return;
    }

//     ticketMtepMonitor->updateStepTableData(ReadDataFromMysql::instance()->TwoTicketExecMonitor(tickettype));
//     ticketMtepMonitor->setGeometry(ui->stackedWidget->geometry());
//     ticketMtepMonitor->show();

//     qDebug()<<QStringLiteral("点击工作票操作")<<item->text()<<item->row()<<item->column();
//    emit Signal_ticketMtepMonitor(tickettype);
}

/************************************************************************
  Description：操作票表格添加单击事件发送一个信号
  @param：
  Output:
  @Return:
  @author:
************************************************************************/
void TwoTicketExeMonitor::ClickedOperationWidget(QTableWidgetItem *item)
{
    QString tickettype="1";
   //只能点击0行0列
   if(0==item->row()&& 0==item->column())
   {
    //    qDebug()<<QStringLiteral("点击工作票操作")<<item->text()<<item->row()<<item->column();
         emit Signal_OperationWidget(tickettype);;
   }
   else
   {
       QMessageBox::information(NULL,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("请点击表头"));
       return;
   }
//    Q_UNUSED(item);
//    QString tickettype="1";
//    qDebug()<<QStringLiteral("点击操作票操作");
//    emit Signal_OperationWidget(tickettype);
}
