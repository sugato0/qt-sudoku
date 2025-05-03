#include <QApplication>
#include "SudokuGame.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    SudokuGame window;
    window.show();

    return app.exec();
}
