#ifndef TILES_H
#define TILES_H

#define TILE_X 28
#define TILE_Y 31
#define TILE_LEN 24
#define TILE_HLF 12
#define TILE_QTR 6

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

bool inMap(const Coord pos)
{
    return pos.x >= 0 && pos.y >= 0 && pos.x < TILE_X && pos.y < TILE_Y;
}

void drawWall(const Coord pos, int board[28][31])
{
    if (!inMap(pos) || board[pos.x][pos.y] != TILE_ID_WALL)
        return;

    const Coord screenPos = {.x = pos.x * TILE_LEN + TILE_HLF, .y = pos.y * TILE_LEN + TILE_HLF};
    setColor(BLUE);
    fillCircleCoord(screenPos, TILE_QTR-1);

    for (uint dir = 0; dir < 4; dir++)
    {
        const Coord adj = coordShift(pos, dir, 1);
        if (inMap(adj) && board[adj.x][adj.y] == TILE_ID_WALL){
            const Coord screenAdj = {.x = adj.x * TILE_LEN + TILE_HLF, .y = adj.y * TILE_LEN + TILE_HLF};
            const Coord c1 = coordShift(screenPos, dirROL(dir), TILE_QTR);
            const Coord c2 = coordShift(screenAdj, dirROR(dir), TILE_QTR);
            fillRectCoords(c1, c2);
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
            const Coord screenPos = {.x = pos.x * TILE_LEN + TILE_HLF, .y = pos.y * TILE_LEN + TILE_HLF};
            (void)screenPos;
            switch (tile) {
                case TILE_ID_WALL:
                    drawWall(pos, map);
                    break;
                case TILE_ID_EMPTY:
                case TILE_ID_GHOST:
                    break;
                case TILE_ID_COIN_SMALL:
                    setColor(WHITE);
                    fillCircleCoord(pos, 2);
                    break;
                case TILE_ID_COIN_LARGE:
                    setColor(WHITE);
                    fillCircleCoord(pos, TILE_LEN/2);
                    break;
                default:
                    printf("[mapDraw] Error! Found unknown tile id at (%d,%d).\n", x, y);
                    break;
            }
        }
    }
}

#endif