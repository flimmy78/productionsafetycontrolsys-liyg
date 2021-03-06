﻿#include "hk_video.h"
#include "ui_hk_video.h"
#include <qdebug.h>
#include "readdatafrommysql/readdatafrommysql.h"
#include <QMessageBox>
#include<QMouseEvent>   //kfss add
QLabel *Taget_label;
QImage Taget_Temp;

extern HWND hWnd;
extern long lPort;
long lUserID=0;

HK_Video::HK_Video(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::HK_Video)
{
  ui->setupUi(this);
  Taget_label=ui->HK_Video_Label;

  Front_Button=NULL;
  Target_Camera_Name.clear();

//
  Timer = new QTimer(this);
  connect(Timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
  connect(ui->Area_Alarm_tablwidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(CameraTableWiget(QTableWidgetItem*)));
  //connect(this,SIGNAL(postAlarmID(QString)),database,SLOT(deleteAlarmInfo(QString)));
 // Init_Alarm_Table();
  Flag=0;
  ui->HK_Video_Label->installEventFilter(this);
  QString str=QStringLiteral("6kV工作Ⅳ段#4F磨煤机2470开关检修，责任人张涤平，三讲一落实");
  QString str1=QStringLiteral("6kV工作ⅣB段#4F磨煤机#2470开关由“热备用”转检修（合地刀），责任人谢鹏，许可人确认安措");
  ui->personLabel->setWordWrap(true);
  ui->personLabel->setText(QString("%1\n%2").arg(str).arg(str1));
//  list=ReadDataFromMysql::instance()->CameraAlarmInfo(Target_Area_Name);  //获得表格的显示数据
//  for(int i=0;i<list.size();i++)
//  {
//     Cameradata name=list.at(i);
//     qDebug()<<"NAME="<<name.employee_name;
//  }
}

HK_Video::~HK_Video()
{
  if(Target_Video.Video_ID!=-1)
  {
    Target_Video.Stop_Video(Target_Video.Video_ID);
    Target_Video.Video_Logout(Target_Video.Video_ID);
    Target_Video.Video_Clean();
    Target_Video.Video_ID=-1;
  }
  delete Timer;
  delete ui;
}

void HK_Video::timerUpdate()
{

  if(0==Flag)
  {
    Flag=1;
    CameraWidgetRealize();
  }

  //CameraTableDisp(database->CameraAlarmInfo());
  //获取当前摄像头所在的区域  kfss add 4
  Target_Area_Name=ReadDataFromMysql::instance()->Get_Area_Name_From_Camera_Name(Curremt_Camera_Name);
  CameraTableDisp(ReadDataFromMysql::instance()->CameraAlarmInfo(Target_Area_Name));
}

void HK_Video::CameraWidgetRealize()
{
  //检查区域名称   
  Init_Alarm_Table();

  QStringList Curremt_Area;
  Curremt_Area=ReadDataFromMysql::instance()->Get_Area_ID();
  int Counter=Curremt_Area.size();
  QList<Video> Current_Video;
  Video Tager_Video;
  int m=0;

  for(int i=0;i<Counter;i++)
  {
    //依据区域ID获取区域名和摄像头名称
    Current_Video=ReadDataFromMysql::instance()->Get_Area_Name(Curremt_Area.at(i));
    for(int j=0;j<Current_Video.size();j++)
    {
      Tager_Video=Current_Video.at(j);
      for(m=0;m<Current_Area_Name.count();m++)
      {
          if(Tager_Video.Area_Name==Current_Area_Name.at(m))
          {
            break;
          }
      }
      if(m==Current_Area_Name.count())
      {
          Current_Area_Name<<Tager_Video.Area_Name;
      }
      QListWidgetItem *item2=new QListWidgetItem();
      item2->setSizeHint(QSize(200,40));
      QPalette pal;
      pal.setColor(QPalette::ButtonText,QColor(255,255,255));
      CameraABtn=new QPushButton(tr("%1").arg(Tager_Video.Video_Name));
     // CameraABtn->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
      CameraABtn->setPalette(pal);
      Last_List<<CameraABtn;
      qDebug()<<"CameraABtn="<< CameraABtn->text();
    /*  CameraABtn->setStyleSheet("QPushButton:hover{background-color: rgb(0, 0, 255)} \
        QPushButton:hover:pressed {background-color: rgb(245, 0, 255)} \
        QPushButton:{background-color: rgb(0, 85, 255)}");  */
      CameraABtn->setFocusPolicy(Qt::NoFocus);    //
      CameraABtn->setFlat(true);
      connect(CameraABtn,SIGNAL(clicked(bool)),this,SLOT(CameraBBtn_Clicked()));
      ui->Video_listWidget->addItem(item2);
      ui->Video_listWidget->setItemWidget(item2,CameraABtn);
      if(Target_Camera_Name==Tager_Video.Video_Name)
      {
          Target_Camera_Name.clear();
          Curremt_Camera_Name=Tager_Video.Video_Name;
          emit CameraABtn->clicked(true);
      }
      else if((0==j)&&(i==0))
      {
          Curremt_Camera_Name=Tager_Video.Video_Name;
          emit CameraABtn->clicked(true);
      }
    }
  }
  ui->Video_listWidget->setFlow(QListView::LeftToRight);
}

void HK_Video::Init_Alarm_Table()
{
    //设置listwidget的显示格式
    qDebug()<<"datalist1111="<<datalist.size();
    ui->listWidget->setViewMode(QListView::IconMode);
    ui->listWidget->setFlow(QListWidget::LeftToRight);
    ui->listWidget->setResizeMode(QListView::Adjust);//设置QListView大小改变时，图标的调整模式，默认是固定的，但可以改成自动调整：
    ui->listWidget->setTextElideMode(Qt::ElideRight);
    ui->listWidget->setSpacing(12);
    QList<Cameradata> list= ReadDataFromMysql::instance()->Hk_VideoPerson();

    Cameradata data;
    for(int i=0;i<list.size();i++)
    {
        data=list.at(i);

        QListWidgetItem *Listitem=new QListWidgetItem(QIcon(":/image/TicketStep/person.png"),data.employee_name);
        Listitem->setTextColor(QColor(255,255,255));

        ui->listWidget->addItem(Listitem);
    }


  //初始化tableWidget
  ui->Area_Alarm_tablwidget->verticalHeader()->setVisible(false);  //隐藏垂直表头
  ui->Area_Alarm_tablwidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  //禁止编辑表格

  QStringList header;
  header.push_back(QStringLiteral("序号"));
  header.push_back(QStringLiteral("安全区域"));
  header.push_back(QStringLiteral("作业名称"));
  header.push_back(QStringLiteral("告警源"));
  header.push_back(QStringLiteral("告警类型"));
  header.push_back(QStringLiteral("告警信息"));
  header.push_back(QStringLiteral("告警时间"));
  header.push_back(QStringLiteral("人员名称"));
  //header.push_back(tr(""));
  ui->Area_Alarm_tablwidget->setHorizontalHeaderLabels(header);  //设置表头内容
  ui->Area_Alarm_tablwidget->horizontalHeader()->setStretchLastSection(true);
  ui->Area_Alarm_tablwidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);    //单元格高度自适应
  ui->Area_Alarm_tablwidget->setFocusPolicy(Qt::NoFocus);   //设置表格点击无焦点
  ui->Area_Alarm_tablwidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置表格点击为整行选中
  ui->Area_Alarm_tablwidget->setAlternatingRowColors(true);  //设置表格颜色交替显示
}

