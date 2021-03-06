#ifndef READDATAFROMMYSQL_H
#define READDATAFROMMYSQL_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QStringList>
#include <QDateTime>
#include "Systemfile/myfile.h"
#include <QtNetwork/QNetworkConfigurationManager>


/*****************************kfss*********************************/
typedef struct _Door_Cmd
{
    QString EVENT_ID;
    QString CMD_ID;
    QString DEVICE_ID;
    QString CMD_TYPE;
    int CMD_CONTENT;
    QString SEND_TIME;
    int Cmd;
}Door_Cmd;

typedef struct _Alarm_Table
{
    QString Control_Name;   //作业名称
    QString Controler;      //责任人
    QString Status;         //作业状态
    QString Safe_Status;    //安全状态
    //QString Work_ID;        //工作ID
}Work_Table;

typedef struct _AlarmTableRt
{

}AlarmTableRt;
typedef struct _Alarm_Table2
{
    QString Alarm_Event;    //告警事件
    QString Alarm_Lever;    //告警级别
    QString Controler;      //责任人
    QString Control_Desc;   //作业内容
    QString Alarm_Source;   //告警源
    QString Time;           //发生时间
    QString Video;          //调取视频
    QString Shot;           //调取快照
    QString Enter;          //确定
    QString Alarm_ID;
    QString Safe_Mode_ID;   //安全模型ID
    QString Device_ID;      //设备ID
}Alarm_Table;

//告警记录数据查询字段表
typedef struct  _Alarm_His
{
    QString safety_zone;    //安全区域
    QString workmanship;    //作业名称
    QString devname;        //设备名称
    QString Alarm_type;   //告警类型
    QString Alarm_info;   //告警信息
    QString Alarm_time;   //告警时间
    QString eventID;      //事件ID
    QString persion_name;  //人员名称

}Alarm_His;



typedef struct _WorkTotalPic
{
    QString  alarmtime;  //告警时间
    QString  alarmtypetime;  //告警类型名
    QString  alarmcount;     //选区个数
}WorkTotalPic;

//两票执行监控数据类型
typedef struct _TwoTicket_workstation
{
    QString workname;//作业名称
    QString workplace; //作业地点
    QString worktype;  //作业类型
    QString planbegintime; //作业开始时间
    QString planendtime;    //作业结束时间
    QString workstation;    //作业状态
    QString alarmNums;      //告警数
    QString employeename;   //负责人
    QString dutyperion;     //责任人
    QString arrive_persion;                //人员是否到齐
    QString tobedue;        //应到
    QString realityPersion;  //实到
}TwoTicketWorkstation;

/*
 * 曲线图数据
 * */
typedef struct _AniLayout_Chart
{
    int Alarm_Num; //警报数
    QDateTime Data_Time;//日期
}AniLayoutChart;

//区域人员查询
typedef struct _AreaPerson
{
    QString areaname;  //人员姓名
    QString  workname;    //工作名
    QString  employeename; //顾员名
    QString  deptname;     //所在部门
    QString  starttime;   //开始时间
}Area_person;

//安全趋势分析表，堆叠区域图
typedef struct _AreaLayout_Chart
{
    int Alarm_Num; //警报数
    QString Alarm_Type; //警报类型
    QDateTime Data_Time;//日期
}AreaLayoutChart;

//重点区域人员查询
typedef struct _Important_Area
{
    QString Area_Name;     //区域名称
    QString SafeDeviceNum;    //安全设备数
    QString MakeDeviceNum;       //生产设备数
    QString WorkNum;     //作业数
    QString AlarmNum;     //告警数
    QString PersonNum;     //作业人数
}Import_Area;

typedef struct _Alarm_Info
{
    QString AlarmTime; //警告时间
    QString AlarmContent; //警告内容
}AllAlarm_Info;

//工作票
typedef struct _Work_Sheet
{
    QString Work_Name;  //作业标题
    QString Work_Addr;  //作业地点
    QString Plan_BeginTime; //计划开始时间
    QString Plan_EndTime;  //计划结束时间
    QString Act_BeginTime; //实际开始时间
    QString Act_EndTime;    //实际结束时间
    QString Work_Desc;  //内容
    QString DutyPerson_Name; //责任人
    QString DutyPerson_IsSign; //责任人是否签到
    QString OperPerson_Name; //操作人名字
    QString OperPerson_IsSign; //操作人是否签到
    QList<AllAlarm_Info> AllAlarmInfoList; //所有告警信息
}WorkSheet_Info;
//操作票
typedef struct _Work_Step
{
    QString Step_Order; //操作序号
    QString Step_Desc; //操作项
    QString Is_Op; //是否完成
    QString Op_Time; //完成时间
}OperSheet_WorkStep;
//摄像头分类数据
typedef struct _Video
{
    QString Area_ID;      //区域ID
    QString Video_Name;   //摄像头名
    QString Area_Name;    //区域名
}Video;

