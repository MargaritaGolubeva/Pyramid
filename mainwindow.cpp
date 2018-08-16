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

    QDoubleValidator* validator =  new QDoubleValidator(1.1, 10000.0, 1);
    validator->setLocale(QLocale::English);
    ui->lineEdit_ratio->setValidator(validator);

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

    QFileInfo file(path);
    QString format = file.completeSuffix();

    int init_w = (*pix).width();
    int init_h = (*pix).height();

    if (format=="png" || format=="jpg")
    {
        if (init_w>=1 && init_h>=1)
        {
            ui->label_image->setPixmap(*pix);
            ui->scrollArea_image->setWidget(ui->label_image);

            diagFiles.insert((init_w*init_h)/2.,path);

            ui->comboBox_file->clear();
            foreach (QString path, diagFiles)
            {
                QStringList parts = path.split("/");
                QString lastBit = parts.at(parts.size()-1);

                ui->comboBox_file->addItem(lastBit);
            }
            ui->label_size->setText("Size: " + QString::number(init_w) + "x" + QString::number(init_h));

            ratio = 2.;

            ui->label_ratio->setText(QString::number(ratio));

            ConstructImagePyramid();

            connect(ui->lineEdit_ratio, SIGNAL(returnPressed()), this, SLOT(ChangeRatio()));
        }
        else QMessageBox::warning(this, "Warning!", "Perhaps the file format is not supported, please, choose another file");
    }
    else QMessageBox::critical(this, "Attention!", "Invalid file format");
}

void MainWindow::ConstructImagePyramid()
{
    int i = 0;

    int w = (*pix).width();
    int h = (*pix).height();

    ui->comboBox_layer->clear();
    while (w>=1 && h>=1)
    {
        w/=ratio;
        h/=ratio;

        ui->comboBox_layer->addItem(QString::number(i));
        i++;
    }
}

void MainWindow::SelectLayer(int index)
{
    QPixmap map=*pix;

    int new_w = static_cast<int>((*pix).width()/qPow(ratio, index));
    int new_h = static_cast<int>((*pix).height()/qPow(ratio, index));

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

void MainWindow::ChangeRatio()
{
   ui->label_ratio->setText((ui->lineEdit_ratio->text()).replace(",","."));
   ratio = ui->label_ratio->text().toDouble();

   ConstructImagePyramid();
}
