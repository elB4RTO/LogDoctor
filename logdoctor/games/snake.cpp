
#include "snake.h"
#include "ui_snake.h"

#include <QMessageBox>
#include <QGraphicsView>
#include <QGraphicsScene>


Snake:: Snake(const QPalette& style, const QFont& term_font, QWidget* parent ) :
    QWidget(parent),
    ui(new Ui::Snake)
{
    this->ui->setupUi(this);

    this->setPalette( style );
    QFont font = QFont( term_font );
    font.setPointSize( 64 );
    this->ui->button_Play->setFont( font );

    this->field_scene = new QGraphicsScene( this );
    this->field_scene->setSceneRect( 0,0, 512, 512 );
    this->field_scene->setBackgroundBrush( Qt::black );
    // add the scene to the view
    this->ui->view_Field->setScene( this->field_scene );

    // initialize an empty field
    for (int x=0; x<16; x++) {
        for (int y=0; y<16; y++) {
            this->field[x][y] = Tile::EMPTY;
        }
    }

    // snake initial position
    const unsigned int head_x = (rand()%4)+6;
    const unsigned int head_y = (rand()%4)+6;
    if ( head_x > 15 || head_y > 15 ) {
        // should be unreachable
        throw("Unexpected initial position: ("+std::to_string(head_x)+","+std::to_string(head_y)+")");
    }
    this->field[ head_x ][ head_y ] = Tile::SNAKE;

    // snake initial direction
    const int rand_d = rand()%4;
    switch ( rand_d ) {
        case 0:
            this->head_direction = Direction::UP;
            break;
        case 1:
            this->head_direction = Direction::DOWN;
            break;
        case 2:
            this->head_direction = Direction::LEFT;
            break;
        case 3:
            this->head_direction = Direction::RIGHT;
            break;
        default:
            // should be unreachable
            throw("Unexpected initial direction: "+std::to_string(rand_d));
    }

    // build the body with a head
    this->snake.push_back(
        { head_x, head_y,
          this->head_direction, this->head_direction,
          new QGraphicsPixmapItem( this->img_snakeHead ),
          true }
    );
    this->field_scene->addItem( this->snake.front().image );
    this->snake.front().update( head_x, head_y, this->head_direction );
    // a body part
    this->increaseSnakeBody();
    // and a tail
    this->increaseSnakeBody();
    // et voila! a snake is born

    // now put some food on the field for it to eat
    this->food = Food{ 0, 0, new QGraphicsPixmapItem( this->img_food ) };
    this->field_scene->addItem( this->food.image );
    this->spawnFood();
}

Snake::~Snake()
{
    delete this->ui;
    delete this->field_scene;
    delete this->game_loop;
}


void Snake::keyPressEvent( QKeyEvent* event )
{
    // move only if not moved already
    if ( ! this->lock_movements ) {
        // get the key pressed
        const int& key = event->key();
        // update direction if needed
        switch (key) {
            case Qt::Key_Up:
            case Qt::Key_W:
                if ( this->head_direction != Direction::DOWN ) {
                    this->head_direction = Direction::UP;
                    this->lock_movements = true;
                }
                break;
            case Qt::Key_Down:
            case Qt::Key_S:
                if ( this->head_direction != Direction::UP ) {
                    this->head_direction = Direction::DOWN;
                    this->lock_movements = true;
                }
                break;
            case Qt::Key_Left:
            case Qt::Key_A:
                if ( this->head_direction != Direction::RIGHT ) {
                    this->head_direction = Direction::LEFT;
                    this->lock_movements = true;
                }
                break;
            case Qt::Key_Right:
            case Qt::Key_D:
                if ( this->head_direction != Direction::LEFT ) {
                    this->head_direction = Direction::RIGHT;
                    this->lock_movements = true;
                }
                break;
        }
    }
}


//////////////
//// MENU ////
void Snake::on_button_Play_clicked()
{
    // switch to game board
    this->ui->stackedWidget_GameDisplay->setCurrentIndex( 1 );
    // start playing
    this->game_loop = new QTimer(this);
    connect(this->game_loop, SIGNAL(timeout()), this, SLOT(processGameLogic()));
    this->game_loop->start(200);
    this->lock_movements = false;
    this->playing = true;
}


//////////////
//// GAME ////
void Snake::processGameLogic()
{
    this->updateSnakePosition();
    if ( game_over ) {
        this->lock_movements = true;
        this->game_loop->stop();
        this->playing = false;
        QMessageBox::about(
            this,
            Snake::tr("Game Over"),
            this->game_over_msg );
    }/* else {
        //this->update();
        //this->repaint();
    }*/
    this->lock_movements = false;
}