//视频信息
typedef struct _Video_Info
{
    QString video_ip;
    QString video_port;
    QString user_name;
    QString pwd;
    QString Channel_No;
}Video_Info;

//告警信息数据
typedef struct _Alarm_data
{
    QString alarmID;
    QString eventID;
    QString status;
    QString alarm_event;       //告警事件
    QString areaID;       //区域ID;
} Alarm_data;

//告警记录条件查询
typedef struct _AlarmDemand
{
    QString safetyArea;    //安全区域
    QString dev_type;       //设备类型
    QString dev_name;     //设备名称
    QString work_name;    //作业名称
    QString alarm_type;   //告警类型
    QString employee_name;  //人员名
}AlarmDemand;

typedef struct _Cameradata
{
    QString num;
    QString area_name;
    QString work_name;
    QString device_name;
    QString alarm_type_name;
    QString alarm_content;
    QString alarm_time;
    QString employee_name;
    QString alarm_id;
}Cameradata;

typedef struct _tickettype
{
    QString job_count; //作业数
    QString doing_work; //正在作业数
    QString alarmticket_num;  //告警票数
}tickettype;


//作业步骤监控
typedef struct _Work_Monitor
{
    QString TicketType; //票种类
    QString Work_Name; //作业名
    QString Work_Id; //作业ID
    int Work_Type; //作业类型
    int isExecute[7]; //步骤是否执行
    int isHaveAlarm; //是否有扫码错误的警告
}WorkMonitor;


//首页警告信息表
typedef struct _Alarm_First_Page
{
    QString Employy_Name; //人员
    QString Area_Name; //作业区域
    QString Alarm_Content; //告警事件
    QString Alarm_Level; //告警级别
    QString Alarm_ID; //告警ID
    QString Alarm_Time; //告警时间

}AlarmFirstPage;

//首页作业表显示内容
typedef struct _operation
{
    QString work_name;
    QString work_time;
}opertion_station;
//海清：
#define PLAT_STATUS_ACOPEN      4   //门禁开门
#define PLAT_STATUS_ACCLOSE     5   //门禁关门
#define PLAT_STATUS_RELEASE     6   //常开
#define PLAT_STATUS_LOCK        7   //常闭、锁

#define SQL_SELSEC \
    "select ws_work.WORK_NAME,ws_work.WORK_STATUS, ws_work.WORK_TYPE,ws_work_employee.EMPLOYEE_ID from ws_work,ws_work_employee where ws_work.WORK_ID = ws_work_employee.WORK_ID"

