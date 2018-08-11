#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QMenu *file = new QMenu("&File");
    menuBar()->addMenu(file);
    QAction *open = new QAction("&Open", this);
    file->addAction(open);

    connect(open, SIGNAL(triggered()), this, SLOT(LoadImage()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::LoadImage()
{
    auto path = QFileDialog::getOpenFileName(this, "Open Dialog", "", "*.png *.jpg");
    if (path.isEmpty())
        return;
    pix = new QPixmap(path);
    ui->label_image->setPixmap(*pix);
    ui->scrollArea_image->setWidget(ui->label_image);
}