//
void HK_Video::CameraBBtn_Clicked()
{
      QObject *obj = sender();
      QPushButton *button_tmp = qobject_cast<QPushButton *>(obj);  //获取到发送信号的对象名
      qDebug()<<QStringLiteral("视频按钮按下了");
      if(button_tmp)
      {
     //   if(Front_Button!=button_tmp)
        {
          timerUpdate();
          if(Front_Button!=NULL)
          {
            Front_Button->setDown(false);
            Front_Button->setStyleSheet("QPushButton:{background-color: rgb(0,85,255)}"
                                        "QPushButton:hover{background-color:rgb(0,0,255)}"
                                        "QPushButton:pressed{background-color: rgb(245,0,255)}");
          }

          if(Target_Video.Video_ID!=-1)
          {
            Target_Video.Stop_Video(Target_Video.Video_ID);
            Target_Video.Video_ID=-1;
            ui->HK_Video_Label->setStyleSheet("border-image: url(:/image/securitycamera_hover.png)");
          }
          QString actionName = button_tmp->text();
          Curremt_Camera_Name=actionName;
          Front_Button=button_tmp;    //备份按下的按钮
          button_tmp->setDown(true);
//          button_tmp->setStyleSheet("background-color: rgb(85, 0, 255)");
          Video_Info_Temp=ReadDataFromMysql::instance()->Get_Video_Info(actionName);
          qDebug()<<"Video_Info_Temp="<<actionName;

          if(Curremt_Camera_Name.compare("6kV工作Ⅳ段配电室Camera1")==0)
          {
              ui->listWidget->clear();
              Init_Alarm_Table();
              QString str =QStringLiteral("6kV工作Ⅳ段#4F磨煤机2470开关检修，责任人张涤平，三讲一落实");
              QString str1 = QStringLiteral("6kV工作ⅣB段#4F磨煤机#2470开关由“热备用”转检修（合地刀），责任人谢鹏，许可人确认安措");
		
              ui->personLabel->setWordWrap(true);
              ui->personLabel->setText(QString("%1\n %2").arg(str).arg(str1));
          }

          if(Curremt_Camera_Name.compare("#9皮带秤")==0)
          {
              qDebug()<<"111";
              ui->Area_Alarm_tablwidget->clearContents();  //清除表格内容不包括标题栏
              ui->personLabel->clear();
              ui->listWidget->clear();
              qDebug()<<"222";
          }

          if(Curremt_Camera_Name.compare("一期煤场东Camera3")==0)
          {
              qDebug()<<"333";
              ui->Area_Alarm_tablwidget->clearContents();  //清除表格内容不包括标题栏
              ui->personLabel->clear();
              ui->listWidget->clear();
              qDebug()<<"444";
          }

          if(-1==Target_Video.Start_Video(Video_Info_Temp))
          {
			  QMessageBox::critical(NULL, QStringLiteral("提示"), QStringLiteral("视频连接失败"));
          }
        }
      }
}

