
#include "food.h"

#include "snake.h"

#include <QGraphicsPixmapItem>


Food::Food( const bool can_move )
    : image{ new QGraphicsPixmapItem( (can_move) ? this->img_rat : this->img_egg ) }
    , movable{ can_move }
    , x{ 0 }
    , y{ 0 }
{
}
Food::Food( const Food& other )
    : image{ new QGraphicsPixmapItem( (other.movable) ? this->img_rat : this->img_egg ) }
    , movable{ other.movable }
    , x{ other.x }
    , y{ other.y }
{
}
const Food& Food::operator=( const Food& other )
{
    if ( this == &other ) return other;
    this->x = other.x;
    this->y = other.y;
    this->movable = other.movable;
    this->image = new QGraphicsPixmapItem( (this->movable) ? this->img_rat : this->img_egg );
    return *this;
}


unsigned Food::X() const
{
    return this->x;
}
unsigned Food::Y() const
{
    return this->y;
}


QGraphicsPixmapItem* Food::getImageItem() const
{
    return this->image;
}


bool Food::inTile(  const unsigned x, const unsigned y  ) const
{
    if ( this->x == x && this->y == y ) {
        return true;
    } else {
        return false;
    }
}


void Food::update( const unsigned new_x, const unsigned new_y )
{
    this->x = new_x;
    this->y = new_y;
    this->image->setOffset( 16+(new_x*32), 16+(new_y*32) );
}


void Food::spawn( const Snake& snake, const Snake& snake_ )
{
    // pick a new random position
    unsigned new_x, new_y;
    while (true) {
        new_x = static_cast<unsigned>( rand()%16 );
        new_y = static_cast<unsigned>( rand()%16 );
        // check it's actually inside the field
        if ( new_x < 16u && new_y < 16u ) {
            // check the tile is empty
            if ( new_x != this->x && new_y != this->y ) {
                if ( !(snake.inTile( new_x, new_y, false ) || snake_.inTile( new_x, new_y, false )) ) {
                    break;
                }
            }
        }
    }

    // update to new position
    this->update( new_x, new_y );

    // randomly rotate the image
    int rand_{ rand()%4 };
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


void Food::move( const Snake& snake )
{
    int move_up    { 0 },
        move_down  { 0 },
        move_left  { 0 },
        move_right { 0 };
    const unsigned
        snake_x{ snake.front().x },
        snake_y{ snake.front().y };


    // check the field's limits
    if ( this->y == 0u ) {
        move_up -= 100;
    } else if ( this->y == 15u ) {
        move_down -= 100;
    }
    if ( this->x == 0u ) {
        move_left -= 100;
    } else if ( this->x == 15u ) {
        move_right -= 100;
    }

    // check the snake
    if ( ! snake.inTile( this->x, this->y-1u, false ) ) {
        move_up += 100;
    } else {
        move_up -= 100;
    }
    if ( ! snake.inTile( this->x, this->y+1u, false ) ) {
        move_down += 100;
    } else {
        move_down -= 100;
    }
    if ( ! snake.inTile( this->x-1u, this->y, false ) ) {
        move_left += 100;
    } else {
        move_left -= 100;
    }
    if ( ! snake.inTile( this->x+1u, this->y, false ) ) {
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
        } else /*if ( this->y == snake_y )*/ {
            move_up += 30;
            move_down += 30;
        }
    } else /*if ( this->x > snake_x )*/ {
        move_right += 30;
        if ( this->y < snake_y ) {
            move_up += 30;
        } else if ( this->y > snake_y ) {
            move_down += 30;
        } else /*if ( this->y == snake_y )*/ {
            move_up += 30;
            move_down += 30;
        }
    }

    // decide
    int max{ -1000 };
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
            this->update( this->x, this->y-1u );
            this->image->setPixmap(
                this->img_rat );
            break;
        case Direction::DOWN:
            this->update( this->x, this->y+1u );
            this->image->setPixmap(
                this->img_rat.transformed(
                    QTransform().rotate( 180.0 ) ) );
            break;
        case Direction::LEFT:
            this->update( this->x-1u, this->y );
            this->image->setPixmap(
                this->img_rat.transformed(
                    QTransform().rotate( -90.0 ) ) );
            break;
        case Direction::RIGHT:
            this->update( this->x+1u, this->y );
            this->image->setPixmap(
                this->img_rat.transformed(
                    QTransform().rotate( 90.0 ) ) );
            break;
        default:
            // should be unreachable
            throw("Unexpected choice direction: "+std::to_string(choice));
    }
}
