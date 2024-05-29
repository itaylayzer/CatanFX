#ifndef _INC_PLAYER

#include <stdlib.h>
#include <stdbool.h>

#include "../../structures/stack/stack.h"
#include "../../structures/queue/queue.h"
#include "../../structures/heap/heap.h"
#include "../../controller/types.h"
#include "../../controller/constants.h"
#include "../../utils/convert/convert.h"
#include "./vec_utils.h"
#include "./macro.h"

VertexPtr moveRobberTo(PlayerPtr player, GraphPtr graph);

Stack buyableRoads(PlayerPtr player);

Stack buyableSettlements(PlayerPtr player, GraphPtr graph, bool needRoad);

Stack upgradeableSettlements(PlayerPtr player, GraphPtr graph);

bool buyable_product(signed char playerMats[TOTAL_MATERIALS],
                     signed char playerHarbors,
                     unsigned char productMats[TOTAL_MATERIALS],
                     StackPtr actionsStack);
#endif