#include "mainwindow.hh"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    coordinates = {};
    ui->setupUi(this);
    seconds = 0;
    paused = false;
    minion_mode = false;
    upper_tile_color = "background-color: green";
    bottom_tile_color = "background-color: red";
    empty_tile_color = "background-color: white";
    gameboard = GameBoard();
    init_board();
    init_reset();
    init_infoBoard();
    init_lcd();
    init_game_moves();
    init_pause();
    init_toggle_minionmode();
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(time_process()));
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handle_character_clicks()
{
    Point temp_coordinate;
    if(!paused)
    {
        for (int row=0;row<=4;++row)
        {
            for(int col=0;col<=3;++col)
            {
                if (grid_.at(row).at(col)==sender())
                {
                    temp_coordinate.x= col;
                    temp_coordinate.y = row;
                    coordinates.push_back(temp_coordinate);
                    if(coordinates.size()==2)
                    {
                        bool tile_moved = gameboard.move(coordinates.at(0),coordinates.at(1));
                        if(coordinates.at(0)==coordinates.at(1)) //so that colors are not changed unnecessarily
                        {
                            QString Error_message = "destination tile same as start";
                            info_board->setText(Error_message);

                        }
                        else if(tile_moved)
                        {
                            Slot_type first_color = gameboard.which_slot(coordinates.at(0));
                            Slot_type second_color = gameboard.which_slot(coordinates.at(1));
                            if((second_color==RED)and(first_color==EMPTY))
                            {
                                grid_.at(coordinates.at(0).y).at(coordinates.at(0).x)->setStyleSheet(empty_tile_color);
                                grid_.at(coordinates.at(1).y).at(coordinates.at(1).x)->setStyleSheet(bottom_tile_color);
                            }
                            else
                            {
                                grid_.at(coordinates.at(0).y).at(coordinates.at(0).x)->setStyleSheet(empty_tile_color);
                                grid_.at(coordinates.at(1).y).at(coordinates.at(1).x)->setStyleSheet(upper_tile_color);
                            }
                            info_board->setText("");
                            move_count = move_count+1;
                            moves->setText(QString::number(move_count));
                            if(gameboard.is_game_over())
                            {
                                if (move_count<=31)
                                {
                                    QTextBrowser* min_points_bg_ = new QTextBrowser(this);
                                    min_points_bg_->setText("Congrats you won with the minimum moves possible");
                                }
                                info_board->setText("Congratulations you win");
                                timer->stop();
                            }

                        }
                        else
                        {
                            QString error_message = "tiles can only move vertically and horizontally over empty/clear tiles";
                            info_board->setText(error_message);
                        }
                        coordinates.clear(); //store only 2 coordinates at a time
                    }
                    return;


                }
            }
        }
    }
    else
    {
        QString error_message = "game is paused...click on resume to resume again";
        info_board->setText(error_message);
        return;
    }
}

void MainWindow::handle_reset_clicks()
{
    //reset gameboard logically to reset it on the gui level
    gameboard = GameBoard();
    init_board();
    move_count = 0;
    moves->setText(QString::number(move_count));
    seconds = 0;
    game_time->display(seconds);

}

void MainWindow::time_process()
{
    seconds = seconds + 1;
    game_time->display(seconds);
}

void MainWindow::handle_pause_clicks()
{
    if(!paused)
    {
        paused = true;
        pause_button->setText("resume");
        timer->stop();
    }
    else
    {
        paused = false;
        pause_button->setText("pause");
        timer->start(1000);
    }
}

