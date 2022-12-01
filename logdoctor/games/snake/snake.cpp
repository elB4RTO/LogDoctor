
#include "snake.h"


Snake::Snake( const bool& is_adversary )
{
    this->will_grow = false;
    this->adversary = is_adversary;
    if ( is_adversary ) {
        for ( size_t x=0; x<16; x++ ) {
            this->field_map.push_back( std::vector<Tile>{} );
            std::vector<Tile>& v = this->field_map.back();
            for ( size_t y=0; y<16; y++ ) {
                v.push_back({ Entity::N, 0 });
            }
        }
    }
}


QPixmap& Snake::getHeadImage()
{
    if ( this->adversary ) {
        return this->img_snakeHead_;
    } else {
        return this->img_snakeHead;
    }
}


const bool Snake::inTile( const unsigned int& x, const unsigned int& y , const bool& avoid_tail )
{
    bool result = false;
    if ( this->size() > 0 ) {
        size_t i = 0,
               max = this->size()-1;
        if ( !avoid_tail ) {
            max ++;
        }
        for ( std::vector<BodyPart>::const_iterator bp = this->begin(); bp != this->end(); ++bp ) {
            if ( bp->x == x && bp->y == y ) {
                result = true;
                break;
            }
            i++;
            if ( i >= max ) {
                break;
            }
        }
    }
    return result;
}


void Snake::setDirection( const Direction new_direction )
{
    this->head_direction = new_direction;
}
const Direction& Snake::direction()
{
    return this->head_direction;
}

void Snake::willGrow()
{
    this->will_grow = true;
}
void Snake::grow( const bool& is_borning )
{
    // build from the tail
    const BodyPart& tail = this->back();
    unsigned int x = tail.x;
    unsigned int y = tail.y;
    const Direction d  = tail.direction;
    const Direction ld = tail.prev_direction;
    if ( is_borning ) {
        // one tile back
        switch ( d ) {
            case Direction::UP:
                y ++;
                break;
            case Direction::DOWN:
                y --;
                break;
            case Direction::LEFT:
                x ++;
                break;
            case Direction::RIGHT:
                x --;
                break;
            default:
                // should be unreachable
                throw("Unexpected direction: "+std::to_string(d));
        }
    }
    this->push_back(
        { x, y,
          d, ld,
          new QGraphicsPixmapItem( (this->adversary) ? this->img_snakeTail_ : this->img_snakeTail ) }
    );
    this->update( nullptr, true );
    this->back().update( x, y, d );
}


