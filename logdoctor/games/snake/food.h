#ifndef FOOD_H
#define FOOD_H

#include "snake.h"

#include <QGraphicsPixmapItem>

class Food
{
public:
    Food( const bool& can_move=false );

    //! Returns the position on the X-axis
    const unsigned int& X();
    //! Returns the position on the Y-axis
    const unsigned int& Y();

    //!< Returns the image
    QGraphicsPixmapItem* getImageItem();

    //! Checks whether is there a part of the snake in the given position
    const bool inTile( const unsigned int& x, const unsigned int& y );

    //! Spawns the egg/rat in a new position
    void spawn( Snake& snake, Snake& snake_ );

    //! Moves the rat
    void move( Snake& snake );

    //! Updates the position and direction of the entity
    void update( const unsigned int& new_x, const unsigned int& new_y );


private:

    QPixmap img_egg = QPixmap(":/games/games/snake/egg.png");

    QPixmap img_rat = QPixmap(":/games/games/snake/rat.png");

    //!< Whether the food is a rat or an egg
    bool movable;

    //!< The position on the X-axis
    unsigned int x;

    //!< The position on the Y-axis
    unsigned int y;

    //!< The image which graphically represents the food
    QGraphicsPixmapItem* image;
};

#endif // FOOD_H
