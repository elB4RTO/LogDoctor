#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include "snake.h"
#include "food.h"

#include <queue>

#include <QTimer>
#include <QWidget>
#include <QKeyEvent>

#include <QGraphicsScene>


namespace Ui {
    class SnakeGame;
}

//! Snake
/*!
    A reproduction of the timeless classic game
*/
class SnakeGame : public QWidget
{
    Q_OBJECT

public:
    SnakeGame( const int& theme_id, const QFont& term_font, QWidget* parent=nullptr );
    ~SnakeGame();


private slots:

    // Override
    void closeEvent( QCloseEvent* event );

    // Starts the game
    void on_button_Play_clicked();

    //! Processes the logic of the game
    void processGameLogic();


private:
    Ui::SnakeGame *ui;

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


    //////////////////
    //// GRAPHICS ////

    QGraphicsScene* field_scene;

    QPixmap img_water = QPixmap(":/games/games/snake/water.png");


    //////////////
    //// GAME ////

    //! Enumerates the available game modes
    enum GameMode {
        Classic, //!< Classic snake game
        Hunt,    //!< Game variant in which the food moves too
        Battle   //!< Game variant in which you play against another snake
    };

    GameMode game_mode;

    bool playing = false;

    QTimer* game_loop = new QTimer();

    bool game_over = false;

    QString game_over_msg;


    ///////////////
    //// SNAKE ////

    //! The maximum length of the snake
    const unsigned int MAX_SNAKE_LENGTH = 64;

    //! The snake
    Snake snake;
    void newSnake();

    // The adversary snake
    Snake snake_ = Snake(true);
    void newSnake_();

    //! Checks if a snake will collide with another entity
    void checkCollision( Snake& snake, Snake& adv_snake, const bool& is_adv );


    //////////////
    //// FOOD ////

    //! Instance of the egg/rat which will be eat by the snake
    Food food;
    void newFood( const bool& movable );

    bool spawn_food = false;

    unsigned int moving_countdown = 5;
    unsigned int moving_rate = 5;


    ///////////////
    //// SCORE ////

    int game_score = 0;

    //! Increases the player's score by one
    void increaseGameScore();

    //! Decreases the player's score by one
    void decreaseGameScore();

    void adjustLcdDigits();

};

#endif // SNAKE_GAME_H
