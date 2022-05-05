#include "draw_shapes.h"

void change_color(int color_switch) {
  switch(color_switch){
    case 0:
      circle_color = COLOR_BLACK;
      break;
    case 1:
      circle_color = COLOR_RED;
      break;
    case 2:
      circle_color = COLOR_GREEN;
      break;
    default:
      circle_color = COLOR_SIENNA;
      break;
  }

}