#define SQL_DOOR_STATUS   "select Door_Status from WS_DOOR_STATUS"
#define SQL_ALARM_UPDATA "SELECT ALARM_TYPE_NAME,ALARM_ID,EVENT_ID,SAFEMODEL_ID,WORK_ID,DEVICE_ID,ALARM_TYPE,ALARM_CONTENT,ALARM_TIME,EMPLOYEE_ID,UPDATE_TIME,CARD_ID,ALARM_LEVEL,DEVICE_NAME,WORK_NAME,EMPLOYEE_NAME,AREA_ID,AREA_NAME,READ_STATUS FROM V_WS_ALARM_RT_WK"
#define SQL_LIST_UPDATA "SELECT AREA_NAME FROM ws_area"
#define SQL_ALARM_UPDATA_1 "select *  FROM v_ws_work"
#define SQL_ALARM_HIS    "select AREA_NAME,WORK_NAME,DEVICE_NAME,ALARM_TYPE_NAME,ALARM_CONTENT,ALARM_TIME,EMPLOYEE_NAME FROM v_ws_alarm_his order by alarm_time desc "
#define SQL_ALARM_TWOTICKET "select a.area_name,a.work_name,a.work_type,a.plan_begintime,a.plan_endtime,a.work_status,a.nums,a.employee_name,a.IS_SIGN,a.place,a.ying_dao,a.shi_dao FROM v_ws_work_statistics a"    //两票执行
#define SQL_AREAEMPLOYEE "SELECT w.area_name,w.WORK_NAME,te.EMPLOYEE_NAME,te.dept_name,w.plan_begin_time  FROM ws_work_employee E INNER JOIN v_ws_area_work W ON  W.WORK_NAME LIKE '%6kV%' AND W.WORK_ID=E.WORK_ID and w.work_status in(1,2,3,5,6,7) LEFT JOIN V_WS_EMPLOYEE te on te.EMPLOYEE_ID=e.EMPLOYEE_ID order by w.plan_begin_time desc"
#define SQL_IMPORTAREA "select * FROM v_ws_area_statistics"
#define SQL_ALARM_TOTAL_PIC "select * from v_ws_alarm_total_pic"
#define SQL_WORKTICKETTYPE  "SELECT WORK_TYPE from v_ws_work_statistics"
#define SQL_Get_AREA_ID  "select AREA_ID from ws_area"
#define SQL_SELECTBIGHT "SELECT `c`.`ALARM_TIME` AS `ALARM_TIME`,SUM(`c`.`ALARM_COUNT`) AS `ALARM_COUNT` from(select `a`.`ALARM_TIME` AS `ALARM_TIME`,`a`.`ALARM_COUNT` AS `ALARM_COUNT` from `v_ws_alarm_his_time` `a` union select `b`.`ALARM_TIME` AS `ALARM_TIME`,`b`.`ALARM_COUNT` AS `ALARM_COUNT`from `v_ws_alarm_rt_time` `b`) c GROUP BY ALARM_TIME"
#define SQL_SELECTALARMDATA " select alarm_id ,event_id, alarm_content,area_name from  v_ws_alarm_rt_wk where read_status<100"
// //#define SQL_ALARM_AREA  "select count(AREA_ID) from V_WS_ALARM_RT_WK where AREA_ID = '0'"
#define SQL_ALARM_AREA "SELECT COUNT(ALARM_CONTENT) FROM v_ws_alarm_rt_wk"
#define SQL_CAMERAINFO  "select AREA_NAME,WORK_NAME,DEVICE_NAME,ALARM_TYPE_NAME,ALARM_CONTENT,ALARM_TIME,EMPLOYEE_NAME,ALARM_ID from V_WS_ALARM_RT_WK"
#define SQL_HOMEPAGECURVE "SELECT ALARM_TIME,ALARM_TYPE_NAME,SUM(ALARM_COUNT) AS ALARM_COUNT  FROM v_ws_alarm_total_pic GROUP BY ALARM_TIME,ALARM_TYPE_NAME"
#define SQL "select * from ws_alarm_rt"
#define SQL_HOMEPAGEOPERTION "select * from ws_work t where ( t.work_type = 0 and t.work_status in (1,2)) or ( t.work_type = 1 and t.work_status not in (4)) and DATE_SUB(CURDATE(), INTERVAL 7 DAY) <= date(t.plan_begintime) ORDER BY PLAN_BEGINTIME DESC"
/*****************************************************************/



class ReadDataFromMysql
{

public:

    ~ReadDataFromMysql();

    /************************************************************************
      Description：返回本日告警总数
      Input：
      Output:
      Return:
      Others:
    ************************************************************************/
    QString TodayAlarmTotalNum();

    /************************************************************************
      Description：返回当前告警数
      Input：
      Output:
      Return:
      Others:
    ************************************************************************/
    QString CurAlarmNum();

    /************************************************************************
      Description：返回当前工作数
      Input：
      Output:
      Return:
      Others:
    ************************************************************************/
    QString CurExecWorkNum();


    ///////////////////////////
    QString HasSignPersonNum();

    QString NoSignPersonNum();

    QString OperatingWorkNum();

    QString NoStartWorkNum();


    /************************************************************************
      Description：返回系统运行状态
      Input：
      Output:
      Return:
      Others:
    ************************************************************************/
    QString SysRunStatus();

    /************************************************************************
      Description：返回今日告警信息列表
      Input：
      Output:
      Return:
      Others:
    ************************************************************************/
    QStringList TodayAlarmInfo();


    /************************************************************************
      Description：返回主页告警信息列表
      Input：
      Output:
      Return:
      Others:
    ************************************************************************/
    QStringList HomePageAlarmInfo();

