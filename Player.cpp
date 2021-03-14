//
//  Player.cpp
//  Checkers
//
//  Created by rick gessner on 2/22/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#include "Player.hpp"
#include "Game.hpp"
//#include "Move.hpp"
//#include <vector>

namespace ECE141 {
  
  int Player::pcount = 0; //init our static member to track # of players...
  
  static PieceColor autoColor() { //auto assigns a color
    return (++Player::pcount % 2) ? PieceColor::blue : PieceColor::gold;
  }
  
  Player::Player() : color(autoColor()) {};

  bool Player::checkSpace(Game &aGame, const Piece *thePiece, std::vector<Move*> &possibleMoves, int colOff, int rowOff) {
	  Location PieceLocation = thePiece->getLocation();
	  Move aMove;

	  //Top right case

	  Location tempLocation(PieceLocation.row+rowOff, PieceLocation.col+colOff);

	  if (tempLocation.col < 0 || tempLocation.col > 7) {
		  return false;
	  } else if (tempLocation.row < 0 || tempLocation.row > 7) {
		  return false;
	  }

	  const Tile *tempTile = aGame.getTileAt(tempLocation);

	  if (tempTile->getPiece()) {
		  //Consider jump case
		  Location tempLocation2(tempLocation.row+rowOff, tempLocation.col+colOff);
		  const Tile *tempTile2 = aGame.getTileAt(tempLocation2);
		  if (tempTile2->getPiece()) {
			  return false;
		  } else {
			  if (tempLocation2.row == 0 && thePiece->getKind() != PieceKind::king) {
				  aMove.willKing = true;
			  }

			  aMove.Jumps++;

			  aMove.willConquer = true;

			  aMove.startLocation = Location(PieceLocation.row, PieceLocation.col);
			  aMove.endLocation = Location(tempLocation2.row, tempLocation2.col);

			  //possibleMoves.push_back(&aMove);
			  Move* moveCopy = new Move;
			  moveCopy = &aMove;
			  possibleMoves.push_back(moveCopy);
		  }
	  } else {
		  if (tempLocation.row == 0 && thePiece->getKind() != PieceKind::king) {
			  aMove.willKing = true;
		  }

		  aMove.startLocation = Location(PieceLocation.row, PieceLocation.col);
		  aMove.endLocation = Location(tempLocation.row, tempLocation.col);

//		  Event* eventCopy = new Event;
//		  *eventCopy = *anEvent;
//
//		  events.push_back(eventCopy);
		  Move* moveCopy = new Move;
		  moveCopy = &aMove;
		  possibleMoves.push_back(moveCopy);
		  //possibleMoves.push_back(&aMove);
	  }
	  return true;
  }

	
  bool Player::takeTurn(Game &aGame, Orientation aDirection, std::ostream &aLog) {
    size_t theCount=aGame.countAvailablePieces(color);
    for(int pos=0;pos<theCount;pos++) {
		// Create Neighborhood Function
		if (pos == 1) {
			
			std::vector<std::vector<Move*>*> allMoves;
			if (const Piece *thePiece = aGame.getAvailablePiece(this->color, pos)) {
				
				
				std::vector<Move*> possibleMoves;
				
				int dir = (this->color == PieceColor::blue) ? -1 : 1;
				
				checkSpace(aGame, thePiece, possibleMoves, 1, dir * 1);
				checkSpace(aGame, thePiece, possibleMoves, -1, dir * 1);
				if (thePiece->getKind() == PieceKind::king) {
					checkSpace(aGame, thePiece, possibleMoves, 1, dir * -1);
					checkSpace(aGame, thePiece, possibleMoves, -1, dir * -1);
				}
				
				
				//allMoves.push_back(&possibleMoves);
				std::cout << "test" << "\n";
			}
		}
    }
    return false; //if you return false, you forfeit!
  }
}
