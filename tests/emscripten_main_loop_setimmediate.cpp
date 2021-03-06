#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <emscripten.h>

int numFrames = 0;

void looper() {
  static double frame0 = emscripten_get_now();

  double start = emscripten_get_now();

  ++numFrames;
  if (numFrames % 10 == 0) printf("Frame %d\n", numFrames);
  if (numFrames == 100) {
    double now = emscripten_get_now();
    double msecsPerFrame = (now - frame0) / (numFrames-1); // Sub one to account for intervals vs endpoints
    printf("Avg. msecs/frame: %f\n", msecsPerFrame);
#ifdef REPORT_RESULT
    int result = (msecsPerFrame < 5); // Expecting to run extremely fast unthrottled, and certainly not bounded by vsync, so less than common 16.667 msecs per frame (this is assuming 60hz display)
    REPORT_RESULT();
#endif
    emscripten_cancel_main_loop();
  }
}

int main() {
  emscripten_set_main_loop(looper, 1, 0);
  emscripten_set_main_loop_timing(EM_TIMING_SETIMMEDIATE, 0);
}
