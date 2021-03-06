#include "workthreader.h"

WorkThreader::WorkThreader(QObject *parent) : QObject(parent)
{
    data_base = QSqlDatabase::addDatabase(Config.DbType);
    data_base.setHostName(Config.HostName);  //设置主机地址
    data_base.setPort(Config.HostPort);             //设置端口
    data_base.setDatabaseName(Config.Databasename);   //设置数据库名称
    data_base.setUserName(Config.UserName);       //设置用户名
    data_base.setPassword(Config.UserPassword);        //设置密码
    if(data_base.open())
        qDebug()<<"success to connect to mysql";
    else
        qDebug()<<"failed to connect to mysql";
}

void WorkThreader::UpdataTablewidgetImportarea()
{
    importAreaList.clear();
    QSqlQuery query("select * from v_ws_area_statistics",data_base);
    while(query.next())
    {
        Import_Area ImportArea_table;
        ImportArea_table.Area_Name = query.value(0).toString();     //区域名称
        ImportArea_table.SafeDeviceNum = query.value(1).toString(); //安全设备名称
        ImportArea_table.MakeDeviceNum = query.value(2).toString(); //生产设备名称
        ImportArea_table.WorkNum = query.value(3).toString();       //作业数
        ImportArea_table.AlarmNum = query.value(4).toString();      //告警数
        ImportArea_table.PersonNum = query.value(5).toString();     //作业人数
        qDebug()<<ImportArea_table.Area_Name<< ImportArea_table.SafeDeviceNum;
        importAreaList<<ImportArea_table;
    }
    emit postImportAreaList(this);
}
