#ifndef LOGDOCTOR__GAMES__CRISSCROSS__GAME_H
#define LOGDOCTOR__GAMES__CRISSCROSS__GAME_H


#include <QIcon>
#include <QWidget>

class QPushButton;


namespace Ui {
    class CrissCross;
}


//! CrissCross
/*!
    Player vs AI criss-cross game
*/
class CrissCross final : public QWidget
{
    Q_OBJECT

public:
    explicit CrissCross( QWidget* parent=nullptr );


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
    QSharedPointer<Ui::CrissCross> ui;

    // player turn
    unsigned p_turn{ 1 };

    // players identity
    const bool p1_human{ static_cast<bool>( rand()%2 ) };
    const bool p2_human{ !p1_human };

    // victory related
    std::vector<std::size_t> victory_sequence;

    // game data
    unsigned board[9]{
        0,0,0,
        0,0,0,
        0,0,0
    };

    QPushButton* board_buttons[9];

    const QIcon icons[2]{
        QIcon(":/games/games/crisscross/o.png"),
        QIcon(":/games/games/crisscross/x.png")
    };

    const std::size_t sequences[8][3]{
        {0,1,2},{3,4,5},{6,7,8}, // horizontal
        {0,3,6},{1,4,7},{2,5,8}, // vertical
        {0,4,8},{2,4,6}          // diagonal
    };

    // AI data
    unsigned board_weights[9]{
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


    //! Checks whether somebody won or not
    bool checkVictory() noexcept;

    //! Checks whether the game is draw or not
    bool gameDraw() const noexcept;

    //! Someone won, process the victory
    void victory() noexcept;

    //! The match is over but nobody won, the game is draw
    void draw() noexcept;


    ////////////
    //// AI ////

    //! Main function for the AI to play its turn
    void AI_playTurn() noexcept;

    //! Updates the weights of the tiles
    /*!
        \see AI_playTurn();
    */
    void AI_updateWeights() noexcept;

    //! Makes the choice depending on the weights
    /*!
        \return The tile to select
        \see AI_playTurn();
    */
    unsigned AI_makeChoice() const noexcept;

};


#endif // LOGDOCTOR__GAMES__CRISSCROSS__GAME_H
