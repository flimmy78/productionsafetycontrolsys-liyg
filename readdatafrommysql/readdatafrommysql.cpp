﻿#include "readdatafrommysql.h"
#include "../log/ris_log.h"
#include "../core/settings.h"
#include <QDebug>
#include <QSqlError>
#include <QCoreApplication>
#include <QSettings>
ReadDataFromMysql* ReadDataFromMysql::instance_=NULL;

//曲线图查询实现
QList<AniLayoutChart> ReadDataFromMysql::Updata_AniLayoutChart()
{
    QList<AniLayoutChart> AniLayoutChartList;

    db_ = getDb("mydb");
    if(!db_.isOpen())
    {
        if(!db_.open())
        {
            print_info("Updata_AniLayoutChart open database is error!");
            return AniLayoutChartList;
        }
        else
        {
            print_info("Updata_AniLayoutChart open database is success!");
        }
    }

        AniLayoutChart AniLayoutChart_table;
        QSqlQuery query(SQL_SELECTBIGHT,db_);
        bool ok = true;
        while(query.next())
        {
            AniLayoutChart_table.Data_Time = query.value("alarm_time").toDateTime();
            AniLayoutChart_table.Alarm_Num = query.value("alarm_count").toInt(&ok);
            AniLayoutChartList<<AniLayoutChart_table;
        }
        return AniLayoutChartList;
}

//安全管控系统首页曲线图数据查询结果
 void ReadDataFromMysql::Updata_AreaLayoutChart(QList<AreaLayoutChart> &list)
{
     db_ = getDb("mydb");
     if(!db_.isOpen())
     {
         if(!db_.open())
         {
             print_info("Updata_AreaLayoutChart open database is error!");
         }
         else
         {
             print_info("Updata_AreaLayoutChart open database is success!");
         }
     }

       QSqlQuery query(SQL_HOMEPAGECURVE,db_);

       while(query.next())
       {
           AreaLayoutChart AreaLayoutChart_table;
           AreaLayoutChart_table.Data_Time = query.value("ALARM_TIME").toDateTime();
           AreaLayoutChart_table.Alarm_Type = query.value("ALARM_TYPE_NAME").toString();
           AreaLayoutChart_table.Alarm_Num = query.value("ALARM_COUNT").toInt();
           list<<AreaLayoutChart_table;

       }
}


ReadDataFromMysql::ReadDataFromMysql()
{

}



bool ReadDataFromMysql::openDb()
{
    db_ = QSqlDatabase::addDatabase(Config.DbType,"mydb");
    db_.setHostName(Config.HostName);
    db_.setPort(Config.HostPort);
    db_.setDatabaseName(Config.Databasename);
    db_.setUserName(Config.UserName);
    db_.setPassword(Config.UserPassword);
    return db_.open();
}

QSqlDatabase &ReadDataFromMysql::getDb(QString name)
{
    db_= QSqlDatabase::database(name);
    return db_;
}

void ReadDataFromMysql::closeDb()
{
    db_.close();
}

ReadDataFromMysql *ReadDataFromMysql::instance()
{
    if(ReadDataFromMysql::instance_==NULL)
    {
        ReadDataFromMysql::instance_=new ReadDataFromMysql();
    }
    return ReadDataFromMysql::instance_;
}



ReadDataFromMysql::~ReadDataFromMysql()
{
    if(NULL !=instance_)
    {
        delete instance_;
    }
}


QString ReadDataFromMysql::TodayAlarmTotalNum()              //查看本日告警总数
{
    QString strRet="0";
    db_ = getDb("mydb");
    if(!db_.isOpen())
    {
        if(!db_.open())
        {
            print_info("TodayAlarmTotalNum open database is error!");
            return strRet;
        }
        else
        {
            print_info("TodayAlarmTotalNum open database is success!");
        }
    }

    QString strSql = QString("select count(*) from ws_employee_area_rt rt  where date_format(rt.UPDATE_TIME,'%Y%m%d') = date_format(now(),'%Y%m%d')");
    QSqlQuery query(strSql,db_);

    while(query.next())
    {
        strRet = query.value(0).toString();    //如果记录为0
    }
    qDebug()<<"人员数量"<<strRet;
    return strRet;
}

QString ReadDataFromMysql::CurAlarmNum()                  //查看当前告警数
{

    QString strRet="0";
    db_ = getDb("mydb");
    if(!db_.isOpen())
    {
        if(!db_.open())
        {
            print_info("CurAlarmNum open database is error!");
            return strRet;
        }
        else
        {
            print_info("CurAlarmNum open database is success!");
        }
    }

    QSqlQuery query("select count(*) from v_ws_alarm_rt",db_);

    while(query.next())
    {
       strRet =  query.value(0).toString();
    }
//    qDebug()<<"告警总数="<<strRet;
    return strRet;
}

QString ReadDataFromMysql::CurExecWorkNum()                      //查看当前工作数
{
    QString strRet="0";
    db_ = getDb("mydb");
    if(!db_.isOpen())
    {
        if(!db_.open())
        {
            print_info("CurExecWorkNum open database is error!");
            return strRet;
        }
        else
        {
            print_info("CurExecWorkNum open database is success!");
        }
    }

    QSqlQuery query("select count(*) from ws_work ws where ws.WORK_STATUS in (0,1)",db_);

    while(query.next())
    {
        strRet =  query.value(0).toString();
    }
    qDebug()<<"作业总数"<<strRet;
    return strRet;
}

QString ReadDataFromMysql::HasSignPersonNum()                      //查看已签到人员数
{
    QString strRet="0";
    db_ = getDb("mydb");
    if(!db_.isOpen())
    {
        if(!db_.open())
        {
            print_info("HasSignPersonNum open database is error!");
            return strRet;
        }
        else
        {
            print_info("HasSignPersonNum open database is success!");
        }
    }

    QSqlQuery query("select count(distinct employee_id) from ws_work_employee where SIGN = 1  and work_id in (select work_id from ws_work where work_status in(1,2,3,5,6,7) and WORK_NAME LIKE '%6kV%')",db_);

    while(query.next())
    {
        strRet =  query.value(0).toString();
    }
    qDebug()<<QString::fromLocal8Bit("已签到人员数")<<strRet;
    return strRet;
}

QString ReadDataFromMysql::NoSignPersonNum()                      //查看未签到人员数
{
    QString strRet="0";
    db_ = getDb("mydb");
    if(!db_.isOpen())
    {
        if(!db_.open())
        {
            print_info("NoSignPersonNum open database is error!");
            return strRet;
        }
        else
        {
            print_info("NoSignPersonNum open database is success!");
        }
    }

    QSqlQuery query("select count(distinct employee_id) from ws_work_employee where SIGN = 0 and work_id in (select work_id from ws_work where work_status in(1,2,3,5,6,7) and WORK_NAME LIKE '%6kV%')",db_);

    while(query.next())
    {
        strRet =  query.value(0).toString();
    }
    qDebug()<<QString::fromLocal8Bit("未签到人员数")<<strRet;
    return strRet;
}