void MainWindow::handle_minionmode_clicks()
{
    if(!minion_mode)
    {
        minion_mode = true;
        upper_tile_color = "background-color: black";
        bottom_tile_color = "background-color: deepskyblue";
        empty_tile_color = "background-color: yellow";
        toggle_minionmode->setText("Normal Mode");
    }
    else
    {
        minion_mode = false;
        upper_tile_color = "background-color: green";
        bottom_tile_color = "background-color: red";
        empty_tile_color = "background-color: white";
        toggle_minionmode->setText("Minion Mode");

    }
    for(int row=0;row<=4;++row)
    {
        for(int col=0;col<=3;++col)
        {
            Point temp_point;
            temp_point.x = col;
            temp_point.y = row;
            Slot_type temp_color = gameboard.which_slot(temp_point);
            if (temp_color==GREEN)
            {
                grid_.at(row).at(col)->setStyleSheet(upper_tile_color);
            }
            else if (temp_color==RED)
            {
                grid_.at(row).at(col)->setStyleSheet(bottom_tile_color);
            }
            else if (temp_color==EMPTY)
            {
                grid_.at(row).at(col)->setStyleSheet(empty_tile_color);
            }
        }
    }
}
void MainWindow::init_board()
{
    for(int row=0;row<=4;++row)
    {
        std::vector<QPushButton*> temp_row = {};
        for(int col=0;col<=3;++col)
        {
            QPushButton* tile = new QPushButton(this);
            tile->setGeometry(20+col*70,20+row*70,60,60);
            temp_row.push_back(tile);
            connect(tile,&QPushButton::clicked,this,&MainWindow::handle_character_clicks);
        }
        grid_.push_back(temp_row);

    }
    for (int row=0;row<=4;++row)
    {
        for(int col=0;col<=3;++col)
        {
            if (row==0)
            {
                grid_.at(row).at(col)->setStyleSheet(upper_tile_color);
            }
            else if(row==4)
            {
                grid_.at(row).at(col)->setStyleSheet(bottom_tile_color);
            }
            else if(((row==1) and (col==1))or((row==2) and (col==1))or((row==2) and (col==2))
                    or((row==3) and (col==1)))
            {
                grid_.at(row).at(col)->setStyleSheet(empty_tile_color);
            }
            else
            {
                QSizePolicy sp_retain = grid_.at(row).at(col)->sizePolicy();
                sp_retain.setRetainSizeWhenHidden(true);
                grid_.at(row).at(col)->setSizePolicy(sp_retain);
                grid_.at(row).at(col)->setDisabled(true);
                grid_.at(row).at(col)->hide();

            }

        }
    }


}
void MainWindow::init_reset()
{
    QPushButton* reset_button = new QPushButton(this);
    reset_button->setText("reset");
    int width = reset_button->width(); // We don't want to change width
    reset_button->setGeometry(20,
                               370,
                               width,
                               30);
    reset_button->setStyleSheet("color: green"); // Setting text color

    connect(reset_button, SIGNAL(clicked()),
            this, SLOT(handle_reset_clicks()));

}

void MainWindow::init_infoBoard()
{
    info_board = new QTextBrowser(this);
    info_board ->setGeometry(340, 20, 300, 380);
    info_board->setText(INSTRUCTIONS);
}

void MainWindow::init_lcd()
{
    game_time = new QLCDNumber(this);
    game_time->setGeometry(125,370,80,30);
    game_time->setStyleSheet("background-color: beige");
}

void MainWindow::init_game_moves()
{
    move_count = 0;
    QLabel* moves_info = new QLabel(this);
    moves = new QLabel(this);
    moves_info->setGeometry(20,410,200,30);
    moves_info->setText("Number of moves: ");
    moves->setGeometry(150,410,50,30);
    moves->setText(QString::number(move_count));
}

void MainWindow::init_pause()
{
    pause_button = new QPushButton(this);
    pause_button->setText("pause");
    int width = pause_button->width(); // We don't want to change width
    pause_button->setGeometry(220,
                               370,
                               width-20,
                               30);
    pause_button->setStyleSheet("color: green"); // Setting text color

    connect(pause_button, SIGNAL(clicked()),
            this, SLOT(handle_pause_clicks()));

}

void MainWindow::init_toggle_minionmode()
{
    toggle_minionmode = new QPushButton(this);
    toggle_minionmode->setText("Minion Mode");
    int width = pause_button->width(); // We don't want to change width
    toggle_minionmode->setGeometry(220,
                               410,
                               width+15,
                               30);
    toggle_minionmode->setStyleSheet("color: green"); // Setting text color

    connect(toggle_minionmode, SIGNAL(clicked()),
            this, SLOT(handle_minionmode_clicks()));
}