void Snake::update( QGraphicsScene* field_scene, const bool& dry , const bool& is_borning )
{
    // grow if planned
    if ( this->will_grow ) {
        this->will_grow = false;
        this->grow( is_borning );
        field_scene->addItem( this->back().image );
    }
    // anyway, update the whole body
    size_t i = 0,
           max_i = this->size()-1;
    unsigned int new_x, prev_x, new_y, prev_y;
    Direction new_direction, prev_direction, prev_body_d;
    QPixmap& head_img  = (this->adversary) ? this->img_snakeHead_  : this->img_snakeHead;
    QPixmap& body_img  = (this->adversary) ? this->img_snakeBody_  : this->img_snakeBody;
    QPixmap& curve_img = (this->adversary) ? this->img_snakeCurve_ : this->img_snakeCurve;
    QPixmap& tail_img  = (this->adversary) ? this->img_snakeTail_  : this->img_snakeTail;
    for ( std::vector<BodyPart>::iterator bp = this->begin(); bp != this->end(); ++bp ) {
        if ( ! dry ) {
            // future position
            if ( i == 0 ) {
                // head doesn't follow any other part of the body
                switch ( this->head_direction ) {
                    case Direction::UP:
                        new_y = bp->y - 1;
                        new_x = bp->x;
                        break;
                    case Direction::DOWN:
                        new_y = bp->y + 1;
                        new_x = bp->x;
                        break;
                    case Direction::LEFT:
                        new_x = bp->x - 1;
                        new_y = bp->y;
                        break;
                    case Direction::RIGHT:
                        new_x = bp->x + 1;
                        new_y = bp->y;
                        break;
                    default:
                        // should be unreachable
                        throw("Unexpected direction: "+std::to_string(this->head_direction));
                }
                new_direction = this->head_direction;
            } else {
                // follow the previous part of the body
                new_x = prev_x;
                new_y = prev_y;
                new_direction = prev_direction;
            }
            // store for the next part
            prev_x = bp->x;
            prev_y = bp->y;
            prev_direction = bp->direction;

            // update the body-part position
            bp->update( new_x, new_y, new_direction );
        }

        // finally set the image to be shown
        switch ( bp->direction ) {

            case Direction::UP:
                if ( i == 0 ) {
                    bp->image->setPixmap(
                        head_img );
                } else if ( i == max_i ) {
                    switch ( prev_body_d ) {
                        case Direction::UP:
                            bp->image->setPixmap(
                                tail_img );
                            break;
                        case Direction::LEFT:
                            bp->image->setPixmap(
                                tail_img.transformed(
                                    QTransform().rotate( -90.0 ) ) );
                            break;
                        case Direction::RIGHT:
                            bp->image->setPixmap(
                                tail_img.transformed(
                                    QTransform().rotate( 90.0 ) ) );
                            break;
                        default:
                            // should be unreachable
                            throw("Unexpected direction: "+std::to_string(prev_body_d));
                    }
                } else {
                    switch ( prev_body_d ) {
                        case Direction::UP:
                            bp->image->setPixmap(
                                body_img );
                            break;
                        case Direction::LEFT:
                            bp->image->setPixmap(
                                curve_img.transformed(
                                    QTransform().rotate( 90.0 ) ) );
                            break;
                        case Direction::RIGHT:
                            bp->image->setPixmap(
                                curve_img );
                            break;
                        default:
                            // should be unreachable
                            throw("Unexpected direction: "+std::to_string(prev_body_d));
                    }
                }
                break;

            case Direction::DOWN:
                if ( i == 0 ) {
                    bp->image->setPixmap(
                        head_img.transformed(
                            QTransform().rotate( 180.0 ) ) );
                } else if ( i == max_i ) {
                    switch ( prev_body_d ) {
                        case Direction::DOWN:
                            bp->image->setPixmap(
                                tail_img.transformed(
                                    QTransform().rotate( 180.0 ) ) );
                            break;
                        case Direction::LEFT:
                            bp->image->setPixmap(
                                tail_img.transformed(
                                    QTransform().rotate( -90.0 ) ) );
                            break;
                        case Direction::RIGHT:
                            bp->image->setPixmap(
                                tail_img.transformed(
                                    QTransform().rotate( 90.0 ) ) );
                            break;
                        default:
                            // should be unreachable
                            throw("Unexpected direction: "+std::to_string(prev_body_d));
                    }
                } else {
                    switch ( prev_body_d ) {
                        case Direction::DOWN:
                            bp->image->setPixmap(
                                body_img );
                            break;
                        case Direction::LEFT:
                            bp->image->setPixmap(
                                curve_img.transformed(
                                    QTransform().rotate( 180.0 ) ) );
                            break;
                        case Direction::RIGHT:
                            bp->image->setPixmap(
                                curve_img.transformed(
                                    QTransform().rotate( -90.0 ) ) );
                            break;
                        default:
                            // should be unreachable
                            throw("Unexpected direction: "+std::to_string(prev_body_d));
                    }
                }
                break;

            case Direction::LEFT:
                if ( i == 0 ) {
                    bp->image->setPixmap(
                        head_img.transformed(
                            QTransform().rotate( -90.0 ) ) );
                } else if ( i == max_i ) {
                    switch ( prev_body_d ) {
                        case Direction::LEFT:
                            bp->image->setPixmap(
                                tail_img.transformed(
                                    QTransform().rotate( -90.0 ) ) );
                            break;
                        case Direction::UP:
                            bp->image->setPixmap(
                                tail_img );
                            break;
                        case Direction::DOWN:
                            bp->image->setPixmap(
                                tail_img.transformed(
                                    QTransform().rotate( 180.0 ) ) );
                            break;
                        default:
                            // should be unreachable
                            throw("Unexpected direction: "+std::to_string(prev_body_d));
                    }
                } else {
                    switch ( prev_body_d ) {
                        case Direction::LEFT:
                            bp->image->setPixmap(
                                body_img.transformed(
                                    QTransform().rotate( -90.0 ) ) );
                            break;
                        case Direction::UP:
                            bp->image->setPixmap(
                                curve_img.transformed(
                                    QTransform().rotate( -90.0 ) ) );
                            break;
                        case Direction::DOWN:
                            bp->image->setPixmap(
                                curve_img );
                            break;
                        default:
                            // should be unreachable
                            throw("Unexpected direction: "+std::to_string(prev_body_d));
                    }
                }
                break;

            case Direction::RIGHT:
                if ( i == 0 ) {
                    bp->image->setPixmap(
                        head_img.transformed(
                            QTransform().rotate( 90.0 ) ) );
                } else if ( i == max_i ) {
                    switch ( prev_body_d ) {
                        case Direction::RIGHT:
                            bp->image->setPixmap(
                                tail_img.transformed(
                                    QTransform().rotate( 90.0 ) ) );
                            break;
                        case Direction::UP:
                            bp->image->setPixmap(
                                tail_img );
                            break;
                        case Direction::DOWN:
                            bp->image->setPixmap(
                                tail_img.transformed(
                                    QTransform().rotate( 180.0 ) ) );
                            break;
                        default:
                            // should be unreachable
                            throw("Unexpected direction: "+std::to_string(prev_body_d));
                    }
                } else {
                    switch ( prev_body_d ) {
                        case Direction::RIGHT:
                            bp->image->setPixmap(
                                body_img.transformed(
                                    QTransform().rotate( 90.0 ) ) );
                            break;
                        case Direction::UP:
                            bp->image->setPixmap(
                                curve_img.transformed(
                                    QTransform().rotate( 180.0 ) ) );
                            break;
                        case Direction::DOWN:
                            bp->image->setPixmap(
                                curve_img.transformed(
                                    QTransform().rotate( 90.0 ) ) );
                            break;
                        default:
                            // should be unreachable
                            throw("Unexpected direction: "+std::to_string(prev_body_d));
                    }
                }
                break;

            default:
                // should be unreachable
                throw("Unexpected direction: "+std::to_string(bp->direction));
        }
        prev_body_d = bp->direction;
        i++;
    }
}


