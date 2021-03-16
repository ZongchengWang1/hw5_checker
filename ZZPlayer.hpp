
//
//  ZZPlayer.hpp
//  Checkers
//
//  Created by rick gessner on 2/22/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#ifndef ZZPlayer_hpp
#define ZZPlayer_hpp

#include <stdio.h>

#include "Piece.hpp"
#include <iostream>
#include <vector>
#include "Player.hpp"

namespace ECE141 {
  
    class Game; //forward declare...
  
    struct Move {
        Location endLocation;
        const Piece *piece;
        int rating;
        
        Move(Location endLocation, const Piece *piece, int rating) : endLocation(endLocation.row, endLocation.col), piece(piece), rating(rating) {};
    };

	struct jumpContainer {
		int col;
		int row;
		bool runCond = true;
	};

    class ZZPlayer: public Player {
    public:
                          ZZPlayer();
        virtual bool      takeTurn(Game &aGame, Orientation aDirection, std::ostream &aLog);

        const PieceColor  color;
        static int        pcount; //how many created so far?
        
        int sign(PieceColor color);
        
        bool checkThreat(Game &aGame, Location &curLocation, Location &prevLocation, int colOff, int rowOff);
		bool tileHasEnemyKing(Game &aGame, Location &aLocation);
		bool threatExists(Game &aGame, Location &curLocation, Location &prevLocation);
		
        bool checkJump(Game &aGame, Location &curLocation, std::vector<Location> locations, int colOff, int rowOff);
        jumpContainer jumpExists(Game &aGame, Location &curLocation, std::vector<Location> locations, PieceKind kind, PieceColor color);
        
        bool checkKing(Game &aGame, const Piece &aPiece, Location &nextLocation);
        
		bool guaranteeIllegal(int columns, int rows);
		
        int rateMove(Game &aGame, const Piece &aPiece, int colOff, int rowOff);
		bool analyzeDirection(Game &aGame, const Piece &aPiece, int colOff, int rowOff, std::vector<Move> &moves);
		bool neighberhood(Game &aGame, Orientation aDirection, std::ostream &aLog,std::vector<Move> &moves);
		bool doExtraJumps(Game &aGame, std::vector<Location> locations, Move aMove, jumpContainer newJump);
    };

}

#endif /* ZZPlayer_hpp */