QString ReadDataFromMysql::OperatingWorkNum()                      //查看正在进行作业数
{
    QString strRet="0";
    db_ = getDb("mydb");
    if(!db_.isOpen())
    {
        if(!db_.open())
        {
            print_info("OperatingWorkNum open database is error!");
            return strRet;
        }
        else
        {
            print_info("OperatingWorkNum open database is success!");
        }
    }

    QSqlQuery query("select count(distinct work_id) from ws_work where work_status = 2 and WORK_NAME LIKE '%6kV%'",db_);

    while(query.next())
    {
        strRet =  query.value(0).toString();
    }
    qDebug()<<QString::fromLocal8Bit("正在进行作业数")<<strRet;
    return strRet;
}

QString ReadDataFromMysql::NoStartWorkNum()                      //查看未开始作业数
{
    QString strRet="0";
    db_ = getDb("mydb");
    if(!db_.isOpen())
    {
        if(!db_.open())
        {
            print_info("NoStartWorkNum open database is error!");
            return strRet;
        }
        else
        {
            print_info("NoStartWorkNum open database is success!");
        }
    }

    QSqlQuery query("select count(distinct work_id) from ws_work where work_status in(1,3,5,6,7) and WORK_NAME LIKE '%6kV%'",db_);

    while(query.next())
    {
        strRet =  query.value(0).toString();
    }
    qDebug()<<QString::fromLocal8Bit("未开始作业数")<<strRet;
    return strRet;
}

//QString ReadDataFromMysql::SysRunStatus()                  //首页作业数、作业人数、区域数
//{
//    QString strRet="";
//    db_ = getDb("mydb");
//    if(!db_.isOpen())
//    {
//        if(!db_.open())
//        {
//            print_info("SysRunStatus open database is error!");
//            return strRet;
//        }
//        else
//        {
//            print_info("SysRunStatus open database is success!");
//        }
//    }


//    QSqlQuery query1("select count(DEVICE_ID) from WS_DEVICE_RUN_RT",db_),query2("select count(INTERFACE_ID) from WS_INTERFACE_RUN_RT",db_);
//    while(query1.next() && query2.next())
//    {

//        if(query1.value(0).toInt() == 0 && query2.value(0).toInt() == 0)    //如果记录为0
//        {
//            strRet =  "正常";
//        }
//        else
//        {
//            strRet =  "";
//        }
//    }
//    return strRet;  //kfss add
//}

QStringList ReadDataFromMysql::TodayAlarmInfo()                   //查看今日告警信息
{
    QStringList AlarmInfoList;
    db_ = getDb("mydb");
    if(!db_.isOpen())
    {
        if(!db_.open())
        {
            print_info("TodayAlarmInfo open database is error!");
            return AlarmInfoList;
        }
        else
        {
            print_info("TodayAlarmInfo open database is success!");
        }
    }

    QSqlQuery query("select * from WS_ALARM_HIS",db_);

    while(query.next())
    {
        AlarmInfoList<<query.value(0).toString();
        AlarmInfoList<<query.value(1).toString();
        AlarmInfoList<<query.value(2).toString();
        AlarmInfoList<<query.value(3).toString();
        AlarmInfoList<<query.value(4).toString();
        AlarmInfoList<<query.value(5).toString();
        AlarmInfoList<<query.value(6).toString();
    }

    return AlarmInfoList;
}


QStringList ReadDataFromMysql::HomePageAlarmInfo()
{
    QStringList AlarmInfoList;

    db_ = getDb("mydb");
    if(!db_.isOpen())
    {
        if(!db_.open())
        {
            print_info("HomePageAlarmInfo open database is error!");
            return AlarmInfoList;
        }
        else
        {
            print_info("HomePageAlarmInfo open database is success!");
        }
    }

    QSqlQuery query("select EMPLOYEE_NAME,AREA_NAME,ALARM_CONTENT,ALARM_LEVEL,ALARM_ID,ALARM_TIME from V_WS_ALARM_RT_WK",db_);
    while(query.next())
    {
         AlarmInfoList<<query.value("EMPLOYEE_NAME").toString();
         AlarmInfoList<<query.value("AREA_NAME").toString();
         AlarmInfoList<<query.value("ALARM_CONTENT").toString();
         AlarmInfoList<<query.value("ALARM_LEVEL").toString();
         AlarmInfoList<<query.value("ALARM_ID").toString();
         AlarmInfoList<<query.value("ALARM_TIME").toString();
    }

    return AlarmInfoList;
}

//区域告警状态显示
bool ReadDataFromMysql::IsAreaAlarm()
{
    bool ret = false;
    db_ = getDb("mydb");
    if(!db_.isOpen())
    {
        if(!db_.open())
        {
            print_info("IsAreaAlarm open database is error!");
            return ret;
        }
        else
        {
            print_info("IsAreaAlarm open database is success!");
        }
    }


    QSqlQuery query(SQL_ALARM_AREA,db_);
    while(query.next())
    {
       if(query.value(0).toInt() != 0)
       {
          ret =  true;
       }
       else
       {
          ret =  false;
       }
    }

    return ret;
}



//点击tableWidet里确认按钮删除相关信息操作
void ReadDataFromMysql::deleteAlarmInfo(QString alarm_ID)
{
    if(!db_.isOpen())
    {
        if(!db_.open())
        {
            print_info("deleteAlarmInfo open database is error!");
            return;
        }
        else
        {
            print_info("deleteAlarmInfo open database is success!");
        }
    }

    QString strSql = QString("call P_WS_ALARM_CONFIRM('%1')").arg(alarm_ID);
    QSqlQuery query(db_);

    if(!query.exec(strSql))
    {
         strSql += "执行失败!";
         print_info(strSql);

    }
}

/************************************************************************
  Description：按条件查找告警记录并显示到告警记录表里面显示
  Input：
  Output:
  Return:
  Others:
************************************************************************/

