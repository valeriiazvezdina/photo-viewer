#include <QScrollArea>
#include <QLabel>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QCloseEvent>
#include <QSettings>
#include <QPixmap>
#include <QStringList>

#include "viewer.h"

Viewer::Viewer(QWidget *parent)
    : QMainWindow(parent), workDir(QDir::homePath())
{
    loadSettings();
    setupForm();
    setupActions();
    toggleZoomActions(false);
    toggleNextPrevActions(false);
    actZoomFitToWindow->setEnabled(false);
    setupConnections();
    currentIndex = 0;
}

void Viewer::setupForm()
{
    lblImage = new QLabel;
    lblImage->setSizePolicy(QSizePolicy::Ignored,
                            QSizePolicy::Ignored);
    lblImage->setScaledContents(true);
    scrollArea = new QScrollArea;
    scrollArea->setWidget(lblImage);
    this->setCentralWidget(scrollArea);
    this->setWindowTitle(tr("Simple Image Viewer"));
    this->resize(1024, 768);
    this->statusBar()->showMessage(tr("Application initialized"), 2000);
}

void Viewer::setupActions()
{
    QIcon iconQuit =
            QIcon::fromTheme("application-exit",
                    QIcon("/Users/valeriia.zvezdina/Desktop/c++/GUI/viewer/icons/quit.png"));

    actQuit = new QAction(tr("E&xit"));
    actQuit->setShortcut(QKeySequence::Quit);
    actQuit->setToolTip(tr("Exits the viewer"));
    actQuit->setStatusTip(tr("Exits the viewer"));
    actQuit->setIcon(iconQuit);

    QIcon iconOpen = QIcon::fromTheme("document-open",
                     QIcon("/Users/valeriia.zvezdina/Desktop/c++/GUI/viewer/icons/open.png"));

    actOpen = new QAction(tr("&Open..."));
    actOpen->setIcon(iconOpen);
    actOpen->setShortcut(QKeySequence::Open);
    actOpen->setToolTip(tr("Opens image file"));
    actOpen->setStatusTip(tr("Opens image file"));

    mnuFile = new QMenu(tr("&File"));
    mnuFile->addAction(actOpen);
    mnuFile->addAction(actQuit);

    QIcon iconZoomIn = QIcon::fromTheme("zoom-in",
                      QIcon("/Users/valeriia.zvezdina/Desktop/c++/GUI/viewer/icons/zoomIn.png"));
    actZoomIn = new QAction(tr("Zoom &In"));
    actZoomIn->setShortcut(QKeySequence::ZoomIn);
    actZoomIn->setIcon(iconZoomIn);
    actZoomIn->setToolTip(tr("Zooms image in"));
    actZoomIn->setStatusTip(tr("Zooms image in"));


    QIcon iconZoom100 = QIcon::fromTheme("zoom-original",
                      QIcon("/Users/valeriia.zvezdina/Desktop/c++/GUI/viewer/icons/zoom100.png"));
    actZoom100 = new QAction(tr("&Reset zoom"));

    actZoom100->setIcon(iconZoom100);
    actZoom100->setToolTip(tr("Resets zoom to 100%"));
    actZoom100->setStatusTip(tr("Resets zoom to 100%"));

    QIcon iconZoomOut = QIcon::fromTheme("zoom-Out",
                      QIcon("/Users/valeriia.zvezdina/Desktop/c++/GUI/viewer/icons/zoomOut.png"));
    actZoomOut = new QAction(tr("Zoom &Out"));
    actZoomOut->setShortcut(QKeySequence::ZoomOut);
    actZoomOut->setIcon(iconZoomOut);
    actZoomOut->setToolTip(tr("Zooms image out"));
    actZoomOut->setStatusTip(tr("Zooms image out"));

    QIcon iconZoomFitToWindow = QIcon::fromTheme("zoom-fit-best",
                      QIcon("/Users/valeriia.zvezdina/Desktop/c++/GUI/viewer/icons/fitToWindow.png"));
    actZoomFitToWindow = new QAction(tr("Fit to &window"));
    actZoomFitToWindow->setIcon(iconZoomFitToWindow);
    actZoomFitToWindow->setToolTip(tr("Fits image to window"));
    actZoomFitToWindow->setStatusTip(tr("Fits image to window"));
    actZoomFitToWindow->setCheckable(true);

    QIcon iconNext = QIcon::fromTheme("go-next",
                         QIcon("/Users/valeriia.zvezdina/Desktop/c++/GUI/viewer/icons/next_icon.png"));
    actNext = new QAction(tr("Next picture"));
    actNext->setIcon(iconNext);
    actNext->setToolTip(tr("Next picture"));
    actNext->setStatusTip(tr("Next picture"));
    actNext->setShortcut(QKeySequence::Forward);

    QIcon iconPrev = QIcon::fromTheme("go-previous",
                             QIcon("/Users/valeriia.zvezdina/Desktop/c++/GUI/viewer/icons/prev_icon.png"));
    actPrev = new QAction(tr("Previous picture"));
    actPrev->setIcon(iconPrev);
    actPrev->setToolTip(tr("Previous picture"));
    actPrev->setStatusTip(tr("Previous picture"));
    actPrev->setShortcut(QKeySequence::Back);

    mnuFile->addAction(actNext);
    mnuFile->addAction(actPrev);

    mnuZoom = new QMenu(tr("&Zoom"));
    mnuZoom->addAction(actZoomIn);
    mnuZoom->addAction(actZoom100);
    mnuZoom->addAction(actZoomOut);
    mnuZoom->addAction(actZoomFitToWindow);

    this->menuBar()->addMenu(mnuFile);
    this->menuBar()->addMenu(mnuZoom);

    toolBar = addToolBar(tr("Main toolbar"));
    toolBar->addAction(actOpen);
    toolBar->addAction(actQuit);
    toolBar->addAction(actZoomIn);
    toolBar->addAction(actZoomOut);
    toolBar->addAction(actZoom100);
    toolBar->addAction(actZoomFitToWindow);
    toolBar->addAction(actPrev);
    toolBar->addAction(actNext);
}

