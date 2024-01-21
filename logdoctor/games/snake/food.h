#ifndef LOGDOCTOR__GAMES__SNAKE__FOOD_H
#define LOGDOCTOR__GAMES__SNAKE__FOOD_H


#include <QPixmap>

class Snake;
class QGraphicsPixmapItem;


class Food final
{
public:
    explicit Food( const bool can_move=false );
    explicit Food( Food&& other ) noexcept;
    Food& operator=( Food&& other ) noexcept;
    Q_DISABLE_COPY(Food)

    //! Returns the position on the X-axis
    unsigned X() const noexcept;
    //! Returns the position on the Y-axis
    unsigned Y() const noexcept;

    //!< Returns the image
    QGraphicsPixmapItem* getImageItem() const noexcept;

    //! Checks whether is there a part of the snake in the given position
    bool inTile( const unsigned x, const unsigned y ) const noexcept;

    //! Spawns the egg/rat in a new position
    void spawn( const Snake& snake, const Snake& snake_ ) noexcept;

    //! Moves the rat
    void move( const Snake& snake );

    //! Updates the position and direction of the entity
    void update( const unsigned new_x, const unsigned new_y ) noexcept;


private:

    const QPixmap img_egg{ ":/games/games/snake/egg.png" };

    const QPixmap img_rat{ ":/games/games/snake/rat.png" };

    //!< The image which graphically represents the food
    QGraphicsPixmapItem* image = nullptr; // QGraphicsScene takes ownerhips of it

    //!< Whether the food is a rat or an egg
    bool movable;

    //!< The position on the X-axis
    unsigned x;

    //!< The position on the Y-axis
    unsigned y;
};


#endif // LOGDOCTOR__GAMES__SNAKE__FOOD_H
