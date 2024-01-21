
#include "game.h"
#include "ui_crisscross.h"

#include "globals/global_configs.h"

#include "games/game_dialog.h"

#include "modules/stylesheets.h"

#include <QPushButton>


CrissCross::CrissCross( QWidget* parent )
    : QWidget{ parent }
    , ui{ new Ui::CrissCross }
{
    this->ui->setupUi(this);

    QString stylesheet;
    StyleSec::Games::CrissCross::getStyleSheet( stylesheet );
    this->setStyleSheet( stylesheet );

    // verify that one player is human and the other is not
    if ( !(p1_human^p2_human)  ) {
        throw("Players identities error: "+std::to_string(p1_human)+" - "+std::to_string(p2_human));
    }

    this->victory_sequence.reserve( 3 );

    this->board_buttons[0] = this->ui->button_NW;
    this->board_buttons[1] = this->ui->button_N;
    this->board_buttons[2] = this->ui->button_NE;
    this->board_buttons[3] = this->ui->button_W;
    this->board_buttons[4] = this->ui->button_C;
    this->board_buttons[5] = this->ui->button_E;
    this->board_buttons[6] = this->ui->button_SW;
    this->board_buttons[7] = this->ui->button_S;
    this->board_buttons[8] = this->ui->button_SE;

    if ( ! p1_human ) {
        // AI starts
        this->AI_playTurn();
    }

}


/////////////////////
//// BOARD TILES ////
void CrissCross::on_button_NW_clicked()
{
    if ( ! this->ui->button_NW->isFlat() ) {
        this->board[0] = this->p_turn;
        this->ui->button_NW->setIcon( this->icons[ this->p_turn-1 ] );
        this->ui->button_NW->setFlat( true );
        this->endTurn();
    }
}

void CrissCross::on_button_N_clicked()
{
    if ( ! this->ui->button_N->isFlat() ) {
        this->board[1] = this->p_turn;
        this->ui->button_N->setIcon( this->icons[ this->p_turn-1 ] );
        this->ui->button_N->setFlat( true );
        this->endTurn();
    }
}

void CrissCross::on_button_NE_clicked()
{
    if ( ! this->ui->button_NE->isFlat() ) {
        this->board[2] = this->p_turn;
        this->ui->button_NE->setIcon( this->icons[ this->p_turn-1 ] );
        this->ui->button_NE->setFlat( true );
        this->endTurn();
    }
}

void CrissCross::on_button_W_clicked()
{
    if ( ! this->ui->button_W->isFlat() ) {
        this->board[3] = this->p_turn;
        this->ui->button_W->setIcon( this->icons[ this->p_turn-1 ] );
        this->ui->button_W->setFlat( true );
        this->endTurn();
    }
}

void CrissCross::on_button_C_clicked()
{
    if ( ! this->ui->button_C->isFlat() ) {
        this->board[4] = this->p_turn;
        this->ui->button_C->setIcon( this->icons[ this->p_turn-1 ] );
        this->ui->button_C->setFlat( true );
        this->endTurn();
    }
}

void CrissCross::on_button_E_clicked()
{
    if ( ! this->ui->button_E->isFlat() ) {
        this->board[5] = this->p_turn;
        this->ui->button_E->setIcon( this->icons[ this->p_turn-1 ] );
        this->ui->button_E->setFlat( true );
        this->endTurn();
    }
}

void CrissCross::on_button_SW_clicked()
{
    if ( ! this->ui->button_SW->isFlat() ) {
        this->board[6] = this->p_turn;
        this->ui->button_SW->setIcon( this->icons[ this->p_turn-1 ] );
        this->ui->button_SW->setFlat( true );
        this->endTurn();
    }
}

void CrissCross::on_button_S_clicked()
{
    if ( ! this->ui->button_S->isFlat() ) {
        this->board[7] = this->p_turn;
        this->ui->button_S->setIcon( this->icons[ this->p_turn-1 ] );
        this->ui->button_S->setFlat( true );
        this->endTurn();
    }
}

void CrissCross::on_button_SE_clicked()
{
    if ( ! this->ui->button_SE->isFlat() ) {
        this->board[8] = this->p_turn;
        this->ui->button_SE->setIcon( this->icons[ this->p_turn-1 ] );
        this->ui->button_SE->setFlat( true );
        this->endTurn();
    }
}


//////////////////////
//// TURN RELATED ////
void CrissCross::endTurn()
{
    if ( this->checkVictory() ) {
        // a player won!
        this->victory();
    } else {
        // nobody won yet
        if ( this->gameDraw() ) {
            // game is draw
            this->draw();
        } else {
            // change turn and keep playing
            this->nextTurn();
        }
    }
}

