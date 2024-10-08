#ifndef LOGDOCTOR__GAMES__SNAKE__GAME_H
#define LOGDOCTOR__GAMES__SNAKE__GAME_H


#include "snake.h"
#include "food.h"

#include <queue>

#include <QWidget>

class QTimer;
class QKeyEvent;
class QGraphicsScene;


namespace Ui {
    class SnakeGame;
}


//! Snake
/*!
    A reproduction of the timeless classic game
*/
class SnakeGame final : public QWidget
{
    Q_OBJECT

public:
    explicit SnakeGame( const QFont& term_font, QWidget* parent=nullptr );


private slots:

    // Override
    void closeEvent( QCloseEvent* event ) override;

    // Starts the game
    void on_button_Play_clicked();

    //! Processes the logic of the game
    void processGameLogic();


private:
    QSharedPointer<Ui::SnakeGame> ui;

    ////////////////////
    //// KEY EVENTS ////

    //! Stores the valid key events in a queue
    /*!
        Valid keys are UP/DOWN/LEFT/RIGHT arrows
        and W/S/A/D letters
        \see key_events, processNextKeyEvent()
    */
    void keyPressEvent( QKeyEvent* event ) noexcept override;

    //! Stores the key events
    std::queue<Direction> key_events;

    //! Processes the key events in the queue
    /*!
        \see key_events, keyPressEvent()
    */
    void processNextKeyEvent() noexcept;


    //////////////////
    //// GRAPHICS ////

    QScopedPointer<QGraphicsScene> field_scene;

    const QPixmap img_water{ ":/games/games/snake/water.png" };


    //////////////
    //// GAME ////

    //! Enumerates the available game modes
    enum class GameMode {
        Classic, //!< Classic snake game
        Hunt,    //!< Game variant in which the food moves too
        Battle   //!< Game variant in which you play against another snake
    };

    GameMode game_mode;

    bool playing{ false };

    QScopedPointer<QTimer> game_loop;

    bool game_over{ false };

    QString game_over_msg;


    ///////////////
    //// SNAKE ////

    //! The maximum length of the snake
    const std::size_t MAX_SNAKE_LENGTH{ 64 };

    //! The snake
    Snake snake;
    void newSnake();

    // The adversary snake
    Snake snake_{ true };
    void newSnake_();

    //! Checks if a snake will collide with another entity
    void checkCollision( Snake& snake, Snake& adv_snake, const bool is_adv );


    //////////////
    //// FOOD ////

    //! Instance of the egg/rat which will be eat by the snake
    Food food;
    void newFood( const bool movable ) noexcept;

    bool spawn_food{ false };

    unsigned moving_countdown{ 5 };
    unsigned moving_rate{ 5 };


    ///////////////
    //// SCORE ////

    int game_score{ 0 };

    //! Increases/Decreases the player's score by one
    void updateGameScore();
    int score_step{ 1 };

};


#endif // LOGDOCTOR__GAMES__SNAKE__GAME_H