void HK_Video::CameraTableDisp(QList<Cameradata> list)
{
    for(int i=0;i<list.size();i++)
    {
        data=list.at(i);
        datalist.append(data);
        QTableWidgetItem *NumItem=new QTableWidgetItem(data.num);
        NumItem->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *AreanameItem=new QTableWidgetItem(data.area_name);
        AreanameItem->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *WorknameItem=new QTableWidgetItem(data.work_name);
        WorknameItem->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *DevnameItem=new QTableWidgetItem(data.device_name);
        AreanameItem->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *AlarmTypeName=new QTableWidgetItem(data.alarm_type_name);
        AlarmTypeName->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *AlarmContent=new QTableWidgetItem(data.alarm_content);
        AlarmContent->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *AlarmTime=new QTableWidgetItem(data.alarm_time);
        AlarmTime->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *Employeename=new QTableWidgetItem(data.employee_name);
        Employeename->setTextAlignment(Qt::AlignCenter);



        ui->Area_Alarm_tablwidget->setItem(i,0,NumItem);
        ui->Area_Alarm_tablwidget->setItem(i,1,AreanameItem);
        ui->Area_Alarm_tablwidget->setItem(i,2,WorknameItem);
        ui->Area_Alarm_tablwidget->setItem(i,3,DevnameItem);
        ui->Area_Alarm_tablwidget->setItem(i,4,AlarmTypeName);
        ui->Area_Alarm_tablwidget->setItem(i,5,AlarmContent);
        ui->Area_Alarm_tablwidget->setItem(i,6,AlarmTime);
        ui->Area_Alarm_tablwidget->setItem(i,7,Employeename);
    }
}

void HK_Video::CameraTableWiget(QTableWidgetItem *item)
{
    if(item->column() == 8)
    {
        QString Temp=ReadDataFromMysql::instance()->CameraAlarmInfo().at(item->row() * 9 + item->column());
        ReadDataFromMysql::instance()->delete_Alarm_Info(Temp);
        timerUpdate();
    }
}


bool HK_Video::eventFilter(QObject *obj, QEvent *event)
{
    static int Flag=0;
    if (obj == ui->HK_Video_Label)
    {
        if (event->type() == QEvent::MouseButtonDblClick)
        {
            if(0==Flag)
            {
                Flag=1;
                ui->Video_listWidget->setVisible(false);
                ui->Area_Alarm_tablwidget->setVisible(false);
                ui->groupBox->setVisible(false);
                ui->groupBox_2->setVisible(false);
                ui->personLabel->setVisible(false);
                ui->gridLayout->setRowStretch(0,0);
                ui->listWidget->setVisible(false);
            }
            else if(1==Flag)
            {
                Flag=0;
                ui->Video_listWidget->setVisible(true);
                ui->Area_Alarm_tablwidget->setVisible(true);
//                ui->Text_Label->setVisible(true);
                ui->personLabel->setVisible(true);
                ui->groupBox->setVisible(true);
                ui->groupBox_2->setVisible(true);
                ui->listWidget->setVisible(true);
                ui->gridLayout->setRowStretch(0,2);

            }
            return true;
        }
        else
        {
            return false;
        }
    }
    return QObject::eventFilter(obj,event);

}

//显示指定摄像头内容
void HK_Video::Appoint_Camera(QString Camera_Name)
{
    Target_Camera_Name=Camera_Name;
    QPushButton* Button_Temp;
    for(int i=0;i<Last_List.count();i++)
    {
      Button_Temp=Last_List.at(i);
      if(Button_Temp->text()==Camera_Name)
      {
        emit Button_Temp->clicked(true);
        break;
      }
    }
}

void HK_Video::Stop_Camera()
{
  Target_Video.Stop_Video(Target_Video.Video_ID);  
}

void HK_Video::ClearData()
{
  ui->Video_listWidget->clear();
  Current_Area_Name.clear();
}





