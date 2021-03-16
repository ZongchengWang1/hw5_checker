//
//  ZZPlayer.cpp
//  Checkers
//

//

#include "ZZPlayer.hpp"
#include "Game.hpp"
#include <algorithm>

#define ILLEGALMOVE  -1000
#define DEFAULTBESTRATING -2000
#define JUMPBONUS 100
#define KINGBONUS 50
#define ESCAPEBONUS 20
#define SAFEBONUS 10

namespace ECE141 {
  
    int ZZPlayer::pcount = 0; //init our static member to track # of ZZPlayers...

    static PieceColor autoColor() { //auto assigns a color
        return (++ZZPlayer::pcount % 2) ? PieceColor::blue : PieceColor::gold;
    }
  
    ZZPlayer::ZZPlayer() : color(autoColor()) {}

    int ZZPlayer::sign(PieceColor color) {
        if (color == PieceColor::blue) {
            return -1;
        } else if (color == PieceColor::gold) {
            return 1;
        }
        return 0;
    }
    
    bool ZZPlayer::checkThreat(Game &aGame, Location &curLocation, Location &prevLocation, int colOff, int rowOff) {
        Location nextLocation(curLocation.row+rowOff, curLocation.col+colOff);
        const Tile *nextTile = aGame.getTileAt(nextLocation);
        if (nextTile) {
            const Piece* enemyPiece = nextTile->getPiece();
            if (enemyPiece && !enemyPiece->hasColor(color)) {
                Location jumpThreat(curLocation.row-rowOff, curLocation.col-colOff);
                const Tile *jumpThreatTile = aGame.getTileAt(jumpThreat);
                if (jumpThreatTile) {
                    const Piece* jumpThreatPiece = jumpThreatTile->getPiece();
                    if (jumpThreatPiece && (jumpThreat.col != prevLocation.col || jumpThreat.row != prevLocation.row)) {
                        return false;
                    } else {
                        return true;
                    }
                }
            }
        }
        return false;
    }

	bool ZZPlayer::tileHasEnemyKing(Game &aGame, Location &aLocation) {
		const Tile* aTile = aGame.getTileAt(aLocation);
		if (aTile) {
			const Piece* aPiece = aTile->getPiece();
			if (aPiece && aPiece->getKind()==PieceKind::king && !(aPiece->hasColor(color))) {
				return true;
			}
		}
		return false;
	}

    bool ZZPlayer::threatExists(Game &aGame, Location &curLocation, Location &prevLocation) {
        if (checkThreat(aGame, curLocation, prevLocation, sign(color)*1, sign(color)*1)) {
            return true;
        }
        
        if (checkThreat(aGame, curLocation, prevLocation, sign(color)*-1, sign(color)*1)) {
            return true;
        }
        
        Location enemyLocation(curLocation.row+sign(color)*1, curLocation.col+sign(color)-1);
		if (tileHasEnemyKing(aGame, enemyLocation)) {
			if (checkThreat(aGame, curLocation, prevLocation, sign(color)*1, sign(color)*-1)) {
				return true;
			}
		}
		
        Location enemyLocation2(curLocation.row+sign(color)*-1, curLocation.col+sign(color)-1);
		if (tileHasEnemyKing(aGame, enemyLocation)) {
			if (checkThreat(aGame, curLocation, prevLocation, sign(color)*-1, sign(color)*-1)) {
				return true;
			}
		}
        return false;
    };

    bool ZZPlayer::checkJump(Game &aGame, Location &curLocation, std::vector<Location> locations, int colOff, int rowOff) {
        Location nextLocation(curLocation.row+rowOff, curLocation.col+colOff);
        
        const Tile* nextTile = aGame.getTileAt(nextLocation);
        if (nextTile) {
            const Piece* nextPiece = nextTile->getPiece();
            bool locationCheck = false;
            
            for (Location check : locations){
                if ((nextLocation.row == check.row) && (nextLocation.col == check.col)) {
                    locationCheck = true;
                }
            }
            
            if (nextPiece && !locationCheck && !nextPiece->hasColor(color)) {
                Location jumpLocation(nextLocation.row+rowOff, nextLocation.col+colOff);
                
                const Tile *jumpTile = aGame.getTileAt(jumpLocation);
                if (jumpTile) {
                    return jumpTile->getPiece()==nullptr;
                }
            }
        }
        return false;
    }

