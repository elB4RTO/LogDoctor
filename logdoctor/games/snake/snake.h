#ifndef SNAKE_H
#define SNAKE_H

#include <vector>

#include <QPixmap>
#include <QGraphicsScene>
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
    void update( const unsigned int new_x, const unsigned int new_y, const Direction& new_direction ) {
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
    explicit Snake( const bool is_adversary=false );

    const QPixmap& getHeadImage() const;

    //! Checks whether is there a part of the snake in the given position
    const bool inTile( const unsigned int x, const unsigned int y, const bool avoid_tail=true ) const;

    //! Sets the new direction (of the head)
    void setDirection( const Direction new_direction );

    //! Returns the current direction (of the head)
    const Direction& direction() const;

    //! Updates the position and direction of the entire snake
    void update( QGraphicsScene* field_scene=nullptr, const bool dry=false, const bool is_borning=false );

    // Schedules to grow the snake on the next update
    void willGrow();

    // [AI] Chooses a new direction for the snake
    void move( const Snake& adv_snake, const unsigned int& food_x, const unsigned int& food_y );


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

    bool will_grow;

    //! Increases the length of the body of the snake of 1 part
    void grow( const bool is_borning );

    bool adversary;

    //// ADVERSARY ////

    const unsigned int aggressiveness = 10 - (rand()%9);

    enum Entity {
        N, // none
        S, // self
        A, // adversary
        F, // food
    };

    struct Tile {
        Entity entity;
        unsigned int s_index;
    };

    std::vector<std::vector<Tile>> field_map;

    // [AI] Updates the map of the field
    void updateFieldMap( const Snake& adv_snake, const unsigned int& food_x, const unsigned int& food_y );

    // [AI] As inTile(), but works on the field_map and only checks the adersary
    const bool inTileAdv( const unsigned int x, const unsigned int y ) const;

    // [AI] Checks whether is there a snake in the tile, without counting as much trailing BodyParts as the number of steps
    const bool inTileMinusSteps( const unsigned int x, const unsigned int y, const unsigned int steps ) const;

    // [AI] Checks which of the surrounding positions are blocked
    const std::vector<unsigned int> checkAround( const Direction& direction, const unsigned int& x, const unsigned int& y ) const;

    // [AI] Checks if a direction is a closed path and should be avoided
    const unsigned int isDeadHole( const unsigned int& start_x, const unsigned int& start_y, Direction start_direction ) const;

    // [AI] Collects data about the possible movements
    void collectData( std::vector<float>& data, Direction& direction, const Snake& adv_snake, const unsigned int& food_x, const unsigned int& food_y ) const;

    // [AI] Processes the collected data to predict the best movement
    const Direction predictDirection( const std::vector<std::vector<float>>& data, const std::vector<float>& weights, const std::vector<Direction>& classes ) const;
};

#endif // SNAKE_H
