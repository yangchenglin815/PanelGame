#ifndef ENUMHEADER_H
#define ENUMHEADER_H

#define PEN_WIDTH 0.5
#define PIECE_DIAMETER 19.5
#define ROWSCORE 10

enum  BackendLV {
    LV0 = 0,
    LV1,
    LV2,
    LV3,
    LV4,
    LV5
};

enum GameSpeed {
    LV0_SPEED = 1000,
    LV1_SPEED,
    LV2_SPEED,
    LV3_SPEED,
    LV4_SPEED,
    LV5_SPEED
};

enum BoxType{
    RandomShape = 0,
    IShape = 1,
    JShape = 2,
    LShape = 3,
    OShape = 4,
    SShape = 5,
    TShape = 6,
    Zshape = 7
};

#endif // ENUMHEADER_H
