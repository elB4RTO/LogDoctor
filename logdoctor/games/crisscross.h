#ifndef CRISSCROSS_H
#define CRISSCROSS_H

#include <QIcon>
#include <QWidget>
#include <QPushButton>


namespace Ui {
    class CrissCross;
}

//! CrissCross
/*!
    Player vs AI criss-cross game
*/
class CrissCross : public QWidget
{
    Q_OBJECT

public:
    CrissCross( const int& theme_id, QWidget* parent=nullptr );
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


    //////////////
    //// GAME ////

    //! Ends the current turn
    void endTurn();

    //! Switches to the next turn
    void nextTurn();

    //! Checks whether it's the human player turn or not
    const bool isPlayerTurn();


    //! Checks whether somebody won or not
    const bool checkVictory();

    //! Checks whether the game is draw or not
    const bool gameDraw();

    //! Someone won, process the victory
    void victory();

    //! The match is over but nobody won, the game is draw
    void draw();


    ////////////
    //// AI ////

    //! Main function for the AI to play its turn
    void AI_playTurn();

    //! Updates the weights of the tiles
    /*!
        \see AI_playTurn();
    */
    void AI_updateWeights();

    //! Makes the choice depending on the weights
    /*!
        \return The tile to select
        \see AI_playTurn();
    */
    const unsigned int AI_makeChoice();

};

#endif // CRISSCROSS_H
