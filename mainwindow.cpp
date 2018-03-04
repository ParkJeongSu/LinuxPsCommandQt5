#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include "qps.h"
#include "proc.h"
#include <QDebug>
#include <QString>
#include <QStandardItem>
#include <QStandardItemModel>
#include <stdlib.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int seconds=uptime();
    Proc *mproc;
    unsigned i;
    int pcpu;

    Plist ProcList;
    mproc = new Proc;
    mproc->MakeProcInfo();

    // 프로세스정보 리스트를 얻어온다.
    ProcList = mproc->GetProcList();

    int total_cpu = 0;
    // OS의 uptime을 얻어온다.
    // 얻어온 정보는 프로세스의 CPU 사용율을 구하기 위한 기초정보로
    // 사용된다.

    QString Qhours=QString::number(seconds/3600);
    QString Qminutes=QString::number((seconds/60)%60);
    QString Qseconds=QString::number(seconds%60);
    ui->hours->setText(Qhours);
    ui->minutes->setText(Qminutes);
    ui->seconds->setText(Qseconds);



    QStandardItemModel *model = new QStandardItemModel(mproc->ProcNum(),4,this);
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("USER")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("PID")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("CPU")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString("VMEM")));

    for (i = 0; i < mproc->ProcNum(); i++)
    {
        // CPU사용율을 얻어온다.
        pcpu = getcputime(atol(ProcList[i].process[13].c_str()),
        atol(ProcList[i].process[14].c_str()),
        atol(ProcList[i].process[21].c_str()), seconds);

        QStandardItem *firstRow = new QStandardItem(QString(ProcList[i].username));
        model->setItem(i,0,firstRow);
        QStandardItem *secondRow = new QStandardItem(QString(ProcList[i].process[0].c_str()));
        model->setItem(i,1,secondRow);
        QString temp=QString::number(pcpu/10);
        temp.append('.');
        temp.append(QString::number(pcpu%10));
        QStandardItem *thridRow = new QStandardItem(temp);
        model->setItem(i,2,thridRow);
        QStandardItem *fourRow = new QStandardItem(QString(ProcList[i].process[22].c_str()));
        model->setItem(i,3,fourRow);

    }




    ui->tableView->setModel(model);


}

MainWindow::~MainWindow()
{
    delete ui;
}