QList<Alarm_His> ReadDataFromMysql::QueryByCriteriaToDatabase(QString safety_name,QString work_name,
                                                              QString dev_name,QString alarmtype, QString begintime,
                                                              QString endtime,QString person)
{
    qDebug()<<"safety="<<safety_name;
    QList<Alarm_His> alarmlist;
    Alarm_His alarm_his;
    if(!db_.isOpen())
    {
        if(!db_.open())
        {
            print_info("deleteAlarmInfo open database is error!");
            return alarmlist;
        }
        else
        {
            print_info("deleteAlarmInfo open database is success!");
        }
    }

    QSqlQuery query(QString("select * from v_ws_alarm_his where area_name like '%1%' and alarm_time between '%2' and '%3' "
                            "and device_name like '%4%' and work_name like '%5%' and alarm_type_name like '%6%' and employee_name like '%7%' ").arg(safety_name)
                    .arg(begintime).arg(endtime).arg(dev_name).arg(work_name).arg(alarmtype).arg(person),db_);
    while(query.next())
    {
        alarm_his.safety_zone=query.value("area_name").toString();  //安全区域
        alarm_his.workmanship=query.value("work_name").toString();  //作业名称
        alarm_his.devname=query.value("device_name").toString();   //设备名
        alarm_his.Alarm_type=query.value("alarm_type_name").toString(); //告警类型
        alarm_his.Alarm_info=query.value("alarm_content").toString(); //告警信息
        alarm_his.Alarm_time=query.value("alarm_time").toDateTime().toString("yyyy:MM:dd hh:mm"); //告警时间
        alarm_his.persion_name=query.value("employee_name").toString();  //人员名称
        alarmlist<<alarm_his;

        qDebug()<<"alarm12="<<alarm_his.safety_zone;
    }

    return alarmlist;
}

QStringList ReadDataFromMysql::DemandConditionData( QString strsql)
{
    QStringList list;
    if(!db_.isOpen())
    {
        if(!db_.open())
        {
            print_info("CameraAlarmInfo open database is error!");
            return list;
        }
        else
        {
            print_info("CameraAlarmInfo open database is success!");
        }
    }

    QSqlQuery query(strsql,db_);
    while(query.next())
    {
         list<<query.value(0).toString();
    }

    return list;
}



//查询两票操作表统计作业数、正在作业数、告警票数
bool ReadDataFromMysql::InquireTwoTicketSum(tickettype &ticketdata)
{
    if(!db_.isOpen())
    {
        if(!db_.open())
        {
            print_info("deleteAlarmInfo open database is error!");
            return 0;
        }
        else
        {
            print_info("deleteAlarmInfo open database is success!");
        }
    }

     QString worknum="select count(distinct work_id) from ws_work where work_status in(1,2,3,5,6,7) and WORK_TYPE = 1 and WORK_NAME LIKE '%6kV%'";
     QString startwork="select count(distinct work_id) from ws_work where work_status = 2 and WORK_TYPE = 1 and WORK_NAME LIKE '%6kV%'";
     QString alarmticket="select wo.work_id from ws_work wo ,(select DISTINCT(WORK_ID) from ws_alarm_rt )rt where wo.WORK_ID = rt.work_id and wo.WORK_TYPE = 1";

    QSqlQuery query(QString("select(%1),(%2),(select count(*) from (%3) aa) from dual").arg(worknum).arg(startwork).arg(alarmticket),db_);
    while (query.next())
    {
        ticketdata.job_count=query.value(0).toString();
        ticketdata.doing_work=query.value(1).toString();
        ticketdata.alarmticket_num=query.value(2).toString();
    }

  //  qDebug()<<"123456="<<ticketdata.job_count<<ticketdata.doing_work<<ticketdata.alarmticket_num;
    return true;
}

QList<Cameradata> ReadDataFromMysql::Hk_VideoPerson()
{
    Cameradata data;
    QList<Cameradata> list;
    if(!db_.isOpen())
    {
        if(!db_.open())
        {
            print_info("TwoTicketExecMonitor open database is error!");
            return list;
        }
        else
        {
            print_info("TwoTicketExecMonitor open database is success!");
        }
    }

    QSqlQuery query("select DISTINCT EMPLOYEE_NAME from V_WS_ALARM_RT_WK WHERE EMPLOYEE_NAME is NOT NULL ",db_);
    while (query.next()) {
        data.employee_name=query.value(0).toString();
        list<<data;

        qDebug()<<"data.employee="<<data.employee_name;
    }

    return list;
}



//查询两票执行过程
QList<WorkMonitor> ReadDataFromMysql::TwoTicketExecMonitor(QString ticketType)
{
    WorkMonitor tmpWorkStep;
    QList<WorkMonitor> tmpWorkStepList;

    int i = 0;
    if(!db_.isOpen())
    {
        if(!db_.open())
        {
            print_info("TwoTicketExecMonitor open database is error!");
            return tmpWorkStepList;
        }
        else
        {
            print_info("TwoTicketExecMonitor open database is success!");
        }
    }

    QSqlQuery query(QString("SELECT a.WORK_ID,a.WORK_NAME,a.WORK_TYPE FROM ws_work a WHERE a.WORK_TYPE = %1").arg(ticketType),db_);
    while(query.next())
    {
        memset(tmpWorkStep.isExecute, 0, sizeof(tmpWorkStep.isExecute));
        tmpWorkStep.Work_Id = query.value("WORK_ID").toString();  //作业ID
        tmpWorkStep.Work_Name = query.value("WORK_NAME").toString(); //作业名称
        tmpWorkStep.Work_Type = query.value("WORK_TYPE").toInt();

        if(ticketType == "0")
        {
            tmpWorkStep.TicketType =QStringLiteral("电气第一种工作票");
        }
        else if(ticketType == "1")
        {
            tmpWorkStep.TicketType =QStringLiteral("电气倒闸操作票");
        }

        QSqlQuery query1(QString("SELECT b.ORDER_NO,b.IS_EXECUTE FROM ws_work_step_act b WHERE b.WORK_ID = \"%1\"").arg(tmpWorkStep.Work_Id),db_);
        i = 0;
        while(query1.next())
        {
            //qDebug()<<"monitor:"<<tmpWorkStep.Work_Id<<query1.value("IS_EXECUTE").toInt();

            tmpWorkStep.isExecute[i] = query1.value("IS_EXECUTE").toInt();
            i++;
        }

        QSqlQuery query2(QString("SELECT COUNT(*) FROM ws_alarm_rt c WHERE c.WORK_ID = \"%1\" AND c.ALARM_TYPE = \"SCAN_EQP_ERR\"").arg(tmpWorkStep.Work_Id),db_);
        while(query2.next())
        {
            tmpWorkStep.isHaveAlarm = query2.value("COUNT(*)").toInt();
        }

        tmpWorkStepList<<tmpWorkStep;
    }

    return tmpWorkStepList;
}

//更新首页表格数据
QList<AlarmFirstPage> ReadDataFromMysql::UpdateAlarm_FirstPage()
{
    AlarmFirstPage tmpAlarmInfo;
    QList<AlarmFirstPage> AlarmInfoList;

    db_ = getDb("mydb");
    if(!db_.isOpen())
    {
        if(!db_.open())
        {
            print_info("HomePageAlarmInfo open database is error!");
            return AlarmInfoList;
        }
        else
        {
            print_info("HomePageAlarmInfo open database is success!");
        }
    }

    QSqlQuery query("select EMPLOYEE_NAME,AREA_NAME,ALARM_CONTENT,ALARM_LEVEL,ALARM_ID,ALARM_TIME from V_WS_ALARM_RT_WK order by ALARM_TIME desc",db_);
    while(query.next())
    {
         tmpAlarmInfo.Employy_Name = query.value("EMPLOYEE_NAME").toString();
         tmpAlarmInfo.Area_Name = query.value("AREA_NAME").toString();
         tmpAlarmInfo.Alarm_Content = query.value("ALARM_CONTENT").toString();
         tmpAlarmInfo.Alarm_Level = query.value("ALARM_LEVEL").toString();
         tmpAlarmInfo.Alarm_ID = query.value("ALARM_ID").toString();
         tmpAlarmInfo.Alarm_Time = query.value("ALARM_TIME").toDateTime().toString("MM.dd hh:mm");
         AlarmInfoList<<tmpAlarmInfo;
    }

    return AlarmInfoList;
}

