
#include "food.h"


Food::Food( const bool& can_move )
{
    this->movable = can_move;
    this->image = new QGraphicsPixmapItem( (can_move) ? this->img_rat : this->img_egg );
    this->x = 0;
    this->y = 0;
}


const unsigned int& Food::X()
{
    return this->x;
}
const unsigned int& Food::Y()
{
    return this->y;
}


QGraphicsPixmapItem* Food::getImageItem()
{
    return this->image;
}


const bool Food::inTile(  const unsigned int& x, const unsigned int& y  )
{
    if ( this->x == x && this->y == y ) {
        return true;
    } else {
        return false;
    }
}


void Food::update( const unsigned int& new_x, const unsigned int& new_y ) {
    this->x = new_x;
    this->y = new_y;
    this->image->setOffset( 16+(new_x*32), 16+(new_y*32) );
}


void Food::spawn( Snake& snake, Snake& snake_ )
{
    // pick a new random position
    unsigned int x, y;
    while (true) {
        x = rand() % 16;
        y = rand() % 16;
        // check it's actually inside the field
        if ( x < 16 && y < 16 ) {
            // check the tile is empty
            if ( x != this->x && y != this->y ) {
                if ( !(snake.inTile( x, y, false ) || snake_.inTile( x, y, false )) ) {
                    break;
                }
            }
        }
    }

    // update to new position
    this->update( x, y );

    // randomly rotate the image
    int rand_ = rand()%4;
    switch (rand_) {
        case 1:
            this->image->setPixmap(
                this->image->pixmap().transformed(
                    QTransform().rotate( 90.0 ) ) );
            break;
        case 2:
            this->image->setPixmap(
                this->image->pixmap().transformed(
                    QTransform().rotate( 180.0 ) ) );
            break;
        case 3:
            this->image->setPixmap(
                this->image->pixmap().transformed(
                    QTransform().rotate( -90.0 ) ) );
            break;
        default:
            // do not rotate
            break;
    }
}


void Food::move( Snake& snake )
{
    int move_up    = 0,
        move_down  = 0,
        move_left  = 0,
        move_right = 0;
    const unsigned int
        snake_x = snake.front().x,
        snake_y = snake.front().y;


    // check the field's limits
    if ( this->y == 0 ) {
        move_up -= 100;
    } else if ( this->y == 15 ) {
        move_down -= 100;
    }
    if ( this->x == 0 ) {
        move_left -= 100;
    } else if ( this->x == 15 ) {
        move_right -= 100;
    }

    // check the snake
    if ( ! snake.inTile( this->x, this->y-1, false ) ) {
        move_up += 100;
    } else {
        move_up -= 100;
    }
    if ( ! snake.inTile( this->x, this->y+1, false ) ) {
        move_down += 100;
    } else {
        move_down -= 100;
    }
    if ( ! snake.inTile( this->x-1, this->y, false ) ) {
        move_left += 100;
    } else {
        move_left -= 100;
    }
    if ( ! snake.inTile( this->x+1, this->y, false ) ) {
        move_right += 100;
    } else {
        move_right -= 100;
    }

    // check the snake position
    if ( this->x == snake_x ) {
        move_left += 30;
        move_right += 30;
        if ( this->y < snake_y ) {
            move_up += 30;
        } else if ( this->y > snake_y ) {
            move_down += 30;
        }
    } else if ( this->x < snake_x ) {
        move_left += 30;
        if ( this->y < snake_y ) {
            move_up += 30;
        } else if ( this->y > snake_y ) {
            move_down += 30;
        } else if ( this->y == snake_y ) {
            move_up += 30;
            move_down += 30;
        }
    } else if ( this->x > snake_x ) {
        move_right += 30;
        if ( this->y < snake_y ) {
            move_up += 30;
        } else if ( this->y > snake_y ) {
            move_down += 30;
        } else if ( this->y == snake_y ) {
            move_up += 30;
            move_down += 30;
        }
    }

    // decide
    int max = -1000;
    Direction choice;
    if ( move_up > max || (move_up == max && rand()%2) ) {
        max = move_up;
        choice = Direction::UP;
    }
    if ( move_down > max || (move_down == max && rand()%2) ) {
        max = move_down;
        choice = Direction::DOWN;
    }
    if ( move_left > max || (move_left == max && rand()%2) ) {
        max = move_left;
        choice = Direction::LEFT;
    }
    if ( move_right > max || (move_right == max && rand()%2) ) {
        choice = Direction::RIGHT;
    }

    // apply the move
    switch ( choice ) {
        case Direction::UP:
            this->update( this->x, this->y-1 );
            this->image->setPixmap(
                this->img_rat );
            break;
        case Direction::DOWN:
            this->update( this->x, this->y+1 );
            this->image->setPixmap(
                this->img_rat.transformed(
                    QTransform().rotate( 180.0 ) ) );
            break;
        case Direction::LEFT:
            this->update( this->x-1, this->y );
            this->image->setPixmap(
                this->img_rat.transformed(
                    QTransform().rotate( -90.0 ) ) );
            break;
        case Direction::RIGHT:
            this->update( this->x+1, this->y );
            this->image->setPixmap(
                this->img_rat.transformed(
                    QTransform().rotate( 90.0 ) ) );
            break;
        default:
            // should be unreachable
            throw("Unexpected choice direction: "+std::to_string(choice));
    }
}
