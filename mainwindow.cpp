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
    connect(ui->comboBox_file, SIGNAL(activated(int)), this, SLOT(SelectFile(int)));
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

    diagFiles.insert(((*pix).width()*(*pix).height())/2.,path);

    QStringList parts = path.split("/");
    QString lastBit = parts.at(parts.size()-1);

    ui->comboBox_file->addItem(lastBit);

    ui->label_size->setText("Size: " + QString::number((*pix).width()) + "x" + QString::number((*pix).height()));

    ConstructImagePyramid();
}

void MainWindow::ConstructImagePyramid()
{
    int i = 0;

    int w = (*pix).width();
    int h = (*pix).height();

    ui->comboBox_layer->clear();
    while (w>=1 && h>=1)
    {
        w/=2;
        h/=2;

        ui->comboBox_layer->addItem(QString::number(i));
        i++;
    }
}

void MainWindow::SelectLayer(int index)
{
    QPixmap map=*pix;

    int new_w = static_cast<int>((*pix).width()/qPow(2, index));
    int new_h = static_cast<int>((*pix).height()/qPow(2, index));

    map=map.scaled(new_w, new_h);

    ui->label_image->setPixmap(map.scaled((*pix).width(), (*pix).height()));
    ui->scrollArea_image->setWidget(ui->label_image);

    ui->label_size->setText("Size: " + QString::number(new_w) + "x" + QString::number(new_h));
}

void MainWindow::SelectFile(int index)
{
   QMultiMap <double, QString>::const_iterator it = diagFiles.begin();

   it+=index;

   pix = new QPixmap(it.value());
   ui->label_image->setPixmap(*pix);
   ui->scrollArea_image->setWidget(ui->label_image);

   ConstructImagePyramid();

   ui->label_size->setText("Size: " + QString::number((*pix).width()) + "x" + QString::number((*pix).height()));
}
