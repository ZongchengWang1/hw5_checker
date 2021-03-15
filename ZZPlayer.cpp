//
//  ZZPlayer.cpp
//  Checkers
//

//

#include "ZZPlayer.hpp"
#include "Game.hpp"
#include <algorithm>

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

    bool ZZPlayer::threatExists(Game &aGame, Location &curLocation, Location &prevLocation) {
        if (checkThreat(aGame, curLocation, prevLocation, sign(color)*1, sign(color)*1)) {
            return true;
        }
        
        if (checkThreat(aGame, curLocation, prevLocation, sign(color)*-1, sign(color)*1)) {
            return true;
        }
        
        Location enemyLocation(-1, sign(color)-1);
        if(const Tile* enemyTile = aGame.getTileAt(enemyLocation)) {
            if(const Piece* enemyPiece = enemyTile->getPiece()) {
                if(enemyPiece->getKind() == PieceKind::king) {
                    if (checkThreat(aGame, curLocation, prevLocation, sign(color)*1, sign(color)*-1)) {
                        return true;
                    }
                }
            }
        }
        Location enemyLocation2(1, sign(color)-1);
        if(const Tile* enemyTile2 = aGame.getTileAt(enemyLocation2)) {
            if(const Piece* enemyPiece2 = enemyTile2->getPiece()){
                if(enemyPiece2->getKind() == PieceKind::king) {
                    if (checkThreat(aGame, curLocation, prevLocation, sign(color)*-1, sign(color)*-1)) {
                        return true;
                    }
                }
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
                
                if (jumpLocation.col > 7 || jumpLocation.col < 0 ) {
                    return false;
                } else if ((jumpLocation.row > 7 || jumpLocation.row < 0 )) {
                    return false;
                }
                
                const Tile *jumpTile = aGame.getTileAt(jumpLocation);
                if (jumpTile) {
                    return jumpTile->getPiece()==nullptr;
                }
            }
        }
        return false;
    }

    twoInts ZZPlayer::jumpExists(Game &aGame, Location &curLocation, std::vector<Location> locations, PieceKind kind, PieceColor color) {
        twoInts ints;
        if(checkJump(aGame, curLocation, locations, sign(color)*1, sign(color)*1)) {
            ints.row = sign(color)*1;
            ints.col = sign(color)*1;
            return ints;
        }
        
        if(checkJump(aGame, curLocation, locations, sign(color)*-1, sign(color)*1)) {
            ints.row = sign(color)*1;
            ints.col = sign(color)*-1;
            return ints;
        }
        
        if (kind == PieceKind::king) {
            if(checkJump(aGame, curLocation, locations, sign(color)*1, sign(color)*-1)) {
                ints.row = sign(color)*-1;
                ints.col = sign(color)*1;
                return ints;
            }
            
            if(checkJump(aGame, curLocation, locations, sign(color)*-1, sign(color)*-1)) {
                ints.row = sign(color)*-1;
                ints.col = sign(color)*-1;
                return ints;
            }
        }
        ints.row = 0;
        ints.col = 0;
        ints.runCond = false;
        return ints;
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

    int ZZPlayer::rateMove(Game &aGame, const Piece &aPiece, int colOff, int rowOff) {
        std::vector<Location> locations;
        int rating = 0;
        
        Location curLocation = aPiece.getLocation();
        
        if (curLocation.col+colOff > 7 || curLocation.col+colOff < 0 ) {
            return -1000;
        } else if ((curLocation.row+rowOff > 7 || curLocation.row+rowOff < 0 )) {
            return -1000;
        }
        
        if (checkJump(aGame, curLocation, locations, colOff, rowOff)) {
            rating += 100;
            Location adjLocation(curLocation.row+rowOff, curLocation.col+colOff);
            locations.push_back(adjLocation);
            
            Location jumpLocation(adjLocation.row+rowOff, adjLocation.col+colOff);
            
            if (jumpLocation.col > 7 || jumpLocation.col < 0 ) {
                return -1000;
            } else if ((jumpLocation.row > 7 || jumpLocation.row < 0 )) {
                return -1000;
            }
            
            PieceKind pieceKind = aPiece.getKind();
            twoInts ints = jumpExists(aGame, jumpLocation, locations, pieceKind, color);
            while (ints.runCond == true) {
                ints = jumpExists(aGame, jumpLocation, locations, pieceKind, color);
                Location newAdj(jumpLocation.row+ints.row, jumpLocation.col+ints.col);
                locations.push_back(newAdj);
                jumpLocation.row += 2*ints.row;
                jumpLocation.col += 2*ints.col;
                rating += 100;
                if (pieceKind != PieceKind::king && checkKing(aGame, aPiece, jumpLocation)) {
                    pieceKind = PieceKind::king;
                    rating += 20;
                }
            }
        } else {
            Location below(-1, -1);
            if (threatExists(aGame, curLocation, below)) {
                rating += 20;
            }
            
            Location nextLocation(curLocation.row+rowOff, curLocation.col+colOff);
            const Tile *nextTile = aGame.getTileAt(nextLocation);
            if (nextTile) {
                const Piece* nextPiece = nextTile->getPiece();
                if (!nextPiece) {
                    if (checkKing(aGame, aPiece, nextLocation)) {
                        rating += 30;
                    }
                    
                    if (threatExists(aGame, nextLocation, curLocation)) {
                        rating -= 20;
                    } else {
                        rating += 10;
                    }
                } else {
                    return -1000;
                }
            } else {
                return -1000;
            }
            
        }
        return rating;
    }


    bool ZZPlayer::takeTurn(Game &aGame, Orientation aDirection, std::ostream &aLog) {
        size_t theCount=aGame.countAvailablePieces(color);
        
        std::vector<Move> moves;
        for(int pos=0;pos<theCount;pos++) {
            const Piece *aPiece = aGame.getAvailablePiece(this->color, pos);
            if (aPiece) {
                // Case 1
                int rating = rateMove(aGame, *aPiece, sign(color)*1, sign(color)*1);
                if (rating != -1000) {
                    Location *moveLocation;
                    if (rating >= 100) {
                        moveLocation = new Location(aPiece->getLocation().row+2*sign(color)*1, aPiece->getLocation().col+2*sign(color)*1);
                    } else {
                        moveLocation = new Location(aPiece->getLocation().row+sign(color)*1, aPiece->getLocation().col+sign(color)*1);
                    }
                    
                    Move newMove(*moveLocation, aPiece, rating);
                    moves.push_back(newMove);
                }
                
                rating = rateMove(aGame, *aPiece, sign(color)*-1, sign(color)*1);
                if (rating != -1000) {
                    Location *moveLocation;
                    if (rating >= 100) {
                        moveLocation = new Location(aPiece->getLocation().row+2*sign(color)*1, aPiece->getLocation().col+2*sign(color)*-1);
                    } else {
                        moveLocation = new Location(aPiece->getLocation().row+sign(color)*1, aPiece->getLocation().col+sign(color)*-1);
                    }
                    
                    Move newMove(*moveLocation, aPiece, rating);
                    moves.push_back(newMove);
                }

                if (aPiece->getKind() == PieceKind::king) {
                    rating = rateMove(aGame, *aPiece, sign(color)*1, sign(color)*-1);
                    if (rating != -1000) {
                        Location *moveLocation;
                        if (rating >= 100) {
                            moveLocation = new Location(aPiece->getLocation().row+2*sign(color)*-1, aPiece->getLocation().col+2*sign(color)*1);
                        } else {
                            moveLocation = new Location(aPiece->getLocation().row+sign(color)*-1, aPiece->getLocation().col+sign(color)*1);
                        }
                        
                        Move newMove(*moveLocation, aPiece, rating);
                        moves.push_back(newMove);
                    }
                    
                    rating = rateMove(aGame, *aPiece, sign(color)*-1, sign(color)*-1);
                    if (rating != -1000) {
                        Location *moveLocation;
                        if (rating >= 100) {
                            moveLocation = new Location(aPiece->getLocation().row+2*sign(color)*-1, aPiece->getLocation().col+2*sign(color)*-1);
                        } else {
                            moveLocation = new Location(aPiece->getLocation().row+sign(color)*-1, aPiece->getLocation().col+sign(color)*-1);
                        }
                        
                        Move newMove(*moveLocation, aPiece, rating);
                        moves.push_back(newMove);
                    }
                }
            }
        }
        if (moves.size() == 0) {
            return false; //if you return false, you forfeit!
        }
        
        int bestRating = -2000;
        int curRating = 0;
        
        for (auto move : moves) {
            curRating = move.rating;
            if (curRating > bestRating) {
                bestRating = curRating;
            }
        }
        
        std::vector<Move> bestMoves;
        for (auto move: moves) {
            if (move.rating == bestRating) {
                bestMoves.push_back(move);
            }
        }
        
        
        
        int numMoves = (int)bestMoves.size();
        int random = std::rand() % numMoves;
        Move bestMove = bestMoves[random];
        
        
        aGame.movePieceTo(*(bestMove.piece), bestMove.endLocation);

        std::vector<Location> locs;
        twoInts ints = jumpExists(aGame, bestMove.endLocation, locs, bestMove.piece->getKind(), color);
        while (bestMove.rating >= 100 &&
               (ints.runCond == true)) {
            ints = jumpExists(aGame, bestMove.endLocation, locs, bestMove.piece->getKind(), color);
            if (ints.col != 0 && ints.row != 0) {
                bestMove.endLocation.row += 2 * ints.row;
                bestMove.endLocation.col += 2 * ints.col;
                aGame.movePieceTo(*(bestMove.piece), bestMove.endLocation);
            }
        }
        return true;
    }
}