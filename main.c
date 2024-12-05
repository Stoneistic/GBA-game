#include <gba.h>
#include "goomba.h" // Include the generated header file for Goomba
#include "mario.h"  // Include the generated header file for Mario
#include "background.h" // Include the generated header file for Background

void setupBackground() {
    // Set background control register for BG2
    REG_DISPCNT = MODE_0 | BG2_ENABLE;

    // Load the tile data and map data into VRAM
    DMA3COPY(backgroundTiles, CHAR_BASE_BLOCK(0), backgroundTilesLen / 2);
    DMA3COPY(backgroundMap, SCREEN_BASE_BLOCK(31), backgroundMapLen / 2);

    // Load the background palette
    DMA3COPY(backgroundPal, BG_PALETTE, backgroundPalLen / 2);

    // Set BG2 control register
    REG_BG2CNT = BG_SIZE_0 | CHAR_BASE(0) | SCREEN_BASE(31) | BG_COLOR_256;
}

void setupSprites() {
    // Enable sprites
    REG_DISPCNT |= OBJ_ON | OBJ_1D_MAP;

    // Load the sprite tiles into OAM
    DMA3COPY(marioTiles, SPRITE_GFX, marioTilesLen / 2);
    DMA3COPY(goombaTiles, SPRITE_GFX + marioTilesLen, goombaTilesLen / 2);

    // Load the sprite palettes
    DMA3COPY(marioPal, SPRITE_PALETTE, marioPalLen / 2);
    DMA3COPY(goombaPal, SPRITE_PALETTE + 16, goombaPalLen / 2);

    // Set up Mario sprite
    OAM[0].attr0 = ATTR0_Y(80) | ATTR0_COLOR_256 | ATTR0_SQUARE;
    OAM[0].attr1 = ATTR1_X(60) | ATTR1_SIZE_32;
    OAM[0].attr2 = ATTR2_ID(0); // Tile ID starts at 0

    // Set up Goomba sprite
    OAM[1].attr0 = ATTR0_Y(120) | ATTR0_COLOR_256 | ATTR0_SQUARE;
    OAM[1].attr1 = ATTR1_X(140) | ATTR1_SIZE_32;
    OAM[1].attr2 = ATTR2_ID(marioTilesLen / 32); // Tile ID offset by Mario's tile count
}

int main() {
    // Set up interrupts
    irqInit();
    irqEnable(IRQ_VBLANK);

    // Initialize background and sprites
    setupBackground();
    setupSprites();

    while (1) {
        VBlankIntrWait(); // Wait for vertical blank

        // Example: Move Mario sprite horizontally
        OAM[0].attr1 = ATTR1_X((OAM[0].attr1 + 1) % 240) | ATTR1_SIZE_32;

        // Example: Move Goomba sprite vertically
        OAM[1].attr0 = ATTR0_Y((OAM[1].attr0 + 1) % 160) | ATTR0_COLOR_256 | ATTR0_SQUARE;
    }

    return 0;
}

