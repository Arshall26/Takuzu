#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGraphicsEffect>
#include <QInputDialog>
#include <QPropertyAnimation>
#include <QTimer>
#include "model.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->submitButton->setEnabled(false);
    connect(ui->newGame, SIGNAL(triggered()), this, SLOT(setNewGame()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(quitPressed()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(aboutPressed()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


QWidget *MainWindow::getGrid() const
{
    return ui->gridWidget;
}

QPushButton *MainWindow::getSubmitButton() const
{
    return ui->submitButton;
}


QToolButton *MainWindow::getUndoButton() const
{
    return ui->undoButton;
}

QLabel *MainWindow::getLabelUndo() const
{
    return ui->labelUndo;
}


void MainWindow::setNewGame()
{
    QString size;
    QString difficulty;
    if (setGame(difficulty, size) == QMessageBox::Yes) {
        ui->gridWidget->setRows(size.toInt());
        emit notifyMapChosen(difficulty, size.toInt());
        ui->submitButton->setEnabled(true);
        ui->gridWidget->setEnabled(true);
    }
}

void MainWindow::newGameChoosen()
{
    QString size;
    QString difficulty;
    if (setGame(difficulty, size) == QMessageBox::Yes) {
        ui->gridWidget->setRows(size.toInt());
        emit notifySizeMapPicked(difficulty, size.toInt());
    }
}

QMessageBox::StandardButton MainWindow::setGame(QString &difficulty, QString &size)
{
    bool ok;
    QStringList sizes;
    sizes << "6" << "8" << "10";
    size = QInputDialog::getItem(this, "Choose map size", "Size :", sizes, 0, false, &ok);
    if (ok) {
        QStringList levels;
        levels << "Easy" << "Hard";
        QString level = QInputDialog::getItem(this, "Choose difficulty level", "Level :", levels, 0, false, &ok);
        if(level == "Easy"){
            difficulty = "Easy";
        } else{
            difficulty ="Hard";
        }
        if (ok) {
            QMessageBox::StandardButton button;
            button = QMessageBox::question(this, "Choices",
                                           " "+size+"x"+size+" grid    ----    '"+level+"'.\n"
                                           "Are you sure ?",
                                           QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
            return button;
        }
    }
    return QMessageBox::NoButton;
}

void MainWindow::chooseMap(int nbMaps)
{
    bool ok;
    QStringList maps;
    for (int i = 1; i <= nbMaps; i++) {
        maps << QString::number(i);
    }
    QString mapPicked = QInputDialog::getItem(this, "Choose one grid", "Grid number :", maps, 0, false, &ok);
    if (ok && !mapPicked.isEmpty()) {
        ui->submitButton->setEnabled(true);
    }
}

void MainWindow::numberMap(QString difficulty, int sizeMap, int chosenMap, int nbMaps)
{
    ui->mapLabel->setText("Grid : " + QString::number(sizeMap)+ "x" + QString::number(sizeMap)
                           + " " + difficulty + " "
                           + QString::number(chosenMap+1) + "/" + QString::number(nbMaps));
}

void MainWindow::quitPressed()
{
    QMessageBox::StandardButton button;
    button = QMessageBox::question(this, "Quit",
                                   "Are you sure ?",
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (button == QMessageBox::Yes) {
        close();
    }
}


void MainWindow::aboutPressed()
{
    QMessageBox::information(this, "About",
                             "this application has been developped by El Kilali Hamza & Baccour Mohamed");
}

void MainWindow::endGame(bool winStatus)
{
    if (winStatus) {
        QMessageBox::information(this, "Victory !", "YOU ARE A WINNER !");
        ui->gridWidget->setEnabled(false);
    } else {
        ui->incorrectLabel->setText("The grid is incorrect :(");
    };
}



