#ifndef WUHANSUBWAY_H
#define WUHANSUBWAY_H

#pragma once
#include <QWidget>
#include <QTime>
#include <stdio.h>
#include <iostream>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <fstream>
#include <vector>
#include <string>
#include <iterator>
#include <QDesktopServices>
#include <QMessageBox>
#include "define.h"
#include "route.h"
#include "mycaltime.h"

using namespace std;

extern string path;

QT_BEGIN_NAMESPACE
namespace Ui { class WuhanSubway; }
QT_END_NAMESPACE

class WuhanSubway : public QWidget
{
    Q_OBJECT

public:
    WuhanSubway(QWidget *parent = nullptr);
    ~WuhanSubway();

private slots:
    void on_Time_OK_clicked();

    void on_Time_hh_activated(int index);

    void on_Time_mm_activated(int index);

    void on_Time_ss_activated(int index);

    void on_Sline_activated(int index);

    void on_emptyop_clicked();

    void on_CloseWindow_clicked();

    void on_Sstation_activated(int index);

    void on_SetAsStartP_clicked();

    void on_Dline_activated(int index);

    void on_Dstation_activated(int index);

    void on_SetAsDestP_clicked();

    void on_ShowMap_clicked();

    void on_UseSystemTimeBtn_clicked();

    void on_ResetAll_clicked();

    status ShowStartStation(TimeHMS now, int Sline, int SID);

    void on_MDepthSlider_actionTriggered(int action);

    void on_MDepthBox_valueChanged(int arg1);

    status ShowDestStation(int Dline, int DID);

    void on_AboutQT_clicked();

    void on_MDepthSlider_valueChanged(int value);

    void on_R1_clicked();

    void on_R2_clicked();

    void on_R3_clicked();

    void on_PlanRoute_clicked();

    void on_CButn_clicked();

    void on_Outp_ret_clicked();

    void on_SetPathBtn_clicked();

    void on_DDButn_clicked();

    void on_Website_butn_clicked();

    void on_PrevStation_clicked();

    void on_NextStation_clicked();

    void on_TransferBtn_clicked();

    void on_DistBox_valueChanged(double arg1);

    void on_isManualBox_stateChanged(int arg1);

    void on_R6_stateChanged(int arg1);

    void on_R4_clicked();

    void on_R5_clicked();

    void on_TrafficYesBtn_clicked();

    void on_Traffic_Line_Set_activated(int index);

    void on_PrtTrafBtn_clicked();

private:
    Ui::WuhanSubway *ui;
};

#endif // WUHANSUBWAY_H