void CrissCross::nextTurn()
{
    switch ( this->p_turn ) {
        case 1:
            this->p_turn += 1;
            if ( ! this->p2_human ) {
                this->AI_playTurn();
            }
            break;
        case 2:
            this->p_turn -= 1;
            if ( ! this->p1_human ) {
                this->AI_playTurn();
            }
            break;
        default:
            // wrong
            throw("Unexpected turn: "+std::to_string(this->p_turn));
            break;
    }
}


bool CrissCross::checkVictory()
{
    bool result{ false };
    unsigned streak;
    for ( const auto& sequence : this->sequences ) {
        streak = 0;
        for ( const auto& index : sequence ) {
            if ( this->board[ index ] == this->p_turn ) {
                ++streak;
                this->victory_sequence.push_back( index );
            } else {
                break;
            }
        }
        if ( streak == 3 ) {
            // victory
            result |= true;
            break;
        } else {
            this->victory_sequence.clear();
        }
    }
    return result;
}

void CrissCross::victory()
{
    // disable all buttons except the victory sequence ones
    bool disable{ true };
    for ( unsigned i=0; i<9; ++i ) {
        disable |= true;
        for ( const auto& j : this->victory_sequence ) {
            if ( i == j ) {
                disable &= false;
                break;
            } else if ( i < j ) {
                break;
            }
        }
        this->board_buttons[ i ]->setFlat( true );
        if ( disable ) {
            this->board_buttons[ i ]->setEnabled( false );
        }
    }

    // display a dialog
    QString title, message;
    if ( (this->p_turn == 1 && this->p1_human)
      || (this->p_turn == 2 && this->p2_human) ) {
        // user won
        title = CrissCross::tr("Victory");
        message = CrissCross::tr("You beated me!");
    } else {
        // AI won
        message = CrissCross::tr("This time you lost!");
    }
    GameDialog dialog(
        title,
        message,
        this
    );
    std::ignore = dialog.exec();
}


bool CrissCross::gameDraw() const
{
    bool result{ false };
    unsigned empty_tiles{ 9 };
    for ( const auto& tile : this->board ) {
        if ( tile > 0 ) {
            -- empty_tiles;
        }
    }
    if ( empty_tiles == 0 ) {
        // no movement left
        result |= true;
    }
    return result;
}

void CrissCross::draw()
{
    // disable all buttons
    for ( const auto& button : this->board_buttons ) {
        button->setEnabled( false );
    }

    // display a dialog
    GameDialog dialog(
        CrissCross::tr("Draw"),
        CrissCross::tr("Nice match"),
        this
    );
    std::ignore = dialog.exec();
}


////////////
//// AI ////
void CrissCross::AI_playTurn()
{
    this->AI_updateWeights();
    emit this->board_buttons[ this->AI_makeChoice() ]->clicked();
}

void CrissCross::AI_updateWeights()
{
    // reset the weights
    for ( size_t i{0ul}; i<9ul; ++i ) {
        this->board_weights[ i ] = 0;
    }
    // calculate the new weights
    unsigned win_streak, lose_streak;
    std::vector<size_t> empty_tiles (3);
    for ( const auto& sequence : this->sequences ) {
        // reset data
        win_streak = lose_streak = 0;
        empty_tiles.clear();
        // check the tiles in the sequence
        for ( const auto index : sequence ) {
            if ( this->board[ index ] == this->p_turn ) {
                ++ win_streak;
            } else if ( this->board[ index ] > 0 ) {
                ++ lose_streak;
            } else {
                empty_tiles.emplace_back( std::move(index) );
            }
        }
        // set the new weight for the empty tiles
        const unsigned new_weight = (win_streak>=lose_streak)
            ? (win_streak==2) ? win_streak+2 : win_streak+1
            : lose_streak+1;
        for ( const auto& index : empty_tiles ) {
            if ( new_weight > this->board_weights[ index ] ) {
                this->board_weights[ index ] = new_weight;
            }
        }
    }
}

unsigned CrissCross::AI_makeChoice() const
{
    // get a list of the heaviest tiles
    std::vector<unsigned> moves;
    unsigned max_weight{ 0 };
    unsigned index{ 0 };
    for ( const auto& weight : this->board_weights ) {
        if ( weight > max_weight ) {
            // heavier weight found
            max_weight = weight;
            moves.clear();
            moves.push_back( index );
        } else if ( weight == max_weight ) {
            // same weight
            moves.push_back( index );
        }/* else {
            // lighter weight
            ;
        }*/
        ++ index;
    }
    // decide the movement (or better, randomly pick one)
    unsigned next_move;
    if ( max_weight == 0 ) {
        // first turn
        next_move = rand() % 9;
    } else {
        next_move = moves[ rand() % moves.size() ];
    }
    return next_move;
}