//两票监控获取工作票作业详情
tickettype ReadDataFromMysql::getWorkTicketInfoNum()
{
    tickettype tmpTicketInfo;
    if(!db_.isOpen())
    {
        if(!db_.open())
        {
            print_info("deleteAlarmInfo open database is error!");
            return tmpTicketInfo;
        }
        else
        {
            print_info("deleteAlarmInfo open database is success!");
        }
    }

     QString worknum="select count(distinct work_id) from ws_work where work_status in(1,2,3,5,6,7) and WORK_TYPE = 0 and WORK_NAME LIKE '%6kV%'";
     QString startwork="select count(distinct work_id) from ws_work where work_status = 2 and WORK_TYPE = 0 and WORK_NAME LIKE '%6kV%'";
  //  QString startwork="SELECT count(*) from ws_work where DATE_SUB(CURDATE(),INTERVAL 7 DAY)<=DATE('2016-11-23') and WORK_TYPE = 0 ";
     QString alarmticket="select wo.work_id from ws_work wo ,(select DISTINCT(WORK_ID) from ws_alarm_rt )rt where wo.WORK_ID = rt.work_id and wo.WORK_TYPE = 0";

    QSqlQuery query(QString("select(%1),(%2),(select count(*) from (%3) aa) from dual").arg(worknum).arg(startwork).arg(alarmticket),db_);
    while (query.next()) {
        tmpTicketInfo.job_count=query.value(0).toString();
        tmpTicketInfo.doing_work=query.value(1).toString();
        tmpTicketInfo.alarmticket_num=query.value(2).toString();
    }

  //  qDebug()<<"123456="<<ticketdata.job_count<<ticketdata.doing_work<<ticketdata.alarmticket_num;
    return tmpTicketInfo;
}

//首页作业表显示数据查询接口
void ReadDataFromMysql::homepage_operationticket(QList<opertion_station> &opertionList)
{
    opertion_station oper;
    if(!db_.isOpen())
    {
        if(!db_.open())
        {
            print_info("deleteAlarmInfo open database is error!");
            return ;
        }
        else
        {
            print_info("deleteAlarmInfo open database is success!");
        }
    }

    QSqlQuery query(SQL_HOMEPAGEOPERTION,db_);
    while (query.next()) {
        oper.work_name=query.value("WORK_NAME").toString();
        oper.work_time=query.value("plan_begintime").toDateTime().toString("yyyy-MM-dd");
        opertionList.push_back(oper);
    //    qDebug()<<QStringLiteral("首页作业表格数据显示:")<<oper.work_name;
    }


}


QStringList ReadDataFromMysql::CameraAlarmInfo()
{

    int i = 1;
    QStringList AlarmInfoList;

    if(!db_.isOpen())
    {
        if(!db_.open())
        {
            print_info("CameraAlarmInfo open database is error!");
            return AlarmInfoList;
        }
        else
        {
            print_info("CameraAlarmInfo open database is success!");
        }
    }

    QSqlQuery query(SQL_CAMERAINFO,db_);
    while(query.next())
    {
         AlarmInfoList<<QString::number(i);
         AlarmInfoList<<query.value("AREA_NAME").toString();
         AlarmInfoList<<query.value("WORK_NAME").toString();
         AlarmInfoList<<query.value("DEVICE_NAME").toString();
         AlarmInfoList<<query.value("ALARM_TYPE_NAME").toString();
         AlarmInfoList<<query.value("ALARM_CONTENT").toString();
         AlarmInfoList<<query.value("ALARM_TIME").toString();
         AlarmInfoList<<query.value("EMPLOYEE_NAME").toString();
         AlarmInfoList<<query.value("ALARM_ID").toString();
         i++;
    }

    return AlarmInfoList;
}
/**********************kfss add************************************/
QList<QString> ReadDataFromMysql::Updata_Area_List()
{
   QList<QString> Area_List;

   if(!db_.isOpen())
   {
       if(!db_.open())
       {
           print_info("Updata_Area_List open database is error!");
           return Area_List;
       }
       else
       {
           print_info("Updata_Area_List open database is success!");
       }
   }


   QSqlQuery query(SQL_LIST_UPDATA,db_);
   while(query.next())
   {
        Area_List<<query.value("area_name").toString();
//        qDebug()<<"query="<<query.value(0).toString();
   }
   return Area_List;
}


QList<Work_Table> ReadDataFromMysql::Updata_tableWidget_Work(QString Area_Name)
{
  QList<Work_Table> tableWidget_2_Value;

  db_ = getDb("mydb");

  if(!db_.isOpen())
  {
      if(!db_.open())
      {
          print_info("Updata_tableWidget_Work open database is error");
          return tableWidget_2_Value;
      }
      else
      {
          print_info("Updata_tableWidget_Work open database is success");
      }
  }

  Work_Table RealData;

  QSqlQuery query(SQL_ALARM_UPDATA_1,db_);
  while(query.next())
  {
    if(Area_Name!=query.value(1).toString())
    {
      continue;
    }
    RealData.Control_Name=query.value(3).toString();   //作业名称
    RealData.Controler=query.value(4).toString();     //责任人
    RealData.Status=query.value(5).toString();         //作业状态
    if(RealData.Status=="0")
    {
      RealData.Status="未开始";
    }
    else if(RealData.Status=="1")
    {
      RealData.Status="正在进行";
    }
    else if(RealData.Status=="2")
    {
      RealData.Status="已结束";
    }
    RealData.Safe_Status=query.value(6).toString();    //安全状态
    tableWidget_2_Value<<RealData;
  }


  return tableWidget_2_Value;

}


