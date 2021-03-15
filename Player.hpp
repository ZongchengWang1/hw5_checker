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
		Location endLocation;
		const Piece *piece;
		int rating;
		
		Move(Location endLocation, const Piece *piece, int rating) : endLocation(endLocation.row, endLocation.col), piece(piece), rating(rating) {};
		
//		Move(Move &aMove) : endLocation(aMove.endLocation), piece(aMove.piece), rating(aMove.rating) {};
	};

//struct MoveOption {
//	const Piece   *piece;
//	Location      location;
//	int           score;
//
//	MoveOption(const Piece *p, Location l, int s):piece(p), location(l.row, l.col), score(s){};
//};

	struct twoInts {
		int col;
		int row;
		bool runCond = true;
	};

	class Player {
	public:
						  Player();
		virtual bool      takeTurn(Game &aGame, Orientation aDirection, std::ostream &aLog);
		const PieceColor  color;
		static int        pcount; //how many created so far?
		
		int sign(PieceColor color);
		
		bool checkThreat(Game &aGame, Location &curLocation, Location &prevLocation, int colOff, int rowOff);
		bool threatExists(Game &aGame, Location &curLocation, Location &prevLocation);
		
		bool checkJump(Game &aGame, Location &curLocation, std::vector<Location> locations, int colOff, int rowOff);
		twoInts jumpExists(Game &aGame, Location &curLocation, std::vector<Location> locations, PieceKind kind, PieceColor color);
		
		bool checkKing(Game &aGame, const Piece &aPiece, Location &nextLocation);
		
		int rateMove(Game &aGame, const Piece &aPiece, int colOff, int rowOff);
	};

}

#endif /* Player_hpp */
