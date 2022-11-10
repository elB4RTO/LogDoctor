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

class Snake : public QWidget
{
    Q_OBJECT

public:
    explicit Snake( const int& theme_id, const QFont& term_font, QWidget* parent=nullptr );
    ~Snake();

private slots:

    void on_button_Play_clicked();

    void processGameLogic();

private:
    Ui::Snake *ui;

    void keyPressEvent( QKeyEvent* event ) override;
    std::queue<unsigned short> key_events;
    void processNextKeyEvent();

    bool playing = false;
    QTimer* game_loop = new QTimer();
    bool game_over = false;
    QString game_over_msg;

    // graphics
    QGraphicsScene* field_scene;

    QPixmap img_water = QPixmap(":/games/games/water.png");

    QPixmap img_food = QPixmap(":/games/games/food.png");

    QPixmap img_snakeHead  = QPixmap(":/games/games/head.png");
    QPixmap img_snakeTail  = QPixmap(":/games/games/tail.png");
    QPixmap img_snakeBody  = QPixmap(":/games/games/body_s.png");
    QPixmap img_snakeCurve = QPixmap(":/games/games/body_c.png");


    // snake
    const unsigned int MAX_SNAKE_LENGTH = 64;

    enum Direction {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    Direction head_direction;

    struct BodyPart {
        unsigned int x;
        unsigned int y;
        Direction direction;
        Direction prev_direction;
        QGraphicsPixmapItem* image;
        void update( const unsigned int& new_x, const unsigned int& new_y, const Direction& new_direction ) {
            this->x = new_x;
            this->y = new_y;
            this->image->setOffset( 16+(new_x*32), 16+(new_y*32) );
            this->prev_direction = this->direction;
            this->direction = new_direction;
        }
    };

    std::vector<BodyPart> snake;

    const bool snakeInTile( const unsigned int& x, const unsigned int& y );

    void increaseSnakeBody( const bool& initial=false );

    void updateSnakePosition( const bool& dry=false );

    void checkCollision();


    // food
    struct Food {
        unsigned int x;
        unsigned int y;
        QGraphicsPixmapItem* image;
        void update( const unsigned int& new_x, const unsigned int& new_y ) {
            this->x = new_x;
            this->y = new_y;
            this->image->setOffset( 16+(new_x*32), 16+(new_y*32) );
        }
    };

    Food food;

    void spawnFood();
    bool spawn_food = true;


    // score
    int game_score = 0;

    void increaseGameScore();
};

#endif // SNAKE_H
