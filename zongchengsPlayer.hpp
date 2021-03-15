//
//  ZongchengsPlayer.hpp
//  checker
//
//  Created by apple on 3/12/21.
//

#ifndef ZongchengsPlayer_hpp
#define ZongchengsPlayer_hpp

#include <stdio.h>
#include "Piece.hpp"
#include "Player.hpp"
#include <iostream>
#include <vector>
#include "Game.hpp"

namespace ECE141 {
    
class MoveOption{//class 和structure的区别
public:
    const Piece *piece;
    Location location;
    int score;
    MoveOption(const Piece *aPiece, Location aLocation, int aScore): piece(aPiece),location(aLocation.row,aLocation.col),score(aScore) {};
    
};

class Game;

class ZongchengsPlayer: public Player{
public:
    ZongchengsPlayer();
    const PieceColor color;//the current player use
    static int pcount; //how many pieces created so far
    virtual bool takeTurn(Game &aGame, Orientation aDirection, std::ostream &aLog);
    
//    std::vector<int> testoptions ;
    
protected:
    bool isKingable(Game &aGame, const Piece &aPiece, const Location &aLocation);//变成canbeking//inplemented
    int moveScore(Game &aGame, const Piece &aPiece, int aDeltaX, int aDeltaY);
    bool oppoNearBy(Game &aGame,const Location &aLocation,const Location &anOldLoc);//inplemented
    const Piece* isOpponent(Game &aGame, const Location &aLoc, int aDeltaX, int aDeltaY, const Location &before);//inplemented
    bool capturedLoc(const Location &a, std::vector<Location> locs);//inplemented
    bool canJumpTo(Game &aGame, const Location &aLoc, int aDeltaX, int aDeltaY, std::vector<Location> locs);//inplemented

    Location* pieceHasJump(Game &aGame, const PieceKind pk, const Location &aLoc, PieceColor aColor, std::vector<Location> locs);
    
    bool neiberhood(Game &aGame, Orientation aDirection, std::ostream &aLog,std::vector<MoveOption> &options);
//    bool neiberhoodtest(Game &aGame, Orientation aDirection, std::ostream &aLog, std::vector<int> &testoptions);

    
    
    int moreY(PieceColor aColor);
    int lessY(PieceColor aColor);
    int moreX(PieceColor aColor);
    int lessX(PieceColor aColor);
    
    
    
    
};

}

#endif /* ZongchengsPlayer_hpp */
