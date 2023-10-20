#include "mainwindow.hh"

#include <QApplication>
/* Path game instructions:
this game involves moving the red tiles to where the green tiles are and vice versa.
the rules are as follows:
tiles can only move in horizontal and vertical paths, meaning red tiles cannot go over green tiles and vice versa.
Every move is counted in the move counter and the time taken to complete the game is displayed on the lcd timer between the reset and pause button.
The reset button can be used to revert the board to its original state this also resets the timer and move count.
The pause button can be used to pause the timer and stop any tile movements, incase the user tries to move a tile in pause mode an error message is displayed,
game can be unpaused by pressing the resume button (the text on the pause button changes to resume when game is in pause mode).
The color/theme of the tile can be changed to minion mode based off the very popular movie series, This mode can be reverted by clicking normal mode.
If the user finishes the game with the minimum amount of moves (31) the background is changed and the user is congratulated.
*/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
