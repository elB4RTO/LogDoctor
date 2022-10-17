
#include "snake.h"
#include "ui_snake.h"


Snake:: Snake(const QPalette& style, QWidget* parent ) :
    QWidget(parent),
    ui(new Ui::Snake)
{
    this->ui->setupUi(this);
    this->setPalette( style );

    // initialize an empty field
    for (int x=0; x<16; x++) {
        for (int y=0; y<16; y++) {
            this->field[x][y] = Tile::EMPTY;
        }
    }

    // snake initial position
    const unsigned int head_x = (rand()%4)+6;
    const unsigned int head_y = (rand()%4)+6;
    if ( (head_x < 0 || head_x > 15)
      || (head_y < 0 || head_y > 15) ) {
        // should be unreachable
        throw("Unexpected initial position: ("+std::to_string(head_x)+","+std::to_string(head_y)+")");
    }
    this->field[ head_x ][ head_y ] = Tile::SNAKE;

    // snake initial direction
    Direction head_d;
    const int rand_d = rand()%4;
    switch ( rand_d ) {
        case 0:
            head_d = Direction::UP;
            break;
        case 1:
            head_d = Direction::DOWN;
            break;
        case 2:
            head_d = Direction::LEFT;
            break;
        case 3:
            head_d = Direction::RIGHT;
            break;
        default:
            // should be unreachable
            throw("Unexpected initial direction: "+std::to_string(rand_d));
    }

    // build the body with a head
    this->snake_body.push_back(
        { head_x, head_y, head_d, head_d, true }
    );
    // and a tail
    this->increaseSnakeBody();
    // a snake is born
}

Snake::~Snake()
{
    delete ui;
}



void Snake::increaseSnakeBody()
{
    // build from the tail
    const BodyPart& tail = this->snake_body.back();
    unsigned int x = tail.x;
    unsigned int y = tail.y;
    Direction d  = tail.direction;
    Direction ld = tail.prev_direction;
    // set the new body-part position
    bool done = false;
    while (done) {
        switch ( d ) {
            case Direction::UP:
                y ++;
                if ( y >= 16 ) { y--; d=ld; } // out of field limit
                else { done=true; }
                break;
            case Direction::DOWN:
                y --;
                if ( y < 0 ) { y++; d=ld; }
                else { done=true; }
                break;
            case Direction::LEFT:
                x --;
                if ( x < 0 ) { x++; d=ld; }
                else { done=true; }
                break;
            case Direction::RIGHT:
                x ++;
                if ( x >= 16 ) { x--; d=ld; }
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
                directions[0]=Direction::DOWN;directions[1]=Direction::LEFT;directions[2]=Direction::RIGHT;
                break;
            case Direction::DOWN:
                directions[0]=Direction::UP;directions[1]=Direction::RIGHT;directions[2]=Direction::LEFT;
                break;
            case Direction::LEFT:
                directions[0]=Direction::RIGHT;directions[1]=Direction::DOWN;directions[2]=Direction::UP;
                break;
            case Direction::RIGHT:
                directions[0]=Direction::LEFT;directions[1]=Direction::UP;directions[2]=Direction::DOWN;
                break;
            default:
                // should be unreachable
                throw("Unexpected direction: "+std::to_string(d));
        }
        done = false;
        for ( const auto& dir : directions ) {
            switch ( dir ) {
                case Direction::UP:
                    y ++;
                    if ( y >= 16 || this->field[x][y] != EMPTY )
                         { y--; d=ld=dir; }
                    else { done=true; }
                    break;
                case Direction::DOWN:
                    y --;
                    if ( y < 0 || this->field[x][y] != EMPTY )
                         { y++; d=ld=dir; }
                    else { done=true; }
                    break;
                case Direction::LEFT:
                    x --;
                    if ( x < 0 || this->field[x][y] != EMPTY )
                         { x++; d=ld=dir; }
                    else { done=true; }
                    break;
                case Direction::RIGHT:
                    x ++;
                    if ( x >= 16 || this->field[x][y] != EMPTY )
                         { x--; d=ld=dir; }
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
        this->snake_body.push_back(
            { x, y, d, ld }
        );
    }/* else {
        // no suitable position found, can't grow
    }*/
}


void Snake::increaseScore()
{
    this->game_score ++;
    this->ui->lcd_Score->setDigitCount( std::to_string(this->game_score).size() );
    this->ui->lcd_Score->display( this->game_score );
    // snake grows
    if ( this->snake_body.size() < this->MAX_SNAKE_LENGTH ) {
        this->increaseSnakeBody();
    }
}