void Snake::move( Snake& adv_snake, const unsigned int& food_x, const unsigned int& food_y )
{
    std::vector<Direction> classes = {
        Direction::UP,
        Direction::DOWN,
        Direction::LEFT,
        Direction::RIGHT,
    };

    std::vector<std::vector<float>> dataset = {
        {0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0}
    };

    std::vector<float> weights = {
        -1.0,   // blocked way
        -0.4,   // dead way
         0.003, // dead way steps
         0.1,   // food way
         0.4,   // aggressive way
         0.01,  // same direction
        -1.0    // opposite direction
    };

    this->updateFieldMap( adv_snake, food_x, food_y );

    for ( int i=0; i<4; i++ ) {
        this->collectData( dataset.at(i), classes.at(i), adv_snake, food_x, food_y );
    }

    // decide
    this->head_direction = this->predictDirection( dataset, weights, classes );
}


const Direction Snake::predictDirection( std::vector<std::vector<float>>& data, std::vector<float> weights , std::vector<Direction> classes )
{
    float results[] = { 1.0, 1.0, 1.0, 1.0 };
    bool keep_current = false;
    Direction class_label;

    // process data
    for ( int i=0; i<4; i++ ) {
        std::vector<float>& d = data.at(i);
        float& r = results[i];
        for ( int j=0; j<7; j++ ) {
            r += d.at(j) * weights.at(j);
        }
    }

    // normalize results (not really a need here...)
    float min=10.0, max=-10.0;
    for ( const float& r : results ) {
        if ( r < min ) {
            min = r;
        }
        if ( r > max ) {
            max = r;
        }
    }
    if ( max != min ) {
        for ( int i=0; i<4; i++ ) {
            results[i] = (results[i]-min) / (max-min);
        }
    } else {
        keep_current = true;
        for ( int i=0; i<4; i++ ) {
            results[i] = 0.0;
        }
    }

    // choose the best result
    if ( keep_current ) {
        class_label = this->head_direction;
    } else {
        max = 0;
        for ( int i=0; i<4; i++ ) {
            if ( results[i] > max ) {
                class_label = classes.at(i);
                max = results[i];
            } else if ( results[i] == max ) {
                if ( rand()%2 ) {
                    class_label = classes.at(i);
                }
            }
        }
    }

    return class_label;
}


