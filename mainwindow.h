#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void LoadImage();
    void SelectLayer(int index);
private:
    Ui::MainWindow *ui;

    QPixmap *pix;
    QVector <QVector <int> > imageSizes;

    void ConstructImagePyramid();
};

#endif // MAINWINDOW_H
