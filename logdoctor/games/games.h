#ifndef GAMES_H
#define GAMES_H

class QString;


//! GameSec
/*!
    Utilities for the games
*/
namespace GameSec
{

//! Provides the stylesheet for the CrissCross game
void crisscrossStyleSheet( QString& stylesheet, const int theme_id );

//! Provides the stylesheet for the Snake game
void snakeStyleSheet( QString& stylesheet, const int theme_id );

} // namespace GameSec

#endif // GAMES_H