void Snake::collectData( std::vector<float>& data, Direction& direction, Snake& adv_snake, const unsigned int& food_x, const unsigned int& food_y )
{
    unsigned int
        blocked_way        = 0,
        dead_way           = 0,
        dead_way_steps     = 0,
        food_way           = 0,
        aggressive_way     = 0,
        same_direction     = 0,
        opposite_direction = 0;

    const unsigned int head_x = this->front().x;
    const unsigned int head_y = this->front().y;

    unsigned int x, y;

    // avoid choosing the opposite direction
    switch ( direction ) {

        case Direction::UP:
            x = head_x;
            y = head_y-1;
            if ( this->head_direction == Direction::UP ) {
                same_direction = 1;
            } else if ( this->head_direction == Direction::DOWN ) {
                opposite_direction = 1;
            }
            if ( head_y == 0 ) {
                blocked_way = 1;
            }
            break;

        case Direction::DOWN:
            x = head_x;
            y = head_y+1;
            if ( this->head_direction == Direction::DOWN ) {
                same_direction = 1;
            } else if ( this->head_direction == Direction::UP ) {
                opposite_direction = 1;
            }
            if ( head_y == 15 ) {
                blocked_way = 1;
            }
            break;

        case Direction::LEFT:
            x = head_x-1;
            y = head_y;
            if ( this->head_direction == Direction::LEFT ) {
                same_direction = 1;
            } else if ( this->head_direction == Direction::RIGHT ) {
                opposite_direction = 1;
            }
            if ( head_x == 0 ) {
                blocked_way = 1;
            }
            break;

        case Direction::RIGHT:
            x = head_x+1;
            y = head_y;
            if ( this->head_direction == Direction::RIGHT ) {
                same_direction = 1;
            } else if ( this->head_direction == Direction::LEFT ) {
                opposite_direction = 1;
            }
            if ( head_x == 15 ) {
                blocked_way = 1;
            }
            break;

        default:
            // should be unreachable
            throw("Unexpected direction: "+std::to_string(this->head_direction));
    }


    if ( !(blocked_way || opposite_direction) ) {

        // check snakes
        switch ( this->field_map.at( x ).at( y ).entity ) {
            case Entity::S:
            case Entity::A:
                blocked_way = 1;
                break;
        }

        if ( ! blocked_way ) {

            // check for deadhole
            dead_way_steps = this->isDeadHole( x, y, direction );
            if ( dead_way_steps > 0 ) {
                dead_way = 1;
            }

            // check for aggressivity purposes
            switch ( direction ) {

                case Direction::UP:
                    if ( adv_snake.direction() == Direction::LEFT && this->head_direction == Direction::RIGHT ) {
                        if ( this->inTileAdv( x+2, y+1 ) ) {
                            aggressive_way = 1;
                        }
                    } else if ( adv_snake.direction() == Direction::RIGHT && this->head_direction == Direction::LEFT ) {
                        if ( this->inTileAdv( x-2, y+1 ) ) {
                            aggressive_way = 1;
                        }
                    } else if ( adv_snake.direction() == this->head_direction ) {
                        if ( this->inTileAdv( x+2, y )
                          || this->inTileAdv( x-2, y ) ) {
                            if ( !rand()%this->aggressiveness ) {
                                aggressive_way = 1;
                            }
                        }
                    }
                    break;

                case Direction::DOWN:
                    if ( adv_snake.direction() == Direction::LEFT && this->head_direction == Direction::RIGHT ) {
                        if ( this->inTileAdv( x+2, y-1 ) ) {
                            aggressive_way = 1;
                        }
                    } else if ( adv_snake.direction() == Direction::RIGHT && this->head_direction == Direction::LEFT ) {
                        if ( this->inTileAdv( x-2, y-1 ) ) {
                            aggressive_way = 1;
                        }
                    } else if ( adv_snake.direction() == this->head_direction ) {
                        if ( this->inTileAdv( x+2, y )
                          || this->inTileAdv( x-2, y ) ) {
                            if ( !rand()%this->aggressiveness ) {
                                aggressive_way = 1;
                            }
                        }
                    }
                    break;

                case Direction::LEFT:
                    if ( adv_snake.direction() == Direction::UP && this->head_direction == Direction::DOWN ) {
                        if ( this->inTileAdv( x+1, y+2 ) ) {
                            aggressive_way = 1;
                        }
                    } else if ( adv_snake.direction() == Direction::DOWN && this->head_direction == Direction::UP ) {
                        if ( this->inTileAdv( x+1, y-2 ) ) {
                            aggressive_way = 1;
                        }
                    } else if ( adv_snake.direction() == this->head_direction ) {
                        if ( this->inTileAdv( x, y+2 )
                          || this->inTileAdv( x, y-2 ) ) {
                            if ( !rand()%this->aggressiveness ) {
                                aggressive_way = 1;
                            }
                        }
                    }
                    break;

                case Direction::RIGHT:
                    if ( adv_snake.direction() == Direction::UP && this->head_direction == Direction::DOWN ) {
                        if ( this->inTileAdv( x-1, y+2 ) ) {
                            aggressive_way = 1;
                        }
                    } else if ( adv_snake.direction() == Direction::DOWN && this->head_direction == Direction::UP ) {
                        if ( this->inTileAdv( x-1, y-2 ) ) {
                            aggressive_way = 1;
                        }
                    } else if ( adv_snake.direction() == this->head_direction ) {
                        if ( this->inTileAdv( x, y+2 )
                          || this->inTileAdv( x, y-2 ) ) {
                            if ( !rand()%this->aggressiveness ) {
                                aggressive_way = 1;
                            }
                        }
                    }
                    break;

                default:
                    // should be unreachable
                    throw("Unexpected direction: "+std::to_string(this->head_direction));
            }
        }
    }


    if ( !(blocked_way || dead_way) ) {

        // check for food
        switch ( direction ) {

            case Direction::UP:
                if ( food_y < head_y ) {
                    if ( ! opposite_direction ) {
                        food_way = 1;
                    }
                } else if ( food_y == head_y ) {
                    if ( food_x > head_x && this->head_direction == Direction::LEFT ) {
                        food_way = 1;
                    } else if ( food_x < head_x && this->head_direction == Direction::RIGHT ) {
                        food_way = 1;
                    }
                }
                break;

            case Direction::DOWN:
                if ( food_y > head_y ) {
                    if ( ! opposite_direction ) {
                        food_way = 1;
                    }
                } else if ( food_y == head_y ) {
                    if ( food_x > head_x && this->head_direction == Direction::LEFT ) {
                        food_way = 1;
                    } else if ( food_x < head_x && this->head_direction == Direction::RIGHT ) {
                        food_way = 1;
                    }
                }
                break;

            case Direction::LEFT:
                if ( food_x < head_x ) {
                    if ( ! opposite_direction ) {
                        food_way = 1;
                    }
                } else if ( food_x == head_x ) {
                    if ( food_y > head_y && this->head_direction == Direction::UP ) {
                        food_way = 1;
                    } else if ( food_y < head_y && this->head_direction == Direction::DOWN ) {
                        food_way = 1;
                    }
                }
                break;

            case Direction::RIGHT:
                if ( food_x > head_x ) {
                    if ( ! opposite_direction ) {
                        food_way = 1;
                    }
                } else if ( food_x == head_x ) {
                    if ( food_y > head_y && this->head_direction == Direction::UP ) {
                        food_way = 1;
                    } else if ( food_y < head_y && this->head_direction == Direction::DOWN ) {
                        food_way = 1;
                    }
                }
                break;

            default:
                // should be unreachable
                throw("Unexpected direction: "+std::to_string(this->head_direction));
        }
    }


    // update data
    data.at(0) = blocked_way;
    data.at(1) = dead_way;
    data.at(2) = dead_way_steps;
    data.at(3) = food_way;
    data.at(4) = aggressive_way;
    data.at(5) = same_direction;
    data.at(6) = opposite_direction;
}


