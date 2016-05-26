#include <iostream>
#include <math.h>
#include <ctime>
#include "mainwindow.h"
#include <QApplication>
#include <QFont>
#include <QGridLayout>
#include <QLCDNumber>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QLabel>
#include <QDialog>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QGuiApplication>

//using namespace std;

// KL = Cap lanceur
// KBinit = Cap But initial
// VL = Vitesse lanceur
// VBinit = Vitesse but de travail
// AZd = Azimut de détection
// GISd = Gisement de détection
// GISaff = gisement affiché de 0 à 359 (et non de -180 à 180
// RL = radiale lanceur (VL*cos gisement)
// LL = laterale lanceur (VL*sin gisement)

double KBinit,VL,VBinit,AZd,GISd,GISaff;
double RL,LL,w,krcst,krcstliss,Vrap,Pi1,kcoffey,kcoffeyliss;
int KL;

//prototype de calcul gisement
void calcul_gisement (int KL,double AZd,double VL,double& GISd,double& LL, double& RL, double& krcst, double VBinit,double w);

//debut du widget fenetre update
class Updater_v1 : public QWidget
{
public:
    Updater_v1(QWidget *parent = 0);
};

Updater_v1::Updater_v1(QWidget *parent)
    : QWidget(parent)
{

    QPushButton *quit_updater = new QPushButton(tr("Quit"), this);
    quit_updater->setGeometry(10, 10, 200, 75);
    quit_updater->setFont(QFont("Times", 18, QFont::Bold));

    QPushButton *update_updater = new QPushButton(tr("Update"), this);
    update_updater->setGeometry(10, 180, 200, 75);
    update_updater->setFont(QFont("Times", 18, QFont::Bold));

    connect(quit_updater, SIGNAL(clicked()), this , SLOT(close()));
    connect(quit_updater, SIGNAL(clicked()), qApp , SLOT(close()));
}


//debut du widget fenetre principale (spinboxs)
class Reglages : public QWidget
{
public:
Reglages(QWidget *parent = 0);

public slots:
void calcul_g(double AZd,double GISd);
//void calcul_rcst(double);
//void calcul_rcst(double);
};



