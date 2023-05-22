#include "DogeLib/Includes.h"
#include "Tiles.h"
#include "MapData.h"
#include "Spawn.h"
typedef enum{PS_WAIT, PS_NORM, PS_BLUE, PS_DEAD, PS_WIN, PS_NUM}PlayerState;
const char *PlayerStateStr[PS_NUM] = {"PS_WAIT", "PS_NORM", "PS_BLUE", "PS_DEAD", "PS_WIN"};

typedef struct{
    Direction dir;
    Direction turn;
    PlayerState state;
    Coord pos;
}Player, Ghost;

void playerDraw(const Player player)
{
    setColor(YELLOW);
    fillCircleCoord(player.pos, TILE_HLF);
}

Player playerGetInput(Player player)
{
    const Scancode wasd[4] = {SDL_SCANCODE_W, SDL_SCANCODE_D, SDL_SCANCODE_S, SDL_SCANCODE_A};
    bool keydown = false;
    for(uint i = 0; i < 4; i++){
        if(keyPressed(wasd[i]) && !keyPressed(wasd[(i + 2) % 4])){
            keydown = true;
            player.turn = i;
        }
    }
    if(!keydown){
        player.turn = player.dir;
        return player;
    }
    for(uint i = 0; i < 4; i++){
        if(keyState(wasd[i]) && !keyState(wasd[(i+2)%4])){
            player.turn = i;
        }
    }
    return player;
}

Player playerMove(Player player, int map[28][31])
{
    if(player.pos.x % TILE_LEN == TILE_HLF && player.pos.x % TILE_LEN == TILE_HLF){
        const Coord pos = coordDivi(coordAddi(player.pos, -TILE_HLF), TILE_LEN);
        const Coord dirPos = coordShift(pos, player.dir, 1);
        const Coord turnPos = coordShift(pos, player.turn, 1);
        if(!inMap(turnPos) || map[turnPos.x][turnPos.y] == TILE_ID_WALL){
            if(inMap(dirPos) && map[dirPos.x][dirPos.y] != TILE_ID_WALL){
                player.pos = coordShift(player.pos, player.dir, 1);
                return player;
            }
        }else if(inMap(turnPos) && map[turnPos.x][turnPos.y] != TILE_ID_WALL){
                player.dir = player.turn;
                player.pos = coordShift(player.pos, player.dir, 1);
                return player;
        }
    }
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
        .state = PS_WAIT,
        .pos = (const Coord){.x = TILE_X/2*TILE_LEN, .y = 23*TILE_LEN+TILE_HLF}
    };

    setColor(RED);
    while(1){
        const uint t = frameStart();

        player = playerMove(player, map);
        mapDraw(map);
        playerDraw(player);

        frameEnd(t);
    }
    return 0;

}
