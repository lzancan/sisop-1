//***************************************************************************
//********************* estados possíveis das threads ***********************
//***************************************************************************
#ifndef __enumthreads__
#define __enumthreads__

enum ThreadStates { NewThread=0, Ready=1, Running=2, Blocked=3, Zombie=4};

#endif