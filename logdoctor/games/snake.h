#ifndef SNAKE_H
#define SNAKE_H

#include <QWidget>

namespace Ui {
    class Snake;
}

class Snake : public QWidget
{
    Q_OBJECT

public:
    explicit Snake( const QPalette& style, QWidget* parent=nullptr );
    ~Snake();

private:
    Ui::Snake *ui;

    // snake
    const unsigned int MAX_SNAKE_LENGTH = 64;

    enum Direction {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    struct BodyPart {
        unsigned int x;
        unsigned int y;
        Direction direction;
        Direction prev_direction;
        const bool head=false;
        const bool& isHead() {
            return this->head;
        }
        void updatePosition( unsigned int& new_x, unsigned int& new_y, Direction& new_direction ) {
            this->x = new_x;
            this->y = new_y;
            this->prev_direction = this->direction;
            this->direction = new_direction;
        }
    };

    std::vector<BodyPart> snake_body;

    void increaseSnakeBody();

    // field
    enum Tile{
        EMPTY,
        SNAKE,
        FOOD
    };

    Tile field[16][16];

    int game_score = 0;

    void increaseScore();
};

#endif // SNAKE_H
