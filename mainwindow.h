#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMap>
#include <QtMath>
#include <QValidator>
#include <QMessageBox>

using namespace std;

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
    void SelectFile(int index);
    void ChangeRatio();
private:
    Ui::MainWindow *ui;

    double ratio;
    QPixmap *pix;
    QMultiMap <double, QString> diagFiles;

    void ConstructImagePyramid();
};

#endif // MAINWINDOW_H
