//
//  ZongchengsPlayer.cpp
//  checker
//
//  Created by apple on 3/12/21.
//

#include "ZongchengsPlayer.hpp"

namespace ECE141 {
int ZongchengsPlayer::pcount = 0;
static PieceColor autoColor1(){
    return (++ZongchengsPlayer::pcount %2) ? PieceColor::blue : PieceColor::gold ;//要弄清楚哪种颜色先走,好像是blue
    
}

ZongchengsPlayer:: ZongchengsPlayer() :color(autoColor1()){}


bool  ZongchengsPlayer::isKingable(Game &aGame, const Piece &aPiece, const Location &aLocation){
    if (PieceKind::pawn == aPiece.getKind()){
        return color == PieceColor::blue ? 0 == aLocation.row: (aGame.kBoardHeight - 1) == aLocation.row;
    }
    return false;
    
}

const Piece* ZongchengsPlayer::isOpponent(Game &aGame, const Location &aLoc, int aDeltaX, int aDeltaY, const Location &before){
    Location near(aLoc.row+aDeltaY,aLoc.col+aDeltaX);
    if (const Tile *theTile=aGame.getTileAt(near)){
        const Piece* const thePiece=theTile->getPiece();
        if(thePiece && !thePiece->hasColor(color)) {
//            std::cout << "cur loc: " <<  aLoc.row << " " << aLoc.col << std::endl;
//            std::cout << "opponent: " <<  thePiece->getLocation().row << " " << thePiece->getLocation().col << std::endl;
            Location theJump(aLoc.row-aDeltaY,aLoc.col-aDeltaX);
//            std::cout << "jump: " <<  theJump.row << " " << theJump.col << std::endl;
            const Tile *jumpTile=aGame.getTileAt(theJump);
            if (jumpTile != nullptr) {
                const Piece* const jumpPiece=jumpTile->getPiece();
                if (jumpPiece &&( theJump.row != before.row || theJump.col != before.col))
                    return nullptr;
                else
                    return theTile->getPiece();
            }
            else{
                return nullptr;
            }
        }
        else{
            return nullptr;
        }
    }
    else{
        return nullptr;
    }

}

bool ZongchengsPlayer::oppoNearBy(Game &aGame, const Location &aLocation, const Location &anOldLoc){
    if(isOpponent(aGame, aLocation, lessX(color), moreY(color), anOldLoc) != nullptr) {
        return true;
    }
    if(isOpponent(aGame, aLocation, moreX(color), moreY(color), anOldLoc) != nullptr) {
        return true;
    }
    const Piece* oppo;
    if((oppo = isOpponent(aGame, aLocation, lessX(color), lessY(color), anOldLoc)) != nullptr && oppo->getKind() == PieceKind::king) {
        return true;
    }
    if((oppo = isOpponent(aGame, aLocation, moreX(color), lessY(color), anOldLoc)) != nullptr && oppo->getKind() == PieceKind::king) {
        return true;
    }
    return false;
}

bool ZongchengsPlayer::capturedLoc(const Location &a, std::vector<Location> locs){
    for (Location b : locs){
        if ((a.row == b.row) && (a.col == b.col))
            return true;
    }
    return false;
}

bool ZongchengsPlayer::canJumpTo(Game &aGame, const Location &aLoc, int aDeltaX, int aDeltaY, std::vector<Location> locs){
    Location theStep(aLoc.row+aDeltaY,aLoc.col+aDeltaX);
    if(const Tile *theTile=aGame.getTileAt(theStep)) {
        const Piece* const thePiece=theTile->getPiece();
        if(thePiece && !capturedLoc(thePiece->getLocation(), locs) && !thePiece->hasColor(color)) {
            Location theJump(theStep.row+aDeltaY,theStep.col+aDeltaX);
            if(const Tile *theTile=aGame.getTileAt(theJump)) {
                return theTile->getPiece()==nullptr;
            }
        }
    }
    return false;
}

Location* ZongchengsPlayer::pieceHasJump(Game &aGame, const PieceKind pk, const Location &aLoc, PieceColor aColor, std::vector<Location> locs){
    if(canJumpTo(aGame, aLoc, lessX(aColor), moreY(aColor), locs)) {
        return new Location(moreY(aColor), lessX(aColor));
    }

    if(canJumpTo(aGame, aLoc, moreX(aColor), moreY(aColor), locs)) {
        return new Location(moreY(aColor), moreX(aColor));
    }

    if (PieceKind::king==pk) {
        if(canJumpTo(aGame, aLoc, lessX(aColor), lessY(aColor), locs)) {
            return new Location(lessY(aColor), lessX(aColor));
        }

        if(canJumpTo(aGame, aLoc, moreX(aColor), lessY(aColor), locs)) {
            return new Location(lessY(aColor), moreX(aColor));
        }
    }
    return nullptr;
}

int ZongchengsPlayer::moveScore(Game &aGame, const Piece &aPiece, int aDeltaX, int aDeltaY){
    int score = 0;
    std::vector<Location> locs;
    Location curLoc = aPiece.getLocation();
    if (canJumpTo(aGame, aPiece.getLocation(), aDeltaX, aDeltaY, locs)){
        score += 100;
        Location theStep(curLoc.row+aDeltaY,curLoc.col+aDeltaX);
        locs.push_back(theStep);
        Location theJump(curLoc.row+2*aDeltaY,curLoc.col+2*aDeltaX);
        PieceKind tmp = aPiece.getKind();
        Location* change;
        while ((change = pieceHasJump(aGame, tmp, theJump, color, locs)) != nullptr){
            Location newStep(theJump.row + change->row, theJump.col + change->col);
            locs.push_back(newStep);
            theJump.row += 2 * change->row;
            theJump.col += 2 * change->col;
            score += 100;
            if (tmp != PieceKind::king && isKingable(aGame, aPiece, theJump)) {
                tmp = PieceKind::king;
                score += 20;
            }
        }
    }
    else{
        Location noNeed(-1, -1);
        if (oppoNearBy(aGame, curLoc, noNeed)){
            score += 15;
        }

        Location theStep(curLoc.row+aDeltaY,curLoc.col+aDeltaX);
        if(const Tile *theTile=aGame.getTileAt(theStep)) {
            const Piece* const thePiece=theTile->getPiece();
            if (!thePiece){
                if (isKingable(aGame, aPiece, theStep))
                    score += 20;

                if (oppoNearBy(aGame, theStep, curLoc)){
                    score -= 15;
                }
                else{
                    score += 10;
                }
            }
            else{
                return INT32_MIN;
            }
        }
        else{
            return INT32_MIN;
        }
    }

    return score;

}




int ZongchengsPlayer::moreY(PieceColor aColor) {return PieceColor::blue==aColor ? -1 : 1;}
int ZongchengsPlayer::lessY(PieceColor aColor) {return PieceColor::blue==aColor ? 1 : -1;}
int ZongchengsPlayer::moreX(PieceColor aColor) {return PieceColor::blue==aColor ? 1 : -1;}
int ZongchengsPlayer::lessX(PieceColor aColor) {return PieceColor::blue==aColor ? -1 : 1;}

bool ZongchengsPlayer::neiberhood(Game &aGame, Orientation aDirection, std::ostream &aLog,std::vector<MoveOption> &options){
    
    options.clear();
    size_t theCount = aGame.countAvailablePieces(color);
//    std::vector<MoveOption> options1;
//    std::vector<MoveOption> &options = options1;
    for (int pos = 0; pos < theCount; pos++) {
        //NOTICE HOW WE CHECK FOR CAPTURED PIECES?
        if (const Piece *thePiece = aGame.getAvailablePiece(this->color, pos)) {
            int score;
            if ((score = moveScore(aGame, *thePiece, lessX(color), moreY(color))) != INT32_MIN) {
                Location *newLoc;
                if (score >= 100)
                    newLoc = new Location(thePiece->getLocation().row + 2 * moreY(color),
                                          thePiece->getLocation().col + 2 * lessX(color));
                else
                    newLoc = new Location(thePiece->getLocation().row + moreY(color),
                                          thePiece->getLocation().col + lessX(color));

//                std::cout << "old location:" << thePiece->getLocation().row << " " << thePiece->getLocation().col
//                          << std::endl;
//                std::cout << "new location:" << newLoc->row << " " << newLoc->col << std::endl;
//                std::cout << "score:" << score << std::endl;
                MoveOption newMove(thePiece, *newLoc, score);
                options.push_back(newMove);
            }

            if ((score = moveScore(aGame, *thePiece, moreX(color), moreY(color))) != INT32_MIN) {
                Location *newLoc;
                if (score >= 100)
                    newLoc = new Location(thePiece->getLocation().row + 2 * moreY(color),
                                          thePiece->getLocation().col + 2 * moreX(color));
                else
                    newLoc = new Location(thePiece->getLocation().row + moreY(color),
                                          thePiece->getLocation().col + moreX(color));

//                std::cout << "old location:" << thePiece->getLocation().row << " " << thePiece->getLocation().col
//                          << std::endl;
//                std::cout << "new location:" << newLoc->row << " " << newLoc->col << std::endl;
//                std::cout << "score:" << score << std::endl;
                MoveOption newMove(thePiece, *newLoc, score);
                options.push_back(newMove);
            }

            if (PieceKind::king == thePiece->getKind()) {
                if ((score = moveScore(aGame, *thePiece, lessX(color), lessY(color))) != INT32_MIN) {
                    Location *newLoc;
                    if (score >= 100)
                        newLoc = new Location(thePiece->getLocation().row + 2 * lessY(color),
                                              thePiece->getLocation().col + 2 * lessX(color));
                    else
                        newLoc = new Location(thePiece->getLocation().row + lessY(color),
                                              thePiece->getLocation().col + lessX(color));

                    MoveOption newMove(thePiece, *newLoc, score);
                    options.push_back(newMove);
                }

                if ((score = moveScore(aGame, *thePiece, moreX(color), lessY(color))) != INT32_MIN) {
                    Location *newLoc;
                    if (score >= 100)
                        newLoc = new Location(thePiece->getLocation().row + 2 * lessY(color),
                                              thePiece->getLocation().col + 2 * moreX(color));
                    else
                        newLoc = new Location(thePiece->getLocation().row + lessY(color),
                                              thePiece->getLocation().col + moreX(color));

                    MoveOption newMove(thePiece, *newLoc, score);
                    options.push_back(newMove);
                }
            }

        }
    }
    
    
    
    
    
    
    return true;
    
}

//bool ZongchengsPlayer::neiberhoodtest(Game &aGame, Orientation aDirection, std::ostream &aLog, std::vector<int> &testoptions){
////    std::vector<int> testoptions1;
//    testoptions.push_back(3);
////    std::vector<int> &testoptions = testoptions1;
//    return true;
//}

bool ZongchengsPlayer:: takeTurn(Game &aGame, Orientation aDirection, std::ostream &aLog){
    
//
//    size_t theCount = aGame.countAvailablePieces(color);
//    std::vector<MoveChoice> choices;
//    for (int pos = 0; pos < theCount; pos ++){
//        //check captured pieces
//
//
//        if (const Piece *thePiece = aGame.getAvailablePiece(this->color, pos)){
//            int  score;
//            //要看看为什么怎么设置，看看score的值是多少
//            if((score = moveScore(aGame, *thePiece, lessX(color), moreY(color))) != INT32_MIN){
//
//            }
//        }
//    }
    std::vector<MoveOption> options;
    bool getMO= neiberhood(aGame, aDirection, aLog,options);
//    std::vector<int> testoptions;

//    bool getnt = neiberhoodtest(aGame, aDirection, aLog,testoptions);

    
    
    
    if (options.size() == 0)
        return false;

    std::sort(options.begin(), options.end(), [](MoveOption a, MoveOption b) { return a.score > b.score; });
    int maxScore = options[0].score;
    int index = 0;
    for (MoveOption option : options) {
        if (option.score == maxScore)
            index++;
        else
            break;
    }

    int rand = std::rand() % index;
    MoveOption maxMove = options[rand];

//    std::cout << "------ turn:" << static_cast<std::underlying_type<PieceColor>::type>(color) << " "
//              << maxMove.piece->getLocation().row << maxMove.piece->getLocation().col << " " << maxMove.location.row
//              << maxMove.location.col << std::endl;
    aGame.movePieceTo(*(maxMove.piece), maxMove.location);

    Location *change;
    std::vector<Location> locs;
    while (maxMove.score >= 100 &&
           (change = pieceHasJump(aGame, maxMove.piece->getKind(), maxMove.location, color, locs)) != nullptr) {
        maxMove.location.row += 2 * change->row;
        maxMove.location.col += 2 * change->col;
        aGame.movePieceTo(*(maxMove.piece), maxMove.location);
    }
//    std::cout << "end" << std::endl;
    return true;
    
   
    
    
}

}
