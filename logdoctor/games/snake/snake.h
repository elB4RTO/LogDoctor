#ifndef SNAKE_H
#define SNAKE_H

#include <vector>

#include <QPixmap>
#include <QGraphicsPixmapItem>


//! Enumerates the possible directions
enum Direction {
    UP,   //!< Up
    DOWN, //!< Down
    LEFT, //!< Left
    RIGHT //!< Right
};


//! Instance of a part of the body of the snake
struct BodyPart {
    unsigned int x;             //!< The position on the X-axis
    unsigned int y;             //!< The position on the Y-axis
    Direction direction;        //!< The current direction of the part
    Direction prev_direction;   //!< The previous direction of the part
    QGraphicsPixmapItem* image; //!< The image which graphically represents the part
     //! Updates the position and direction of the part
    void update( const unsigned int& new_x, const unsigned int& new_y, const Direction& new_direction ) {
        this->x = new_x;
        this->y = new_y;
        this->image->setOffset( 16+(new_x*32), 16+(new_y*32) );
        this->prev_direction = this->direction;
        this->direction = new_direction;
    }
};


class Snake : public std::vector<BodyPart>
{
public:
    Snake( const bool& is_adversary=false );

    QPixmap& getHeadImage();

    //! Checks whether is there a part of the snake in the given position
    const bool inTile( const unsigned int& x, const unsigned int& y );

    //! Sets the new direction of the head
    void setDirection( const Direction new_direction );

    //! Returns the current direction
    const Direction& direction();

    //! Increases the length of the body of the snake of 1 part
    void grow( const bool& initial=false );

    //! Update the position and direction of the entire snake
    void update( const bool& dry=false );

    //! Chooses a new direction for the snake (only used by the AI)
    void move( Snake& snake, const unsigned int& food_x, const unsigned int& food_y );


private:

    QPixmap img_snakeHead  = QPixmap(":/games/games/snake/head.png");
    QPixmap img_snakeTail  = QPixmap(":/games/games/snake/tail.png");
    QPixmap img_snakeBody  = QPixmap(":/games/games/snake/body_s.png");
    QPixmap img_snakeCurve = QPixmap(":/games/games/snake/body_c.png");

    QPixmap img_snakeHead_  = QPixmap(":/games/games/snake/head_.png");
    QPixmap img_snakeTail_  = QPixmap(":/games/games/snake/tail_.png");
    QPixmap img_snakeBody_  = QPixmap(":/games/games/snake/body_s_.png");
    QPixmap img_snakeCurve_ = QPixmap(":/games/games/snake/body_c_.png");

    Direction head_direction;

    bool adversary;

    //// ADVERSARY ////

    const unsigned int aggressiveness = 10 - (rand()%9);
};

#endif // SNAKE_H
