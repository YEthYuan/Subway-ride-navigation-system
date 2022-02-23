#include "wuhansubway.h"
#include "ui_wuhansubway.h"

string path;
int _h, _m, _s;
TimeHMS tt;
int lineid_s, stid_s;
int lindid_d, stid_d;
int yline, yst;
status Status;
double dist = 0.0;
int depth_m;
vector<int> transfer_list;
vector<int>::iterator trans_it;

WuhanSubway::WuhanSubway(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WuhanSubway)
{
    ui->setupUi(this);
    path="gbk.in";
    if(INFEASTABLE== input(path.c_str()))
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("���ش���"), QString::fromLocal8Bit("δ�ҵ������ļ�,�������Ŀ¼���Ƿ����gbk.in,�Լ����Ƿ���."));
    }
    BuildMap();
    ui->MDepthBox->setMinimum(3);
    ui->MDepthBox->setMaximum(25);
    ui->MDepthBox->setSingleStep(1);
    ui->MDepthSlider->setOrientation(Qt::Horizontal);
    ui->MDepthSlider->setMinimum(3);
    ui->MDepthSlider->setMaximum(25);
    ui->MDepthSlider->setSingleStep(1);
    ui->MDepthSlider->setTickPosition(QSlider::TicksBothSides);
    connect(ui->MDepthBox, SIGNAL(valueChanged(int)), ui->MDepthSlider, SLOT(setValue(int)));
    connect(ui->MDepthSlider, SIGNAL(valueChanged(int)), ui->MDepthBox, SLOT(setValue(int)));
    connect(ui->MDepthSlider, SIGNAL(valueChanged(int)), ui->MdepthLCD, SLOT(setValue(int)));
    ui->MDepthSlider->setValue(12);
    depth_m=12;
    ui->op->append(QString::fromLocal8Bit("�����Ѿ�����......"));
    ui->Time_mm->setDisabled(true);
    ui->Time_ss->setDisabled(true);
    ui->Time_OK->setDisabled(true);
    ui->Sline->setDisabled(true);
    ui->Sstation->setDisabled(true);
    ui->Dline->setDisabled(true);
    ui->Dstation->setDisabled(true);
    ui->SetAsStartP->setDisabled(true);
    ui->SetAsDestP->setDisabled(true);
    ui->MDepthBox->setDisabled(true);
    ui->MDepthSlider->setDisabled(true);
    ui->R1->setDisabled(true);
    ui->R2->setDisabled(true);
    ui->R3->setDisabled(true);
    ui->R4->setDisabled(true);
    ui->R5->setDisabled(true);
    ui->R6->setDisabled(true);
    ui->R6_Box->setDisabled(true);
    ui->isManualBox->setDisabled(true);
    ui->TrafficBox->setDisabled(true);
    ui->Traffic_Line_Set->setDisabled(true);
    ui->PlanRoute->setDisabled(true);
    ui->Outp_ret->setDisabled(true);
    ui->NextStation->setDisabled(true);
    ui->PrevStation->setDisabled(true);
    ui->TransferBtn->setDisabled(true);
    ui->TrafficYesBtn->setDisabled(true);
    ui->PrtTrafBtn->setDisabled(true);
    ui->MdepthLCD->display(12);
}

WuhanSubway::~WuhanSubway()
{
    delete ui;
}

void WuhanSubway::on_Time_OK_clicked()
{
    _h=ui->Time_hh->currentIndex();
    _m=ui->Time_mm->currentIndex();
    _s=ui->Time_ss->currentIndex();
    tt.h=_h;
    tt.m=_m;
    tt.s=_s;
    QString display_now_time=QString::fromLocal8Bit("��ǰʱ��:")+QString("%1").arg(_h, 2, 10, QChar('0'))+QString::fromLocal8Bit("ʱ")+QString("%1").arg(_m, 2, 10, QChar('0'))+QString::fromLocal8Bit("��")+QString("%1").arg(_s, 2, 10, QChar('0'))+QString::fromLocal8Bit("��.");
    ui->op->append(display_now_time);
    ui->Time_OK->setDisabled(true);
    ui->Time_hh->setDisabled(true);
    ui->Time_mm->setDisabled(true);
    ui->Time_ss->setDisabled(true);
    ui->Sline->setDisabled(false);
}

void WuhanSubway::on_Time_hh_activated(int index)
{
    ui->Time_mm->setDisabled(false);
}

void WuhanSubway::on_Time_mm_activated(int index)
{
    ui->Time_ss->setDisabled(false);
}

void WuhanSubway::on_Time_ss_activated(int index)
{
    ui->Time_OK->setDisabled(false);
}

