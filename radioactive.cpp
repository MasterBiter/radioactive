#include "radioactive.h"
#include "ui_radioactive.h"
#include "connectdb.h"

//#include "radioactivity.h"

#include <QtWidgets>
#include <QVBoxLayout>
#include <qcustomplot.h>
#include <QHBoxLayout>
#include <QGridLayout>
radioactive::radioactive(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::radioactive)
{
    log_to_save.setFileName("log.txt");
    log_to_save.open(QIODevice::ReadWrite | QIODevice::Text);
    log_to_save.remove();
    log_to_save.open(QIODevice::ReadWrite | QIODevice::Text);
    unlocked = false;
    ln2 = 2;
    ttmath::Ln(ln2);

    ui->setupUi(this);
    newLog("App Started");

    createMenu();
    newLog("Menu Created");
    createMainPlot();
    newLog("Plot Created");
    createEditGroup();
    createProcessGroup();
    newLog("Edit and Process boxes Created");

    initializeModel();
    newLog("SQL DB initialized");




    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addWidget(mainPlot,3);
    mainLayout->addWidget(editGroupBox,1);
    mainLayout->addWidget(processGroupBox,1);

    mainLayout->setMenuBar(menuBar);
    setLayout(mainLayout);


    mainPlot->addGraph();
    mainPlot->graph(0)->addData(mainGraphData.timeData,mainGraphData.activityData);

    mainPlot->xAxis->setLabel("Time");
    mainPlot->yAxis->setLabel("Activity");


    mainPlot->graph(0)->setPen(QPen(Qt::red)); // line color blue for first graph
    mainPlot->graph(0)->setBrush(QBrush(Qt::yellow)); // first graph will be filled with translucent blue



    qDebug() << qSetRealNumberPrecision(10);
    //TEST
    big_shtuka startNumU = "2550000000000000000000000";
    big_shtuka endTime = "2,83824e+17";
    endTime *= 3;
    big_shtuka timeIt = "4,7304e+14";
    for(big_shtuka i=0;i<endTime;i+=timeIt)
    {
        big_shtuka pow2thingU = 2;
        big_shtuka hl_thU = "7,041797292e-18";
        pow2thingU.Pow(  hl_thU * i * (-1));
        mainGraph_put( i.ToDouble()  , (startNumU*ln2*hl_thU*pow2thingU).ToDouble() );
    }
    //
    mainGraphRedraw();

}

radioactive::~radioactive()
{
    log_to_save.close();
    delete ui;
}

void radioactive::createMenu()
{
        menuBar = new QMenuBar;
        fileMenu = new QMenu(tr("&File"), this);
        exitAction = fileMenu->addAction(tr("E&xit"));
        minimaxiAction = fileMenu->addAction(tr("Maximize/minimize"));
        minimaxiAction->setShortcut(QKeySequence("F1"));
        menuBar->addMenu(fileMenu);
        connect(exitAction, SIGNAL(triggered()) , this , SLOT(close()) );
        connect(minimaxiAction, SIGNAL(triggered()) ,this, SLOT(minimize_maximize()) );

        logMenu = new QMenu(tr("&Log"),this);
        getLogAction_qDebug = logMenu->addAction(tr("Get log in qDebug"));
        getLogAction_qDebug->setShortcut(QKeySequence("F2"));
        menuBar->addMenu(logMenu);
        connect(getLogAction_qDebug, SIGNAL(triggered()), this , SLOT(getMainLog_qDebug()) );

        dbMenu = new QMenu(tr("&Database"));
        showIsotopesDataBase = dbMenu->addAction(tr("Show isotopes database"));
        showIsotopesDataBase->setShortcut(QKeySequence("F3"));

        openDBConsole = dbMenu->addAction(tr("SQL request to isotopes database"));
        openDBConsole->setShortcut(QKeySequence("F4"));
        connect(openDBConsole, SIGNAL(triggered()), this, SLOT(openConsoleDB()) );

        menuBar->addMenu(dbMenu);
        connect(showIsotopesDataBase, SIGNAL(triggered()), this, SLOT(showTable()) );
}

void radioactive::minimize_maximize()
{
    if(isMaximized())
    {
        showNormal();
        newLog("Minimized");
    }
    else
    {
        showMaximized();
        newLog("Maximized");
    }
}

void radioactive::createMainPlot()
{
    mainPlot = new QCustomPlot;

}