void Snake::updateFieldMap( Snake& adv_snake, const unsigned int& food_x, const unsigned int& food_y )
{
    // reset to default state
    for ( size_t x=0; x<16; x++ ) {
        for ( size_t y=0; y<16; y++ ) {
            Tile& t = this->field_map.at(x).at(y);
            t.entity = Entity::N;
            t.s_index = 0;
        }
    }
    // update food position
    this->field_map.at(food_x).at(food_y).entity = Entity::F;
    // update self
    unsigned int i = this->size();
    for ( std::vector<BodyPart>::const_iterator bp = this->begin(); bp != this->end(); ++bp ) {
        Tile& t = this->field_map.at(bp->x).at(bp->y);
        t.entity = Entity::S;
        t.s_index = i;
        i--;
    }
    // update adversary
    i = adv_snake.size();
    for ( const auto& bp : adv_snake ) {
        Tile& t = this->field_map.at(bp.x).at(bp.y);
        t.entity = Entity::A;
        t.s_index = i;
        i--;
    }
}


const bool Snake::inTileAdv(const unsigned int& x, const unsigned int& y )
{
    bool result = false;
    if ( x < 16 && y < 16 ) {
        switch ( this->field_map.at(x).at(y).entity ) {
            case Entity::A:
                result = true;
                break;
        }
    }
    return result;
}

