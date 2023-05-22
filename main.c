#include "DogeLib/Includes.h"
#include "Tiles.h"
#include "MapData.h"
#include "Spawn.h"
typedef enum{PS_WAIT, PS_DEAD, PS_WIN, PS_NORM, PS_BLUE, PS_NUM}PlayerState;
const char *PlayerStateStr[PS_NUM] = {"PS_WAIT", "PS_NORM", "PS_BLUE", "PS_DEAD", "PS_WIN"};

typedef struct{
    Direction dir;
    Direction turn;
    PlayerState state;
    Coord pos;
}Player, Ghost;

Coord toGrid(const Coord spos)
{
    return coordDivi(coordAddi(spos, TILE_HLF), TILE_LEN);
}

bool midTile(const Coord pos)
{
    return coordSame(coordModi(pos, TILE_LEN), iC(TILE_HLF, TILE_HLF));
}

void playerDraw(const Player player)
{
    setColor(midTile(player.pos)?GREEN:YELLOW);
    fillCircleCoord(player.pos, TILE_HLF);
    setColor(GREY);
    drawLineCoords(player.pos, coordShift(player.pos, player.turn, TILE_HLF));
    setColor(WHITE);
    drawLineCoords(player.pos, coordShift(player.pos, player.dir, TILE_HLF));
}

bool playerFacingIsect(const Player player)
{
    const Coord gpos = toGrid(player.pos);

    return gpos.x;
}

Player playerGetInput(Player player)
{
    if(keyPressed(SDL_SCANCODE_ESCAPE))
        exit(EXIT_SUCCESS);
    const Scancode wasd[4] = {SDL_SCANCODE_W, SDL_SCANCODE_D, SDL_SCANCODE_S, SDL_SCANCODE_A};
    for(uint i = 0; i < 4; i++){
        if(!dirAXA(i,player.dir) && keyState(wasd[i]) && !keyState(wasd[dirINV(i)]))
            player.turn = i;
    }
    return player;
}

Direction nearTile(const Coord spos)
{
    const Coord off = coordSub(toScreen(toGrid(spos)), spos);
    if(iabs(off.x) > iabs(off.y))
        return off.x < 0 ? DIR_L : DIR_R;
    return off.y < 0 ? DIR_U : DIR_D;
}

Direction fromTile(const Coord spos)
{
    return dirINV(nearTile(spos));
}

bool validPos(const Coord pos, int map[28][31])
{
    const Coord gpos = toGrid(pos);
    if(!inMap(gpos) || map[gpos.x][gpos.y] == TILE_ID_WALL)
        return false;
    if(midTile(pos))
        return true;
    const Coord fgpos = coordShift(gpos, fromTile(pos), 1);
    return inMap(fgpos)  && map[fgpos.x][fgpos.y] != TILE_ID_WALL;
}

Player playerMove(Player player, int map[28][31])
{
    (void)map;
    if(keyPressed(SDL_SCANCODE_SPACE))
        player.state = player.state == PS_NORM ? PS_WAIT : PS_NORM;
    if(player.state < PS_NORM)
        return player;
    const Coord npos = coordShift(player.pos, player.dir, 1);
    if(validPos(npos, map))
        player.pos = npos;
    return player;
}

Ghost ghostMove(Ghost ghost, int map[28][31])
{
    (void)map;return ghost;
}

int main(void)
{
    init();
    setWindowSize(TILE_X*TILE_LEN, TILE_Y*TILE_LEN);

    // load the board
    int map[TILE_X][TILE_Y] = MAPDATA;

    Player player = {
        .dir = DIR_L,
        .turn = DIR_L,
        .state = PS_NORM,
        .pos = (const Coord){.x = TILE_X/2*TILE_LEN, .y = 23*TILE_LEN+TILE_HLF}
    };

    setColor(RED);
    while(1){
        const uint t = frameStart();

        player = playerGetInput(player);
        player = playerMove(player, map);
        mapDraw(map);
        playerDraw(player);

        frameEnd(t);
    }
    return 0;

}
