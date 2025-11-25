#ifndef FW_VERSION
#define FW_VERSION "v0.6"     
#endif

/************************************************************************
//v0.6  Main() reworked so analog sampling done using millis+timer value rather than Delay()


//v0.5  Reworked power off - moved into new function. Set speed = 0, and turn off track power.
        changes to calculating speed. Filtering now done after /4 to reduce jitter.

//v0.4  Added timer so goes to sleep if speed set to zero for 30 minutes


************************************************************************/