    QStringList CameraAlarmInfo();

    /************************************************************************
      Description：是否产生告警，并发送告警信息
      Input：
      Output:
      Return:
      Others:
    ************************************************************************/
    bool IsAreaAlarm();


    /************************************************************************
      Description：获取区域ID号
      Input：
      Output:
      Return:
      Others:
    ************************************************************************/
    QList<QString> Updata_Area_List();

    /************************************************************************
      Description：获取区域ID号
      Input：
      Output:
      Return:
      Others:
    ************************************************************************/
    QString Get_Area_ID(QString AREA_NAME);

    /************************************************************************
      Description：获取区域ID号
      Input：
      Output:
      Return:
      Others:
    ************************************************************************/
    QStringList Get_Area_ID();

    /************************************************************************
      Description：更新告警数据，以区域ID为条件查询
      Input：
      Output:
      Return:
      Others:
    ************************************************************************/
    QList<Alarm_Table> Updata_tableWidget_Alarm(QString Area_Id);  //

    /************************************************************************
      Description：更新工作表内容，以区域名为条件查询
      Input：
      Output:
      Return:
      Others:
    ************************************************************************/
    QList<Work_Table> Updata_tableWidget_Work(QString Area_Name);

    /************************************************************************
      Description：查询告警记录内容并得到结果
      Input：
      Output:
      Return:
      Others:
    ************************************************************************/
    QList<Alarm_His> SelectTableToAlarmTableWidget();

    /************************************************************************
      Description：重点区域监控
      Input：
      Output:
      Return:
      Others:
    ************************************************************************/
    QList<Import_Area> Updata_tableWidget_ImportArea();

    /************************************************************************
      Description：区域人员查询
      Input：
      Output:
      Return:
      Others:
    ************************************************************************/
    bool Updata_tableWidget_AreaEmployee(QList<Area_person>&);

    /************************************************************************
      Description：工作票查询
      Input：
      Output:
      Return:
      Others:
    ************************************************************************/
    QString WorkTicketType();

    /************************************************************************
      Description：
      Input：
      Output:
      Return:
      Others:
    ************************************************************************/
    QList<_WorkTotalPic> SelectWorkTotalPic();

    /************************************************************************
      Description：
      Input：
      Output:
      Return:
      Others:
    ************************************************************************/
    bool Control_Door(Door_Cmd Cmd_Door);


    unsigned int Connect_Flag;

    /************************************************************************
      Description：
      Input：
      Output:
      Return:
      Others:
    ************************************************************************/
    bool Alarm_Lamp_Do_State(QString EVENT_ID);

    /************************************************************************
      Description：返回门的状态
      Input：
      Output:
      Return:
      Others:
    ************************************************************************/
    int Door_State();


    /************************************************************************
      Description：
      Input：
      Output:
      Return:
      Others:
    ************************************************************************/
    int Stored_Procedure(QString CMD);


    /************************************************************************
      Description：两票执行监控数据库查询实现
      Input：
      Output:
      Return:
      Others:
    ************************************************************************/
    int QueryWorkStatisticsTable(QList<TwoTicketWorkstation>& lst);


    /************************************************************************
      Description：视频界面通过告警ID查询要删除的数据
      Input：
      Output:
      Return:
      Others:
    ************************************************************************/
    void delete_Alarm_Info(QString alarm_ID);

    /************************************************************************
      Description：工作表信息查询
      Input：
      Output:
      Return:
      Others:
    ************************************************************************/
    WorkSheet_Info Update_tableWidget_WorkSheet(QString workId);


    /************************************************************************
      Description：操作表第二个表
      Input：
      Output:
      Return:
      Others:
    ************************************************************************/
    QList<OperSheet_WorkStep> Updata_tableWidget_OperSheet();

    /************************************************************************
      Description：区域ID返回，区域信息
      Input：
      Output:
      Return:
      Others:
    ************************************************************************/
    QList<Video>  Get_Area_Name(QString Area_Id);

    /************************************************************************
      Description：返回摄像头告警信息
      Input：
      Output:
      Return:
      Others:
    ************************************************************************/

    QList<Cameradata> CameraAlarmInfo(QString Area_Name);

    /************************************************************************
      Description：从区域名里面得到摄像头的名字
      Input：
      Output:
      Return:
      Others:
    ************************************************************************/