void WuhanSubway::on_Sline_activated(int index)
{
    if(index!=0&&index!=5)
    {
        ui->Sstation->setDisabled(false);
        ui->Sstation->clear();
        QString starters=QString::fromLocal8Bit("��ѡ��")+QString::number(index)+QString::fromLocal8Bit("����վ��");
        ui->Sstation->addItem(starters);
        ui->op->append("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        QString outp_op=QString::fromLocal8Bit("�人����")+QString::number(index)+QString::fromLocal8Bit("����");
        ui->op->append(outp_op);
        outp_op.clear();
        outp_op=QString::fromLocal8Bit("��վ����:")+QString::number(line[index].St_Num)+QString::fromLocal8Bit("��;     ��·ȫ��:")+QString::number(line[index].length)+QString::fromLocal8Bit("km;     ����ؿ���:")+QString::number(line[index].fullNum)+QString::fromLocal8Bit("�ˡ�");
        ui->op->append(outp_op);
        outp_op.clear();

        for(vector<Station>::iterator itSt=line[index].st_list.begin();itSt!=line[index].st_list.end();itSt++)
        {
            ui->Sstation->addItem(QString::fromLocal8Bit(itSt->name.data()));
            QString outptext=QString::number(itSt->id.line)+QString("%1").arg(itSt->id.station_number, 2, 10, QChar('0'))+QChar(' ')+QString::fromLocal8Bit(itSt->name.data());
            if(itSt->transfer==true)
            {
                outptext+=QString::fromLocal8Bit("  �ɻ���");
                for(vector<int>::iterator itInt=itSt->TransTo.begin();itInt!=itSt->TransTo.end();itInt++)
                {
                    outptext+=QString::number(*itInt);
                    outptext+=QString::fromLocal8Bit("����,");
                }
            }
            ui->op->append(outptext);
        }
        ui->op->append("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    }
    else
    {
        QString outp_err_line_message=QString::number(index)+QString::fromLocal8Bit("������δ��ͨ");
        ui->Dstation->setCurrentText(outp_err_line_message);
        ui->op->append(outp_err_line_message);
        QMessageBox::warning(this, QString::fromLocal8Bit("����"), outp_err_line_message);
        ui->Sstation->setDisabled(true);
    }
}

void WuhanSubway::on_emptyop_clicked()
{
    ui->op->clear();
}

void WuhanSubway::on_CloseWindow_clicked()
{
    if(QMessageBox::Yes==QMessageBox::question(this, "Exit?", QString::fromLocal8Bit("�Ƿ�Ҫ�˳�����"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No))
        QApplication::exit(0);
}

void WuhanSubway::on_Sstation_activated(int index)
{
    if(index!=0)
    {
        ui->SetAsStartP->setDisabled(false);
    }
}

void WuhanSubway::on_SetAsStartP_clicked()
{
    ui->Dline->setDisabled(false);
    ui->NextStation->setDisabled(false);
    ui->PrevStation->setDisabled(false);
    lineid_s=ui->Sline->currentIndex();
    stid_s=ui->Sstation->currentIndex();
    yline=lineid_s;
    yst=stid_s;
    ui->SID_Label->setText(QString::number(lineid_s)+QString::fromLocal8Bit("���ߵ�")+QString::number(stid_s)+QString::fromLocal8Bit("վ"));
    ui->SName_Label->setText(QString::fromLocal8Bit(line[lineid_s].st_list[stid_s-1].name.data()));
    if(line[lineid_s].st_list[stid_s-1].transfer==true)
    {
        ui->TransferBtn->setDisabled(false);
        QString btntext=QString::fromLocal8Bit("��\n��\n")+QString::number(*line[lineid_s].st_list[stid_s-1].TransTo.begin())+QString::fromLocal8Bit("\n��\n��");
        ui->TransferBtn->setText(btntext);
        transfer_list=line[yline].st_list[yst-1].TransTo;
        transfer_list.push_back(lineid_s);
        trans_it=transfer_list.begin();
    }
    else
    {
        ui->TransferBtn->setDisabled(true);
        ui->TransferBtn->setText(QString::fromLocal8Bit("��\n��\n��\n��"));
    }
    if(stid_s==0)
    {
        ui->PrevStation->setText(QString::fromLocal8Bit("��PrevStn"));
        ui->PrevStation->setDisabled(true);
        ui->NextStation->setText(QString::fromLocal8Bit("��NextStn"));
        ui->NextStation->setDisabled(true);
    }
    else if(stid_s==1)
    {
        ui->PrevStation->setDisabled(true);
        ui->PrevStation->setText(QString::fromLocal8Bit("T�յ�վ"));
        QString btntext;
        btntext=QString::fromLocal8Bit("��")+QString::fromLocal8Bit(line[lineid_s].st_list[stid_s].name.data());
        ui->NextStation->setText(btntext);
    }
    else if(stid_s==line[lineid_s].St_Num)
    {
        QString btntext=QString::fromLocal8Bit("��")+QString::fromLocal8Bit(line[lineid_s].st_list[stid_s-2].name.data());
        ui->PrevStation->setText(btntext);
        ui->NextStation->setDisabled(true);
        ui->NextStation->setText(QString::fromLocal8Bit("-�յ�վ"));
    }
    else
    {
        QString btntext=QString::fromLocal8Bit("��")+QString::fromLocal8Bit(line[lineid_s].st_list[stid_s-2].name.data());
        ui->PrevStation->setText(btntext);
        btntext.clear();
        btntext=QString::fromLocal8Bit("��")+QString::fromLocal8Bit(line[lineid_s].st_list[stid_s].name.data());
        ui->NextStation->setText(btntext);
    }
    QString startinlabel=QString("%1").arg(lineid_s, 1, 10, QChar('0'))+QString("%1").arg(stid_s, 2, 10, QChar('0'))+ui->Sstation->currentText();
    ui->StartLabel->setText(startinlabel);
    Status = ShowStartStation(tt, lineid_s, stid_s);
    if (Status == FALSE)
    {
        ui->Time_hh->setCurrentIndex(0);
        ui->Time_mm->setCurrentIndex(0);
        ui->Time_ss->setCurrentIndex(0);
        ui->Time_hh->setDisabled(false);
        ui->Time_mm->setDisabled(true);
        ui->Time_ss->setDisabled(true);
        ui->Time_OK->setDisabled(true);
        ui->UseSystemTimeBtn->setDisabled(false);
        ui->Sline->setCurrentIndex(0);
        ui->Sline->setDisabled(true);
        ui->Sstation->clear();
        ui->Sstation->setDisabled(true);
        ui->SetAsStartP->setDisabled(true);
        ui->Dline->setDisabled(true);
        ui->StartLabel->setText(QString::fromLocal8Bit("���������"));
    }
}

void WuhanSubway::on_Dline_activated(int index)
{
    if(index!=0&&index!=5)
    {
        ui->Dstation->setDisabled(false);
        ui->Dstation->clear();
        QString starters=QString::fromLocal8Bit("��ѡ��")+QString::number(index)+QString::fromLocal8Bit("����վ��");
        ui->Dstation->addItem(starters);
        ui->op->append("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        QString outp_op=QString::fromLocal8Bit("�人����")+QString::number(index)+QString::fromLocal8Bit("����");
        ui->op->append(outp_op);
        outp_op.clear();
        outp_op=QString::fromLocal8Bit("��վ����:")+QString::number(line[index].St_Num)+QString::fromLocal8Bit("��;     ��·ȫ��:")+QString::number(line[index].length)+QString::fromLocal8Bit("km;     ����ؿ���:")+QString::number(line[index].fullNum)+QString::fromLocal8Bit("�ˡ�");
        ui->op->append(outp_op);
        outp_op.clear();

        for(vector<Station>::iterator itSt=line[index].st_list.begin();itSt!=line[index].st_list.end();itSt++)
        {
            ui->Dstation->addItem(QString::fromLocal8Bit(itSt->name.data()));
            QString outptext=QString::number(itSt->id.line)+QString("%1").arg(itSt->id.station_number, 2, 10, QChar('0'))+QChar(' ')+QString::fromLocal8Bit(itSt->name.data());
            if(itSt->transfer==true)
            {
                outptext+=QString::fromLocal8Bit("  �ɻ���");
                for(vector<int>::iterator itInt=itSt->TransTo.begin();itInt!=itSt->TransTo.end();itInt++)
                {
                    outptext+=QString::number(*itInt);
                    outptext+=QString::fromLocal8Bit("����,");
                }
            }
            ui->op->append(outptext);
        }
        ui->op->append("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    }
    else
    {
        QString outp_err_line_message=QString::number(index)+QString::fromLocal8Bit("������δ��ͨ");
        ui->Dstation->setCurrentText(outp_err_line_message);
        QMessageBox::warning(this, QString::fromLocal8Bit("����"), outp_err_line_message);
        ui->Dstation->setDisabled(true);
    }
}

void WuhanSubway::on_Dstation_activated(int index)
{
    if(index!=0)
    {
        ui->SetAsDestP->setDisabled(false);
    }
}

void WuhanSubway::on_SetAsDestP_clicked()
{
    lindid_d=ui->Dline->currentIndex();
    stid_d=ui->Dstation->currentIndex();
    QString destinlabel=QString("%1").arg(lindid_d, 1, 10, QChar('0'))+QString("%1").arg(stid_d, 2, 10, QChar('0'))+ui->Dstation->currentText();
    ui->DestLabel->setText(destinlabel);
    ShowDestStation(lindid_d, stid_d);
    ui->MDepthBox->setDisabled(false);
    ui->MDepthSlider->setDisabled(false);
    ui->isManualBox->setDisabled(false);
    ui->R1->setDisabled(false);
    ui->R2->setDisabled(false);
    ui->R3->setDisabled(false);
    ui->R4->setDisabled(false);
    ui->R5->setDisabled(false);
    ui->R6->setDisabled(false);
}

void WuhanSubway::on_ShowMap_clicked()
{
    QDesktopServices::openUrl(QUrl(QLatin1String("https://www.wuhanrt.com/public_forward.aspx??url=public_content_page.aspx?newid=1cae7552-d9fa-4f15-a013-f53ae2582a27&dtag=menu_motion_1")));
}

void WuhanSubway::on_UseSystemTimeBtn_clicked()
{
    QTime nowsystemtime=QTime::currentTime();
    _h=nowsystemtime.hour();
    _m=nowsystemtime.minute();
    _s=nowsystemtime.second();
    tt.h=_h;
    tt.m=_m;
    tt.s=_s;
    ui->Time_hh->setCurrentIndex(_h);
    ui->Time_mm->setCurrentIndex(_m);
    ui->Time_ss->setCurrentIndex(_s);
    ui->Time_hh->setDisabled(false);
    ui->Time_mm->setDisabled(false);
    ui->Time_ss->setDisabled(false);
    ui->Time_OK->setDisabled(false);
}

void WuhanSubway::on_ResetAll_clicked()
{
    if(QMessageBox::Yes==QMessageBox::question(this, "Reset?", QString::fromLocal8Bit("�Ƿ����������ȫ�����ģ�"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No))
    {
        ui->MDepthSlider->setValue(12);
        depth_m=12;
        ui->op->clear();
        ui->lu->clear();
        ui->Time_hh->setCurrentIndex(0);
        ui->Time_mm->setCurrentIndex(0);
        ui->Time_ss->setCurrentIndex(0);
        ui->Sline->setCurrentText(0);
        ui->Sstation->clear();
        ui->Dline->setCurrentIndex(0);
        ui->Dstation->clear();
        ui->MDepthBox->setValue(12);
        ui->NowTimeLCD->display(0);
        ui->op->append(QString::fromLocal8Bit("�����Ѿ�����......"));
        ui->Time_mm->setDisabled(true);
        ui->Time_ss->setDisabled(true);
        ui->Time_OK->setDisabled(true);
        ui->Sline->setDisabled(true);
        ui->Sstation->setDisabled(true);
        ui->Dline->setDisabled(true);
        ui->Dstation->setDisabled(true);
        ui->SetAsStartP->setDisabled(true);
        ui->SetAsDestP->setDisabled(true);
        ui->MDepthBox->setDisabled(true);
        ui->MDepthSlider->setDisabled(true);
        ui->PlanRoute->setDisabled(true);
        ui->Outp_ret->setDisabled(true);
        ui->Time_hh->setDisabled(false);
        ui->Sline->setCurrentIndex(0);
        ui->StartLabel->setText(QString::fromLocal8Bit("���������"));
        ui->DestLabel->setText(QString::fromLocal8Bit("�������յ�"));
        ui->MDepthBox->setValue(12);
        ui->MdepthLCD->display(12);
        ui->isManualBox->setChecked(false);
        ui->isManualBox->setDisabled(true);
        ui->PrtTrafBtn->setDisabled(true);
        ui->Traffic_Line_Set->setCurrentIndex(0);
        ui->TrafficBox->setCurrentIndex(0);
        ui->TrafficYesBtn->setDisabled(true);
        ui->R6->setChecked(false);
        ui->R6_Box->setValue(0.70);
        ui->R5->setChecked(false);
        ui->R4->setChecked(false);
        ui->R3->setChecked(false);
        ui->R2->setChecked(false);
        ui->R1->setChecked(false);
        ui->R1->setDisabled(true);
        ui->R2->setDisabled(true);
        ui->R3->setDisabled(true);
        ui->R4->setDisabled(true);
        ui->R5->setDisabled(true);
        ui->R6->setDisabled(true);
        ui->DistBox->setValue(0.0);
    }
}

status WuhanSubway::ShowStartStation(TimeHMS now, int Sline, int SID)
{
    vector<Station>::iterator itSt;
    for (itSt = line[Sline].st_list.begin(); itSt != line[Sline].st_list.end(); itSt++)
    {
        if (itSt->id.station_number == SID)
            break;
    }
    ui->op->append("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");
    QString outp_1;
    outp_1=QString::fromLocal8Bit("������ʼվ���ǣ�")+QString::fromLocal8Bit(itSt->name.data());
    ui->op->append(outp_1);
    QString outp_2;
    outp_2.clear();
    if (itSt->transfer == true)
    {
        outp_2+=QString::fromLocal8Bit("��վ�ǻ���վ���ɻ���:");
        vector<int>::iterator itInt;
        for (itInt = itSt->TransTo.begin(); itInt != itSt->TransTo.end(); itInt++)
        {
            outp_2+=QString::fromLocal8Bit(" �人����");
            outp_2+=QString::number(*itInt);
            outp_2+=QString::fromLocal8Bit("����,");
        }
    }
    else
    {
        outp_2+=QString::fromLocal8Bit("��վ���ǻ���վ.");
    }
    ui->op->append(outp_2);
    TimeHMS result = NextTrainTime(now);
    QString outp_3;
    outp_3.clear();
    if (result.h == -1)
    {
        outp_3+=QString::fromLocal8Bit("��ǰ���ڵ�������Ӫʱ���ڣ���ı����ʱ�䣡");
        ui->op->append(outp_3);
        ui->op->append("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");
        QMessageBox::warning(this, QString::fromLocal8Bit("��Ӫʱ����ʾ"), outp_3);
        return FALSE;
    }
    else if (result.m >= 2)
    {
        outp_3+=QString::fromLocal8Bit("�г��Ѿ���վ������");
        outp_3+=QString::number(result.m - 2);
        outp_3+=QString::fromLocal8Bit("��");
        outp_3+=QString::number(result.s);
        outp_3+=QString::fromLocal8Bit("��󷢳�����ץ���ϳ���");
        ui->op->append(outp_3);
        outp_3.clear();
        outp_3+=QString::fromLocal8Bit("�����г�����");
        outp_3+=QString::number(result.m);
        outp_3+=QString::fromLocal8Bit("��");
        outp_3+=QString::number(result.s);
        outp_3+=QString::fromLocal8Bit("��󵽴����");
        outp_3+=QString::number(result.m + 1);
        outp_3+=QString::fromLocal8Bit("��");
        outp_3+=QString::number(result.s);
        outp_3+=QString::fromLocal8Bit("�����վ.");
        ui->op->append(outp_3);
        outp_3.clear();
        ui->op->append("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");
        QString LCDdis=QString("%1").arg(result.m, 2, 10, QChar('0'))+QChar(':')+QString("%1").arg(result.s, 2, 10, QChar('0'));
        ui->NowTimeLCD->display(LCDdis);
        return OK;
    }
    else
    {
        outp_3.clear();
        outp_3+=QString::fromLocal8Bit("�����г�����");
        outp_3+=QString::number(result.m);
        outp_3+=QString::fromLocal8Bit("��");
        outp_3+=QString::number(result.s);
        outp_3+=QString::fromLocal8Bit("��󵽴����");
        outp_3+=QString::number(result.m + 1);
        outp_3+=QString::fromLocal8Bit("��");
        outp_3+=QString::number(result.s);
        outp_3+=QString::fromLocal8Bit("�����վ.");
        ui->op->append(outp_3);
        outp_3.clear();
        ui->op->append("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");
        QString LCDdis=QString("%1").arg(result.m, 2, 10, QChar('0'))+QChar(':')+QString("%1").arg(result.s, 2, 10, QChar('0'));
        ui->NowTimeLCD->display(LCDdis);
        return OK;
    }
}

void WuhanSubway::on_MDepthSlider_actionTriggered(int action)
{
    depth_m=action;
    ui->MdepthLCD->display(QString::number(depth_m));
}

void WuhanSubway::on_MDepthBox_valueChanged(int arg1)
{
    depth_m=arg1;
    ui->MdepthLCD->display(QString::number(depth_m));
}

status WuhanSubway::ShowDestStation(int Dline, int DID)
{
    vector<Station>::iterator itSt;
    for (itSt = line[Dline].st_list.begin(); itSt != line[Dline].st_list.end(); itSt++)
    {
        if (itSt->id.station_number == DID)
            break;
    }
    ui->op->append("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");
    QString outp1=QString::fromLocal8Bit("���ĵ���վ���ǣ�");
    outp1+=QString::fromLocal8Bit(itSt->name.data());
    ui->op->append(outp1);
    outp1.clear();
    if (itSt->transfer == true)
    {
        outp1+=QString::fromLocal8Bit("��վ�ǻ���վ���ɻ���:");
        vector<int>::iterator itInt;
        for (itInt = itSt->TransTo.begin(); itInt != itSt->TransTo.end(); itInt++)
        {
            outp1+=QString::fromLocal8Bit(" �人����");
            outp1+=QString::number(*itInt);
            outp1+=QString::fromLocal8Bit("����,");
        }
    }
    else
    {
        outp1+=QString::fromLocal8Bit("��վ���ǻ���վ.");
    }
    ui->op->append(outp1);
    ui->op->append("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");
    return OK;
}


void WuhanSubway::on_AboutQT_clicked()
{
    QMessageBox::aboutQt(this, QString::fromLocal8Bit("���������Qt��ܱ�д"));
}

void WuhanSubway::on_MDepthSlider_valueChanged(int value)
{
    if(value<=6)
        QMessageBox::warning(this, QString::fromLocal8Bit("���͵�������ȣ�"), QString::fromLocal8Bit("ʹ�ù��͵�������ȿ��ܻᵼ��·������ʧ���Լ����ָ����޷�Ԥ��ı������֣��Ƽ��������Ĭ��Ϊ12."));
    else if(value>=20)
        QMessageBox::warning(this, QString::fromLocal8Bit("���ߵ�������ȣ�"), QString::fromLocal8Bit("ʹ��̫�ߵ�������ȿ��ܻᵼ��·�������ٶ�ʮ�ֻ�����Ҳ�п��ܵ�������ķ������Ƽ��������Ĭ��Ϊ12."));
}

void WuhanSubway::on_R1_clicked()
{
    ui->PlanRoute->setDisabled(false);
}

void WuhanSubway::on_R2_clicked()
{
    ui->PlanRoute->setDisabled(false);
}

void WuhanSubway::on_R3_clicked()
{
    ui->PlanRoute->setDisabled(false);
}

void WuhanSubway::on_PlanRoute_clicked()
{
    route_main(tt, lineid_s, stid_s, lindid_d, stid_d, depth_m);
    ui->lu->append(QString::fromLocal8Bit("=======================·���Ƽ�======================="));
    ui->lu->append(QString::fromLocal8Bit("����ʱ��:  ")+QString("%1").arg(tt.h, 2, 10, QChar('0'))+QChar(':')+QString("%1").arg(tt.m, 2, 10, QChar('0')));
    ui->lu->append(QString::fromLocal8Bit(line[R[0].way.begin()->line].st_list[R[0].way.begin()->station_number-1].name.data())+" -------> "+QString::fromLocal8Bit(line[R[0].way.back().line].st_list[R[0].way.back().station_number-1].name.data()));
    if(ui->R1->isChecked())
    {
        sort(R.begin(), R.end(), comprehensive_cmp);
        qDebug()<<"Mode 1";
        ui->lu->append(QString::fromLocal8Bit("���ۺ��������ŷ�ʽ�Ƽ�·��<br />"));
    }
    else if(ui->R2->isChecked())
    {
        sort(R.begin(), R.end(), time_cmp);
        qDebug()<<"Mode 2";
        ui->lu->append(QString::fromLocal8Bit("����ʱ��̷�ʽ�Ƽ�·��<br />"));
    }
    else if(ui->R3->isChecked())
    {
        sort(R.begin(), R.end(), transfer_cmp);
        qDebug()<<"Mode 3";
        ui->lu->append(QString::fromLocal8Bit("�Ի��˴������ٷ�ʽ�Ƽ�·��<br />"));
    }
    else if(ui->R4->isChecked())
    {
        sort(R.begin(), R.end(), price_cmp);
        qDebug()<<"Mode 4";
        ui->lu->append(QString::fromLocal8Bit("��Ʊ������˷�ʽ�Ƽ�·��<br />"));
    }
    else if(ui->R5->isChecked())
    {
        sort(R.begin(), R.end(), traffic_cmp);
        qDebug()<<"Mode 5";
        ui->lu->append(QString::fromLocal8Bit("�����ʶ����(·������ӵ�������)��ʽ�Ƽ�·��<br />"));
    }

    for(int i=0, c=0;c<3;i++)
    {
        if(i>=R.end()-R.begin())
            break;
        Route Ro=R[i];
        if(ui->R6->isChecked()&&Ro.overall_traffic>ui->R6_Box->value())
            continue;
        ui->lu->append(QString::fromLocal8Bit("�Ƽ�·��")+QString::number(c+1)+QChar(':'));
        vector<StNum>::iterator itSN;
        TimeHMS begintime=tt;
        TimeHMS usetime=Sec2Time(Ro.time_sec);
        int nowt_sec=Time2Sec(tt);
        int forecast_sec=nowt_sec+Ro.time_sec;
        TimeHMS forecast=Sec2Time(forecast_sec);
        ui->lu->append(QString::fromLocal8Bit("��ʱ: ")+QString("%1").arg(usetime.h, 2, 10, QChar('0'))+QString::fromLocal8Bit("ʱ")+QString("%1").arg(usetime.m, 2, 10, QChar('0'))+QString::fromLocal8Bit("��")+QString("%1").arg(usetime.s, 2, 10, QChar('0'))+QString::fromLocal8Bit("��;"));
        ui->lu->append(QString::fromLocal8Bit("Ʊ��: ")+QString::number(Ro.price)+QString::fromLocal8Bit("Ԫ;"));
        ui->lu->append(QString::fromLocal8Bit("���˴���: ")+QString::number(Ro.transfer_times)+QString::fromLocal8Bit("��;"));
        ui->lu->append(QString::fromLocal8Bit("�����: ")+QString::number(Ro.dist)+QString::fromLocal8Bit("����;"));
        ui->lu->append(QString::fromLocal8Bit("��������: ")+QString::number(Ro.s2s_traffic[0])+QString::fromLocal8Bit("��, һ������: ")+QString::number(Ro.s2s_traffic[1])+QString::fromLocal8Bit("��, ӵ������: ")+QString::number(Ro.s2s_traffic[2])+QString::fromLocal8Bit("��."));
        ui->lu->append(QString::fromLocal8Bit("����ӵ��ָ��: ")+QString::number(Ro.overall_traffic,'f',3));
        if (forecast_sec > CLOSE_TIME)
        {
            ui->lu->append(QString::fromLocal8Bit("��Ӫʱ��Ԥ������ѡ������·�ߣ��������޷�����Ŀ�ĵأ�������ѡ��������ͨ��ʽ��"));
            QMessageBox::warning(this, QString::fromLocal8Bit("�����޷�����Ŀ�ĵ�"), QString::fromLocal8Bit("��Ӫʱ��Ԥ������ѡ������·�ߣ��������޷�����Ŀ�ĵأ�������ѡ��������ͨ��ʽ��"));
        }
        else
        {
            ui->lu->append(QString::fromLocal8Bit("Ԥ�Ƶ���ʱ��: ")+QString("%1").arg(forecast.h, 2, 10, QChar('0'))+QChar(':')+QString("%1").arg(forecast.m, 2, 10, QChar('0')));
        }
        QString outp=QString::fromLocal8Bit("���-(")+QString::number(Ro.way.begin()->line)+QString::fromLocal8Bit("����--");
        for (itSN = Ro.way.begin(); itSN != Ro.way.end() - 1; itSN++)
        {
            vector<StNum>::iterator nxtNode = itSN + 1;
            if (itSN->line != nxtNode->line)
            {
                outp+="-->";
                outp+=QString::fromLocal8Bit(line[itSN->line].st_list[itSN->station_number - 1].name.data());
                outp+="--";
                outp+=QString::number(itSN->line);
                outp+=QString::fromLocal8Bit("����)-{����");
                outp+=QString::number(nxtNode->line);
                outp+=QString::fromLocal8Bit("����}-(");
                outp+=QString::number(nxtNode->line);
                outp+=QString::fromLocal8Bit("����--");
            }
            else
            {
                int delta = JudgeDirection(itSN->station_number, nxtNode->station_number);
                for (int i = itSN->station_number - 1; i != nxtNode->station_number; i += delta)
                {
                    if (i == nxtNode->station_number - delta || i == nxtNode->station_number)
                        break;
                    outp+="-->";
                    outp+=QString::fromLocal8Bit(line[itSN->line].st_list[i].name.data());
                }
            }
        }
        outp+="-->";
        outp+=QString::fromLocal8Bit(line[itSN->line].st_list[itSN->station_number - 1].name.data());
        outp+="--";
        outp+=QString::number(itSN->line);
        outp+=QString::fromLocal8Bit("����)-�յ�<br />");
        ui->lu->append(outp);
        c++;
    }
    ui->lu->append("<br />");
    ui->Outp_ret->setDisabled(false);
}

void WuhanSubway::on_CButn_clicked()
{
    ui->lu->clear();
}

void WuhanSubway::on_Outp_ret_clicked()
{
    QString path;
    QDir d;
    d.mkdir(QDir::currentPath()+"/SaveFile/");
    path=QDir::currentPath()+"/SaveFile/"+QString::fromLocal8Bit(line[R[0].way.begin()->line].st_list[R[0].way.begin()->station_number-1].name.data())+QString::fromLocal8Bit("��")+QString::fromLocal8Bit(line[R[0].way.back().line].st_list[R[0].way.back().station_number-1].name.data())+QString::fromLocal8Bit("�Ľ���·��.txt");
    QString filename=QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("���Ϊ"), path, QString::fromLocal8Bit("�ı��ĵ� (*.txt)"));
    if(!filename.isNull())
    {
        QFile newfile;
        //newfile.open()
    }
    else
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("�������ش���"), QString::fromLocal8Bit("�ļ�����ʧ��"));
        return;
    }
}

void WuhanSubway::on_SetPathBtn_clicked()
{
    QString config_path=QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("��.in�ļ�"), "/home", QString::fromLocal8Bit("ԬҲ�ļ� (*.in)"));
    ui->op->append(QString::fromLocal8Bit("�����ļ���: ")+config_path);
    if(INFEASTABLE== input(config_path.toStdString().c_str()))
    {
        QMessageBox::critical(this, QString::fromLocal8Bit("���ش���"), QString::fromLocal8Bit("���������ļ��Ƿ���."));
    }
    BuildMap();
}

void WuhanSubway::on_DDButn_clicked()
{

}

void WuhanSubway::on_Website_butn_clicked()
{
    QDesktopServices::openUrl(QUrl(QLatin1String("https://www.wuhanrt.com/public_forward.aspx")));
}

void WuhanSubway::on_PrevStation_clicked()
{
    yst--;
    transfer_list.clear();
    trans_it=transfer_list.begin();
    ui->SID_Label->setText(QString::number(yline)+QString::fromLocal8Bit("���ߵ�")+QString::number(yst)+QString::fromLocal8Bit("վ"));
    ui->SName_Label->setText(QString::fromLocal8Bit(line[yline].st_list[yst-1].name.data()));
    if(line[yline].st_list[yst-1].transfer==true)
    {
        ui->TransferBtn->setDisabled(false);
        QString btntext=QString::fromLocal8Bit("��\n��\n")+QString::number(*line[yline].st_list[yst-1].TransTo.begin())+QString::fromLocal8Bit("\n��\n��");
        ui->TransferBtn->setText(btntext);
        transfer_list=line[yline].st_list[yst-1].TransTo;
        transfer_list.push_back(yline);
        trans_it=transfer_list.begin();
    }
    else
    {
        ui->TransferBtn->setDisabled(true);
        ui->TransferBtn->setText(QString::fromLocal8Bit("��\n��\n��\n��"));
    }
    if(yst==0)
    {
        ui->PrevStation->setText(QString::fromLocal8Bit("��PrevStn"));
        ui->PrevStation->setDisabled(true);
        ui->NextStation->setText(QString::fromLocal8Bit("��NextStn"));
        ui->NextStation->setDisabled(true);
    }
    else if(yst==1)
    {
        ui->PrevStation->setDisabled(true);
        ui->PrevStation->setText(QString::fromLocal8Bit("T�յ�վ"));
        QString btntext;
        btntext=QString::fromLocal8Bit("��")+QString::fromLocal8Bit(line[yline].st_list[yst].name.data());
        ui->NextStation->setText(btntext);
        ui->NextStation->setDisabled(false);
    }
    else if(yst==line[yline].St_Num)
    {
        QString btntext=QString::fromLocal8Bit("��")+QString::fromLocal8Bit(line[yline].st_list[yst-2].name.data());
        ui->PrevStation->setText(btntext);
        ui->PrevStation->setDisabled(false);
        ui->NextStation->setDisabled(true);
        ui->NextStation->setText(QString::fromLocal8Bit("-�յ�վ"));
    }
    else
    {
        QString btntext=QString::fromLocal8Bit("��")+QString::fromLocal8Bit(line[yline].st_list[yst-2].name.data());
        ui->PrevStation->setText(btntext);
        ui->PrevStation->setDisabled(false);
        btntext.clear();
        btntext=QString::fromLocal8Bit("��")+QString::fromLocal8Bit(line[yline].st_list[yst].name.data());
        ui->NextStation->setText(btntext);
        ui->NextStation->setDisabled(false);
    }
}

void WuhanSubway::on_NextStation_clicked()
{
    yst++;
    transfer_list.clear();
    trans_it=transfer_list.begin();
    ui->SID_Label->setText(QString::number(yline)+QString::fromLocal8Bit("���ߵ�")+QString::number(yst)+QString::fromLocal8Bit("վ"));
    ui->SName_Label->setText(QString::fromLocal8Bit(line[yline].st_list[yst-1].name.data()));
    if(line[yline].st_list[yst-1].transfer==true)
    {
        ui->TransferBtn->setDisabled(false);
        QString btntext=QString::fromLocal8Bit("��\n��\n")+QString::number(*line[yline].st_list[yst-1].TransTo.begin())+QString::fromLocal8Bit("\n��\n��");
        ui->TransferBtn->setText(btntext);
        transfer_list=line[yline].st_list[yst-1].TransTo;
        transfer_list.push_back(yline);
        trans_it=transfer_list.begin();
    }
    else
    {
        ui->TransferBtn->setDisabled(true);
        ui->TransferBtn->setText(QString::fromLocal8Bit("��\n��\n��\n��"));
    }
    if(yst==0)
    {
        ui->PrevStation->setText(QString::fromLocal8Bit("��PrevStn"));
        ui->PrevStation->setDisabled(true);
        ui->NextStation->setText(QString::fromLocal8Bit("��NextStn"));
        ui->NextStation->setDisabled(true);
    }
    else if(yst==1)
    {
        ui->PrevStation->setDisabled(true);
        ui->PrevStation->setText(QString::fromLocal8Bit("T�յ�վ"));
        QString btntext;
        btntext=QString::fromLocal8Bit("��")+QString::fromLocal8Bit(line[yline].st_list[yst].name.data());
        ui->NextStation->setText(btntext);
        ui->NextStation->setDisabled(false);
    }
    else if(yst==line[yline].St_Num)
    {
        QString btntext=QString::fromLocal8Bit("��")+QString::fromLocal8Bit(line[yline].st_list[yst-2].name.data());
        ui->PrevStation->setText(btntext);
        ui->PrevStation->setDisabled(false);
        ui->NextStation->setDisabled(true);
        ui->NextStation->setText(QString::fromLocal8Bit("-�յ�վ"));
    }
    else
    {
        QString btntext=QString::fromLocal8Bit("��")+QString::fromLocal8Bit(line[yline].st_list[yst-2].name.data());
        ui->PrevStation->setText(btntext);
        ui->PrevStation->setDisabled(false);
        btntext.clear();
        btntext=QString::fromLocal8Bit("��")+QString::fromLocal8Bit(line[yline].st_list[yst].name.data());
        ui->NextStation->setText(btntext);
        ui->NextStation->setDisabled(false);
    }
}

void WuhanSubway::on_TransferBtn_clicked()
{
    for(vector<Station>::iterator it1=line[*trans_it].st_list.begin();it1!=line[*trans_it].st_list.end();it1++)
    {
        if(it1->name==line[yline].st_list[yst-1].name)
        {
            yline=it1->id.line;
            yst=it1->id.station_number;
            break;
        }
    }

    /*................*/
    QString outp_op=QString::fromLocal8Bit("�人����")+QString::number(yline)+QString::fromLocal8Bit("����");
    ui->op->append(outp_op);
    outp_op.clear();
    outp_op=QString::fromLocal8Bit("��վ����:")+QString::number(line[yline].St_Num)+QString::fromLocal8Bit("��;     ��·ȫ��:")+QString::number(line[yline].length)+QString::fromLocal8Bit("km;     ����ؿ���:")+QString::number(line[yline].fullNum)+QString::fromLocal8Bit("�ˡ�");
    ui->op->append(outp_op);
    outp_op.clear();

    for(vector<Station>::iterator itSt=line[yline].st_list.begin();itSt!=line[yline].st_list.end();itSt++)
    {
        ui->Sstation->addItem(QString::fromLocal8Bit(itSt->name.data()));
        QString outptext=QString::number(itSt->id.line)+QString("%1").arg(itSt->id.station_number, 2, 10, QChar('0'))+QChar(' ')+QString::fromLocal8Bit(itSt->name.data());
        if(itSt->transfer==true)
        {
            outptext+=QString::fromLocal8Bit("  �ɻ���");
            for(vector<int>::iterator itInt=itSt->TransTo.begin();itInt!=itSt->TransTo.end();itInt++)
            {
                outptext+=QString::number(*itInt);
                outptext+=QString::fromLocal8Bit("����,");
            }
        }
        ui->op->append(outptext);
    }
    /*................*/

    trans_it++;
    if(trans_it==transfer_list.end())
        trans_it=transfer_list.begin();

    ui->SID_Label->setText(QString::number(yline)+QString::fromLocal8Bit("���ߵ�")+QString::number(yst)+QString::fromLocal8Bit("վ"));
    ui->SName_Label->setText(QString::fromLocal8Bit(line[yline].st_list[yst-1].name.data()));

    ui->TransferBtn->setDisabled(false);
    QString btntext=QString::fromLocal8Bit("��\n��\n")+QString::number(*trans_it)+QString::fromLocal8Bit("\n��\n��");
    ui->TransferBtn->setText(btntext);

    if(yst==0)
    {
        ui->PrevStation->setText(QString::fromLocal8Bit("��PrevStn"));
        ui->PrevStation->setDisabled(true);
        ui->NextStation->setText(QString::fromLocal8Bit("��NextStn"));
        ui->NextStation->setDisabled(true);
    }
    else if(yst==1)
    {
        ui->PrevStation->setDisabled(true);
        ui->PrevStation->setText(QString::fromLocal8Bit("T�յ�վ"));
        QString btntext;
        btntext=QString::fromLocal8Bit("��")+QString::fromLocal8Bit(line[yline].st_list[yst].name.data());
        ui->NextStation->setText(btntext);
        ui->NextStation->setDisabled(false);
    }
    else if(yst==line[yline].St_Num)
    {
        QString btntext=QString::fromLocal8Bit("��")+QString::fromLocal8Bit(line[yline].st_list[yst-2].name.data());
        ui->PrevStation->setText(btntext);
        ui->PrevStation->setDisabled(false);
        ui->NextStation->setDisabled(true);
        ui->NextStation->setText(QString::fromLocal8Bit("-�յ�վ"));
    }
    else
    {
        QString btntext=QString::fromLocal8Bit("��")+QString::fromLocal8Bit(line[yline].st_list[yst-2].name.data());
        ui->PrevStation->setText(btntext);
        ui->PrevStation->setDisabled(false);
        btntext.clear();
        btntext=QString::fromLocal8Bit("��")+QString::fromLocal8Bit(line[yline].st_list[yst].name.data());
        ui->NextStation->setText(btntext);
        ui->NextStation->setDisabled(false);
    }
}

void WuhanSubway::on_DistBox_valueChanged(double arg1)
{
    QString outp=QString::number(PriceCal(arg1))+QString::fromLocal8Bit("Ԫ.");
    ui->Dist_Label->setText(outp);
}

void WuhanSubway::on_isManualBox_stateChanged(int arg1)
{
    if(arg1)
    {
        ui->TrafficBox->setDisabled(false);
        ui->Traffic_Line_Set->setDisabled(false);
        ui->TrafficYesBtn->setDisabled(false);
        ui->PrtTrafBtn->setDisabled(false);
        ui->op->append(QString::fromLocal8Bit("�����ֶ�ӵ��������ģʽ"));
        QMessageBox::information(this, QString::fromLocal8Bit("ģʽ����"), QString::fromLocal8Bit("�����ֶ�ӵ��������ģʽ"));
    }
    else
    {
        ui->TrafficBox->setCurrentIndex(0);
        ui->TrafficBox->setDisabled(true);
        ui->Traffic_Line_Set->setCurrentIndex(0);
        ui->Traffic_Line_Set->setDisabled(true);
        ui->TrafficYesBtn->setDisabled(true);
        ui->PrtTrafBtn->setDisabled(true);
        ui->op->append(QString::fromLocal8Bit("�ر��ֶ�ӵ��������ģʽ"));
        for (int i = 1; i < MAX_LINE_NUM; i++)
        {
            if (!openline[i])
                continue;
            line[i].cong_flag=0;
        }
    }
}

void WuhanSubway::on_R6_stateChanged(int arg1)
{
    if(arg1)
    {
        ui->R6_Box->setDisabled(false);
    }
    else
    {
        ui->R6_Box->setDisabled(true);
    }
}

void WuhanSubway::on_R4_clicked()
{
    ui->PlanRoute->setDisabled(false);
}

void WuhanSubway::on_R5_clicked()
{
    ui->PlanRoute->setDisabled(false);
}

void WuhanSubway::on_TrafficYesBtn_clicked()
{
    if(ui->Traffic_Line_Set->currentIndex()!=0&&ui->Traffic_Line_Set->currentIndex()!=5)
    {
        line[ui->Traffic_Line_Set->currentIndex()].cong_flag=ui->TrafficBox->currentIndex();
        QString outp=QString::fromLocal8Bit("����")+QString::number(ui->Traffic_Line_Set->currentIndex())+QString::fromLocal8Bit("���ߵ�ӵ���̶ȱ��ֶ�����Ϊ: ")+ui->TrafficBox->currentText();
        ui->op->append(outp);
        QMessageBox::information(this, QString::fromLocal8Bit("�������ֶ�����ӵ���̶�"), outp);
    }
}

void WuhanSubway::on_Traffic_Line_Set_activated(int index)
{
    if(index!=0&&index!=5)
    {
        ui->TrafficBox->setCurrentIndex(line[index].cong_flag);
    }
    else if(index==0)
    {
        return;
    }
    else
    {
        QString outp_err_line_message=QString::number(index)+QString::fromLocal8Bit("������δ��ͨ");
        ui->Dstation->setCurrentText(outp_err_line_message);
        ui->op->append(outp_err_line_message);
        QMessageBox::warning(this, QString::fromLocal8Bit("����"), outp_err_line_message);
        ui->Sstation->setDisabled(true);
    }

}

void WuhanSubway::on_PrtTrafBtn_clicked()
{
    ui->op->append(QString::fromLocal8Bit("====================����ӵ���̶�===================="));
    for (int i = 1; i < MAX_LINE_NUM; i++)
    {
        if (!openline[i])
            continue;
        QString outp;
        outp+=QString::fromLocal8Bit("����");
        outp+=QString::number(i);
        outp+=QString::fromLocal8Bit("����ӵ����: ");
        if(line[i].cong_flag==0)
            outp+=QString::fromLocal8Bit("Ĭ��");
        else if(line[i].cong_flag==1)
            outp+=QString::fromLocal8Bit("����");
        else if(line[i].cong_flag==2)
            outp+=QString::fromLocal8Bit("һ��");
        else if(line[i].cong_flag==3)
            outp+=QString::fromLocal8Bit("ӵ��");
        ui->op->append(outp);
    }
}
