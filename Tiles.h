#ifndef TILES_H
#define TILES_H

#define TILE_X 28
#define TILE_Y 31
#define TILE_LEN 24
#define TILE_HLF 12
#define TILE_QTR 6
#define TILE_ETH 3

// Tile Types
#define TILE_ID_EMPTY 0
#define TILE_ID_COIN_SMALL 1
#define TILE_ID_COIN_LARGE 2
#define TILE_ID_WALL 3
#define TILE_ID_GHOST 4

// Board template
#define _BE TILE_ID_EMPTY
#define _BS TILE_ID_COIN_SMALL
#define _BL TILE_ID_COIN_LARGE
#define _BW TILE_ID_WALL
#define _BG TILE_ID_GHOST

Coord toScreen(const Coord gpos)
{
    return coordAddi(coordMuli(gpos, TILE_LEN), TILE_HLF);
}

bool inMap(const Coord pos)
{
    return pos.x >= 0 && pos.y >= 0 && pos.x < TILE_X && pos.y < TILE_Y;
}

void drawWall(const Coord pos, int board[28][31])
{
    if(!inMap(pos) || board[pos.x][pos.y] != TILE_ID_WALL)
        return;

    const Coord screenPos = toScreen(pos);
    setColor(BLUE);
    fillCircleCoord(screenPos, TILE_QTR-1);

    for(uint dir = 0; dir < 4; dir++){
        const Coord adj = coordShift(pos, dir, 1);
        const Coord adl = coordShift(adj, dirROL(dir), 1);
        const Coord adr = coordShift(adj, dirROR(dir), 1);
        if (
            (inMap(adj) && board[adj.x][adj.y] == TILE_ID_WALL) && (
                !(inMap(adl) && board[adl.x][adl.y] == TILE_ID_WALL) ||
                !(inMap(adr) && board[adr.x][adr.y] == TILE_ID_WALL)
            )
        ){
            const Coord screenAdj = toScreen(adj);
            fillRectCoords(
                coordShift(screenPos, dirROL(dir), TILE_QTR),
                coordShift(screenAdj, dirROR(dir), TILE_QTR)
            );
        }
    }
}

void mapDraw(int map[28][31])
{
    for (int x = 0; x < TILE_X; x++) {
        for (int y = 0; y < TILE_Y; y++) {
            /* TODO: render tile */
            const Coord pos = {.x=x,.y=y};
            int tile = map[x][y];
            const Coord screenPos = toScreen(pos);
            switch (tile) {
                case TILE_ID_WALL:
                    drawWall(pos, map);
                    break;
                case TILE_ID_EMPTY:
                case TILE_ID_GHOST:
                    break;
                case TILE_ID_COIN_SMALL:
                    setColor(WHITE);
                    fillCircleCoord(screenPos, TILE_ETH);
                    break;
                case TILE_ID_COIN_LARGE:
                    setColor(WHITE);
                    fillCircleCoord(screenPos, TILE_QTR);
                    break;
                default:
                    printf("[mapDraw] Error! Found unknown tile id at (%d,%d).\n", x, y);
                    break;
            }
            // setColor(WHITE);
            // drawSquareCoord(coordAddi(screenPos, -TILE_HLF), TILE_LEN);
        }
    }
}

#endif