void radioactive::createEditGroup()
{
    editGroupBox = new QGroupBox(tr("Edit"));
    QGridLayout *eGB = new QGridLayout;

    Edit1CollumnDesc = new QLabel("Number of molecules to add:");
    eGB->addWidget(Edit1CollumnDesc,1,1);

    numOfMoleculesToAdd = new QTextEdit;
    numOfMoleculesToAdd->setPlaceholderText("number");
    eGB->addWidget(numOfMoleculesToAdd,2,1);

    isotopeToAdd = new QComboBox;
    getIsotopeList();
    isotopeToAdd->setCurrentText("Choose Isotope");

    eGB->addWidget(isotopeToAdd,3,1);

    addButton = new QPushButton("Add");
    connect(addButton,SIGNAL(clicked()),this,SLOT(addButtonClicked()));
    eGB->addWidget(addButton,4,1);



    eGB->setColumnStretch(1,4);
    eGB->setColumnStretch(2,3);
    eGB->setColumnStretch(3,11);
    eGB->setRowStretch(1,3);
    eGB->setRowStretch(2,4);
    eGB->setRowStretch(3,4);
    eGB->setRowStretch(4,5);

    editGroupBox->setLayout(eGB);

}

void radioactive::createProcessGroup()
{
    processGroupBox = new QGroupBox(tr("Process"));
    QGridLayout *pGB = new QGridLayout;

    Process1CollumnDesc = new QLabel("Iteration interval:");
    pGB->addWidget(Process1CollumnDesc,1,1);

    timeOfIteration = new QTextEdit;
    timeOfIteration->setPlaceholderText("number");
    pGB->addWidget(timeOfIteration,2,1);

    unitsIteration = new QComboBox;

    pGB->addWidget(unitsIteration,3,1);

    //QHBoxLayout *setHL = new QHBoxLayout;

    setTimeButton = new QPushButton("Set");
    //setHL->addWidget(setTimeButton);

    /*continueButton = new QPushButton("Start/continue");
    setHL->addWidget(continueButton);

    stopButton = new QPushButton("Stop");
    setHL->addWidget(stopButton);



    setHL->setStretch(0,8);
    setHL->setStretch(1,4);
    setHL->setStretch(2,5);

    pGB->addLayout(setHL,4,1);*/

    pGB->addWidget(setTimeButton,4,1);



    Process2CollumnDesc = new QLabel("Update interval:");
    pGB->addWidget(Process2CollumnDesc,1,2);

    updateTime = new QTextEdit;
    updateTime->setPlaceholderText("number");
    pGB->addWidget(updateTime,2,2);

    unitsUpdate = new QComboBox;
    unitsUpdate->setCurrentText("2ergty3g");
    pGB->addWidget(unitsUpdate,3,2);

    setUpdateButton = new QPushButton("Set");
    pGB->addWidget(setUpdateButton,4,2);


    /*QLabel *Process4CollumnDesc;
    QPushButton *stopIt;
    QPushButton *continueIt;
    QComboBox *graphUnitsBox;
    QPushButton *setGraphUnits;
    QLabel *pauseunpause;*/

    Process4CollumnDesc = new QLabel("Set graph's time units(DON'T use after long iterations):");
    pGB->addWidget(Process4CollumnDesc,1,4);

    graphUnitsBox = new QComboBox;
    pGB->addWidget(graphUnitsBox,2,4);

    setGraphUnits = new QPushButton("Set");
    pGB->addWidget(setGraphUnits,3,4);

    QHBoxLayout *prL = new QHBoxLayout;

    pauseunpause = new QLabel("Process:");
    prL->addWidget(pauseunpause,4);

    continueIt = new QPushButton("Continue/start");
    prL->addWidget(continueIt,5);

    stopIt = new QPushButton("Stop");
    prL->addWidget(stopIt,3);

    pGB->addLayout(prL,4,4);











    pGB->setColumnStretch(1,4);
    pGB->setColumnStretch(2,3);
    pGB->setColumnStretch(3,9);
    pGB->setColumnStretch(4,2);
    pGB->setRowStretch(1,3);
    pGB->setRowStretch(2,2);
    pGB->setRowStretch(3,4);
    pGB->setRowStretch(4,5);
    pGB->setVerticalSpacing(10);

    processGroupBox->setLayout(pGB);
}

