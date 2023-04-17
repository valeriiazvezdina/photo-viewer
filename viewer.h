#ifndef VIEWER_H
#define VIEWER_H

#include <QMainWindow>

class QLabel;
class QScrollArea;
class QAction;
class QMenu;
class QToolBar;
class QCloseEvent;

class Viewer : public QMainWindow
{
    Q_OBJECT
    QLabel *lblImage;
    QScrollArea *scrollArea;
    QAction *actQuit;
    QAction *actOpen;
    QMenu *mnuFile;
    QMenu *mnuZoom;
    QAction *actZoomIn;
    QAction *actZoom100;
    QAction *actZoomOut;
    QAction *actZoomFitToWindow;
    QAction *actNext;
    QAction *actPrev;
    QToolBar *toolBar;
    QString workDir;
    QString fileName;
    QStringList listPicturesPaths;
    double zoomFactor;
    int currentIndex;

    void setupForm();
    void setupActions();
    void setupConnections();
    void openImage(QString &fileName);
    void toggleZoomActions(bool state);
    void toggleNextPrevActions(bool state);
    void zoom(double factor);
    void closeEvent(QCloseEvent *);
    void saveSettings();
    void loadSettings();
    void condition();
public:
    Viewer(QWidget *parent = nullptr);
    ~Viewer();
public slots:
    void open();
    void zoomIn();
    void zoomOut();
    void zoom100();
    void zoomFitToWindow();
    void next();
    void prev();
};
#endif // VIEWER_H