    jumpContainer ZZPlayer::jumpExists(Game &aGame, Location &curLocation, std::vector<Location> locations, PieceKind kind, PieceColor color) {
        jumpContainer newJump;
        if(checkJump(aGame, curLocation, locations, sign(color)*1, sign(color)*1)) {
            newJump.row = sign(color)*1;
            newJump.col = sign(color)*1;
            return newJump;
        }
        
        if(checkJump(aGame, curLocation, locations, sign(color)*-1, sign(color)*1)) {
            newJump.row = sign(color)*1;
            newJump.col = sign(color)*-1;
            return newJump;
        }
        
        if (kind == PieceKind::king) {
            if(checkJump(aGame, curLocation, locations, sign(color)*1, sign(color)*-1)) {
                newJump.row = sign(color)*-1;
                newJump.col = sign(color)*1;
                return newJump;
            }
            
            if(checkJump(aGame, curLocation, locations, sign(color)*-1, sign(color)*-1)) {
                newJump.row = sign(color)*-1;
                newJump.col = sign(color)*-1;
                return newJump;
            }
        }
        newJump.row = 0;
        newJump.col = 0;
        newJump.runCond = false;
        return newJump;
    }
 
    bool ZZPlayer::checkKing(Game &aGame, const Piece &aPiece, Location &nextLocation) {
        if (aPiece.getKind() == PieceKind::pawn) {
            if (color == PieceColor::blue) {
                return (nextLocation.row == 0);
            } else if (color == PieceColor::gold) {
                return (nextLocation.row == 7);
            }
        }
        return false;
    }

	bool ZZPlayer::guaranteeIllegal(int columns, int rows) {
		if (columns > Game::kBoardHeight-1 || columns < 0) {
			return true;
		} else if (rows > Game::kBoardHeight-1 || rows < 0) {
			return true;
		}
		return false;
	}

    int ZZPlayer::rateMove(Game &aGame, const Piece &aPiece, int colOff, int rowOff) {
        std::vector<Location> locations;
        int rating = 0;
        
        Location curLocation = aPiece.getLocation();
        
        if (checkJump(aGame, curLocation, locations, colOff, rowOff)) {
            rating += JUMPBONUS;
            Location adjLocation(curLocation.row+rowOff, curLocation.col+colOff);
            locations.push_back(adjLocation);
            
            Location jumpLocation(adjLocation.row+rowOff, adjLocation.col+colOff);
			
			if (guaranteeIllegal(jumpLocation.col, jumpLocation.row)) {
				return ILLEGALMOVE;
			}
            
            PieceKind pieceKind = aPiece.getKind();
            jumpContainer newJump = jumpExists(aGame, jumpLocation, locations, pieceKind, color);
            while (newJump.runCond == true) {
                newJump = jumpExists(aGame, jumpLocation, locations, pieceKind, color);
                Location newAdj(jumpLocation.row+newJump.row, jumpLocation.col+newJump.col);
                locations.push_back(newAdj);
                jumpLocation.row += 2*newJump.row;
                jumpLocation.col += 2*newJump.col;
                rating += JUMPBONUS;
                if (pieceKind != PieceKind::king && checkKing(aGame, aPiece, jumpLocation)) {
                    pieceKind = PieceKind::king;
                    rating += KINGBONUS;
                }
            }
        } else {
			if (guaranteeIllegal(curLocation.col+colOff, curLocation.row+rowOff)) {
				return ILLEGALMOVE;
			}
            Location nextLocation(curLocation.row+rowOff, curLocation.col+colOff);
            const Tile *nextTile = aGame.getTileAt(nextLocation);
            if (nextTile) {
                const Piece* nextPiece = nextTile->getPiece();
                if (!nextPiece) {
                    if (checkKing(aGame, aPiece, nextLocation)) {
                        rating += KINGBONUS;
                    }
                    if (threatExists(aGame, nextLocation, curLocation)) {
                        rating -= ESCAPEBONUS;
                    } else {
                        rating += SAFEBONUS;
                    }
                } else {
                    return ILLEGALMOVE;
                }
            } else {
                return ILLEGALMOVE;
            }
        }
        return rating;
    }

