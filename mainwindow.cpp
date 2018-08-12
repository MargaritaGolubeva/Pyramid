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
    connect(ui->comboBox_layer, SIGNAL(activated(int)), this, SLOT(SelectLayer(int)));
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

    ConstructImagePyramid();
}

void MainWindow::ConstructImagePyramid()
{
    int i = 0;

    int w = (*pix).width();
    int h = (*pix).height();

    imageSizes.clear();
    imageSizes.resize(2);

    ui->comboBox_layer->clear();
    while (w>=1 && h>=1)
    {
        imageSizes[0].push_back(w);
        imageSizes[1].push_back(h);
        w/=2;
        h/=2;

        ui->comboBox_layer->addItem(QString::number(i));
        i++;
    }
}

void MainWindow::SelectLayer(int index)
{
    QPixmap map=*pix;
    map=map.scaled(imageSizes[0][index], imageSizes[1][index]);
    ui->label_image->setPixmap(map.scaled(imageSizes[0][0], imageSizes[1][0]));
    ui->scrollArea_image->setWidget(ui->label_image);

    ui->label_size->setText("Size: " + QString::number(imageSizes[0][index]) + "x" + QString::number(imageSizes[1][index]));
}
