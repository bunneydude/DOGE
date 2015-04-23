#ifndef NEIGHBOR_H
#define NEIGHBOR_H

#include <stdint.h>
#include "neighbor-config.h"
#include "../protocol/type.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef uint16_t timerType;
#define MAX_TIMER_VAL (UINT16_MAX)

#define TIMER_OVERFLOW(a,b) ((MAX_TIMER_VAL - (a)) < (b))
#define TIMER_BEGIN(timer) (((timer)->end)-((timer)->duration))
#define TIMER_END(timer)   ((timer)->end)

typedef struct
{
   timerType duration;
   timerType end;
}dogeTimer;

//neighborEntry holds information needed to communicate w/ adjacent nodes
//#if PACK_STRUCT == 1
struct neighborEntry{
	uint16_t shNodeID; //single-hop node ID; each node has a unique ID
	uint16_t shLQE : 12; //single-hop Link Quality Estimator; more details determined by the active LQE method
	uint8_t radioID : 2; //which radio this entry is for. Each node can support 4 radio links (don't have to all be different radios)
	uint8_t networkID : 2; //used for jumping between different RF networks. 
};
/*
#else
struct neighborEntry{
	uint16_t shNodeID;
	uint16_t shLQE;
	uint8_t networkID;
	uint8_t radioID;
};
#endif
*/
timerType current_time();
void timer_init(dogeTimer* timer, timerType duration);
dogeBool timer_expired(dogeTimer* timer);
void timer_reset(dogeTimer* timer);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
