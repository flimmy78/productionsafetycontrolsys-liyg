#ifndef PRODUCTIONSAFETYCONTROLSYS_H
#define PRODUCTIONSAFETYCONTROLSYS_H

#include <QWidget>
#include <QStringList>
#include <QApplication>
#include <QDesktopWidget>
#include <QEvent>
#include <QBitmap>
#include <QHeaderView>
#include <QTableView>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QValueAxis>
#include <QVBoxLayout>
#include <QDebug>
#include <QRect>
#include "chartview/chartview.h"
#include <QTableWidget>
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QDesktopServices>
#include <QAxWidget>
#include <QProcess>
#include <QMenu>
#include <QAction>
#include "twoticketexemonitor/twoticketexemonitor.h"
#include "safetrendanalysis/safetrendanalysis.h"
#include "importantarea/importantarea.h"
#include "textspeech/textspeech.h"
#include "area_employee/areaemployee.h"
#include <QThread>
#include <QChartView>
#include <QLineSeries>
#include <QAreaSeries>
#include <QChart>
#include "hk_video/hk_video.h"
#include "hk_video/video_control.h"
#include <QProcess>
#include "Alarm_record/alarm_record.h"
#include "camerawidget/camerawidget.h"
#include "../core/msg_def.h"
#include "../core/ris_message.h"
#include "thread/db_thread.h"
#include "readdatafrommysql/readdatafrommysql.h"
#include "RegionName/regionname.h"
#include <QDateTime>
#include "AlarmWidget/alarmwidget.h"
#include "include/HCNetSDK.h"
#include "include/plaympeg4.h"
#include "TicketStepMonitor/ticketstepmonitor.h"
//#include "MyCurvenThread/mycurve.h"
#include "workthread/workthreader.h"

typedef QList<Import_Area> ImportAreaListType;

namespace Ui {
class ProductionSafetyControlSys;
}

class ProductionSafetyControlSys : public QWidget
{
    Q_OBJECT
    QThread workThread;
public:
    WorkThreader *worker;

    explicit ProductionSafetyControlSys(QWidget *parent = 0);
    ~ProductionSafetyControlSys();
    void WriDataToTable(QList<AlarmFirstPage>);
    //void TableWidgetInit();
    void updateHomepageInfo();
    void InitWidget();
    void Init_Page_3();
    void repaintTable();
    QProcess Target_Process;    //kfss add
    int Run_Flag;

    //kfss 告警界面
    QTimer *Timer;

    Work_Table Alarm_Table_1;
    Alarm_Table Alarm_Table2_1;
    QString Command_Rule;
    unsigned int Door_Status;    //门状态 数据库上下来的
    unsigned int Door_Status_Real;
    unsigned int Alarm_Lamp_Status; //告警灯状态
    int Area_Count;        //当前区域数量
    int Current_Row;       //当前选中的行
    int Back_Up_Row;
    QList<QString> BackUp_List;
    void Updata_List();
    QList<Alarm_Table> Alarm_Table_Data;
    void Table_Init();
    QList<QString> BackUp_Area_List;
//    void Updata_Alarm_tableWidget(QList<Alarm_Table> Alarm_Data);
//    void Updata_Work_tableWidget(QList<Work_Table> Alarm_Data);
    QString Area_Name;
  //  void CameraWidgetRealize();
 //   void CameraTableDisp(QStringList strList);
    void Updata_Work_Alarm_Table();
    void Updata_Door_State();
    void Showtruce();

	void InitPopuMenu();

    QString Target_Name;
    QList<QPushButton*> Button_List;
//    void Area_Pic(QString AreaID);
    QString Current_Area_Id;


signals:
    void postAlarmID(QString);
    void postImportArea();

private slots:
    void reciverImportAreaList(WorkThreader *work);

    void Video_Button_clicked();
    void timerUpdate();
//    void ReceiveDeleteInfo();
   void tableWidgetClick(QTableWidgetItem*);  //点击单元格
   void AlarmAnalysisActionClicked();
   void TwoTicketMoniorClicked();
   void on_homepageBtnB_clicked();
   void AlarmSelectActionClicked();
   void PersionQueryActionClicked();
   void AreaMonitoringActionClicked();
//   void on_deleteAlarmInfo(QString);
   void tool_tip(bool status, int index);     //显示表格上的鼠标悬浮提示
//   void on_pushButton_2_clicked();

   void on_pushButton_clicked();

   void Signal_DevWrongClicked(QString);
   void Signal_LeaveOutClicked(QString);
   void Signal_PersonForbidClicked(QString);
   void Signal_PersonHelpClicked(QString);
   void Signal_CommissionClicked(QString);
   void Signal_DoorwrongonClicked(QString);
   void Signal_ticketMtepMonitorClicked(QString);
   void Signal_OperationWidgetClicked(QString);
   void Signal_NoCardclicked(QString);
   void Signal_NoTicketClicked(QString str);

   void on_HomepageTable_cellClicked(int row, int column);
   void TwoTicketStepPageReturn(); //两票步骤界面返回按键
   void on_dataViewBtn_clicked();
   void on_SafetyMonitoringpushButton_clicked();  //安全监控按钮响应事件
   void on_AlarmAnalyzepushButton_clicked();     //警告查询按钮响应事件
   void on_HomePagepushButton_clicked();      //全厂总揽按钮事件响应
   void on_tabWidget_tabBarClicked(int index);
   void HomePageTimeUpdate();       //首页时间更新
signals:
   void postDeleteInfo();
//   void View_Id(QString Area_Name);
public slots:   //kfss add
    void ViewShow(QString);
    void AcceptSignalGoingInterval(QString);   //接收走错间隔信号
private:
    Ui::ProductionSafetyControlSys *ui;
    //   ReadDataFromMysql database;
    bool eventFilter(QObject *, QEvent *);
    QPushButton* affirmButton[6];
    //   AlarmRecord *Alrecord;
    QCheckBox *m_checkBox;
    QTimer* m_time;
    QLabel *CameraLabel;
    QPushButton*  CameraABtn;
    QPushButton *CameraBBtn;
    QAxWidget* AxWidget;
    QMenu *safeMenu;
    QMenu *AlarmAnalysisMenu;
    QAction* AlarmSelectAction;  //告警查询
    QAction* AlarmAnalysisAction;  //安全趋势分析
    QAction* ReportQueryAction;    //报表查询
    QAction* TwoTicketExecAction;
    QAction* PersionQueryAction;
    QAction* AreaMonitoringAction;
    int j;
    TwoTicketExeMonitor *TwoTicketMoni;
    SafeTrendAnalysis* SafeTrendWidget;
    ImportantArea* Importarea;
    AreaEmployee* Areaemploy;
    //   DataTable m_dataTable;
    QChartView *chartView;
    QVBoxLayout *layout;
    QLineSeries *series;
    QChart* chart;
    HK_Video *Hk_view;
    TextSpeech textspeech;
    Alarm_record *Alarmrecord;
    CameraWidget *Camerawidget;
    QList<AreaLayoutChart> Arealist;
    RegionName *RegionWidget;
    AlarmWidget *Alarmwidget;
    long Video_ID;
    long lUserID;
    QList<Alarm_His> Hislist;
    TicketStepMonitor* ticketMtepMonitor;
    ImportAreaListType datalist;
    bool click;
    QTimer *HomepageTimer;
//    MyCurve *Mycurve;
//    QWebEngineView *WebEngineView;
};

#endif // PRODUCTIONSAFETYCONTROLSYS_H
