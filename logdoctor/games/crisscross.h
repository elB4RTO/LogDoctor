#ifndef CRISSCROSS_H
#define CRISSCROSS_H

#include <QIcon>
#include <QWidget>
#include <QPushButton>


namespace Ui {
    class CrissCross;
}

class CrissCross : public QWidget
{
    Q_OBJECT

public:
    explicit CrissCross( const int& theme_id, QWidget* parent=nullptr );
    ~CrissCross();

private slots:
    void on_button_NW_clicked();

    void on_button_N_clicked();

    void on_button_NE_clicked();

    void on_button_W_clicked();

    void on_button_C_clicked();

    void on_button_E_clicked();

    void on_button_SW_clicked();

    void on_button_S_clicked();

    void on_button_SE_clicked();

private:
    Ui::CrissCross *ui;

    // player turn
    unsigned int p_turn = 1;

    // players identity
    const bool p1_human = rand() %2;
    const bool p2_human = (p1_human) ? false : true;

    // victory related
    std::vector<unsigned int> victory_sequence;

    // game data
    unsigned int board[9] = {
        0,0,0,
        0,0,0,
        0,0,0
    };

    QPushButton* board_buttons[9];

    const QIcon icons[2] = {
        QIcon(":/games/games/o.png"),
        QIcon(":/games/games/x.png")
    };

    const unsigned int sequences[8][3] = {
        {0,1,2},{3,4,5},{6,7,8}, // horizontal
        {0,3,6},{1,4,7},{2,5,8}, // vertical
        {0,4,8},{2,4,6}          // diagonal
    };

    // AI data
    unsigned int board_weights[9] = {
        1,1,1,
        1,1,1,
        1,1,1
    };


    // game methods
    void endTurn();
    void nextTurn();
    const bool isPlayerTurn();

    const bool checkVictory();
    const bool gameDraw();
    void victory();
    void draw();

    void AI_playTurn();
    void AI_updateWeights();
    const unsigned int AI_makeChoice();
};

#endif // CRISSCROSS_H