void radioactive::newLog(QString nLog)
{
    QString logToPush = "[" + QTime::currentTime().toString("HH:mm:ss") + "] " +  nLog;
    mainLog.push_back(logToPush);
    qDebug() << QString(logToPush);
    QTextStream log_qts(&log_to_save);
    log_qts<<logToPush << "\n";

}

void radioactive::getMainLog_qDebug()
{
    for(int i=0;i<mainLog.size();i++)
    {
        qDebug()<<mainLog[i];
    }
}

void radioactive::addButtonClicked()
{
    newLog("Add button Clicked");
}

void radioactive::mainGraphRedraw()
{
    newLog("Main Graph redrawed");
    mainPlot->graph(0)->setData(mainGraphData.timeData,mainGraphData.activityData);
    mainPlot->xAxis->setRange(0,mainGraphData.timeData[mainGraphData.timeData.size()-1]);
    //mainPlot->yAxis->setRange(0.9999*minimum_rad(),1.0001*maximum_rad());
    mainPlot->yAxis->setRange(0,1.2*maximum_rad());

    mainPlot->replot();
    mainPlot->repaint();
}

double radioactive::minimum_rad()
{
    double res = 999999999999999999999999999999999999999.999;
    for(int i=0;i<mainGraphData.timeData.size();i++)
    {
        if(mainGraphData.activityData[i]<res)
        {
            res=mainGraphData.activityData[i];
        }
    }
    return res;
}

double radioactive::maximum_rad()
{
    double res = 0;
    for(int i=0;i<mainGraphData.timeData.size();i++)
    {
        if(mainGraphData.activityData[i]>res)
        {
            res=mainGraphData.activityData[i];
        }
    }
    return res;
}

void radioactive::mainGraph_put(double t, double a)
{
    newLog("Added dot to mainGraph: (t, a) : (" + QString::number(t,'g',16) + ", " + QString::number(a,'g',16) + " )");
    mainGraphData.activityData.push_back(a);
    mainGraphData.timeData.push_back(t);
}

void radioactive::getIsotopeList()
{
    /*isotopeToAdd->addAction(QAction (tr("Uran-238"),0));
    isotopeToAdd->addAction(QAction (tr("Thorium-234"),0));
    isotopeToAdd->addAction(QAction (tr("Protoactinium-234"),0));*/

}

void radioactive::initializeModel()
{
    if(!createConnection())
    {
        newLog("FATAL! CREATECONNECTION FAILED!");
    }
    isotDB = QSqlDatabase::addDatabase("QSQLITE");
    isotDB.setDatabaseName("isotopes.gdb");
    isotDB.open();
    model = new QSqlTableModel(0,isotDB);

    model->setTable("isotopes");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    qDebug() << model->select();

    qDebug() << model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    qDebug() << model->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
    qDebug() << model->setHeaderData(2, Qt::Horizontal, QObject::tr("Mass"));
    qDebug() << model->setHeaderData(3, Qt::Horizontal, QObject::tr("Charge"));
    qDebug() << model->setHeaderData(4, Qt::Horizontal, QObject::tr("Half-life period"));
    qDebug() << model->setHeaderData(5, Qt::Horizontal, QObject::tr("Alpha decay probability"));
    qDebug() << model->setHeaderData(6, Qt::Horizontal, QObject::tr("Beta decay probability"));
}

void radioactive::showTable()
{
    QTableView *view = new QTableView;
    view->setModel(model);
    view->setWindowTitle("Isotopes");
    newLog("Isotope DB is showing");
    view->show();
}

bool radioactive::accessNeeded()
{
    if(unlocked)
    {
        return true;
    }
    QString trypass = QInputDialog::getText(0,"Insert password",
                                            "Защита от дурака, которого ты тут из себя строишь");
    qDebug() << trypass;
    newLog("Tried to enter password: " + trypass);
    if(trypass=="lockstock")
    {
        unlocked = true;
        return true;
    }
    return false;
}

void radioactive::openConsoleDB()
{
    if(accessNeeded())
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setHostName("iSotopeServer");
        db.setDatabaseName("isotopes.gdb");
        db.open();
        QSqlQuery qr(db);
        QString request = QInputDialog::getText(0,"Isotopes DB","Enter request");
        if(qr.exec(request))
        {
            newLog("Request :''  "+request+"  '' successfully executed");
        }
        else
        {
            newLog("Request :''  "+request+"  '' failed to execute. Error: "+qr.lastError().text() );
        }

    }
    else
    {
        newLog("Wrong password entered!");
    }
}