void Snake::increaseGameScore()
{
    this->game_score ++;
    this->ui->lcd_Score->setDigitCount( std::to_string(this->game_score).size() );
    this->ui->lcd_Score->display( this->game_score );
}


void Snake::spawnFood()
{
    // pick a new random position
    unsigned int x, y;
    while (true) {
        x = rand() % 16;
        y = rand() % 16;
        // check it's actually inside the field
        if ( x < 16 && y < 16 ) {
            // check the tile is empty
            if ( this->field[x][y] == Tile::EMPTY ) {
                break;
            }
        }
    }

    // remove the old food from the field
    this->field[this->food.x][this->food.y] = Tile::EMPTY;
    // add the new food to the field and update the position
    this->field[x][y] = Tile::FOOD;
    this->food.update( x, y );

    // randomly rotate the image
    int rand_ = rand()%4;
    switch (rand_) {
        case 1:
            this->food.image->setPixmap(
                this->food.image->pixmap().transformed(
                    QTransform().rotate( 90.0 ) ) );
            break;
        case 2:
            this->food.image->setPixmap(
                this->food.image->pixmap().transformed(
                    QTransform().rotate( 180.0 ) ) );
            break;
        case 3:
            this->food.image->setPixmap(
                this->food.image->pixmap().transformed(
                    QTransform().rotate( -90.0 ) ) );
            break;
        default:
            // do not rotate
            break;
    }
}


