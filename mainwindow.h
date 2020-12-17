#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QToolBar>
#include <Qt>
#include <QSplitter>
#include <scanner.h>
#include <QString>
#include <QDebug>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QDir>
#include <QFileDialog>
#include <QTextBrowser>
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;
    QPlainTextEdit *Input;
    QPlainTextEdit *ScannerOutput;
    QToolBar *Toolbar;
    QSplitter *Splitter ;
    Scanner *scanner;
    QPushButton *BrowseButton ;
public:
    MainWindow(QWidget *parent = nullptr);
    void init_toolbar(void);
    void init_ui(void);
    ~MainWindow();
public slots:
    void start_Scan();
    void browse();
};
#endif // MAINWINDOW_H
