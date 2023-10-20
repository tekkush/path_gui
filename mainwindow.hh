#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <gameboard.hh>
#include <vector>
#include <QPushButton>
#include <QDebug>
#include <QTextBrowser>
#include <QLCDNumber>
#include <QTimer>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void handle_character_clicks();
    void handle_reset_clicks();
    void time_process();
    void handle_pause_clicks();
    void handle_minionmode_clicks();

private:
    const QString INSTRUCTIONS = "Hello! Welcome to the path game, your goal is to get all the red tiles where the green tiles are and vice versa, Tiles can be moved by clicking the tile to be moved and its destination, Tiles can only be moved horizontally and vertically so red tiles cannot travel ove green ones and vice versa, You can reset the game if you wish to do so and Pause if you need to take a break, You can also switch to a fun minion mode color setting! Good Luck!";
    const Point null = {-1000,-1000};
    std::vector<std::vector<QPushButton*>> grid_;   
    GameBoard gameboard;
    int move_count;
    bool paused;
    bool minion_mode;
    QString upper_tile_color;
    QString bottom_tile_color;
    QString empty_tile_color;
    std::vector<Point> coordinates; //storing coordinates for each valid tile click
    QTextBrowser* info_board;
    QLCDNumber* game_time;
    QTimer* timer;
    QLabel* moves;
    QPushButton* pause_button;
    QPushButton* toggle_minionmode;
    int seconds;
    Ui::MainWindow *ui;
    void init_board();
    void init_reset();
    void init_infoBoard();
    void init_lcd();
    void init_game_moves();
    void init_pause();
    void init_toggle_minionmode(); //minions color style tiles
};

#endif // MAINWINDOW_HH
