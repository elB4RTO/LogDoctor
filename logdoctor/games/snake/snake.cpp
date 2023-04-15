
#include "snake.h"

#include <QGraphicsScene>


Snake::Snake( const bool is_adversary )
    : adversary( is_adversary )
{
    if ( is_adversary ) {
        for ( size_t x{0}; x<16ul; x++ ) {
            this->field_map.push_back( std::vector<Tile>{} );
            std::vector<Tile>& v = this->field_map.back();
            for ( size_t y{0}; y<16ul; y++ ) {
                v.push_back({ Entity::N, 0 });
            }
        }
    }
}


const QPixmap& Snake::getHeadImage() const
{
    if ( this->adversary ) {
        return this->img_snakeHead_;
    } else {
        return this->img_snakeHead;
    }
}


const bool Snake::inTile( const unsigned x, const unsigned y, const bool avoid_tail ) const
{
    if ( this->size() > 0 ) {
        size_t i{ 0 };
        size_t max{ this->size()-1ul };
        if ( !avoid_tail ) {
            max ++;
        }
        for ( auto bp{ this->cbegin() }; bp != this->cend(); ++bp ) {
            if ( bp->x == x && bp->y == y ) {
                return true;
            }
            i++;
            if ( i >= max ) {
                break;
            }
        }
    }
    return false;
}


void Snake::setDirection( const Direction new_direction )
{
    this->head_direction = new_direction;
}
const Direction& Snake::direction() const
{
    return this->head_direction;
}

