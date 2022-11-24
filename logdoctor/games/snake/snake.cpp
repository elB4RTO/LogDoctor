
#include "snake.h"


Snake::Snake( const bool& is_adversary )
{
    this->adversary = is_adversary;
}


QPixmap& Snake::getHeadImage()
{
    if ( this->adversary ) {
        return this->img_snakeHead_;
    } else {
        return this->img_snakeHead;
    }
}


const bool Snake::inTile(  const unsigned int& x, const unsigned int& y  )
{
    bool result = false;
    if ( this->size() > 0 ) {
        for ( std::vector<BodyPart>::const_iterator bp = this->begin(); bp != this->end(); ++bp ) {
            if ( bp->x == x && bp->y == y ) {
                result = true;
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


void Snake::grow( const bool& initial )
{
    // build from the tail
    const BodyPart& tail = this->back();
    unsigned int x = tail.x;
    unsigned int y = tail.y;
    const Direction d  = tail.direction;
    const Direction ld = tail.prev_direction;
    if ( initial ) {
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
    this->update( true );
    this->back().update( x, y, d );
}


void Snake::update( const bool& dry )
{
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


void Snake::move( Snake& snake, const unsigned int& food_x, const unsigned int& food_y )
{
    int move_up    = 0,
        move_down  = 0,
        move_left  = 0,
        move_right = 0;

    const unsigned int head_x = this->front().x;
    const unsigned int head_y = this->front().y;

    // avoid choosing the opposite direction
    switch ( this->head_direction ) {
        case Direction::UP:
            move_down -= 500;
            break;
        case Direction::DOWN:
            move_up -= 500;
            break;
        case Direction::LEFT:
            move_right -= 500;
            break;
        case Direction::RIGHT:
            move_left -= 500;
            break;
        default:
            // should be unreachable
            throw("Unexpected direction: "+std::to_string(this->head_direction));
    }

    // check the field's limits
    if ( head_y == 0 ) {
        move_up -= 500;
    } else if ( head_y == 15 ) {
        move_down -= 500;
    }
    if ( head_x == 0 ) {
        move_left -= 500;
    } else if ( head_x == 15 ) {
        move_right -= 500;
    }

    // check yourself
    if ( ! this->inTile( head_x, head_y-1 ) ) {
        move_up += 100;
    } else {
        move_up -= 100;
    }
    if ( ! this->inTile( head_x, head_y+1 ) ) {
        move_down += 100;
    } else {
        move_down -= 100;
    }
    if ( ! this->inTile( head_x-1, head_y ) ) {
        move_left += 100;
    } else {
        move_left -= 100;
    }
    if ( ! this->inTile( head_x+1, head_y ) ) {
        move_right += 100;
    } else {
        move_right -= 100;
    }

    // check the adversary
    if ( ! snake.inTile( head_x, head_y-1 ) ) {
        move_up += 10;
        if ( snake.direction() == Direction::LEFT && this->head_direction == Direction::RIGHT ) {
            if ( snake.inTile( head_x+2, head_y ) ) {
                move_up += 40;
            }
        } else if ( snake.direction() == Direction::RIGHT && this->head_direction == Direction::LEFT ) {
            if ( snake.inTile( head_x-2, head_y ) ) {
                move_up += 40;
            }
        } else if ( snake.direction() == this->head_direction ) {
            if ( snake.inTile( head_x+2, head_y-1 )
              || snake.inTile( head_x-2, head_y-1 ) ) {
                if ( !rand()%this->aggressiveness ) {
                    move_up += 40;
                }
            }
        }
    } else {
        move_up -= 50;
    }
    if ( ! snake.inTile( head_x, head_y+1 ) ) {
        move_down += 10;
        if ( snake.direction() == Direction::LEFT && this->head_direction == Direction::RIGHT ) {
            if ( snake.inTile( head_x+2, head_y ) ) {
                move_down += 40;
            }
        } else if ( snake.direction() == Direction::RIGHT && this->head_direction == Direction::LEFT ) {
            if ( snake.inTile( head_x-2, head_y ) ) {
                move_down += 40;
            }
        } else if ( snake.direction() == this->head_direction ) {
            if ( snake.inTile( head_x+2, head_y+1 )
              || snake.inTile( head_x-2, head_y+1 ) ) {
                if ( !rand()%this->aggressiveness ) {
                    move_down += 40;
                }
            }
        }
    } else {
        move_down -= 50;
    }
    if ( ! snake.inTile( head_x-1, head_y ) ) {
        move_left += 10;
        if ( snake.direction() == Direction::UP && this->head_direction == Direction::DOWN ) {
            if ( snake.inTile( head_x, head_y+2 ) ) {
                move_left += 40;
            }
        } else if ( snake.direction() == Direction::DOWN && this->head_direction == Direction::UP ) {
            if ( snake.inTile( head_x, head_y-2 ) ) {
                move_left += 40;
            }
        } else if ( snake.direction() == this->head_direction ) {
            if ( snake.inTile( head_x-1, head_y+2 )
              || snake.inTile( head_x-1, head_y-2 ) ) {
                if ( !rand()%this->aggressiveness ) {
                    move_left += 40;
                }
            }
        }
    } else {
        move_left -= 50;
    }
    if ( ! snake.inTile( head_x+1, head_y ) ) {
        move_right += 10;
        if ( snake.direction() == Direction::UP && this->head_direction == Direction::DOWN ) {
            if ( snake.inTile( head_x, head_y+2 ) ) {
                move_right += 40;
            }
        } else if ( snake.direction() == Direction::DOWN && this->head_direction == Direction::UP ) {
            if ( snake.inTile( head_x, head_y-2 ) ) {
                move_right += 40;
            }
        } else if ( snake.direction() == this->head_direction ) {
            if ( snake.inTile( head_x+1, head_y+2 )
              || snake.inTile( head_x+1, head_y-2 ) ) {
                if ( !rand()%this->aggressiveness ) {
                    move_right += 40;
                }
            }
        }
    } else {
        move_right -= 50;
    }

    // check the food
    if ( food_x == head_x ) {
        if ( food_y < head_y ) {
            move_up += 30;
            if ( this->head_direction == Direction::DOWN ) {
                move_left += 30;
                move_right += 30;
                move_up -= 30;
            } else if ( this->head_direction == Direction::UP ) {
                move_up += 10;
            }
        } else if ( food_y > head_y ) {
            move_down += 30;
            if ( this->head_direction == Direction::UP ) {
                move_left += 30;
                move_right += 30;
                move_down -= 30;
            } else if ( this->head_direction == Direction::DOWN ) {
                move_down += 10;
            }
        }
    } else if ( food_x <  head_x ) {
        move_left += 30;
        if ( food_y == head_y ) {
            if ( this->head_direction == Direction::RIGHT ) {
                move_up += 30;
                move_down += 30;
                move_left -= 30;
            } else if ( this->head_direction == Direction::LEFT ) {
                move_left += 10;
            }
        } else if ( food_y < head_y ) {
            move_up += 30;
            if ( this->head_direction == Direction::DOWN ) {
                move_up -= 30;
            } else if ( this->head_direction == Direction::UP ) {
                move_up += 10;
            } else if ( this->head_direction == Direction::LEFT ) {
                move_left += 10;
            }
        } else if ( food_y > head_y ) {
            move_down += 30;
            if ( this->head_direction == Direction::UP ) {
                move_down -= 30;
            } else if ( this->head_direction == Direction::DOWN ) {
                move_down += 10;
            } else if ( this->head_direction == Direction::LEFT ) {
                move_left += 10;
            }
        }
    } else if ( food_x >  head_x ) {
        move_right += 30;
        if ( food_y == head_y ) {
            if ( this->head_direction == Direction::LEFT ) {
                move_up += 30;
                move_down += 30;
                move_right -= 30;
            } else if ( this->head_direction == Direction::RIGHT ) {
                move_right += 10;
            }
        } else if ( food_y < head_y ) {
            move_up += 30;
            if ( this->head_direction == Direction::DOWN ) {
                move_up -= 30;
            } else if ( this->head_direction == Direction::UP ) {
                move_up += 10;
            } else if ( this->head_direction == Direction::RIGHT ) {
                move_right += 10;
            }
        } else if ( food_y > head_y ) {
            move_down += 30;
            if ( this->head_direction == Direction::UP ) {
                move_down -= 30;
            } else if ( this->head_direction == Direction::DOWN ) {
                move_down += 10;
            } else if ( this->head_direction == Direction::RIGHT ) {
                move_right += 10;
            }
        }
    }

    // decide
    int max = -1000;
    Direction choice;
    if ( move_up > max || (move_up == max && rand()%2) ) {
        bool ok = true;
        for ( int y=head_y-1; y>=0; y-- ) {
            if ( this->inTile( head_x, y ) ) {
                ok = false;
                break;
            }
        }
        if ( ok ) {
            max = move_up;
            choice = Direction::UP;
        }
    }
    if ( move_down > max || (move_down == max && rand()%2) ) {
        bool ok = true;
        for ( int y=head_y+1; y<16; y++ ) {
            if ( this->inTile( head_x, y ) ) {
                ok = false;
                break;
            }
        }
        if ( ok ) {
            max = move_down;
            choice = Direction::DOWN;
        }
    }
    if ( move_left > max || (move_left == max && rand()%2) ) {
        bool ok = true;
        for ( int x=head_x-1; x>=0; x-- ) {
            if ( this->inTile( x, head_y ) ) {
                ok = false;
                break;
            }
        }
        if ( ok ) {
            max = move_left;
            choice = Direction::LEFT;
        }
    }
    if ( move_right > max || (move_right == max && rand()%2) ) {
        bool ok = true;
        for ( int x=head_x+1; x<16; x++ ) {
            if ( this->inTile( x, head_y ) ) {
                ok = false;
                break;
            }
        }
        if ( ok ) {
            choice = Direction::RIGHT;
        }
    }

    // apply the move
    this->head_direction = choice;
}