void Snake::updateSnakePosition( const bool& dry )
{
    size_t i = 0,
           max_i = this->snake.size()-1;
    unsigned int new_x, prev_x, new_y, prev_y;
    Direction new_direction, prev_direction, prev_body_d;
    for ( BodyPart& bp : this->snake ) {
        if ( ! dry ) {
            // future position
            if ( bp.head ) {
                // head doesn't follow any other part of the body
                switch ( this->head_direction ) {
                    case Direction::UP:
                        new_y = bp.y - 1;
                        new_x = bp.x;
                        break;
                    case Direction::DOWN:
                        new_y = bp.y + 1;
                        new_x = bp.x;
                        break;
                    case Direction::LEFT:
                        new_x = bp.x - 1;
                        new_y = bp.y;
                        break;
                    case Direction::RIGHT:
                        new_x = bp.x + 1;
                        new_y = bp.y;
                        break;
                    default:
                        // should be unreachable
                        throw("Unexpected direction: "+std::to_string(bp.direction));
                }
                new_direction = this->head_direction;
            } else {
                // follow the previous part of the body
                new_x = prev_x;
                new_y = prev_y;
                new_direction = prev_direction;
            }
            // update for the next part
            prev_x = bp.x;
            prev_y = bp.y;
            prev_direction = bp.direction;

            // if this is the head, check for a possible collision
            if ( bp.head ) {
                this->checkCollision( new_x, new_y );
            }

            // update the body-part position
            bp.update( new_x, new_y, new_direction );
            // and the field
            if ( i == 0 ) {
                // head
                this->field[ new_x ][ new_y ] = Tile::SNAKE;
            } else if ( i == max_i ) {
                // tail
                this->field[ prev_x ][ prev_y ] = Tile::EMPTY;
            }
        }

        // finally set the image to be shown
        switch ( bp.direction ) {

            case Direction::UP:
                if ( i == 0 ) {
                    bp.image->setPixmap(
                        this->img_snakeHead );
                } else if ( i == max_i ) {
                    switch ( prev_body_d ) {
                        case Direction::UP:
                            bp.image->setPixmap(
                                this->img_snakeTail );
                            break;
                        case Direction::LEFT:
                            bp.image->setPixmap(
                                this->img_snakeTail.transformed(
                                    QTransform().rotate( -90.0 ) ) );
                            break;
                        case Direction::RIGHT:
                            bp.image->setPixmap(
                                this->img_snakeTail.transformed(
                                    QTransform().rotate( 90.0 ) ) );
                            break;
                        default:
                            // should be unreachable
                            throw("Unexpected direction: "+std::to_string(prev_body_d));
                    }
                } else {
                    switch ( prev_body_d ) {
                        case Direction::UP:
                            bp.image->setPixmap(
                                this->img_snakeBody );
                            break;
                        case Direction::LEFT:
                            bp.image->setPixmap(
                                this->img_snakeCurve.transformed(
                                    QTransform().rotate( 90.0 ) ) );
                            break;
                        case Direction::RIGHT:
                            bp.image->setPixmap(
                                this->img_snakeCurve );
                            break;
                        default:
                            // should be unreachable
                            throw("Unexpected direction: "+std::to_string(prev_body_d));
                    }
                }
                break;

            case Direction::DOWN:
                if ( i == 0 ) {
                    bp.image->setPixmap(
                        this->img_snakeHead.transformed(
                            QTransform().rotate( 180.0 ) ) );
                } else if ( i == max_i ) {
                    switch ( prev_body_d ) {
                        case Direction::DOWN:
                            bp.image->setPixmap(
                                this->img_snakeTail.transformed(
                                    QTransform().rotate( 180.0 ) ) );
                            break;
                        case Direction::LEFT:
                            bp.image->setPixmap(
                                this->img_snakeTail.transformed(
                                    QTransform().rotate( -90.0 ) ) );
                            break;
                        case Direction::RIGHT:
                            bp.image->setPixmap(
                                this->img_snakeTail.transformed(
                                    QTransform().rotate( 90.0 ) ) );
                            break;
                        default:
                            // should be unreachable
                            throw("Unexpected direction: "+std::to_string(prev_body_d));
                    }
                } else {
                    switch ( prev_body_d ) {
                        case Direction::DOWN:
                            bp.image->setPixmap(
                                this->img_snakeBody );
                            break;
                        case Direction::LEFT:
                            bp.image->setPixmap(
                                this->img_snakeCurve.transformed(
                                    QTransform().rotate( 180.0 ) ) );
                            break;
                        case Direction::RIGHT:
                            bp.image->setPixmap(
                                this->img_snakeCurve.transformed(
                                    QTransform().rotate( -90.0 ) ) );
                            break;
                        default:
                            // should be unreachable
                            throw("Unexpected direction: "+std::to_string(prev_body_d));
                    }
                }
                break;

            case Direction::LEFT:
                if ( i == 0 ) {
                    bp.image->setPixmap(
                        this->img_snakeHead.transformed(
                            QTransform().rotate( -90.0 ) ) );
                } else if ( i == max_i ) {
                    switch ( prev_body_d ) {
                        case Direction::LEFT:
                            bp.image->setPixmap(
                                this->img_snakeTail.transformed(
                                    QTransform().rotate( -90.0 ) ) );
                            break;
                        case Direction::UP:
                            bp.image->setPixmap(
                                this->img_snakeTail );
                            break;
                        case Direction::DOWN:
                            bp.image->setPixmap(
                                this->img_snakeTail.transformed(
                                    QTransform().rotate( 180.0 ) ) );
                            break;
                        default:
                            // should be unreachable
                            throw("Unexpected direction: "+std::to_string(prev_body_d));
                    }
                } else {
                    switch ( prev_body_d ) {
                        case Direction::LEFT:
                            bp.image->setPixmap(
                                this->img_snakeBody.transformed(
                                    QTransform().rotate( -90.0 ) ) );
                            break;
                        case Direction::UP:
                            bp.image->setPixmap(
                                this->img_snakeCurve.transformed(
                                    QTransform().rotate( -90.0 ) ) );
                            break;
                        case Direction::DOWN:
                            bp.image->setPixmap(
                                this->img_snakeCurve );
                            break;
                        default:
                            // should be unreachable
                            throw("Unexpected direction: "+std::to_string(prev_body_d));
                    }
                }
                break;

            case Direction::RIGHT:
                if ( i == 0 ) {
                    bp.image->setPixmap(
                        this->img_snakeHead.transformed(
                            QTransform().rotate( 90.0 ) ) );
                } else if ( i == max_i ) {
                    switch ( prev_body_d ) {
                        case Direction::RIGHT:
                            bp.image->setPixmap(
                                this->img_snakeTail.transformed(
                                    QTransform().rotate( 90.0 ) ) );
                            break;
                        case Direction::UP:
                            bp.image->setPixmap(
                                this->img_snakeTail );
                            break;
                        case Direction::DOWN:
                            bp.image->setPixmap(
                                this->img_snakeTail.transformed(
                                    QTransform().rotate( 180.0 ) ) );
                            break;
                        default:
                            // should be unreachable
                            throw("Unexpected direction: "+std::to_string(prev_body_d));
                    }
                } else {
                    switch ( prev_body_d ) {
                        case Direction::RIGHT:
                            bp.image->setPixmap(
                                this->img_snakeBody.transformed(
                                    QTransform().rotate( 90.0 ) ) );
                            break;
                        case Direction::UP:
                            bp.image->setPixmap(
                                this->img_snakeCurve.transformed(
                                    QTransform().rotate( 180.0 ) ) );
                            break;
                        case Direction::DOWN:
                            bp.image->setPixmap(
                                this->img_snakeCurve.transformed(
                                    QTransform().rotate( 90.0 ) ) );
                            break;
                        default:
                            // should be unreachable
                            throw("Unexpected direction: "+std::to_string(prev_body_d));
                    }
                }
                break;

            default:
                // should be unreachable
                throw("Unexpected direction: "+std::to_string(bp.direction));
        }
        prev_body_d = bp.direction;
        i++;
    }
}


