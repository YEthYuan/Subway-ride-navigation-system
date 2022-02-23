#include "wuhansubway.h"

#include <QApplication>
#include <stdio.h>
#include <iostream>
#include <QDebug>
#include "define.h"
#include "mapping.h"
#include "route.h"
#include "mycaltime.h"
#include "price.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WuhanSubway w;
    w.show();
    return a.exec();
}