Reglages::Reglages(QWidget *parent)
: QWidget(parent)
{



    QPushButton *quitbutton = new QPushButton(QObject::tr("Quit"));
    quitbutton->setFont(QFont("Times", 18, QFont::Bold));

    QPushButton *validbutton = new QPushButton(QObject::tr("Valid"));
    quitbutton->setFont(QFont("Times", 18, QFont::Bold));




    //SpinBoxes

    QSpinBox *spinbox_KL = new QSpinBox();
    spinbox_KL->setRange(0, 359);
    spinbox_KL->setValue(0);

    QSpinBox *spinbox_VL = new QSpinBox();
    spinbox_VL->setRange(0, 30);
    spinbox_VL->setValue(8);

    QSpinBox *spinbox_AZ = new QSpinBox();
    spinbox_AZ->setRange(0, 359);
    spinbox_AZ->setValue(0);

    QSpinBox *spinbox_VB = new QSpinBox();
    spinbox_VB->setRange(0, 30);
    spinbox_VB->setValue(15);

    QDoubleSpinBox *doublespinbox_W = new QDoubleSpinBox();
    doublespinbox_W->setRange(-10, 10);
    doublespinbox_W->setValue(0);
    doublespinbox_W->setSingleStep(0.1);

    //Sliders

    QSlider *slider_KL = new QSlider(Qt::Horizontal);
    slider_KL->setRange(0, 359);
    slider_KL->setValue(0);

    QSlider *slider_AZ = new QSlider(Qt::Horizontal);
    slider_AZ->setRange(0, 359);
    slider_AZ->setValue(0);

    QSlider *slider_VL = new QSlider(Qt::Horizontal);
    slider_VL->setRange(0, 30);
    slider_VL->setValue(8);

    QSlider *slider_VB = new QSlider(Qt::Horizontal);
    slider_VB->setRange(0, 30);
    slider_VB->setValue(15);

    QLabel *label_PI1txt = new QLabel();
    label_PI1txt->setText("PI = ");

    QLabel *label_PI1 = new QLabel();
    label_PI1->setNum(3.14159265);

    QLabel *label_KLtxt = new QLabel();
    label_KLtxt->setText("KL = ");

    QLabel *label_KL = new QLabel();
    label_KL->setNum(spinbox_KL->value());

    QLabel *label_KL2txt = new QLabel();
    label_KL2txt->setText("KL (variable) = ");

    QLabel *label_KL2 = new QLabel();
    label_KL2->setNum(KL);

    QLabel *label_VLtxt = new QLabel();
    label_VLtxt->setText("VL = ");

    QLabel *label_VL = new QLabel();
    label_VL->setNum(spinbox_VL->value());

    QLabel *label_AZtxt = new QLabel();
    label_AZtxt->setText("AZ = ");

    QLabel *label_AZ = new QLabel();
    label_AZ->setNum(spinbox_AZ->value());

    QLabel *label_GBtxt = new QLabel();
    label_GBtxt->setText("GB = ");

    QLabel *label_GB = new QLabel();
    label_GB->setNum((spinbox_AZ->value())-(spinbox_KL->value()));

    QLabel *label_VBtxt = new QLabel();
    label_VBtxt->setText("VB = ");

    QLabel *label_VB = new QLabel();
    label_VB->setNum(spinbox_VB->value());

    QLabel *label_RLtxt = new QLabel();
    label_RLtxt->setText("RL = ");

    RL = (spinbox_VL->value())*(cos((spinbox_AZ->value())-(spinbox_KL->value())));

    QLabel *label_RL = new QLabel();
    label_RL->setNum(RL);




    QLabel *label_KRCST = new QLabel();
    label_KRCST->setNum(spinbox_AZ->value() + 180 + (((asin(Vrap*sin((GISaff*Pi1)/180)))/Pi1)*180));

    //Connecteurs

    QObject::connect(quitbutton, SIGNAL(clicked()), qApp, SLOT(quit()));
    QObject::connect(quitbutton, SIGNAL(clicked()), label_GB, SLOT(update()));
    QObject::connect(slider_KL, SIGNAL(valueChanged(int)), spinbox_KL, SLOT(setValue(int)));
    QObject::connect(slider_VL, SIGNAL(valueChanged(int)), spinbox_VL, SLOT(setValue(int)));
    QObject::connect(slider_VB, SIGNAL(valueChanged(int)), spinbox_VB, SLOT(setValue(int)));
    QObject::connect(slider_AZ, SIGNAL(valueChanged(int)), spinbox_AZ, SLOT(setValue(int)));

    QObject::connect(spinbox_KL, SIGNAL(valueChanged(int)), slider_KL, SLOT(setValue(int)));
    QObject::connect(spinbox_VL, SIGNAL(valueChanged(int)), slider_VL, SLOT(setValue(int)));
    QObject::connect(spinbox_VB, SIGNAL(valueChanged(int)), slider_VB, SLOT(setValue(int)));
    QObject::connect(spinbox_AZ, SIGNAL(valueChanged(int)), slider_AZ, SLOT(setValue(int)));

    QObject::connect(spinbox_KL, SIGNAL(valueChanged(int)), label_KL, SLOT(setNum(int)));
    QObject::connect(spinbox_VL, SIGNAL(valueChanged(int)), label_VL, SLOT(setNum(int)));
    QObject::connect(spinbox_AZ, SIGNAL(valueChanged(int)), label_AZ, SLOT(setNum(int)));
    QObject::connect(spinbox_VB, SIGNAL(valueChanged(int)), label_VB, SLOT(setNum(int)));










    //LAYOUT !

    QFrame *separ1 = new QFrame;
    separ1->setFrameStyle(QFrame::HLine | QFrame::Plain);
    separ1->setLineWidth(2);

    QFrame *separ2 = new QFrame;
    separ2->setFrameStyle(QFrame::HLine | QFrame::Plain);
    separ2->setLineWidth(2);

    QFrame *separ3 = new QFrame;
    separ3->setFrameStyle(QFrame::HLine | QFrame::Plain);
    separ3->setLineWidth(2);

    QFrame *separ4 = new QFrame;
    separ4->setFrameStyle(QFrame::HLine | QFrame::Plain);
    separ4->setLineWidth(2);

    QFrame *separ5 = new QFrame;
    separ5->setFrameStyle(QFrame::HLine | QFrame::Plain);
    separ5->setLineWidth(2);



    QVBoxLayout *layout_KL = new QVBoxLayout;
    layout_KL->addWidget(spinbox_KL);
    layout_KL->addWidget(slider_KL);
    //layout_KL->addWidget(separ1);

    QVBoxLayout *layout_VL = new QVBoxLayout;
    layout_VL->addWidget(spinbox_VL);
    layout_VL->addWidget(slider_VL);
    layout_VL->addWidget(separ2);

    QVBoxLayout *layout_AZ = new QVBoxLayout;
    layout_AZ->addWidget(spinbox_AZ);
    layout_AZ->addWidget(slider_AZ);
    layout_AZ->addWidget(separ3);

    QVBoxLayout *layout_VB = new QVBoxLayout;
    layout_AZ->addWidget(spinbox_VB);
    layout_AZ->addWidget(slider_VB);
    layout_AZ->addWidget(separ4);

    QVBoxLayout *layout_W = new QVBoxLayout;
    layout_W->addWidget(doublespinbox_W);
    layout_W->addWidget(separ5);

    QVBoxLayout *layout_general = new QVBoxLayout;
    layout_general->addLayout(layout_KL);
    layout_general->addLayout(layout_VL);
    layout_general->addLayout(layout_AZ);
    layout_general->addLayout(layout_VB);
    layout_general->addLayout(layout_W);
    layout_general->addWidget(quitbutton);
    layout_general->addWidget(validbutton);
    layout_general->addWidget(label_PI1txt);
    layout_general->addWidget(label_PI1);
    layout_general->addWidget(label_KLtxt);
    layout_general->addWidget(label_KL);
    layout_general->addWidget(label_KL2txt);
    layout_general->addWidget(label_KL2);
    layout_general->addWidget(label_VLtxt);
    layout_general->addWidget(label_VL);
    layout_general->addWidget(label_AZtxt);
    layout_general->addWidget(label_AZ);
    layout_general->addWidget(label_GBtxt);
    layout_general->addWidget(label_GB);
    layout_general->addWidget(label_VBtxt);
    layout_general->addWidget(label_VB);
    setLayout(layout_general);

}



void Reglages::calcul_g(double AZd,double GISd)
{
    AZd = widget->spinbox_AZ->value();
    GISd = widget->spinbox_AZ->value() - widget->spinbox_KL->value();
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Updater_v1 update1;
    update1.show();
    update1.resize(300, 300);
    Reglages widget;
    widget.show();
    widget.resize(600, 1000);

    return app.exec();

}

