#ifndef LOGDOCTOR__GAMES__SNAKE__SNAKE_H
#define LOGDOCTOR__GAMES__SNAKE__SNAKE_H


#include <vector>
#include <array> // leave this here for clang

#include <QPixmap>

class QGraphicsPixmapItem;
class QGraphicsScene;


//! Enumerates the possible directions
enum Direction {
    UP,   //!< Up
    DOWN, //!< Down
    LEFT, //!< Left
    RIGHT //!< Right
};


//! Instance of a part of the body of the snake
struct BodyPart {
    unsigned x;                 //!< The position on the X-axis
    unsigned y;                 //!< The position on the Y-axis
    Direction direction;        //!< The current direction of the part
    Direction prev_direction;   //!< The previous direction of the part
    QGraphicsPixmapItem* image; //!< The image which graphically represents the part
    //! Updates the position and direction of the part
    void update( const unsigned new_x, const unsigned new_y, const Direction& new_direction );
};


class Snake : public std::vector<BodyPart>
{
public:
    explicit Snake( const bool is_adversary=false );

    const QPixmap& getHeadImage() const;

    //! Checks whether is there a part of the snake in the given position
    bool inTile( const unsigned x, const unsigned y, const bool avoid_tail=true ) const;

    //! Sets the new direction (of the head)
    void setDirection( const Direction new_direction );

    //! Returns the current direction (of the head)
    const Direction& direction() const;

    //! Updates the position and direction of the entire snake
    void update( QGraphicsScene* field_scene=nullptr, const bool dry=false, const bool is_borning=false );

    // Schedules to grow the snake on the next update
    void willGrow();

    // [AI] Chooses a new direction for the snake
    void move( const Snake& adv_snake, const unsigned food_x, const unsigned food_y );


private:

    const QPixmap img_snakeHead   { ":/games/games/snake/head.png"   };
    const QPixmap img_snakeTail   { ":/games/games/snake/tail.png"   };
    const QPixmap img_snakeBody   { ":/games/games/snake/body_s.png" };
    const QPixmap img_snakeCurve  { ":/games/games/snake/body_c.png" };

    const QPixmap img_snakeHead_  { ":/games/games/snake/head_.png"   };
    const QPixmap img_snakeTail_  { ":/games/games/snake/tail_.png"   };
    const QPixmap img_snakeBody_  { ":/games/games/snake/body_s_.png" };
    const QPixmap img_snakeCurve_ { ":/games/games/snake/body_c_.png" };

    Direction head_direction;

    bool will_grow{ false };

    //! Increases the length of the body of the snake of 1 part
    void grow( const bool is_borning );

    bool adversary;

    //// ADVERSARY ////

    const unsigned aggressiveness{ 10u - static_cast<unsigned>(rand()%9) };

    enum class Entity {
        N, // none
        S, // self
        A, // adversary
        F, // food
    };

    struct Tile {
        Entity entity;
        unsigned s_index;
    };

    std::array<std::array<Tile, 16>, 16> field_map;

    // [AI] Updates the map of the field
    void updateFieldMap( const Snake& adv_snake, const unsigned& food_x, const unsigned& food_y );

    // [AI] As inTile(), but works on the field_map and only checks the adersary
    bool inTileAdv( const unsigned x, const unsigned y ) const;

    // [AI] Checks whether is there a snake in the tile, without counting as much trailing BodyParts as the number of steps
    bool inTileMinusSteps( const unsigned x, const unsigned y, const unsigned steps ) const;

    // [AI] Checks which of the surrounding positions are blocked
    std::array<unsigned, 8> checkAround( const Direction& direction, const unsigned x, const unsigned y ) const;

    // [AI] Checks if a direction is a closed path and should be avoided
    unsigned isDeadHole( const unsigned start_x, const unsigned start_y, const Direction& start_direction ) const;

    // [AI] Collects data about the possible movements
    void collectData( std::array<float, 7>& data, const Direction& direction, const Snake& adv_snake, const unsigned food_x, const unsigned food_y ) const;

    // [AI] Processes the collected data to predict the best movement
    Direction predictDirection( const std::array<std::array<float, 7>, 4>& data, const std::array<float, 7>& weights, const std::array<Direction, 4>& classes ) const;
};


#endif // LOGDOCTOR__GAMES__SNAKE__SNAKE_H
