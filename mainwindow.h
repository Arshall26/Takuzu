#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMainWindow>
#include <QToolButton>
#include <QLabel>
#include <QPushButton>
#include <model.h>
#include <QMessageBox>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QWidget *getGrid() const;
    QToolButton *getUndoButton() const;
    QLabel *getLabelUndo() const;
    QPushButton *getSubmitButton() const;

signals:
    void notifyMapChosen(QString level, int size);
    void notifySizeMapPicked(QString level, int size);
    void notifMapPicked(int mapPicked);

public slots:
    void setNewGame();
    void newGameChoosen();
    void chooseMap(int nbMaps);
    void quitPressed();
    void aboutPressed();
    void endGame(bool win);
    void numberMap(QString difficulty, int sizeMap, int chosenMap, int nbMaps);

private:
    Ui::MainWindow* ui;

    QMessageBox::StandardButton setGame(QString &difficulty, QString &size);
};
#endif // MAINWINDOW_H