	bool ZZPlayer::analyzeDirection(Game &aGame, const Piece &aPiece, int colOff, int rowOff, std::vector<Move> &moves) {
		int rating = rateMove(aGame, aPiece, colOff, rowOff);
		if (rating != ILLEGALMOVE) {
			Location *moveLocation;
			if (rating >= JUMPBONUS) {
				moveLocation = new Location(aPiece.getLocation().row+2*rowOff,
											aPiece.getLocation().col+2*colOff);
			} else {
				moveLocation = new Location(aPiece.getLocation().row+rowOff,
											aPiece.getLocation().col+colOff);
			}
			Move newMove(*moveLocation, &aPiece, rating);
			moves.push_back(newMove);
		}
		return true;
	}

	bool ZZPlayer::neighberhood(Game &aGame, Orientation aDirection, std::ostream &aLog, std::vector<Move> &moves){
		moves.clear();
		size_t theCount = aGame.countAvailablePieces(color);
		for (int pos = 0; pos < theCount; pos++) {
			if (const Piece *aPiece = aGame.getAvailablePiece(this->color, pos)) {
				analyzeDirection(aGame, *aPiece, sign(color)*1, sign(color)*1, moves);
				analyzeDirection(aGame, *aPiece, sign(color)*-1, sign(color)*1, moves);

				if (PieceKind::king == aPiece->getKind()) {
					analyzeDirection(aGame, *aPiece, sign(color)*1, sign(color)*-1, moves);
					analyzeDirection(aGame, *aPiece, sign(color)*-1, sign(color)*-1, moves);
				}
			}
		}
		return true;
	}

	bool ZZPlayer::doExtraJumps(Game &aGame, std::vector<Location> locations, Move aMove, jumpContainer newJump) {
		while (aMove.rating >= JUMPBONUS && (newJump.runCond == true)) {
			newJump = jumpExists(aGame, aMove.endLocation, locations, aMove.piece->getKind(), color);
			if (newJump.col != 0 && newJump.row != 0) {
				aMove.endLocation.row += 2 * newJump.row;
				aMove.endLocation.col += 2 * newJump.col;
				aGame.movePieceTo(*(aMove.piece), aMove.endLocation);
			}
		}
		return true;
	}

    bool ZZPlayer::takeTurn(Game &aGame, Orientation aDirection, std::ostream &aLog) {
        std::vector<Move> moves;
		
        bool getMO = neighberhood(aGame, aDirection, aLog,moves);
        
        if ((getMO==false) || moves.size() == 0) {
            return false; //if you return false, you forfeit!
        }
        
        int bestRating = DEFAULTBESTRATING;
        int curRating = 0;
        
        for (auto move : moves) {
            curRating = move.rating;
            if (curRating > bestRating) {
                bestRating = curRating;
            }
        }
        
        std::vector<Move> bestMoves;
        for (auto move : moves) {
            if (move.rating == bestRating) {
                bestMoves.push_back(move);
            }
        }
        
        int numMoves = (int)bestMoves.size();
        int random = std::rand() % numMoves;
        Move bestMove = bestMoves[random];
        
        aGame.movePieceTo(*(bestMove.piece), bestMove.endLocation);

        std::vector<Location> locations;
        jumpContainer newJump = jumpExists(aGame, bestMove.endLocation, locations, bestMove.piece->getKind(), color);
		doExtraJumps(aGame, locations, bestMove, newJump);
		
        return true;
	}
}
