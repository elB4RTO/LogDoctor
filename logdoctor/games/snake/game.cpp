
#include "game.h"
#include "ui_snake.h"

#include "modules/stylesheets.h"

#include "games/game_dialog.h"

#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>


SnakeGame::SnakeGame( const QFont& term_font, QWidget* parent )
    : QWidget{ parent }
    , ui{ new Ui::SnakeGame }
{
    this->ui->setupUi(this);

    this->setStyleSheet( StyleSec::Games::Snake::getStyleSheet() );

    QFont font{ term_font };
    font.setPointSize( 64 );
    this->ui->button_Play->setFont( font );
    font.setPointSize( 12 );
    this->ui->box_GameMode->setFont( font );

    // create the field
    this->field_scene.reset( new QGraphicsScene( this ) );
    this->field_scene->setSceneRect( 0,0, 544,544 );
    this->field_scene->setBackgroundBrush( Qt::black );
    // put water limits
    this->field_scene->addItem( new QGraphicsPixmapItem( this->img_water ) );
    // add the scene to the view
    this->ui->view_Field->setScene( this->field_scene.get() );
}

void SnakeGame::closeEvent( QCloseEvent* event )
{
    Q_UNUSED(event)
    if (this->game_loop) {
        this->game_loop->stop();
    }
    this->playing &= false;
}


void SnakeGame::keyPressEvent( QKeyEvent* event ) noexcept
{
    // store the key pressed if needed
    if ( ! this->playing ) {
        return;
    }
    switch ( event->key() ) {
        case Qt::Key_Up:
        case Qt::Key_W:
            if ( this->key_events.empty() || this->key_events.back() != Direction::UP ) {
                this->key_events.push( Direction::UP );
            }
            break;
        case Qt::Key_Down:
        case Qt::Key_S:
            if ( this->key_events.empty() || this->key_events.back() != Direction::DOWN ) {
                this->key_events.push( Direction::DOWN );
            }
            break;
        case Qt::Key_Left:
        case Qt::Key_A:
            if ( this->key_events.empty() || this->key_events.back() != Direction::LEFT ) {
                this->key_events.push( Direction::LEFT );
            }
            break;
        case Qt::Key_Right:
        case Qt::Key_D:
            if ( this->key_events.empty() || this->key_events.back() != Direction::RIGHT ) {
                this->key_events.push( Direction::RIGHT );
            }
            break;
        default:
            return;
    }
}


//////////////
//// MENU ////
void SnakeGame::on_button_Play_clicked()
{
    // set-up the game
    this->newSnake();
    bool food_movable{ false };
    switch ( this->ui->box_GameMode->currentIndex() ) {
        case 0:
            this->game_mode = GameMode::Classic;
            break;
        case 1:
            this->game_mode = GameMode::Hunt;
            food_movable |= true;
            break;
        case 2:
            this->game_mode = GameMode::Battle;
            this->newSnake_();
            break;
        default:
            throw("Unexpected GameMode: "+std::to_string(this->ui->box_GameMode->currentIndex()));
            break;
    }
    this->newFood( food_movable );

    // switch to game board
    this->ui->stackedWidget_GameDisplay->setCurrentIndex( 1 );
    // start playing
    this->game_loop.reset( new QTimer(this) );
    connect( this->game_loop.get(), &QTimer::timeout, this, &SnakeGame::processGameLogic);
    this->game_loop->start(175);
    this->playing |= true;
}


void SnakeGame::newSnake()
{
    // snake initial position
    const unsigned head_x{ static_cast<unsigned>((rand()%4)+6) };
    const unsigned head_y{ static_cast<unsigned>((rand()%4)+6) };
    if ( head_x > 15u || head_y > 15u ) {
        // should be unreachable
        throw("Unexpected initial position: ("+std::to_string(head_x)+","+std::to_string(head_y)+")");
    }

    // snake initial direction
    const Direction rand_d{ static_cast<Direction>( rand()%4 ) };
    switch ( rand_d ) {
        case Direction::UP:
            [[fallthrough]];
        case Direction::DOWN:
            [[fallthrough]];
        case Direction::LEFT:
            [[fallthrough]];
        case Direction::RIGHT:
            this->snake.setDirection( rand_d );
            break;
        default:
            // should be unreachable
            throw("Unexpected initial direction: "+std::to_string(rand_d));
    }
    this->key_events.push( rand_d );

    // build the body with a head
    this->snake.emplace_back(
        head_x, head_y,
        this->snake.direction(), this->snake.direction(),
        new QGraphicsPixmapItem( this->snake.getHeadImage() )
    );
    this->field_scene->addItem( this->snake.front().image );
    this->snake.front().update( head_x, head_y, this->snake.direction() );
    // a body part
    this->snake.willGrow();
    this->snake.update( this->field_scene.get(), true, true );
    // and a tail
    this->snake.willGrow();
    this->snake.update( this->field_scene.get(), true, true );
}

