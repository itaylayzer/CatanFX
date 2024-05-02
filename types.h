#ifndef _INC_TYPES

#define _INC_TYPES

#include "structures/avl_tree/typedef.h"

#define TOTAL_MATERIALS 5         // כמות מצרכים
#define TOTAL_DEVELOPMENT_CARD 5  // כמות קלפי פיתוח
#define TOTAL_ACHIEVEMENTS_CARD 2 // כמות קלפי הישג
#define AREAS 19                  // כמות קודקודי אזור
#define VERTECIES 54              // כמות קודקודי התיישבות
#define TOTAL_STORE 4             // כמות המפתחות בחנות
#define HARBOR_COUNT 9
#define MAX_PLAYERS 4

typedef enum
{
    WOOD,
    WOOL,
    WHEET,
    BRICK,
    ORE,
    GENERAL_DEAL
} MATERIALS;
typedef enum
{
    WHITE,
    BLUE,
    RED,
    ORANGE,
    BLACK,
    GRAY
} COLORS;
typedef enum
{
    KNIGHT_CARD,
    VICTORY_POINTS_CARD,
    ROADS_CARD,
    YEAR_OF_PLANT_CARD,
    MONOPOL_CARD
} DEVELOPMENTS;
typedef enum
{
    BIGGEST_ARMY,
    LOGEST_PATH
} ACHIEVEMENTS;
typedef enum
{
    ROAD,
    SETTLEMENT,
    CITY,
    DEVELOPMENT_CARD
} STORE_KEYS;

typedef struct vertexType
{
    unsigned char color;
    Node edges;
    union
    {
        char harbor;
        unsigned char material_number;
    };
} VertexRec, *VertexPtr;

typedef struct edgeType
{
    VertexPtr vertex;
    unsigned char offset;
    unsigned char color;
} EdgeRec, *EdgePtr;

typedef struct graphType
{
    VertexPtr vertices;
} GraphRec, *GraphPtr;

typedef struct playerType
{
    unsigned char color;
    unsigned char victoryPoints;              // מספר נקודות הניצחון
    unsigned char materials[TOTAL_MATERIALS]; // מצרכים
    // קלפי פיתוח
    unsigned char developmentCards[TOTAL_DEVELOPMENT_CARD];
    // כל ביט יסמן האם העסקת נמל בבעלות השחקן
    char harbors;
    Node settlements; // התיישבויות
    unsigned char amounts[TOTAL_STORE - 1];

} PlayerRec, *PlayerPtr;

#endif