QList<Alarm_His> ReadDataFromMysql::SelectTableToAlarmTableWidget()
{
    QList<Alarm_His> Alarm_his_tablewidget;
    db_ = getDb("mydb");

    if(!db_.isOpen())
    {
        if(!db_.open())
        {
            print_info("SelectTableToAlarmTableWidget open database is error");
            return Alarm_his_tablewidget;
        }
        else
        {
            print_info("SelectTableToAlarmTableWidget open database is success");
        }
    }

       Alarm_His alarm_his_table;
       QSqlQuery query("select ws.WORK_name,typ.ALARM_TYPE_NAME,rt.ALARM_CONTENT,rt.ALARM_TIME,area.AREA_NAME,rt.DEVICE_ID,tt.last_name from ws_alarm_rt  rt  left join ws_work  ws on rt.work_id = ws.work_id  left join ws_area area   on rt.AREA_ID = area.AREA_ID left join tt_employee tt   on tt.user_name = rt.EMPLOYEE_ID  left join ws_alarm_type typ   on typ.ALARM_TYPE = rt.ALARM_TYPE order by rt.ALARM_TIME desc",db_);

       while (query.next())
       {
           alarm_his_table.workmanship=query.value(0).toString();  //作业名称
           alarm_his_table.Alarm_type=query.value(1).toString(); //告警类型
           alarm_his_table.Alarm_info=query.value(2).toString(); //告警信息
           alarm_his_table.Alarm_time=query.value(3).toDateTime().toString("yyyy-MM-dd hh:mm"); //告警时间
           alarm_his_table.safety_zone=query.value(4).toString();  //安全区域
           alarm_his_table.devname=query.value(5).toString();   //设备名
           alarm_his_table.persion_name=query.value(6).toString();  //人员名称
           Alarm_his_tablewidget<<alarm_his_table;
       }

       return Alarm_his_tablewidget;
}

QList<Alarm_Table> ReadDataFromMysql::Updata_tableWidget_Alarm(QString Area_Id)
{
  QList<Alarm_Table> tableWidget_Value;
  db_ = getDb("mydb");

  if(!db_.isOpen())
  {
      if(!db_.open())
      {
          print_info("Updata_tableWidget_Alarm open database is error");
          return tableWidget_Value;
      }
      else
      {
          print_info("Updata_tableWidget_Alarm open database is success");
      }
  }
  Alarm_Table RealData2;
  QString Temp;
  QSqlQuery query(SQL_ALARM_UPDATA,db_);

  while(query.next())
  {
    //  qDebug()<<"123="<<query.value("AREA_ID").toString();
      if(Area_Id!=query.value("AREA_ID").toString())
      {
        continue;
      }
      RealData2.Alarm_Event=query.value("ALARM_TYPE_NAME").toString();     //告警事件
      RealData2.Alarm_Lever=query.value("ALARM_LEVEL").toString();    //告警级别
      RealData2.Controler=query.value("EMPLOYEE_NAME").toString();      //责任人
      RealData2.Control_Desc=query.value("WORK_NAME").toString();   //作业内容
      RealData2.Alarm_Source=query.value("AREA_NAME").toString()+" "+query.value("DEVICE_NAME").toString();   //告警源
      RealData2.Safe_Mode_ID=query.value("SAFEMODEL_ID").toString();
      RealData2.Device_ID=query.value("DEVICE_ID").toString();
      Temp=query.value("ALARM_TIME").toString();
      Temp.replace("T"," ");
      RealData2.Time=Temp;                //发生时间
      RealData2.Video="调取视频";          //调取视频
      RealData2.Shot="调取快照";           //调取快照
      RealData2.Enter="确定";             //确定
      RealData2.Alarm_ID=query.value("ALARM_ID").toString();
  //    qDebug()<<"realdata2"<<RealData2.Alarm_ID<<RealData2.Alarm_Source;
      tableWidget_Value<<RealData2;
  }

  return tableWidget_Value;
}

bool ReadDataFromMysql::Control_Door(Door_Cmd Cmd_Door)
{
  bool ret = false;
  db_ = getDb("mydb");

  if(!db_.isOpen())
  {
      if(!db_.open())
      {
          print_info("Control_Door open database is error");
          return ret;
      }
      else
      {
          print_info("Control_Door open database is success");
      }
  }



  QString strTmp;
  //if(1==Connect_Flag)
  if(1)
  {
      QString Command_Rule=QString("INSERT INTO ws_cmd_his (EVENT_ID,CMD_ID,DEVICE_ID,CMD_TYPE,CMD_CONTENT,"
                           "SEND_TIME,CMD_STATUS) VALUES"
                           "('%1','%2','%3','%4',%5,'%6',%7)")
                            .arg(Cmd_Door.EVENT_ID)
                            .arg(Cmd_Door.CMD_ID)
                            .arg(Cmd_Door.DEVICE_ID)
                            .arg(Cmd_Door.CMD_TYPE)
                            .arg(Cmd_Door.CMD_CONTENT)
                            .arg(Cmd_Door.SEND_TIME)
                            .arg(Cmd_Door.Cmd);

      QSqlQuery query(db_);


      if(query.exec(Command_Rule))
      {
          ret =  true;
      }
      else
      {
          ret = false;
          strTmp = QString("Command_Rule：%1 is failed.").arg(Command_Rule);
          print_info(strTmp);
      }
  }
  return ret;
}



bool ReadDataFromMysql::Alarm_Lamp_Do_State(QString EVENT_ID)
{
    bool ret = false;
    db_ = getDb("mydb");

    if(!db_.isOpen())
    {
        if(!db_.open())
        {
            print_info("Door_State open database is error");
            return ret;
        }
        else
        {
            print_info("Door_State open database is success");
        }
    }

    QString   Command_Rule=QString("select CMD_STATUS from ws_cmd_his where EVENT_ID='%1' ").arg(EVENT_ID);
    QSqlQuery query(Command_Rule,db_);
    int i=0;
      while(query.next())
      {
        i=query.value(0).toInt();
      }

     if(i==0)
     {
       ret =  false;
     }
     else if(i==1)
     {
       ret =  true;
     }

     return ret;

}

int ReadDataFromMysql::Door_State()
{
  int Status=3;
  db_ = getDb("mydb");

  if(!db_.isOpen())
  {
      if(!db_.open())
      {
          print_info("Door_State open database is error");
          return Status;
      }
      else
      {
          print_info("Door_State open database is success");
      }
  }


  QSqlQuery query(SQL_DOOR_STATUS,db_);
  if(query.exec()==true)
  {
    Status=3;
    while(query.next())
    {
      Status=query.value(0).toInt();
    }
  }
  else
  {
    Status=3;
  }
  return Status;
}


int ReadDataFromMysql::Stored_Procedure(QString CMD)
{
  int ret = 0;

  db_ = getDb("mydb");

  if(!db_.isOpen())
  {
      if(!db_.open())
      {
          print_info("Stored_Procedure open database is error");
          return ret;
      }
      else
      {
          print_info("Stored_Procedure open database is success");
      }
  }



  QString Command=QString("CALL P_WS_ALARM_CONFIRM('%1')").arg(CMD);
  QSqlQuery query(Command,db_);
  if(query.exec())
  {

    ret =  1;
  }
  else
  {
    ret =  0;
  }
  return ret;
}