void SnakeGame::newSnake_()
{
    // snake initial position
    unsigned head_x{ this->snake.front().x };
    unsigned head_y{ this->snake.front().y };

    // snake initial direction
    const unsigned rnd{ static_cast<unsigned>(rand()%2) };
    this->snake_.setDirection( this->snake.direction() );
    switch ( this->snake_.direction() ) {
        case Direction::UP:
        case Direction::DOWN:
            if ( rnd ) {
                ++ head_x;
            } else {
                -- head_x;
            }
            break;
        case Direction::LEFT:
        case Direction::RIGHT:
            if ( rnd ) {
                ++ head_y;
            } else {
                -- head_y;
            }
            break;
        default:
            // should be unreachable
            throw("Unexpected initial direction _: "+std::to_string(this->snake_.direction()));
    }

    // build the body with a head
    this->snake_.emplace_back(
        head_x, head_y,
        this->snake_.direction(), this->snake_.direction(),
        new QGraphicsPixmapItem( this->snake_.getHeadImage() )
    );
    this->field_scene->addItem( this->snake_.front().image );
    this->snake_.front().update( head_x, head_y, this->snake_.direction() );
    // a body part
    this->snake_.willGrow();
    this->snake_.update( this->field_scene.get(), true, true );
    // and a tail
    this->snake_.willGrow();
    this->snake_.update( this->field_scene.get(), true, true );
}

void SnakeGame::newFood( const bool movable ) noexcept
{
    // put some food on the field for it to eat
    this->food = Food( movable );
    this->food.spawn( this->snake, this->snake_ );
    this->field_scene->addItem( this->food.getImageItem() );
}


//////////////
//// GAME ////
void SnakeGame::processGameLogic()
{
    if ( game_over ) {
        this->game_loop->stop();
        this->playing &= false;
        GameDialog dialog(
            SnakeGame::tr("Game Over"),
            this->game_over_msg,
            this
        );
        std::ignore = dialog.exec();
    } else {


        if ( !this->key_events.empty() ) {
            this->processNextKeyEvent();
        }
        if ( this->game_mode == GameMode::Battle ) {
            this->snake_.move( this->snake, this->food.X(), this->food.Y() );
        }
        // check for a possible collision of the head
        this->checkCollision( this->snake, this->snake_, false );
        if ( this->game_mode == GameMode::Battle ) {
            this->checkCollision( this->snake_, this->snake, true );
        }
        // check for game over
        if ( ! this->game_over ) {
            // update snake position
            this->snake.update( this->field_scene.get() );
            if ( this->game_mode == GameMode::Battle ) {
                this->snake_.update( this->field_scene.get() );
            }
            if ( this->spawn_food ) {
                // updae the score and spawn food in a new position
                this->updateGameScore();
                this->food.spawn( this->snake, this->snake_ );
                this->spawn_food &= false;
                if ( this->game_mode == GameMode::Hunt ) {
                    this->moving_rate = static_cast<unsigned>( 6ul-((this->snake.size()/13ul)+1ul) );
                    this->moving_countdown = this->moving_rate;
                }
            } else if ( this->game_mode == GameMode::Hunt ) {
                -- this->moving_countdown;
                if ( this->moving_countdown == 0u ) {
                    this->moving_countdown = this->moving_rate;
                    this->food.move( this->snake );
                }
            }
        }
    }
}


