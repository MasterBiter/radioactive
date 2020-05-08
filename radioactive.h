#ifndef RADIOACTIVE_H
#define RADIOACTIVE_H

#include <QWidget>
#include <QResizeEvent>
#include <QDebug>
#include <QMenuBar>
#include <QGroupBox>
#include <QMenu>
#include <QAction>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include "ttmath.h"
#include "qcustomplot.h"

#include <QtSql>

typedef ttmath::Big<TTMATH_BITS(64), TTMATH_BITS(256)> big_shtuka;

namespace Ui {
class radioactive;
}

class radioactive : public QWidget
{
    Q_OBJECT

public:

    big_shtuka ln2;

    void createMenu();
    QMenu *fileMenu;
    QAction *exitAction;
    QAction *minimaxiAction;
    QMenuBar *menuBar;
    QMenu *logMenu;
    QAction *getLogAction_qDebug;
    QMenu *dbMenu;
    QAction *showIsotopesDataBase;
    QAction *openDBConsole;

    void createMainPlot();
    QCustomPlot *mainPlot;

    QGroupBox *editGroupBox;
    QTextEdit *numOfMoleculesToAdd;
    QComboBox *isotopeToAdd;

    std::vector <QAction> isotopesList;
    void getIsotopeList();


    void createEditGroup();
    QLabel *Edit1CollumnDesc;
    QPushButton *addButton;



    QGroupBox *processGroupBox;
    QTextEdit *timeOfIteration;
    void createProcessGroup();

    QLabel *Process1CollumnDesc;

    QComboBox *unitsIteration;
    QAction *secondsIteration;
    QAction *minutesIteration;
    QAction *hoursIteration;
    QAction *yearsIteration;

    QPushButton *continueButton;
    QPushButton *stopButton;
    QPushButton *setTimeButton;

    QTextEdit *updateTime;
    QComboBox *unitsUpdate;
    QLabel *Process2CollumnDesc;
    QPushButton *setUpdateButton;

    QAction *secondsUpdate;
    QAction *minutesUpdate;
    QAction *hoursUpdate;
    QAction *yearsUpdate;


    QLabel *Process4CollumnDesc;
    QPushButton *stopIt;
    QPushButton *continueIt;
    QComboBox *graphUnitsBox;
    QPushButton *setGraphUnits;
    QLabel *pauseunpause;

    QAction *secondsGraph;
    QAction *minutesGraph;
    QAction *hoursGraph;
    QAction *yearsGraph;



    std::vector <QString> mainLog;
    QFile log_to_save;
    void newLog(QString nLog);


    explicit radioactive(QWidget *parent = 0);
    ~radioactive();

    struct graph
    {
         QVector <double> timeData;
         QVector <double> activityData;
    };
    graph mainGraphData;

    double minimum_rad();
    double maximum_rad();


    void mainGraph_put(double t,double a);
    void mainGraphRedraw();



    //SQL
    //
    QSqlDatabase isotDB;
    QSqlTableModel *model;
    void initializeModel();


    //

    //prv
    bool accessNeeded();
    bool unlocked;

private:
    Ui::radioactive *ui;
public slots:
    void minimize_maximize();

    void getMainLog_qDebug();

    void addButtonClicked();
    //SQL
    void showTable();
    void openConsoleDB();

};

#endif // RADIOACTIVE_H
