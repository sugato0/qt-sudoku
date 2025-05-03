#ifndef SUDOKUGAME_H
#define SUDOKUGAME_H

#endif // SUDOKUGAME_H


#include <QWidget>
#include <QVector>
#include <QLineEdit>
#include <QLabel>
#include <QTimer>

class SudokuGame : public QWidget
{
    Q_OBJECT
public:
    explicit SudokuGame(QWidget *parent = nullptr);

private slots:
    void cellChanged();
    void updateTimer();
    void resetGame();

private:
    void setupUI();
    void startTimer();
    bool checkSolved();
    void showCongratulations();

    QVector<QVector<int>> initialPuzzle;
    QVector<QVector<int>> currentPuzzle;
    QLineEdit* cells[9][9];
    QLabel *timerLabel;
    QTimer *stopwatch;
    int elapsedSeconds;
    bool solved;
};