void SnakeGame::processNextKeyEvent() noexcept
{
    // update direction if needed
    switch ( this->key_events.front() ) {
        case Direction::UP:
            if ( this->snake.direction() != Direction::DOWN ) {
                this->snake.setDirection( Direction::UP );
            }
            break;
        case Direction::DOWN:
            if ( this->snake.direction() != Direction::UP ) {
                this->snake.setDirection( Direction::DOWN );
            }
            break;
        case Direction::LEFT:
            if ( this->snake.direction() != Direction::RIGHT ) {
                this->snake.setDirection( Direction::LEFT );
            }
            break;
        case Direction::RIGHT:
            if ( this->snake.direction() != Direction::LEFT ) {
                this->snake.setDirection( Direction::RIGHT );
            }
            break;
    }
    this->key_events.pop();
}


void SnakeGame::updateGameScore()
{
    this->game_score += this->score_step;
    this->ui->lcd_Score->setDigitCount( static_cast<int>(std::to_string(this->game_score).size()) );
    this->ui->lcd_Score->display( this->game_score );
}


void SnakeGame::checkCollision( Snake& snake, Snake& adv_snake, const bool is_adv )
{
    unsigned x{ snake.front().x };
    unsigned y{ snake.front().y };
    unsigned x_, y_;
    switch ( snake.direction() ) {
        case Direction::UP:
            --y;
            break;
        case Direction::DOWN:
            ++y;
            break;
        case Direction::LEFT:
            --x;
            break;
        case Direction::RIGHT:
            ++x;
            break;
        default:
            // should be unreachable
            throw("Unexpected direction: "+std::to_string(snake.direction()));
    }

    if ( adv_snake.size() > 0ul ) {
        x_ = adv_snake.front().x,
        y_ = adv_snake.front().y;
        switch ( adv_snake.direction() ) {
            case Direction::UP:
                --y_;
                break;
            case Direction::DOWN:
                ++y_;
                break;
            case Direction::LEFT:
                --x_;
                break;
            case Direction::RIGHT:
                ++x_;
                break;
            default:
                // should be unreachable
                throw("Unexpected direction: "+std::to_string(adv_snake.direction()));
        }
    } else {
        x_ = y_ = 16u;
    }

    // check the upcoming movement
    if ( x > 15u || y > 15u ) {
        // collision with the field limits
        this->game_over |= true;
        this->game_over_msg = (is_adv)
            ? SnakeGame::tr("Your adversary fell in the water!")+"\n\n"+SnakeGame::tr("YOU WON!")
            : SnakeGame::tr("You fell in the water!")+"\n\n"+SnakeGame::tr("YOU LOST!");

    } else if ( snake.inTile( x, y ) ) {
        // collision with another part of the snake
        this->game_over |= true;
        this->game_over_msg = (is_adv)
            ? SnakeGame::tr("Your adversary ate itself!")+"\n\n"+SnakeGame::tr("YOU WON!")
            : SnakeGame::tr("You ate yourself!")+"\n\n"+SnakeGame::tr("YOU LOST!");

    } else if ( adv_snake.inTile( x, y ) ) {
        // collision with another part of the snake
        if ( x_ != x || y_ != y ) {
            // not the head
            this->game_over |= true;
            this->game_over_msg = (is_adv)
                ? SnakeGame::tr("Your adversary ate you!")+"\n\n"+SnakeGame::tr("YOU WON!")
                : SnakeGame::tr("You ate your adversary!")+"\n\n"+SnakeGame::tr("YOU LOST!");
        } else {
            this->game_over |= true;
            this->game_over_msg = SnakeGame::tr("You ate each other!")+"\n\n"+SnakeGame::tr("MATCH IS DRAW!");
        }

    } else if ( this->food.inTile( x, y ) ) {
        // will eat
        if ( snake.size() < this->MAX_SNAKE_LENGTH ) {
            // below max size, will grow
            snake.willGrow();
        } else {
            // max size reached, increase speed
            const int interval{ this->game_loop->interval() };
            if ( interval > 50 ) {
                this->game_loop->setInterval( interval - 5 );
            }
        }
        if ( is_adv ) {
            this->score_step = -1;
        } else {
            this->score_step = 1;
        }
        this->spawn_food |= true;
    }
}