//两票界面数据库查询语句实现接口，并返回查询到的结果
int ReadDataFromMysql::QueryWorkStatisticsTable(QList<TwoTicketWorkstation> &dataList)
{

        int ret = -1;

        db_ = getDb("mydb");

        if(!db_.isOpen())
        {
            if(!db_.open())
            {
                print_info("QueryWorkStatisticsTable open database is error");
                return ret;
            }
            else
            {
                print_info("QueryWorkStatisticsTable open database is success");
            }
        }

        TwoTicketWorkstation data;
        QSqlQuery query(SQL_ALARM_TWOTICKET,db_);

        while (query.next()) {
              data.workname=query.value("area_name").toString();  //作业名称
              data.workplace=query.value("work_name").toString(); //工作地点
//            data.append(query.value("area_name").toString());
//            data.append(query.value("work_name").toString());

            if(query.value("work_type").toInt()==0)
            {
               //   data.append("工作票");
                  data.worktype="工作票";
            }
            else
            {
                 // data.append("操作票");
                   data.worktype="操作票";
            }
            data.planbegintime=query.value("plan_begintime").toDateTime().toString("MM.dd hh:mm");//开始时间
            data.planendtime=query.value("plan_endtime").toDateTime().toString("MM.dd hh:mm");  //结束时间

//            data.append(query.value("plan_begintime").toDateTime().toString("MM.dd hh:mm"));
//            data.append(query.value("plan_endtime").toDateTime().toString("MM.dd hh:mm"));
            if(query.value("work_status").toInt()==0)
            {
//                  data.append("未开始");
                  data.workstation="未开始";
            }
            else if(query.value("work_status").toInt()==1)
            {
//                  data.append("正进行");
                  data.workstation="正进行";
            }
            else
            {
//                   data.append("已结束");
                   data.workstation="已结束";
            }
            data.alarmNums=query.value("nums").toString(); //告警数
            data.employeename=query.value("employee_name").toString(); //负责人
//            data.dutyperion=query.value("IS_SIGN").toString();  //责任人
            if(query.value("IS_SIGN").toInt()==0)
            {
                data.dutyperion="已到";
            }

            if(query.value("IS_SIGN")==1)
            {
                data.dutyperion="未到";
            }
//            data.arrive_persion=query.value("place").toString(); //人员是否到齐
            if(query.value("place").toInt()==0)
            {
                data.arrive_persion="已到";
            }
            if(query.value("place").toInt()==1)
            {
                data.arrive_persion="未到";
            }
            data.tobedue=query.value("ying_dao").toString();  //应到
            data.realityPersion=query.value("shi_dao").toString();  //实到
            dataList.append(data);

        }

        return dataList.size();
}

//工作表信息查询
WorkSheet_Info ReadDataFromMysql::Update_tableWidget_WorkSheet(QString workId)
{
       WorkSheet_Info WorkSheet_table;
       db_ = getDb("mydb");

       if(!db_.isOpen())
       {
           if(!db_.open())
           {
               print_info("Update_tableWidget_WorkSheet open database is error");
               return WorkSheet_table;
           }
           else
           {
               print_info("Update_tableWidget_WorkSheet open database is success");
           }
       }



       QSqlQuery query1(QString("select a.WORK_NAME,a.WORK_ADDR,a.PLAN_BEGINTIME,a.PLAN_ENDTIME,a.ACT_BEGINTIME,a.ACT_ENDTIME,a.WORK_DESC,a.WORK_ID from ws_work a where a.work_id=('%1')").arg(workId),db_);
       QSqlQuery query4(QString("select b.EMPLOYEE_NAME,IS_SIGN from ws_work_employee a join ws_account b on a.EMPLOYEE_ID=b.EMPLOYEE_ID where a.IS_MGR=1 and a.work_id=('%1')").arg(workId),db_);//责任人
       QSqlQuery query5(QString("select b.EMPLOYEE_NAME,IS_SIGN from ws_work_employee a join ws_account b on a.EMPLOYEE_ID=b.EMPLOYEE_ID where a.IS_MGR=0 and a.work_id=('%1')").arg(workId),db_);//操作人
       QSqlQuery query6(QString("SELECT c.ALARM_CONTENT,c.ALARM_TIME,c.WORK_ID from (select a.ALARM_CONTENT,a.ALARM_TIME,a.WORK_ID from WS_ALARM_RT a union select b.ALARM_CONTENT,b.ALARM_TIME,b.WORK_ID from WS_ALARM_HIS b) c where c.work_id=('%1') and DATE_FORMAT(c.ALARM_TIME,'%Y-%M-%d')= DATE_FORMAT(now(),'%Y-%M-%d')").arg(workId),db_);

       QString Temp;
       while(query1.next())
       {
           WorkSheet_table.Work_Name = query1.value(0).toString();
           WorkSheet_table.Work_Addr = query1.value(1).toString();
           Temp = query1.value(2).toString();
           Temp.replace("T"," ");
           WorkSheet_table.Plan_BeginTime = Temp;
           Temp = query1.value(3).toString();
           Temp.replace("T"," ");
           WorkSheet_table.Plan_EndTime = Temp;
           Temp = query1.value(4).toString();
           Temp.replace("T"," ");
           WorkSheet_table.Act_BeginTime = Temp;
           Temp = query1.value(5).toString();
           Temp.replace("T"," ");
           WorkSheet_table.Act_EndTime = Temp;
           WorkSheet_table.Work_Desc = query1.value(6).toString();
       }

       while(query4.next())
       {
           WorkSheet_table.DutyPerson_Name = query4.value(0).toString();
           WorkSheet_table.DutyPerson_IsSign = query4.value(1).toString();
       }
       while(query5.next())
       {
           WorkSheet_table.OperPerson_Name = query5.value(0).toString();
           WorkSheet_table.OperPerson_IsSign = query5.value(1).toString();
       }
       AllAlarm_Info TempInfo;
       QString Temp6;

       while(query6.next())
       {
           TempInfo.AlarmContent = query6.value(0).toString();
           Temp6 = query6.value(1).toString();
           Temp6.replace("T"," ");
           TempInfo.AlarmTime = Temp6;
           //qDebug()<<TempInfo.AlarmContent<<" "<<TempInfo.AlarmTime;
           WorkSheet_table.AllAlarmInfoList<<TempInfo;
       }

       return WorkSheet_table;
}

//操作票信息查询
QList<OperSheet_WorkStep> ReadDataFromMysql::Updata_tableWidget_OperSheet()
{

    QList<OperSheet_WorkStep> WorkStepList;
    OperSheet_WorkStep WorkStep_table;
    db_ = getDb("mydb");

    if(!db_.isOpen())
    {
        if(!db_.open())
        {
            print_info("Updata_tableWidget_OperSheet open database is error");
            return WorkStepList;
        }
        else
        {
            print_info("Updata_tableWidget_OperSheet open database is success");
        }
    }

    QSqlQuery query("SELECT a.STEP_ORDER,a.STEP_DESC,a.IS_OP,a.OP_TIME from ws_work_step a",db_);

    while (query.next())
    {
        WorkStep_table.Step_Order = query.value(0).toString();
        WorkStep_table.Step_Desc = query.value(1).toString();
        if(query.value(2).toString() == "1")
        {
            WorkStep_table.Is_Op = "√";
        }
        else
        {
            WorkStep_table.Is_Op = "-";
        }
        //WorkStep_table.Is_Op = query.value(2).toString();
        if(query.value(3).toString().isEmpty())
        {
            WorkStep_table.Op_Time = "-";
        }
        else
        {
            WorkStep_table.Op_Time = query.value(3).toString();
        }


        WorkStepList<<WorkStep_table;
    }

    return WorkStepList;
}