void Snake::willGrow()
{
    this->will_grow |= true;
}
void Snake::grow( const bool is_borning )
{
    // build from the tail
    const BodyPart& tail{ this->back() };
    unsigned x{ tail.x };
    unsigned y{ tail.y };
    const Direction d{  tail.direction };
    const Direction ld{ tail.prev_direction };
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


void Snake::update( QGraphicsScene* field_scene, const bool dry , const bool is_borning )
{
    // grow if planned
    if ( this->will_grow ) {
        this->will_grow &= false;
        this->grow( is_borning );
        field_scene->addItem( this->back().image );
    }
    // anyway, update the whole body
    size_t i{ 0 };
    const size_t max_i{ this->size()-1ul };
    unsigned new_x, prev_x, new_y, prev_y;
    Direction new_direction, prev_direction, prev_body_d;
    const QPixmap& head_img{  (this->adversary) ? this->img_snakeHead_  : this->img_snakeHead  };
    const QPixmap& body_img{  (this->adversary) ? this->img_snakeBody_  : this->img_snakeBody  };
    const QPixmap& curve_img{ (this->adversary) ? this->img_snakeCurve_ : this->img_snakeCurve };
    const QPixmap& tail_img{  (this->adversary) ? this->img_snakeTail_  : this->img_snakeTail  };
    for ( auto bp{ this->begin() }; bp != this->end(); ++bp ) {
        if ( ! dry ) {
            // future position
            if ( i == 0ul ) {
                // head doesn't follow any other part of the body
                switch ( this->head_direction ) {
                    case Direction::UP:
                        new_y = bp->y - 1u;
                        new_x = bp->x;
                        break;
                    case Direction::DOWN:
                        new_y = bp->y + 1u;
                        new_x = bp->x;
                        break;
                    case Direction::LEFT:
                        new_x = bp->x - 1u;
                        new_y = bp->y;
                        break;
                    case Direction::RIGHT:
                        new_x = bp->x + 1u;
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
                if ( i == 0ul ) {
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
                if ( i == 0ul ) {
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
                if ( i == 0ul ) {
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
                if ( i == 0ul ) {
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


void Snake::move( const Snake& adv_snake, const unsigned& food_x, const unsigned& food_y )
{
    std::vector<Direction> classes{
        Direction::UP,
        Direction::DOWN,
        Direction::LEFT,
        Direction::RIGHT,
    };

    std::vector<std::vector<float>> dataset{
        {0.f,0.f,0.f,0.f,0.f,0.f,0.f},
        {0.f,0.f,0.f,0.f,0.f,0.f,0.f},
        {0.f,0.f,0.f,0.f,0.f,0.f,0.f},
        {0.f,0.f,0.f,0.f,0.f,0.f,0.f}
    };

    std::vector<float> weights{
        -1.0f,   // blocked way
        -0.4f,   // dead way
         0.003f, // dead way steps
         0.1f,   // food way
         0.4f,   // aggressive way
         0.01f,  // same direction
        -1.0f    // opposite direction
    };

    this->updateFieldMap( adv_snake, food_x, food_y );

    for ( size_t i{0}; i<4ul; i++ ) {
        this->collectData( dataset.at(i), classes.at(i), adv_snake, food_x, food_y );
    }

    // decide
    this->head_direction = this->predictDirection( dataset, weights, classes );
}


const Direction Snake::predictDirection( const std::vector<std::vector<float>>& data, const std::vector<float>& weights, const std::vector<Direction>& classes ) const
{
    float results[]{ 1.f, 1.f, 1.f, 1.f };
    bool keep_current{ false };
    Direction class_label;

    // process data
    for ( size_t i{0}; i<4ul; i++ ) {
        const std::vector<float>& d = data.at(i);
        float& r = results[i];
        for ( size_t j{0}; j<7ul; j++ ) {
            r += d.at(j) * weights.at(j);
        }
    }

    // normalize results (not really a need here...)
    float min{10.f}, max{-10.f};
    for ( const float& r : results ) {
        if ( r < min ) {
            min = r;
        }
        if ( r > max ) {
            max = r;
        }
    }
    if ( max != min ) {
        for ( size_t i{0}; i<4ul; i++ ) {
            results[i] = (results[i]-min) / (max-min);
        }
    } else {
        keep_current |= true;
        for ( size_t i{0}; i<4ul; i++ ) {
            results[i] = 0.f;
        }
    }

    // choose the best result
    if ( keep_current ) {
        class_label = this->head_direction;
    } else {
        max = 0.f;
        for ( size_t i{0}; i<4ul; i++ ) {
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


void Snake::collectData( std::vector<float>& data, Direction& direction, const Snake& adv_snake, const unsigned& food_x, const unsigned& food_y ) const
{
    unsigned
        blocked_way        { 0 },
        dead_way           { 0 },
        dead_way_steps     { 0 },
        food_way           { 0 },
        aggressive_way     { 0 },
        same_direction     { 0 },
        opposite_direction { 0 };

    const unsigned head_x{ this->front().x };
    const unsigned head_y{ this->front().y };

    unsigned x, y;

    // avoid choosing the opposite direction
    switch ( direction ) {

        case Direction::UP:
            x = head_x;
            y = head_y-1u;
            if ( this->head_direction == Direction::UP ) {
                same_direction = 1u;
            } else if ( this->head_direction == Direction::DOWN ) {
                opposite_direction = 1u;
            }
            if ( head_y == 0u ) {
                blocked_way = 1u;
            }
            break;

        case Direction::DOWN:
            x = head_x;
            y = head_y+1u;
            if ( this->head_direction == Direction::DOWN ) {
                same_direction = 1u;
            } else if ( this->head_direction == Direction::UP ) {
                opposite_direction = 1u;
            }
            if ( head_y == 15u ) {
                blocked_way = 1u;
            }
            break;

        case Direction::LEFT:
            x = head_x-1u;
            y = head_y;
            if ( this->head_direction == Direction::LEFT ) {
                same_direction = 1u;
            } else if ( this->head_direction == Direction::RIGHT ) {
                opposite_direction = 1u;
            }
            if ( head_x == 0u ) {
                blocked_way = 1u;
            }
            break;

        case Direction::RIGHT:
            x = head_x+1u;
            y = head_y;
            if ( this->head_direction == Direction::RIGHT ) {
                same_direction = 1u;
            } else if ( this->head_direction == Direction::LEFT ) {
                opposite_direction = 1u;
            }
            if ( head_x == 15u ) {
                blocked_way = 1u;
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
                blocked_way = 1u;
                break;
            default:
                break;
        }

        if ( ! blocked_way ) {

            // check for deadhole
            dead_way_steps = this->isDeadHole( x, y, direction );
            if ( dead_way_steps > 0u ) {
                dead_way = 1u;
            }

            // check for aggressivity purposes
            switch ( direction ) {

                case Direction::UP:
                    if ( adv_snake.direction() == Direction::LEFT && this->head_direction == Direction::RIGHT ) {
                        if ( this->inTileAdv( x+2u, y+1u ) ) {
                            aggressive_way = 1u;
                        }
                    } else if ( adv_snake.direction() == Direction::RIGHT && this->head_direction == Direction::LEFT ) {
                        if ( this->inTileAdv( x-2u, y+1u ) ) {
                            aggressive_way = 1u;
                        }
                    } else if ( adv_snake.direction() == this->head_direction ) {
                        if ( this->inTileAdv( x+2u, y )
                          || this->inTileAdv( x-2u, y ) ) {
                            if ( !rand()%this->aggressiveness ) {
                                aggressive_way = 1u;
                            }
                        }
                    }
                    break;

                case Direction::DOWN:
                    if ( adv_snake.direction() == Direction::LEFT && this->head_direction == Direction::RIGHT ) {
                        if ( this->inTileAdv( x+2u, y-1u ) ) {
                            aggressive_way = 1u;
                        }
                    } else if ( adv_snake.direction() == Direction::RIGHT && this->head_direction == Direction::LEFT ) {
                        if ( this->inTileAdv( x-2u, y-1u ) ) {
                            aggressive_way = 1u;
                        }
                    } else if ( adv_snake.direction() == this->head_direction ) {
                        if ( this->inTileAdv( x+2u, y )
                          || this->inTileAdv( x-2u, y ) ) {
                            if ( !rand()%this->aggressiveness ) {
                                aggressive_way = 1u;
                            }
                        }
                    }
                    break;

                case Direction::LEFT:
                    if ( adv_snake.direction() == Direction::UP && this->head_direction == Direction::DOWN ) {
                        if ( this->inTileAdv( x+1u, y+2u ) ) {
                            aggressive_way = 1u;
                        }
                    } else if ( adv_snake.direction() == Direction::DOWN && this->head_direction == Direction::UP ) {
                        if ( this->inTileAdv( x+1u, y-2u ) ) {
                            aggressive_way = 1u;
                        }
                    } else if ( adv_snake.direction() == this->head_direction ) {
                        if ( this->inTileAdv( x, y+2u )
                          || this->inTileAdv( x, y-2u ) ) {
                            if ( !rand()%this->aggressiveness ) {
                                aggressive_way = 1u;
                            }
                        }
                    }
                    break;

                case Direction::RIGHT:
                    if ( adv_snake.direction() == Direction::UP && this->head_direction == Direction::DOWN ) {
                        if ( this->inTileAdv( x-1u, y+2u ) ) {
                            aggressive_way = 1u;
                        }
                    } else if ( adv_snake.direction() == Direction::DOWN && this->head_direction == Direction::UP ) {
                        if ( this->inTileAdv( x-1u, y-2u ) ) {
                            aggressive_way = 1u;
                        }
                    } else if ( adv_snake.direction() == this->head_direction ) {
                        if ( this->inTileAdv( x, y+2u )
                          || this->inTileAdv( x, y-2u ) ) {
                            if ( !rand()%this->aggressiveness ) {
                                aggressive_way = 1u;
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
                        food_way = 1u;
                    }
                } else if ( food_y == head_y ) {
                    if ( food_x > head_x && this->head_direction == Direction::LEFT ) {
                        food_way = 1u;
                    } else if ( food_x < head_x && this->head_direction == Direction::RIGHT ) {
                        food_way = 1u;
                    }
                }
                break;

            case Direction::DOWN:
                if ( food_y > head_y ) {
                    if ( ! opposite_direction ) {
                        food_way = 1u;
                    }
                } else if ( food_y == head_y ) {
                    if ( food_x > head_x && this->head_direction == Direction::LEFT ) {
                        food_way = 1u;
                    } else if ( food_x < head_x && this->head_direction == Direction::RIGHT ) {
                        food_way = 1u;
                    }
                }
                break;

            case Direction::LEFT:
                if ( food_x < head_x ) {
                    if ( ! opposite_direction ) {
                        food_way = 1u;
                    }
                } else if ( food_x == head_x ) {
                    if ( food_y > head_y && this->head_direction == Direction::UP ) {
                        food_way = 1u;
                    } else if ( food_y < head_y && this->head_direction == Direction::DOWN ) {
                        food_way = 1u;
                    }
                }
                break;

            case Direction::RIGHT:
                if ( food_x > head_x ) {
                    if ( ! opposite_direction ) {
                        food_way = 1u;
                    }
                } else if ( food_x == head_x ) {
                    if ( food_y > head_y && this->head_direction == Direction::UP ) {
                        food_way = 1u;
                    } else if ( food_y < head_y && this->head_direction == Direction::DOWN ) {
                        food_way = 1u;
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


void Snake::updateFieldMap( const Snake& adv_snake, const unsigned& food_x, const unsigned& food_y )
{
    // reset to default state
    for ( size_t x{0}; x<16ul; x++ ) {
        for ( size_t y{0}; y<16ul; y++ ) {
            Tile& t = this->field_map.at(x).at(y);
            t.entity = Entity::N;
            t.s_index = 0u;
        }
    }
    // update food position
    this->field_map.at(food_x).at(food_y).entity = Entity::F;
    // update self
    unsigned i{ static_cast<unsigned>( this->size() ) };
    for ( auto bp{ this->cbegin() }; bp != this->cend(); ++bp ) {
        Tile& t = this->field_map.at(bp->x).at(bp->y);
        t.entity = Entity::S;
        t.s_index = i;
        i--;
    }
    // update adversary
    i = static_cast<unsigned>( adv_snake.size() );
    for ( const auto& bp : adv_snake ) {
        Tile& t = this->field_map.at(bp.x).at(bp.y);
        t.entity = Entity::A;
        t.s_index = i;
        i--;
    }
}


const bool Snake::inTileAdv(const unsigned x, const unsigned y ) const
{
    if ( x < 16 && y < 16 ) {
        switch ( this->field_map.at(x).at(y).entity ) {
            case Entity::A:
                return true;
            default:
                return false;
        }
    }
    return false;
}

const bool Snake::inTileMinusSteps(const unsigned x, const unsigned y, const unsigned steps ) const
{
    switch ( this->field_map.at(x).at(y).entity ) {
        case Entity::S:
        case Entity::A:
            if ( this->field_map.at(x).at(y).s_index > steps ) {
                return true;
            }
        default:
            return false;
    }
    return false;
}


const std::vector<unsigned> Snake::checkAround( const Direction& direction, const unsigned& x, const unsigned& y ) const
{
    std::vector<unsigned> around{
        0u, 0u, 0u,
        0u,     0u,
        0u, 0u, 0u,
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

    unsigned x_, y_;
    for ( size_t i{0}; i<8ul; i++ ) {
        x_  = x;
        x_ += x_pattern.at(i);
        y_  = y;
        y_ += y_pattern.at(i);
        if ( x_ > 15 || y_ > 15 ) {
            around.at(i) = 1u;
        } else {
            switch ( this->field_map.at( x_ ).at( y_ ).entity ) {
                case Entity::S:
                case Entity::A:
                    around.at(i) = 2u;
                    break;
                default:
                    break;
            }
        }
    }

    return around;
}


const unsigned Snake::isDeadHole( const unsigned& start_x, const unsigned& start_y, const Direction& start_direction ) const
{
    bool result{false}, check{false}, check_clockwise{false};
    Direction direction{ start_direction };
    unsigned steps{ 1 };

    const auto blocked_around{ this->checkAround( direction, start_x, start_y ) };
    if ( (blocked_around.at(3)>0u && blocked_around.at(4)>0u)
      && (blocked_around.at(3)>1u || blocked_around.at(4)>1u) ) {
        check |= true;
    } else if ( (blocked_around.at(3)>0u && blocked_around.at(7)>0u)
             && (blocked_around.at(3)>1u || blocked_around.at(7)>1u) ) {
        check |= true;
    } else if ( (blocked_around.at(4)>0u && blocked_around.at(5)>0u)
             && (blocked_around.at(4)>1u || blocked_around.at(5)>1u) ) {
        check |= true;
        check_clockwise |= true;
    } else if ( (blocked_around.at(5)>0u && blocked_around.at(7)>0u)
             && (blocked_around.at(5)>1u || blocked_around.at(7)>1u) ) {
        check |= true;
    } else if ( blocked_around.at(0)>0u && blocked_around.at(1)>0u && blocked_around.at(2)>0u ) {
        check |= true;
    }


    if ( check ) {

        int front_step, front_check, side_check;
        unsigned side, front, aux_side, aux_front;
        const unsigned max_steps{ static_cast<unsigned>(this->size()-1ul) };
        std::vector<std::tuple<unsigned, unsigned>> tried_positions{
            std::make_tuple( start_x, start_y )
        };

        std::function<void(const bool&)> change_direction;
        std::function<const bool(const unsigned& side_, const unsigned& front_, const bool& update)> tried_already;
        std::function<const bool(const unsigned&, const unsigned&)> tile_blocked;
        std::function<void()> check_deadhole;


        change_direction = [&direction, &check_clockwise, &front_step, &front_check, &side_check, &front, &side]
                           (const bool& clockwise)
        {
            switch ( direction ) {
                case Direction::UP:
                    if ( clockwise ) {
                        direction   = Direction::RIGHT;
                        front_check = +1;
                        side_check  = (check_clockwise) ? +1 : -1;
                    } else {
                        direction   = Direction::LEFT;
                        front_check = -1;
                        side_check  = (check_clockwise) ? -1 : +1;
                    }
                    break;
                case Direction::DOWN:
                    if ( clockwise ) {
                        direction   = Direction::LEFT;
                        front_check = -1;
                        side_check  = (check_clockwise) ? -1 : +1;
                    } else {
                        direction   = Direction::RIGHT;
                        front_check = +1;
                        side_check  = (check_clockwise) ? +1 : -1;
                    }
                    break;
                case Direction::LEFT:
                    if ( clockwise ) {
                        direction   = Direction::UP;
                        front_check = -1;
                        side_check  = (check_clockwise) ? +1 : -1;
                    } else {
                        direction   = Direction::DOWN;
                        front_check = +1;
                        side_check  = (check_clockwise) ? -1 : +1;
                    }
                    break;
                case Direction::RIGHT:
                    if ( clockwise ) {
                        direction   = Direction::DOWN;
                        front_check = +1;
                        side_check  = (check_clockwise) ? -1 : +1;
                    } else {
                        direction   = Direction::UP;
                        front_check = -1;
                        side_check  = (check_clockwise) ? +1 : -1;
                    }
                    break;
                default:
                    // should be unreachable
                    throw("Unexpected direction: "+std::to_string(direction));
            }
            // swap front/side
            std::swap( front, side );
            front_step = front_check;
        };


        tried_already = [&direction, &tried_positions]
                        (const unsigned& side_, const unsigned& front_, const bool update)
        {
            bool tried{ false };
            unsigned x, y;
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
                        tried |= true;
                        break;
                    }
                }
            }
            if ( update ) {
                tried_positions.push_back( std::make_tuple( x, y ) );
            }
            return tried;
        };


        tile_blocked = [this, &tried_already, &direction, &front, &side, &steps]
                       (const unsigned& side_, const unsigned& front_)
        {
            bool blocked{ false };
            unsigned x, y, x_, y_;
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
                blocked |= true;
            } else if ( this->inTileMinusSteps( x, y, steps ) ) {
                blocked |= true;
            } else if ( tried_already( side_, front_, false ) ) {
                if ( this->field_map.at( x_ ).at( y_ ).entity == Entity::S ) {
                    blocked |= true;
                }
            }
            return blocked;
        };


        check_deadhole = [&tried_already, &tile_blocked, &change_direction,
                          &start_direction, &start_x, &start_y,
                          &front, &side, &aux_front, &aux_side,
                          &front_step, &front_check, &side_check,
                          &check_clockwise, &max_steps, &steps, &result]
                         ()
        {
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
                    unsigned aux_front_  = front;
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
                            result |= true;
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
        bool aux_result{ false };
        unsigned aux_steps{0}, i{0};
        while ( true ) {
            check_deadhole();
            if ( !result ) {
                steps = 0;
            }
            i ++;
            if ( i == 2u ) {
                break;
            }
            aux_result = result;
            result &= false;
            aux_steps = steps;
            steps = 1u;
            check_clockwise = !check_clockwise;
            direction = start_direction;
            tried_positions.clear();
        }

        // mean result
        result |= aux_result;
        steps = (steps>aux_steps) ? steps : aux_steps;
    }

    if ( !result ) {
        steps = 0u;
    }
    return steps;
}
