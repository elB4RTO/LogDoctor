#ifndef SNAKE_H
#define SNAKE_H

#include <queue>

#include <QTimer>
#include <QWidget>
#include <QKeyEvent>

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>


namespace Ui {
    class Snake;
}

//! Snake
/*!
    A reproduction of the timeless classic game
*/
class Snake : public QWidget
{
    Q_OBJECT

public:
    Snake( const int& theme_id, const QFont& term_font, QWidget* parent=nullptr );
    ~Snake();


private slots:

    //! Starts the game
    void on_button_Play_clicked();

    //! Processes the logic of the game
    void processGameLogic();


private:
    Ui::Snake *ui;

    ////////////////////
    //// KEY EVENTS ////

    //! Stores the valid key events in a queue
    /*!
        Valid keys are UP/DOWN/LEFT/RIGHT arrows
        and W/S/A/D letters
        \see key_events, processNextKeyEvent()
    */
    void keyPressEvent( QKeyEvent* event ) override;

    //! Stores the key events
    std::queue<unsigned short> key_events;

    //! Processes the key events in the queue
    /*!
        \see key_events, keyPressEvent()
    */
    void processNextKeyEvent();


    //////////////
    //// GAME ////

    bool playing = false;
    QTimer* game_loop = new QTimer();
    bool game_over = false;
    QString game_over_msg;


    //////////////////
    //// GRAPHICS ////

    QGraphicsScene* field_scene;

    QPixmap img_water = QPixmap(":/games/games/water.png");

    QPixmap img_food = QPixmap(":/games/games/food.png");

    QPixmap img_snakeHead  = QPixmap(":/games/games/head.png");
    QPixmap img_snakeTail  = QPixmap(":/games/games/tail.png");
    QPixmap img_snakeBody  = QPixmap(":/games/games/body_s.png");
    QPixmap img_snakeCurve = QPixmap(":/games/games/body_c.png");


    ///////////////
    //// SNAKE ////

    //! The maximum length of the snake
    const unsigned int MAX_SNAKE_LENGTH = 64;


    //! Enumerates the possible directions
    enum Direction {
        UP,   //!< Up
        DOWN, //!< Down
        LEFT, //!< Left
        RIGHT //!< Right
    };

    Direction head_direction;


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

    //! The snake
    std::vector<BodyPart> snake;

    //! Checks whether is there a part of the snake in the given position
    const bool snakeInTile( const unsigned int& x, const unsigned int& y );

    //! Increases the length of the body of the snake of 1 part
    void increaseSnakeBody( const bool& initial=false );

    //! Update the position and direction of the entire snake
    void updateSnakePosition( const bool& dry=false );

    //! Checks if the head will collide with another entity
    void checkCollision();


    //////////////
    //// FOOD ////

    //! Instance of the egg which will be eat by the snake
    struct Food {
        unsigned int x;             //!< The position on the X-axis
        unsigned int y;             //!< The position on the Y-axis
        QGraphicsPixmapItem* image; //!< The image which graphically represents the egg
        //! Updates the position and direction of the part
        void update( const unsigned int& new_x, const unsigned int& new_y ) {
            this->x = new_x;
            this->y = new_y;
            this->image->setOffset( 16+(new_x*32), 16+(new_y*32) );
        }
    };

    Food food;

    //! Spawns the egg in a new position
    void spawnFood();

    bool spawn_food = true;


    ///////////////
    //// SCORE ////

    int game_score = 0;

    //! Increases the player's score by one
    void increaseGameScore();

};

#endif // SNAKE_H
