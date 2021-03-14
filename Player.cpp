//
//  Player.cpp
//  Checkers
//
//  Created by rick gessner on 2/22/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#include "Player.hpp"
#include "Game.hpp"

namespace ECE141 {
  
  int Player::pcount = 0; //init our static member to track # of players...
  
  static PieceColor autoColor() { //auto assigns a color
    return (++Player::pcount % 2) ? PieceColor::blue : PieceColor::gold;
  }
  
  Player::Player() : color(autoColor()) {}
 
  bool Player::takeTurn(Game &aGame, Orientation aDirection, std::ostream &aLog) {
    size_t theCount=aGame.countAvailablePieces(color);
    for(int pos=0;pos<theCount;pos++) {
      /*
       //NOTICE HOW WE CHECK FOR CAPTURED PIECES?
       if(const Piece *thePiece = aGame.getAvailablePiece(this->color, pos)) {
       
         //add logic here to find your best move, then move ONE piece per turn...
       
         aGame.movePieceTo(*thePiece, Location(1,2)); -- choose wisely...
       }
      */
		
		// Create Neighborhood Function
		if (pos == 1) {
			if(const Piece *thePiece = aGame.getAvailablePiece(this->color, pos)) {
			  std::cout << thePiece->getLocation().row << ", " <<thePiece->getLocation().col << "\n";
			  Location PieceLocation = thePiece->getLocation();
			  Location tile1(PieceLocation.col+1, PieceLocation.row+1);
			  Location tile2(PieceLocation.col-1, PieceLocation.row-1);
			  Location tile3(PieceLocation.col+1, PieceLocation.row-1);
			  Location tile4(PieceLocation.col-1, PieceLocation.row+1);
			  //aGame.movePieceTo(*thePiece, Location(4,3));
			}
		}
//		if (pos == 1) {
//			if(const Piece *thePiece = aGame.getAvailablePiece(this->color, pos)) {
//			  std::cout << thePiece->hasColor(PieceColor::blue) << "\n";
//			  std::cout << thePiece->getLocation().row << ", " <<thePiece->getLocation().col << "\n";
//			  //add logic here to find your best move, then move ONE piece per turn...
//
//			  aGame.movePieceTo(*thePiece, Location(4,3));
//			}
//		}
    }
    return false; //if you return false, you forfeit!
  }
}