void Viewer::setupConnections()
{
    connect(actQuit, SIGNAL(triggered()),
            this, SLOT(close()));
    connect(actOpen, SIGNAL(triggered()), this, SLOT(open()));
    connect(actZoomIn, SIGNAL(triggered()), this, SLOT(zoomIn()));
    connect(actZoomOut, SIGNAL(triggered()), this, SLOT(zoomOut()));
    connect(actZoom100, SIGNAL(triggered()), this, SLOT(zoom100()));
    connect(actZoomFitToWindow, SIGNAL(triggered()), this, SLOT(zoomFitToWindow()));
    connect(actNext, SIGNAL(triggered()), this, SLOT(next()));
    connect(actPrev, SIGNAL(triggered()), this, SLOT(prev()));
}

void Viewer::toggleZoomActions(bool state)
{
    actZoom100->setEnabled(state);
    actZoomIn->setEnabled(state);
    actZoomOut->setEnabled(state);
}

void Viewer::toggleNextPrevActions(bool state)
{
    actNext->setEnabled(state);
    actPrev->setEnabled(state);
}

void Viewer::open()
{
    fileName =
            QFileDialog::getOpenFileName(this, tr("Open File"),
                             workDir,
                             tr("Images (*.png *.xpm *.jpg);"
                                "All files (*.*)"));

    if(!fileName.isEmpty())
    {
        QFileInfo info(fileName);
        workDir = info.absoluteDir().absolutePath();
        openImage(fileName);

        QDir imageDir(workDir);

        QStringList filters;
        filters << "*.png" << "*.jpg" << "*.bmp" << "*.jpeg";

        for (const QFileInfo &info : imageDir.entryInfoList(filters, QDir::Files))
        {
            listPicturesPaths.append(info.filePath());
        }
        condition();
    }
}

void Viewer::openImage(QString &fileName)
{
    QImage image(fileName);
    if(image.isNull()){
        QMessageBox::warning(this, tr("Warning"),
                       tr("Failed to load selected image"));
        return;
    }
    zoomFactor = 1.0;
    lblImage->setPixmap(QPixmap::fromImage(image));
    lblImage->adjustSize();
    toggleZoomActions(true);
    toggleNextPrevActions(true);
    actZoomFitToWindow->setEnabled(true);
    this->statusBar()->showMessage(tr("Image was opened"), 2000);
}

void Viewer::zoom(double factor)
{
    zoomFactor = zoomFactor * factor;
    lblImage->resize(lblImage->pixmap(Qt::ReturnByValue).size() * zoomFactor);
    actZoomIn->setEnabled(zoomFactor < 3);
    actZoomOut->setEnabled(zoomFactor > 0.2);
}

void Viewer::zoomIn()
{
    zoom(1.2);
}

void Viewer::zoomOut()
{
    zoom(0.8);
}

void Viewer::zoom100()
{
    zoomFactor = 1.0;
    zoom(1.0);
}

void Viewer::zoomFitToWindow()
{
    bool fit = actZoomFitToWindow->isChecked();
    scrollArea->setWidgetResizable(fit);
    if (!fit)
    {
        lblImage->adjustSize();
        zoom100();
    }
}

void Viewer::loadSettings()
{
    QSettings settings(tr("FNSPE CTU"), tr("Viewer 2023"));
    workDir = settings.value("workDir", QDir::homePath()).toString();
}

void Viewer::saveSettings()
{
    QSettings settings(tr("FNSPE CTU"), tr("Viewer2023"));
    settings.setValue("workDir", workDir);
}

void Viewer::closeEvent(QCloseEvent *event)
{
    if (QMessageBox::question(this, "quit", "do you really want to quit?") == QMessageBox::Yes)
    {
        event->accept();
        saveSettings();
    }
    else
    {
        event->ignore();
    }
}

void Viewer::prev()
{
    if (listPicturesPaths.isEmpty())
    {
        return;
    }

    if(currentIndex == 0)
    {
        actPrev->setEnabled(false);
    }
    else
    {
        actPrev->setEnabled(true);
        currentIndex--;
        fileName = listPicturesPaths.at(currentIndex);
        openImage(fileName);
    }
}

void Viewer::next()
{
    if (listPicturesPaths.isEmpty())
    {
        return;
    }

    if(currentIndex == listPicturesPaths.size()-1)
    {
        actNext->setEnabled(false);
    }
    else
    {
        actNext->setEnabled(true);
        currentIndex++;
        fileName = listPicturesPaths.at(currentIndex);
        openImage(fileName);
    }
}

void Viewer::condition()
{
    currentIndex = listPicturesPaths.indexOf(fileName);
    if (currentIndex == listPicturesPaths.size()-1)
    {
        actNext->setEnabled(false);
    }
    if(currentIndex == 0)
    {
        actPrev->setEnabled(false);
    }
}

Viewer::~Viewer()
{

}