QList<Video> ReadDataFromMysql::Get_Area_Name(QString Area_Id)
{
    Video Video_Temp;
    QList<Video> List_Temp;

    db_ = getDb("mydb");

    if(!db_.isOpen())
    {
        if(!db_.open())
        {
            print_info("Get_Area_Name open database is error");
            return List_Temp;
        }
        else
        {
            print_info("Get_Area_Name open database is success");
        }
    }


    QString   Command_Rule=QString("select area_name,cameral_name from ws_video_device where area_id=%1").arg(Area_Id);
    QSqlQuery query(Command_Rule,db_);

    while(query.next())
    {
      Video_Temp.Area_Name=query.value(0).toString();
      Video_Temp.Video_Name=query.value(1).toString();
      Video_Temp.Area_ID=Area_Id;
      List_Temp<<Video_Temp;
    }
    return List_Temp;
}

//摄像界面告警表
QList<Cameradata> ReadDataFromMysql::CameraAlarmInfo(QString Area_Name)
{
    QList<Cameradata> list;
    Cameradata camerawidget;
    int i = 1;
    db_ = getDb("mydb");

    if(!db_.isOpen())
    {
        if(!db_.open())
        {
            print_info("Get_Video_Info open database is error");
            return list;
        }
        else
        {
            print_info("Get_Video_Info open database is success");
        }
    }

    QString Command=QString("select AREA_NAME,WORK_NAME,DEVICE_NAME,ALARM_TYPE_NAME,ALARM_CONTENT,ALARM_TIME,EMPLOYEE_NAME,ALARM_ID from V_WS_ALARM_RT_WK where AREA_NAME='%1' ").arg(Area_Name);
    QSqlQuery query(Command,db_);

    while(query.next())
    {   
         camerawidget.num=QString::number(i);
         camerawidget.area_name=query.value("AREA_NAME").toString();
         camerawidget.work_name=query.value("WORK_NAME").toString();
         camerawidget.device_name=query.value("DEVICE_NAME").toString();
         camerawidget.alarm_type_name=query.value("ALARM_TYPE_NAME").toString();
         camerawidget.alarm_content=query.value("ALARM_CONTENT").toString();
         camerawidget.alarm_time=query.value("ALARM_TIME").toDateTime().toString("yyyy:MM:dd hh:mm:ss");
         camerawidget.employee_name=query.value("EMPLOYEE_NAME").toString();
         camerawidget.alarm_id=query.value("ALARM_ID").toString();
         list<<camerawidget;
         i++;
    }
    return list;
}

QString ReadDataFromMysql::Get_Area_Name_From_Camera_Name(QString Camera_Name)
{
    QString Area_Name;
    db_ = getDb("mydb");

    if(!db_.isOpen())
    {
        if(!db_.open())
        {
            print_info("Get_Area_Name_From_Camera_Name open database is error");
            return Area_Name;
        }
        else
        {
            print_info("Get_Area_Name_From_Camera_Name open database is success");
        }
    }


    QString Command=QString("select area_name from ws_video_device where cameral_name='%1' ").arg(Camera_Name);
    QSqlQuery query(Command,db_);

    while(query.next())
    {
         Area_Name=query.value(0).toString();
    }
    return Area_Name;
}

Video_Info ReadDataFromMysql::Get_Video_Info(QString Video_Name)
{
     Video_Info Video_Temp;

     db_ = getDb("mydb");

     if(!db_.isOpen())
     {
         if(!db_.open())
         {
             print_info("Get_Video_Info open database is error");
             return Video_Temp;
         }
         else
         {
             print_info("Get_Video_Info open database is success");
         }
     }


     QString Command_Rule=QString("select video_ip,video_port,user_name,pwd,Channel_No from ws_video_device where cameral_name='%1' ").arg(Video_Name);
     qDebug()<<"Command_Rule="<<Command_Rule;
     QSqlQuery query(Command_Rule,db_);

     while(query.next())
     {
       Video_Temp.video_ip=query.value(0).toString();
       Video_Temp.video_port=query.value(1).toString();
       Video_Temp.user_name=query.value(2).toString();
       Video_Temp.pwd=query.value(3).toString();
       Video_Temp.Channel_No=query.value(4).toString();
     }

     return Video_Temp;

}

Video_Info ReadDataFromMysql::Get_Video_InfoOne(QString Video_Name)
{

    Video_Info Video_Temp;

    db_ = getDb("mydb");

    if(!db_.isOpen())
    {
        if(!db_.open())
        {
            print_info("Get_Video_Info open database is error");
            return Video_Temp;
        }
        else
        {
            print_info("Get_Video_Info open database is success");
        }
    }


    QString Command_Rule=QString("select video_ip,video_port,user_name,pwd,Channel_No from ws_video_device where area_name='%1' ").arg(Video_Name);
    qDebug()<<"Command_Rule="<<Command_Rule;
    QSqlQuery query(Command_Rule,db_);

    while(query.next())
    {
      Video_Temp.video_ip=query.value(0).toString();
      Video_Temp.video_port=query.value(1).toString();
      Video_Temp.user_name=query.value(2).toString();
      Video_Temp.pwd=query.value(3).toString();
      Video_Temp.Channel_No=query.value(4).toString();
    }

    return Video_Temp;
}

QString ReadDataFromMysql::Get_Area_ID(QString AREA_NAME)
{
  QString Area_Id;
  db_ = getDb("mydb");
  if(!db_.isOpen())
  {
      if(!db_.open())
      {
          print_info(" Get_Area_ID open database is error.");

          return Area_Id;
      }
      else
      {
           print_info(" Get_Area_ID open database is success.");
      }
  }

  QString   Command_Rule=QString("select AREA_ID from ws_area where AREA_NAME='%1'").arg(AREA_NAME);
  QSqlQuery query(Command_Rule,db_);

  while(query.next())
  {
    Area_Id=query.value("AREA_ID").toString();
  }
 // qDebug()<<"Area_id="<<Area_Id;
  return Area_Id;
}

