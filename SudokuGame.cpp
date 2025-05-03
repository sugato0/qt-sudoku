#include "SudokuGame.h"
#include <QGridLayout>
#include <QMessageBox>
#include <QIntValidator>

SudokuGame::SudokuGame(QWidget *parent)
    : QWidget(parent), elapsedSeconds(0), solved(false)
{
    // Initialize the fixed sudoku puzzle (0 means empty)
    initialPuzzle = {
        {5,3,0, 0,7,0, 0,0,0},
        {6,0,0, 1,9,5, 0,0,0},
        {0,9,8, 0,0,0, 0,6,0},
        {8,0,0, 0,6,0, 0,0,3},
        {4,0,0, 8,0,3, 0,0,1},
        {7,0,0, 0,2,0, 0,0,6},
        {0,6,0, 0,0,0, 2,8,0},
        {0,0,0, 4,1,9, 0,0,5},
        {0,0,0, 0,8,0, 0,7,9}
    };

    currentPuzzle = initialPuzzle;

    setupUI();
    startTimer();
}

void SudokuGame::setupUI()
{
    QGridLayout *grid = new QGridLayout;
    QFont font;
    font.setPointSize(16);
    font.setBold(false);

    for(int row = 0; row < 9; ++row)
    {
        for(int col = 0; col < 9; ++col)
        {
            QLineEdit *cell = new QLineEdit;
            cell->setFixedSize(40, 40);
            cell->setFont(font);
            cell->setMaxLength(1);
            cell->setAlignment(Qt::AlignCenter);
            cell->setFrame(false);

            // Style cells with thick border for 3x3 blocks
            QString style = "border: 1px solid gray;";
            if (col == 2 || col == 5)
                style += "border-right: 3px solid black;";
            if (col == 3 || col == 6)
                style += "border-left: 3px solid black;";
            if (row == 2 || row == 5)
                style += "border-bottom: 3px solid black;";
            if (row == 3 || row == 6)
                style += "border-top: 3px solid black;";

            if (initialPuzzle[row][col] != 0)
            {
                cell->setText(QString::number(initialPuzzle[row][col]));
                cell->setReadOnly(true);
                cell->setStyleSheet(style + "background-color: #d3d3d3; font-weight: bold; font-size: 16px; color: black;");
            }
            else
            {
                cell->setText("");
                cell->setReadOnly(false);
                cell->setStyleSheet(style + "background-color: white; font-size: 16px; color: blue;");
            }
            cell->setProperty("row", row);
            cell->setProperty("col", col);

            cell->setValidator(new QIntValidator(1,9,this));

            connect(cell, &QLineEdit::textChanged, this, &SudokuGame::cellChanged);

            grid->addWidget(cell, row, col);
            cells[row][col] = cell;
        }
    }

    timerLabel = new QLabel("Time: 00:00");
    timerLabel->setFont(QFont("Arial", 14));
    timerLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(grid);
    mainLayout->addWidget(timerLabel);

    setLayout(mainLayout);
    setWindowTitle("Sudoku Game");
    setFixedSize(40*9 + 60, 40*9 + 80);

    stopwatch = new QTimer(this);
    connect(stopwatch, &QTimer::timeout, this, &SudokuGame::updateTimer);
}

void SudokuGame::startTimer()
{
    elapsedSeconds = 0;
    timerLabel->setText("Time: 00:00");
    stopwatch->start(1000);
}

void SudokuGame::cellChanged()
{
    if (solved)
        return;

    QLineEdit *cell = qobject_cast<QLineEdit*>(sender());
    if (!cell)
        return;

    int row = cell->property("row").toInt();
    int col = cell->property("col").toInt();

    QString text = cell->text();
    if (text.isEmpty())
    {
        currentPuzzle[row][col] = 0;
    }
    else
    {
        int val = text.toInt();
        if (val < 1 || val > 9)
        {
            cell->setText("");
            return;
        }
        else
        {
            currentPuzzle[row][col] = val;
        }
    }

    if (checkSolved())
    {
        solved = true;
        stopwatch->stop();
        showCongratulations();
    }
}

void SudokuGame::updateTimer()
{
    elapsedSeconds++;
    int minutes = elapsedSeconds / 60;
    int seconds = elapsedSeconds % 60;
    timerLabel->setText(QString("Time: %1:%2")
                            .arg(minutes, 2, 10, QChar('0'))
                            .arg(seconds, 2, 10, QChar('0')));
}

bool SudokuGame::checkSolved()
{
    // Check rows
    for (int r = 0; r < 9; ++r)
    {
        QVector<bool> present(10, false);
        for(int c = 0; c < 9; ++c)
        {
            int val = currentPuzzle[r][c];
            if (val == 0) return false;
            if (present[val]) return false;
            present[val] = true;
        }
    }

    // Check columns
    for (int c = 0; c < 9; ++c)
    {
        QVector<bool> present(10, false);
        for(int r = 0; r < 9; ++r)
        {
            int val = currentPuzzle[r][c];
            if (val == 0) return false;
            if (present[val]) return false;
            present[val] = true;
        }
    }

    // Check 3x3 sub-boxes
    for (int blockRow = 0; blockRow < 3; ++blockRow)
    {
        for (int blockCol = 0; blockCol < 3; ++blockCol)
        {
            QVector<bool> present(10, false);
            for(int r = blockRow * 3; r < blockRow * 3 +3; ++r)
            {
                for(int c = blockCol * 3; c < blockCol * 3 +3; ++c)
                {
                    int val = currentPuzzle[r][c];
                    if (val == 0) return false;
                    if (present[val]) return false;
                    present[val] = true;
                }
            }
        }
    }
    return true;
}

void SudokuGame::showCongratulations()
{
    int minutes = elapsedSeconds / 60;
    int seconds = elapsedSeconds % 60;
    QString timeStr = QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Game Over");
    msgBox.setText("<h2><strong>CONGRATULATIONS!</strong></h2>" +
                   QString("<p>You finished the Sudoku in %1.</p>").arg(timeStr));

    QPushButton *btn = msgBox.addButton("Solve Again", QMessageBox::AcceptRole); // Change to QPushButton*
    msgBox.setDefaultButton(btn); // No need for casting

    msgBox.exec();

    resetGame();
}

void SudokuGame::resetGame()
{
    solved = false;
    elapsedSeconds = 0;
    timerLabel->setText("Time: 00:00");

    for (int r = 0; r < 9; ++r)
    {
        for (int c = 0; c < 9; ++c)
        {
            currentPuzzle[r][c] = initialPuzzle[r][c];
            QLineEdit *cell = cells[r][c];
            if (initialPuzzle[r][c] != 0)
            {
                cell->setText(QString::number(initialPuzzle[r][c]));
                cell->setReadOnly(true);
                cell->setStyleSheet("background-color: #d3d3d3; font-weight: bold; font-size: 16px; color: black;");
            }
            else
            {
                cell->setText("");
                cell->setReadOnly(false);
                cell->setStyleSheet("background-color: white; font-size: 16px; color: blue;");
            }
        }
    }
    stopwatch->start(1000);
}
