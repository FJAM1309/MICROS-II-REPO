#define DELAY       ( 1000U )
#define MAX_LEDS    ( 3U )
#define MIN_LEDS    ( 0U )

void (*vfnLEDWalk[])(void) =
{
    (void (*)(void))vfnDelay,
    (void (*)(void))vfnWalk,
    (void (*)(void))vfnState
};

typedef enum
{
    STATE_DELAY,
    STATE_WALK,
    STATE_FLAG
}InitStates;

static uint8_t bCurrentState = STATE_DELAY;
static uint8_t bNextState = STATE_WALK;

static uint8_t bCnt = 0;

static volatile bool bFlagL = false;
static volatile bool bFlagR = true;

/*
DELAY STATE
*/
void vfnDelay(void) 
{
    if((bCnt--)==0)
    {
        bCnt = DELAY;
        bCurrentState = bNextState;
    }
}

/*
STATE WHERE LEDS "WALK", ADD OR SUBTRACT bCnt TO SHIFT LEDS.
*/
void vfnWalk()
{
    if(bFlagR)
    {
        GpioC->PDOR = (1 << bCnt++);
    }
    if(bFlagL)
    {
        GpioC->PDOR = (1 << bCnt--);
    }
    bCurrentState = STATE_DELAY;
    bNextState = STATE_FLAG;
}

/*
STATE CHECKS IF bCnt HAS REACHED MAXIMUM LEDS (in this case, 3) AND TURNS ON LEFT FLAG 
*/
void vfnFlag(void)
{ 
    if(bCnt == MAX_LEDS)
    {
        bFlagR = false;
        bFlagL = true;
    }
    else if(bCnt == MIN_LEDS)
    {
        bFlagR = true;
        bFlagL = false;
    }
    bCurrentState = STATE_DELAY; 
    bNextState = STATE_WALK;
}

void vfnLEDWalk (void)
{
    vfnLEDWalk[bCurrentState];
}

int main (void)
{
    while(1)
    {
       vfnLEDWalk();     
    }
}