const bool Snake::inTileMinusSteps(const unsigned int& x, const unsigned int& y, const unsigned int& steps )
{
    bool result = false;
    switch ( this->field_map.at(x).at(y).entity ) {
        case Entity::S:
        case Entity::A:
            if ( this->field_map.at(x).at(y).s_index > steps ) {
                result = true;
            }
            break;
    }
    return result;
}


const std::vector<unsigned int> Snake::checkAround( const Direction& direction, const unsigned int& x, const unsigned int& y )
{
    std::vector<unsigned int> around = {
        0, 0, 0,
        0,    0,
        0, 0, 0,
    };

    std::vector<int> x_pattern, y_pattern;

    switch ( direction ) {
        case Direction::UP:
            x_pattern = { -1,  0,  1,
                          -1,      1,
                          -1,  0,  1 };
            y_pattern = { -1, -1, -1,
                           0,      0,
                           1,  1,  1 };
            break;
        case Direction::DOWN:
            x_pattern = {  1,  0, -1,
                           1,     -1,
                           1,  0, -1 };
            y_pattern = {  1,  1,  1,
                           0,      0,
                          -1, -1, -1 };
            break;
        case Direction::LEFT:
            x_pattern = { -1, -1, -1,
                           0,      0,
                           1,  1,  1 };
            y_pattern = {  1,  0, -1,
                           1,     -1,
                           1,  0, -1 };
            break;
        case Direction::RIGHT:
            x_pattern = {  1,  1,  1,
                           0,      0,
                          -1, -1, -1 };
            y_pattern = { -1,  0,  1,
                          -1,      1,
                          -1,  0,  1 };
            break;
        default:
            // should be unreachable
            throw("Unexpected direction: "+std::to_string(direction));
    }

    unsigned int x_, y_;
    for ( int i=0; i<8; i++ ) {
        x_  = x;
        x_ += x_pattern.at(i);
        y_  = y;
        y_ += y_pattern.at(i);
        if ( x_ > 15 || y_ > 15 ) {
            around.at(i) = 1;
        } else {
            switch ( this->field_map.at( x_ ).at( y_ ).entity ) {
                case Entity::S:
                case Entity::A:
                    around.at(i) = 2;
                    break;
            }
        }
    }

    return around;
}


