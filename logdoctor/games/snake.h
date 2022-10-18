#ifndef SNAKE_H
#define SNAKE_H

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
    explicit Snake( const QPalette& style, const QFont& term_font, QWidget* parent=nullptr );
    ~Snake();

private slots:

    void on_button_Play_clicked();

    void processGameLogic();

private:
    Ui::Snake *ui;

    void keyPressEvent( QKeyEvent* event ) override;
    bool lock_movements = true;

    bool playing = false;
    QTimer* game_loop = new QTimer();
    bool game_over = false;
    QString game_over_msg;

    // graphics
    QGraphicsScene* field_scene;

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
        const bool head=false;
        /*bool tail=true;
        void tail2body () {
            this->tail = false;
        }*/
        void update( const unsigned int& new_x, const unsigned int& new_y, const Direction& new_direction ) {
            this->x = new_x;
            this->y = new_y;
            this->image->setOffset( new_x*32, new_y*32 );
            this->prev_direction = this->direction;
            this->direction = new_direction;
        }
    };

    std::vector<BodyPart> snake;

    void increaseSnakeBody();

    void updateSnakePosition( const bool& dry=false );

    void checkCollision( const unsigned int& x, const unsigned int& y );


    // food
    struct Food {
        unsigned int x;
        unsigned int y;
        QGraphicsPixmapItem* image;
        void update( const unsigned int& new_x, const unsigned int& new_y ) {
            this->x = new_x;
            this->y = new_y;
            this->image->setOffset( new_x*32, new_y*32 );
        }
    };

    Food food;

    void spawnFood();


    // field
    enum Tile{
        EMPTY,
        SNAKE,
        FOOD
    };

    Tile field[16][16];


    // score
    int game_score = 0;

    void increaseGameScore();
};

#endif // SNAKE_H
