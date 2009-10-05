#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mwdocview.h"
#include "document.h"

#include <QDebug>
#include <QtGui>

MainWindow::MainWindow(RealityNote& rn, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
    , mRn(rn)
{
    ui->setupUi(this);

    // Goofy, but works for now
    ui->tabWidget->removeTab(1);
    ui->tabWidget->removeTab(0);

    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(tabCloseRequested(int)));

    connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newDocument()));
    connect(ui->actionOpenExisting, SIGNAL(triggered()), this, SLOT(openExisting()));
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(closeDocument()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionShowLog, SIGNAL(triggered()), &mRn, SLOT(showLog()));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::tabCloseRequested(int index)
{
    MWDocView* view = findViewFromWidget(ui->tabWidget->widget(index));

    if (view)
    {
        view->closeView();
    }
}


void MainWindow::newDocument()
{
    mRn.newDocument(*this);
}


void MainWindow::openExisting()
{
    mRn.openExisting(*this);
}


void MainWindow::closeDocument()
{
    MWDocView* view = findViewFromWidget(ui->tabWidget->currentWidget());

    if (view)
    {
        view->closeDoc();
    }
}


void MainWindow::closeEvent(QCloseEvent* event)
{
    qDebug() << "MainWindow::closeEvent()";

    // For now always accept
    event->accept();

    // But in the future, we might ignore
    // event->ignore();

    // Tell our parent we have closed
    mRn.mainWindowClosed(*this);
}


void MainWindow::addDocument(Document& doc)
{
    MWDocView* view = new MWDocView(*this, doc);
    mDocViewList << view;

    // Add a tab for this new document
    int ix = ui->tabWidget->addTab(&(view->mTextEdit), view->mDoc.title());
    ui->tabWidget->setCurrentIndex(ix);

    // Don't worry about storing the index for the tab. It would change
    // later as tabs are removed so it's not worth trying to cache the value
}

void MainWindow::docViewClosed(MWDocView* view)
{
    if (!view) return;

    // Nuke the tab associated with this doc view
    int ix = ui->tabWidget->indexOf(&view->mTextEdit);
    if (ix != -1)
    {
        ui->tabWidget->removeTab(ix);
    }

    // Remove from our list
    mDocViewList.removeOne(view);

    // Nuke it
    delete view;
}

void MainWindow::docViewTitleChanged(MWDocView& view, QString& title)
{
    int ix = ui->tabWidget->indexOf(&view.mTextEdit);
    if (ix != -1)
    {
        ui->tabWidget->setTabText(ix,title);
    }
}


MWDocView* MainWindow::findViewFromWidget(QWidget* widg)
{
    if (!widg) return NULL;

    for(int i=0; i<mDocViewList.length(); i++)
    {
        if(&(mDocViewList.at(i)->mTextEdit) == widg)
        {
            return mDocViewList.at(i);
        }
    }

    return NULL;
}