const unsigned int Snake::isDeadHole( const unsigned int& start_x, const unsigned int& start_y, Direction start_direction )
{
    bool result=false, check=false, check_clockwise=false;
    int front_step, front_check, side_check;
    unsigned int steps=1, side, front;
    Direction direction = start_direction;

    const auto blocked_around = this->checkAround( direction, start_x, start_y );
    if ( (blocked_around.at(3)>0 && blocked_around.at(4)>0)
      && (blocked_around.at(3)>1 || blocked_around.at(4)>1) ) {
        check = true;
    } else if ( (blocked_around.at(3)>0 && blocked_around.at(7)>0)
             && (blocked_around.at(3)>1 || blocked_around.at(7)>1) ) {
        check = true;
    } else if ( (blocked_around.at(4)>0 && blocked_around.at(5)>0)
             && (blocked_around.at(4)>1 || blocked_around.at(5)>1) ) {
        check = true;
        check_clockwise = true;
    } else if ( (blocked_around.at(5)>0 && blocked_around.at(7)>0)
             && (blocked_around.at(5)>1 || blocked_around.at(7)>1) ) {
        check = true;
    } else if ( blocked_around.at(0)>0 && blocked_around.at(1)>0 && blocked_around.at(2)>0 ) {
        check = true;
    }


    if ( check ) {

        unsigned int aux_side, aux_front;
        const unsigned int max_steps = this->size()-1;
        std::vector<std::tuple<unsigned int, unsigned int>> tried_positions = {
            std::make_tuple( start_x, start_y )
        };

        std::function<void(const bool&)> change_direction;
        std::function<const bool(const unsigned int& side_, const unsigned int& front_, const bool& update)> tried_already;
        std::function<const bool(const unsigned int&, const unsigned int&)> tile_blocked;
        std::function<void()> check_deadhole;


        change_direction = [&](const bool& clockwise) {
            switch ( direction ) {
                case Direction::UP:
                    if ( clockwise ) {
                        direction  = Direction::RIGHT;
                        front_check = +1;
                        side_check  = (check_clockwise) ? +1 : -1;
                    } else {
                        direction  = Direction::LEFT;
                        front_check = -1;
                        side_check  = (check_clockwise) ? -1 : +1;
                    }
                    break;
                case Direction::DOWN:
                    if ( clockwise ) {
                        direction  = Direction::LEFT;
                        front_check = -1;
                        side_check  = (check_clockwise) ? -1 : +1;
                    } else {
                        direction  = Direction::RIGHT;
                        front_check = +1;
                        side_check  = (check_clockwise) ? +1 : -1;
                    }
                    break;
                case Direction::LEFT:
                    if ( clockwise ) {
                        direction  = Direction::UP;
                        front_check = -1;
                        side_check  = (check_clockwise) ? +1 : -1;
                    } else {
                        direction  = Direction::DOWN;
                        front_check = +1;
                        side_check  = (check_clockwise) ? -1 : +1;
                    }
                    break;
                case Direction::RIGHT:
                    if ( clockwise ) {
                        direction  = Direction::DOWN;
                        front_check = +1;
                        side_check  = (check_clockwise) ? -1 : +1;
                    } else {
                        direction  = Direction::UP;
                        front_check = -1;
                        side_check  = (check_clockwise) ? +1 : -1;
                    }
                    break;
                default:
                    // should be unreachable
                    throw("Unexpected direction: "+std::to_string(direction));
            }
            // swap front/side
            front = front ^ side;
            side  = front ^ side;
            front = front ^ side;
            front_step = front_check;
        };


        tried_already = [&](const unsigned int& side_, const unsigned int& front_, const bool& update) {
            bool tried = false;
            unsigned int x, y;
            switch ( direction ) {
                case Direction::UP:
                case Direction::DOWN:
                    x = side_;
                    y = front_;
                    break;
                case Direction::LEFT:
                case Direction::RIGHT:
                    y = side_;
                    x = front_;
                    break;
                default:
                    // should be unreachable
                    throw("Unexpected direction: "+std::to_string(direction));
            }
            for ( const auto& position : tried_positions ) {
                if ( std::get<0>(position) == x ) {
                    if ( std::get<1>(position) == y ) {
                        tried = true;
                        break;
                    }
                }
            }
            if ( update ) {
                tried_positions.push_back( std::make_tuple( x, y ) );
            }
            return tried;
        };


        tile_blocked = [&](const unsigned int& side_, const unsigned int& front_) {
            bool blocked = false;
            unsigned int x, y, x_, y_;
            switch ( direction ) {
                case Direction::UP:
                case Direction::DOWN:
                    x = side_;
                    y = front_;
                    x_ = side;
                    y_ = front;
                    break;
                case Direction::LEFT:
                case Direction::RIGHT:
                    y = side_;
                    x = front_;
                    y_ = side;
                    x_ = front;
                    break;
                default:
                    // should be unreachable
                    throw("Unexpected direction: "+std::to_string(direction));
            }
            if ( x > 15 || y > 15 ) {
                blocked = true;
            } else if ( this->inTileMinusSteps( x, y, steps ) ) {
                blocked = true;
            } else if ( tried_already( side_, front_, false ) ) {
                if ( this->field_map.at( x_ ).at( y_ ).entity == Entity::S ) {
                    blocked = true;
                }
            }
            return blocked;
        };


        check_deadhole = [&]() {
            switch ( start_direction ) {
                case Direction::UP:
                    front = start_y;
                    side  = start_x;
                    front_check = -1;
                    side_check  = (check_clockwise) ? +1 : -1;
                    break;
                case Direction::DOWN:
                    front = start_y;
                    side  = start_x;
                    front_check = +1;
                    side_check  = (check_clockwise) ? -1 : +1;
                    break;
                case Direction::LEFT:
                    front = start_x;
                    side  = start_y;
                    front_check = -1;
                    side_check  = (check_clockwise) ? -1 : +1;
                    break;
                case Direction::RIGHT:
                    front = start_x;
                    side  = start_y;
                    front_check = +1;
                    side_check  = (check_clockwise) ? +1 : -1;
                    break;
                default:
                    // should be unreachable
                    throw("Unexpected direction: "+std::to_string(start_direction));
            }
            front_step = front_check;

            while ( true ) {
                // check the side
                aux_side  = side;
                aux_side += side_check;
                if ( !tile_blocked( aux_side, front ) ) {
                    // side is free, check for another deadhole
                    aux_front  = front;
                    aux_front += front_check;
                    unsigned int aux_front_  = front;
                    aux_front_ += front_check*(-2);
                    if ( tile_blocked( side, aux_front ) && tile_blocked( aux_side, aux_front_ ) ) {
                        // may-be deadhole at side, check the opposite
                        aux_side  = side;
                        aux_side += (side_check*(-1));
                        if ( !tile_blocked( aux_side, front ) ) {
                            // opposite side free
                            change_direction( !check_clockwise );
                        } else {
                            // opposite side blocked too
                            change_direction( check_clockwise );
                        }
                    } else {
                        // turn at side
                        change_direction( check_clockwise );
                    }
                } else {
                    // side blocked, check front
                    aux_front  = front;
                    aux_front += front_check;
                    if ( tile_blocked( side, aux_front ) ) {
                        // front blocked, check opposite side
                        aux_side  = side;
                        aux_side += (side_check*(-1));
                        if ( !tile_blocked( aux_side, front ) ) {
                            // opposite side free
                            change_direction( !check_clockwise );
                        } else {
                            // opposite side blocked too
                            result = true;
                            break;
                        }
                    }
                }

                front += front_step;

                if ( tried_already( side, front, true ) ) {
                    result = true;
                    break;
                } else {
                    steps ++;
                    if ( steps >= max_steps ) {
                        break;
                    }
                }
            }
        };


        // start checking
        bool aux_result=false;
        unsigned int aux_steps=0, i=0;
        while ( true ) {
            check_deadhole();
            if ( !result ) {
                steps = 0;
            }
            i ++;
            if ( i == 2 ) {
                break;
            }
            aux_result = result;
            result = false;
            aux_steps = steps;
            steps = 1;
            check_clockwise = !check_clockwise;
            direction = start_direction;
            tried_positions.clear();
        }

        // mean result
        result = result | aux_result;
        steps = (steps>aux_steps) ? steps : aux_steps;
    }

    if ( !result ) {
        steps = 0;
    }
    return steps;
}
