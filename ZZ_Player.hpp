//
//  ZZ_Player.hpp
//  HW5_Partner
//
//  Created by Zi Deng on 3/14/21.
//

#ifndef ZZ_Player_hpp
#define ZZ_Player_hpp

#include <stdio.h>
#include "Piece.hpp"
#include "Player.hpp"
#include <iostream>
#include <vector>

namespace ECE141 {
	
	struct MoveOption {
		const Piece   *piece;
		Location      location;
		int           score;

		MoveOption(const Piece *p, Location l, int s):piece(p), location(l.row, l.col), score(s){};
	};

	class Game; //forward declare...

	class ZZPlayer : public Player {
	public:
		ZZPlayer();
		const PieceColor  color;
		static int        pcount; //how many created so far?
		virtual bool takeTurn(Game &aGame, Orientation aDirection, std::ostream &aLog);
	protected:
		bool isKingable(Game &aGame, const Piece &aPiece, const Location &aDest);
		int moveScore(Game &aGame, const Piece &aPiece, int aDeltaX, int aDeltaY);
		bool oppoNearBy(Game &aGame,const Location &aLoc, const Location& before);
		const Piece* isOpponent(Game &aGame, const Location &aLoc, int aDeltaX, int aDeltaY, const Location &before);
		bool capturedLoc(const Location &a, std::vector<Location> locs);
		bool canJumpTo(Game &aGame, const Location &aLoc, int aDeltaX, int aDeltaY, std::vector<Location> locs);
		Location* pieceHasJump(Game &aGame, const PieceKind pk, const Location &aLoc, PieceColor aColor, std::vector<Location> locs);

		int moreY(PieceColor aColor);
		int lessY(PieceColor aColor);
		int moreX(PieceColor aColor);
		int lessX(PieceColor aColor);
	};
}

#endif /* ZZ_Player_hpp */
