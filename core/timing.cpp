#include "timing.h"

#define TIME_FPS 30
#define TIME_UPS 30

const int Timing::fps=TIME_FPS;
const int Timing::fps_delta_time=1000/TIME_FPS;
const int Timing::ups=TIME_UPS;
const int Timing::ups_delta_time=1000/TIME_FPS;
int Timing::time_since_last_frame=0;
