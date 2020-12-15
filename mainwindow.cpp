#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    Input = new  QPlainTextEdit("{ Sample program in TINY language computes factorial }\n"
                                "read x; {input an integer }\n"
                                "if 0 < x then { don’t compute if x <= 0 }\n"
                                "    fact := 1;\n"
                                "    repeat\n"
                                "        fact := fact * x;\n"
                                "        x := x - 1\n"
                                "    until x = 0;\n"
                                "    write fact { output factorial of x }\n"
                                "end", this);
    ScannerOutput = new QPlainTextEdit(this);
    Toolbar = new QToolBar(this);
    Splitter = new QSplitter(Qt::Horizontal,this);
    scanner = new Scanner();
    //BrowseButton = new QPushButton(tr("Browse"), this);
    ScannerOutput->setReadOnly(true);
    ui->setupUi(this);
}
void MainWindow::init_toolbar()
{
    this->addToolBar(Qt::TopToolBarArea,Toolbar);
    QAction * BrowseAction = Toolbar->addAction("Browse");
    connect(BrowseAction,SIGNAL(triggered(bool)),this,SLOT(browse()));
    QAction * ScanAction = Toolbar->addAction("Scan");
    connect(ScanAction,SIGNAL(triggered(bool)),this,SLOT(start_Scan()));
}
void MainWindow::init_ui()
{
    QGroupBox *inGb = new QGroupBox("Input Program");
    QGroupBox *scannerGb = new QGroupBox("Scanner Output");
    inGb->setLayout(new QHBoxLayout);
    inGb->layout()->addWidget(Input);
    scannerGb->setLayout(new QHBoxLayout);
    scannerGb->layout()->addWidget(ScannerOutput);
    Splitter->addWidget(inGb);
    Splitter->addWidget(scannerGb);
    //to appear on the main window
    this->setCentralWidget(Splitter);
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::start_Scan()
{
    QString result = this->scanner->getToken(Input->toPlainText().toStdString());
    qDebug()<<result;
    ScannerOutput->setPlainText(result);
}
void MainWindow::browse()
{
    QString directory = QFileDialog::getOpenFileName(this, tr("Browsig Files"),
                                                    "QDir::currentPath()",
                                                    tr("Text (*.txt)"));
    QString result = this->scanner->getStringFile(directory);
    Input->setPlainText(result);

}
