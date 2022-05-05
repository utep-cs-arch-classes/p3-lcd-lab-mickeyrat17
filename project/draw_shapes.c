#include <msp430.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "draw_shapes.h"
#include "switches.h"

//global colors
u_int black = COLOR_BLACK;
u_int red = COLOR_RED;
u_int green = COLOR_GREEN;
u_int sienna = COLOR_SIENNA;


// global vars for the rectangle
rectangle rect1;
void circle_color_switch_listener(void);

// global vars for the circle
circle cir1;
int i = 2;
u_int background_color = COLOR_BLUE;

// score update and buzzer variables
char score = '0';
int update_score = 0;
int draw_score = 0;

//color switch variables
int color_switch = -1;
u_int circle_color = COLOR_SIENNA;

// Displays number of bounces
void message() {
  drawChar5x7(118, 152, score, COLOR_WHITE, COLOR_BLACK);
}

void
init_shapes(void)
{
  // vars for the rectangle
  rect1.rect_row = 0;
  rect1.rect_col = screenWidth / 2;
  rect1.old_rect_row = 40;
  rect1.old_rect_col = screenWidth / 2;
  rect1.height = 10;
  rect1.width  = 128;

  // vars for the circle
  cir1.cir_y = 60;
  cir1.cir_x = screenWidth / 2;
  cir1.old_cir_y = 60;
  cir1.old_cir_x = screenWidth / 2;
  cir1.r = 20;

  message();
}

void
draw_moving_shapes(void)
{
  // if switch 4 is pressed, pause
  if (switch4_down) return;
  int left_col = rect1.old_rect_col - (rect1.width / 2);
  int top_row  = rect1.old_rect_row - (rect1.height / 2);

  // blank out the old rectangle
  fillRectangle(left_col, top_row, rect1.width, rect1.height, background_color);

  // blank out the old circle
  draw_circle(cir1.old_cir_x, cir1.old_cir_y, cir1.r, background_color);

  // draw and update the circle
  circle_color_switch_listener();
  moving_circle();

  //increase size of rectangle
  rect1.height +=i;
  draw_rectangle();

  // update score and buzz when wall is hit
  if (draw_score && update_score) {
    update_score--;
    score++;
    message();
    buzzer_set_period(2000);
  }
  else if (draw_score ) {
    draw_score--;
    score++;
    message();
    buzzer_set_period(2000);
  }
  else if(update_score) {
    draw_score++;
    update_score--;
  }
  else {
    buzzer_set_period(0);
  }
}

void
draw_rectangle(void)
{
  int left_col = rect1.rect_col - (rect1.width / 2);
  int top_row  = rect1.rect_row - (rect1.height / 2);

  unsigned int blue = 16, green = 0, red = 31;
  unsigned int color = (blue << 11) | (green << 5) | red;

  fillRectangle(left_col, top_row, rect1.width, rect1.height, color);
}
void
drawHorizontalLine(u_int x_start, u_int x_end, u_int y, u_int colorBGR)
{
  u_int length = x_end - x_start;
  // set the draw area from the start of the line to the end
  // height is 1 since its a line
  lcd_setArea(x_start, y, x_end, y);
  for (u_int i = 0; i < length; i++) {
    lcd_writeColor(colorBGR);
  }
  
}

void
drawLines(u_int x_coord, u_int y_coord, u_int x_point, u_int y_point, u_int color)
{
  // bottom
  drawHorizontalLine(x_coord - x_point, x_coord + x_point, y_coord + y_point, color);
  // top
  drawHorizontalLine(x_coord - x_point, x_coord + x_point, y_coord - y_point, color);
  // and the middle two
  drawHorizontalLine(x_coord - y_point, x_coord + y_point, y_coord + x_point, color);
  drawHorizontalLine(x_coord - y_point, x_coord + y_point, y_coord - x_point, color);
					  
}

void
draw_circle(int x, int y, int r, u_int color)
{
  int x_coord = x;
  int y_coord = y;
  // first point will be the very top;
  int x_point = 0;
  int y_point = r;
  int decision = 3 - (2 * r);

  drawLines(x_coord, y_coord, x_point, y_point, color);

  while (y_point >= x_point) {
    // move x over one
    x_point++;

    // check decision point
    if (decision > 0) {
      // move y down one
      y_point--;
      decision = decision + 4 * (x_point - y_point) + 10;
    } else {
      decision = decision + 4 * x_point + 6;
    }
    drawLines(x_coord, y_coord, x_point, y_point, color);
  }
}

void
moving_circle(void)
{ 
  static int x_vel = 5;
  static int y_vel = 10;  

  // draw at the new position
  draw_circle(cir1.cir_x, cir1.cir_y, cir1.r, circle_color);

  // save current position
  cir1.old_cir_x = cir1.cir_x;
  cir1.old_cir_y = cir1.cir_y;

  // update position
  cir1.cir_x += x_vel;
  cir1.cir_y += y_vel;

  // ( rect1.rect_row + (rect1.height / 2) )
  // check boundaries, see if rectangle has hit the edges
  if ( (cir1.cir_x + cir1.r) >= screenWidth ||
       (cir1.cir_x - cir1.r) <= 0 ) {
    // top or bottom hit, reverse x velocity
    x_vel = x_vel * -1;
    update_score++;
  }
  if ( ( cir1.cir_y - cir1.r ) <= ( rect1.rect_row + (rect1.height / 2) )  ||   // left boundary
       ( cir1.cir_y + cir1.r ) >= screenHeight) { // right boundary
    // right or left hit, reverse y velocity
    y_vel = y_vel * -1;
    update_score++;
  }
}
 
void circle_color_switch_listener(void)
{
  // color change based on switch pressed 
  if(switch1_down) {
    color_switch = 0;
  }
  else if(switch2_down) {
    color_switch = 1;
  }
  else if(switch3_down) {
    color_switch = 2;
  }
  else {
    color_switch = -1;
  }
  change_color(color_switch);
}
