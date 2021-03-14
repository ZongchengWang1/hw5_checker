//
//  Player.hpp
//  Checkers
//
//  Created by rick gessner on 2/22/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include "Piece.hpp"
#include <iostream>
#include <vector>

namespace ECE141 {
  
  class Game; //forward declare...  
  
  struct Move {
	  ECE141::Location startLocation;
	  ECE141::Location endLocation;
	  bool willConquer;
	  bool willKing;
	  bool getConquered;
	  int Jumps;
	
	  Move() : startLocation(Location(-1, -1)), endLocation(Location(-1, -1)),
	   willConquer(0), willKing(0), getConquered(0),
	   Jumps(0) {};
	  Move(Move &aMove) : startLocation(aMove.startLocation), endLocation(aMove.endLocation),
	   willConquer(aMove.willConquer), willKing(aMove.willKing), getConquered(aMove.getConquered),
	   Jumps(aMove.Jumps) {};
  };

  class Player {
  public:
                      Player();
    virtual bool      takeTurn(Game &aGame, Orientation aDirection, std::ostream &aLog);
	bool			  checkSpace(Game &aGame, const Piece *thePiece, std::vector<Move*> &possibleMoves, int 						 colOff, int rowOff);
    const PieceColor  color;
    static int        pcount; //how many created so far?
  };
}

#endif /* Player_hpp */