void Snake::checkCollision( const unsigned int& x, const unsigned int& y )
{
    // check the upcoming movement
    if ( x > 15 || y > 15 ) {
        // collision with the field limits
        this->game_over = true;
        this->game_over_msg = Snake::tr("You fell in the water!");

    } else if ( this->field[x][y] == Tile::SNAKE ) {
        // collision with another part of the snake
        this->game_over = true;
        this->game_over_msg = Snake::tr("You ate yourself!");

    } else if ( this->field[x][y] == Tile::FOOD ) {
        // will eat
        if ( this->snake.size() < this->MAX_SNAKE_LENGTH ) {
            // below max size, increase the size
            this->increaseSnakeBody();
        }
        // increase the score
        this->increaseGameScore();
        // spawn food in a new position
        this->spawnFood();
    }
}


void Snake::increaseSnakeBody()
{
    // build from the tail
    BodyPart& tail = this->snake.back();
    unsigned int x = tail.x;
    unsigned int y = tail.y;
    Direction d  = tail.prev_direction;
    Direction ld = tail.prev_direction;
    // set the new body-part position
    bool done = false;
    while (!done) {
        switch ( d ) {
            case Direction::UP:
                y ++;
                if ( y > 15 ) { y--; d=ld; } // out of field limit
                else { done=true; }
                break;
            case Direction::DOWN:
                y --;
                if ( y > 15 ) { y++; d=ld; }
                else { done=true; }
                break;
            case Direction::LEFT:
                x ++;
                if ( x > 15 ) { x--; d=ld; }
                else { done=true; }
                break;
            case Direction::RIGHT:
                x --;
                if ( x > 15 ) { x++; d=ld; }
                else { done=true; }
                break;
            default:
                // should be unreachable
                throw("Unexpected direction: "+std::to_string(d));
        }
    }

    // check if the tile is empty
    if ( this->field[x][y] != Tile::EMPTY ) {
        // not empty, pick a new position from around the tail
        x = tail.x;
        y = tail.y;
        d = ld = tail.direction;
        Direction directions[3];
        switch ( d ) {
            case Direction::UP:
                directions[0]=Direction::DOWN;
                directions[1]=Direction::LEFT;
                directions[2]=Direction::RIGHT;
                break;
            case Direction::DOWN:
                directions[0]=Direction::UP;
                directions[1]=Direction::RIGHT;
                directions[2]=Direction::LEFT;
                break;
            case Direction::LEFT:
                directions[0]=Direction::RIGHT;
                directions[1]=Direction::DOWN;
                directions[2]=Direction::UP;
                break;
            case Direction::RIGHT:
                directions[0]=Direction::LEFT;
                directions[1]=Direction::UP;
                directions[2]=Direction::DOWN;
                break;
            default:
                // should be unreachable
                throw("Unexpected direction: "+std::to_string(d));
        }
        done = false;
        for ( const auto& dir : directions ) {
            switch ( dir ) {
                case Direction::UP:
                    y ++; // inverse
                    if ( y > 15 || this->field[x][y] != EMPTY )
                         { y--; d=ld=dir; }
                    else { done=true; }
                    break;
                case Direction::DOWN:
                    y --;
                    if ( y > 15 || this->field[x][y] != EMPTY )
                         { y++; d=ld=dir; }
                    else { done=true; }
                    break;
                case Direction::LEFT:
                    x ++;
                    if ( x > 15 || this->field[x][y] != EMPTY )
                         { x--; d=ld=dir; }
                    else { done=true; }
                    break;
                case Direction::RIGHT:
                    x --;
                    if ( x > 15 || this->field[x][y] != EMPTY )
                         { x++; d=ld=dir; }
                    else { done=true; }
                    break;
                default:
                    // should be unreachable
                    throw("Unexpected direction: "+std::to_string(dir));
            }
        }
    }
    if ( done ) {
        // a valid position has been found, append a new body-part
        this->snake.push_back(
            { x, y,
              d, ld,
              new QGraphicsPixmapItem( this->img_snakeTail ) }
        );
        this->snake.back().update( x, y, d );
        this->field_scene->addItem( this->snake.back().image );
        this->updateSnakePosition( true );
    }/* else {
        // no suitable position found, can't grow
    }*/
}


