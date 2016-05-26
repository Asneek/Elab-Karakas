#include "mainwindow.h"
#include "ui_mainwindow.h"
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


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{

}