QStringList ReadDataFromMysql::Get_Area_ID()
{
    QString Area_Id;
    QStringList Temp;

    db_ = getDb("mydb");
    if(!db_.isOpen())
    {
        if(!db_.open())
        {
            print_info(" Get_Area_ID open database is error.");

            return Temp;
        }
        else
        {
             print_info(" Get_Area_ID open database is success.");
        }
    }


    QString   Command_Rule=SQL_Get_AREA_ID;
    QSqlQuery query(Command_Rule,db_);

    while(query.next())
    {
      Area_Id=query.value(0).toString();
      Temp<<Area_Id;
    }
    return Temp;
}


//重点区域监控
QList<Import_Area> ReadDataFromMysql::Updata_tableWidget_ImportArea()
{
    QList<Import_Area> ImportAreaList;//区域人员查询信息链表
    db_ = getDb("mydb");

    if(!db_.isOpen())
    {
        if(!db_.open())
        {
            print_info(" Updata_tableWidget_ImportArea open database is error.");

            return ImportAreaList;
        }
        else
        {
             print_info(" Updata_tableWidget_ImportArea open database is success.");
        }
    }

       Import_Area ImportArea_table;
       QSqlQuery query(SQL_IMPORTAREA,db_);
       if(query.lastError().isValid())
       {
           qDebug()<<query.lastError();
       }
       while (query.next())
       {
           ImportArea_table.Area_Name = query.value(0).toString();  //区域名称
           ImportArea_table.SafeDeviceNum = query.value(1).toString(); //安全设备名称
           ImportArea_table.MakeDeviceNum = query.value(2).toString(); //生产设备名称
           ImportArea_table.WorkNum = query.value(3).toString(); //作业数
           ImportArea_table.AlarmNum = query.value(4).toString(); //告警数
           ImportArea_table.PersonNum = query.value(5).toString(); //作业人数
           ImportAreaList<<ImportArea_table;

           qDebug()<<query.value(0).toString()<<query.value(1).toString();
       }

       return ImportAreaList;

}

//区域人员查询
 bool ReadDataFromMysql::Updata_tableWidget_AreaEmployee(QList<Area_person>& list)
{
    Area_person data;
    bool ret = false;

    db_ = getDb("mydb");

    if(!db_.isOpen())
    {
        if(!db_.open())
        {
            print_info(" Updata_tableWidget_AreaEmployee open database is error.");

            return ret;
        }
        else
        {
             print_info(" Updata_tableWidget_AreaEmployee open database is success.");
        }
    }

    QString strTmp;
    QSqlQuery query(db_);

    if(!query.exec(SQL_AREAEMPLOYEE))
    {
        strTmp = QString("%1 区域人员查询表失败!").arg(SQL_AREAEMPLOYEE);

        ret = false;
    }
    else
    {
        while (query.next())
        {
            data.areaname=query.value(0).toString();
            data.workname=query.value(1).toString();
            data.employeename=query.value(2).toString();
            data.deptname=query.value(3).toString();
            data.starttime=query.value(4).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
            list<<data;
        }
         ret =  true;
    }

    return ret;

}

//查询首页曲线数据库表
 QList<WorkTotalPic> ReadDataFromMysql::SelectWorkTotalPic()
{
    QList<WorkTotalPic> datalist;
    WorkTotalPic work;
    QSqlQuery query(SQL_ALARM_TOTAL_PIC,db_);

    while (query.next()) {
       work.alarmtime=query.value(0).toString();
       work.alarmtypetime=query.value(1).toString();
       work.alarmcount=query.value(2).toString();
       datalist<<work;
    }

    return datalist;
}

 void ReadDataFromMysql::delete_Alarm_Info(QString alarm_ID)
 {

     db_ = getDb("mydb");

     if(!db_.isOpen())
     {
         if(!db_.open())
         {
             print_info(" delete_Alarm_Info open database is error.");

             return;
         }
         else
         {
              print_info(" delete_Alarm_Info open database is success.");
         }
     }

   QString strSql = QString("call P_WS_ALARM_CONFIRM('%1')").arg(alarm_ID);
   QSqlQuery query(db_);

   if(!query.exec(strSql))
   {
       strSql +=" 执行出错！";
       print_info(strSql);
   }

 }

 int ReadDataFromMysql::SelectAlarm_curvenData(QList<Alarm_data> &list)
 {
     Alarm_data data;

     db_ = getDb("mydb");

     if(!db_.isOpen())
     {
         if(!db_.open())
         {
             print_info(" SelectAlarm_curvenData open database is error.");

             return -1;
         }
         else
         {
              print_info(" SelectAlarm_curvenData open database is success.");
         }
     }


     QSqlQuery query(db_);

    if (query.exec(SQL_SELECTALARMDATA))
    {
        while (query.next()) {

            data.alarmID=query.value("alarm_id").toString();
            data.eventID=query.value("event_id").toString();
            data.alarm_event=query.value("alarm_content").toString();
            data.areaID=query.value("area_name").toString();

            list.push_back(data);
        }
    }
    else
    {
        return -1;
    }

     return list.size();
 }

 bool ReadDataFromMysql::Updatastatus_alarmtable(QString str)
 {
     bool ret =false;
     db_ = getDb("mydb");

     if(!db_.isOpen())
     {
         if(!db_.open())
         {
             print_info("open database is error");
             return ret;
         }
         else
         {
             print_info("open database is success");
         }
     }

     QString sql=QString("update ws_alarm_rt set read_status=1 where alarm_id ='%1'").arg(str);
     QSqlQuery query(db_);

     if(!query.exec(sql))
     {

         ret = false;
     }
     else
     {
        ret = true;
     }

     return ret;
 }


 QString ReadDataFromMysql::Get_Cameralname(QString Area_Id)
 {
   QString ameralname;

   db_ = getDb("mydb");

   if(!db_.isOpen())
   {
       if(!db_.open())
       {
           print_info("Get_Camera_Name open database is error");
           return ameralname;
       }
       else
       {
           print_info(" Get_Camera_Name open database is success");
       }
   }




   QString Command=QString("select cameral_name from ws_video_device where area_id ='%1' ").arg(Area_Id);
   QSqlQuery query(Command,db_);
   while(query.next())
   {
      ameralname=query.value(0).toString();
   }
   return ameralname;
 }



 QString ReadDataFromMysql::Get_Camera_Name(QString DEVICE_ID,QString SAFEMODEL_ID)
 {
   QString Camera_Name;

   db_ = getDb("mydb");

   if(!db_.isOpen())
   {
       if(!db_.open())
       {
           print_info("Get_Camera_Name open database is error");
           return Camera_Name;
       }
       else
       {
           print_info(" Get_Camera_Name open database is success");
       }
   }



   QString Command=QString("select REF_DEVICE_ID from ws_device_ref where DEVICE_ID='%1' and SAFEMODEL_ID='%2' ").arg(DEVICE_ID).arg(SAFEMODEL_ID);
   QSqlQuery query(Command,db_);
   while(query.next())
   {
     Camera_Name=query.value(0).toString();
   }
   return Camera_Name;
 }