    QString Get_Area_Name_From_Camera_Name(QString Camera_Name);


    /************************************************************************
      Description：获取视频信息
      Input：
      Output:
      Return:
      Others:
    ************************************************************************/
    Video_Info Get_Video_Info(QString Video_Name);  //获取视频信息

    /************************************************************************
  Description：首页获取视频信息
  Input：
  Output: 输入区域名
  Return: 区域摄像头的ip、端口号、
  Others:
************************************************************************/
    Video_Info Get_Video_InfoOne(QString Video_Name);
    /************************************************************************
      Description：获取摄像头的名字ID号
      Input：  @Area_Id(安全区域ID)
      Output:
      Return:
      Others:
    ************************************************************************/
    QString Get_Cameralname(QString Area_Id);



    /************************************************************************
      Description：获取摄像头的名称
      Input：  @DEVICE_ID(摄像ID),@SAFEMODEL_ID(安全区域ID)
      Output:
      Return:
      Others:
    ************************************************************************/
    QString Get_Camera_Name(QString DEVICE_ID,QString SAFEMODEL_ID);


    /************************************************************************
      Description：获取到告警记录，并取到报警内容通过语音读出来
      Input： @lst(告警列表)
      Output:
      Return:
      Others:
    ************************************************************************/
    int SelectAlarm_curvenData(QList<Alarm_data>& lst);


    /************************************************************************
      Description：
      Input：
      Output:
      Return:
      Others:
    ************************************************************************/
    bool Updatastatus_alarmtable(QString);


    /************************************************************************
      Description：安全趋势分析图
      Input：
      Output:
      Return:
      Others:
    ************************************************************************/
    QList<AniLayoutChart> Updata_AniLayoutChart();



    /************************************************************************
      Description：报警堆叠图
      Input：
      Output:
      Return:
      Others:
    ************************************************************************/
    void Updata_AreaLayoutChart(QList<AreaLayoutChart>&);


    /************************************************************************
      Description：打开默认数据库连接
      Input：
      Output:
      Return: true(成功);false(失败)
      Others:
    ************************************************************************/
    bool openDb();


    /************************************************************************
      Description：获取指定名称的数据库操作对象
      Input：@name(数据库连接名称)
      Output:
      Return:
      Others:
    ************************************************************************/
    QSqlDatabase& getDb(QString name);


    /************************************************************************
      Description：断开数据库连接
      Input：
      Output:
      Return: true(成功);false(失败)
      Others:
    ************************************************************************/
    void closeDb();


    /************************************************************************
      Description：断开数据库连接
      Input：
      Output:
      Return: true(成功);false(失败)
      Others:
    ************************************************************************/
    static ReadDataFromMysql* instance();

    /************************************************************************
      Description：移除指定的告警ID
      Input： @id(告警id)
      Output:
      Return:
      Others:
    ************************************************************************/
    void deleteAlarmInfo(QString id);

    /************************************************************************
  Description：告警记录按条件查询
  Input：
  Output:
  Return:
  Others:
************************************************************************/
    QList<Alarm_His> QueryByCriteriaToDatabase(QString,QString,QString,QString,QString,QString,QString);

    /************************************************************************
  Description：告警记录按条件显示
  Input：
  Output:
  Return:
  Others:
************************************************************************/

 QStringList DemandConditionData(QString);



 /************************************************************************
  Description：查询一张票的总的作业数、正在作业数、告警票数
  Input：
  Output:  作业数、正在作业数、告警票数
  Return:
  Others:
************************************************************************/
 bool InquireTwoTicketSum(tickettype&);

 /************************************************************************
  Description：查找人员名显示到listwidget里面
  @param：
  Output:
  @Return:
  @author:
************************************************************************/
 QList<Cameradata> Hk_VideoPerson();

 QList<WorkMonitor> TwoTicketExecMonitor(QString ticketType); //两票执行监控步骤查询,传入票类型
 QList<AlarmFirstPage> UpdateAlarm_FirstPage(); //更新首页警告数据
 tickettype getWorkTicketInfoNum(); //两票监控工作票数据获得

 //首页作业表显示操作
 void homepage_operationticket(QList<opertion_station>&);
private:
    static ReadDataFromMysql* instance_;
    QSqlDatabase db_;
    ReadDataFromMysql();

};

#endif // READDATAFROMMYSQL_H
