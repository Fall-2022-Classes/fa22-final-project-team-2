/*****************************************************************//**
 * @file main_video_test.cpp
 *
 * @brief Basic test of 4 basic i/o cores
 *
 * @author p chu
 * @version v1.0: initial release
 *********************************************************************/

//#define _DEBUG
#include "chu_init.h"
#include "gpio_cores.h"
#include "vga_core.h"
#include "sseg_core.h"
#include "chu_init.h"
#include "xadc_core.h"
#include "spi_core.h"
#include "i2c_core.h"
#include "ps2_core.h"
#include "ddfs_core.h"
#include "adsr_core.h"



float map(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void test_start(GpoCore *led_p) {
   int i;

   for (i = 0; i < 20; i++) {
      led_p->write(0xff00);
      sleep_ms(50);
      led_p->write(0x0000);
      sleep_ms(50);
   }
}

/**
 * check bar generator core
 * @param bar_p pointer to Gpv instance
 */
void bar_check(GpvCore *bar_p) {
   bar_p->bypass(0);
   sleep_ms(3000);
}

/**
 * check color-to-grayscale core
 * @param gray_p pointer to Gpv instance
 */
void gray_check(GpvCore *gray_p) {
   gray_p->bypass(0);
   sleep_ms(3000);
   gray_p->bypass(1);
}

/**
 * check osd core
 * @param osd_p pointer to osd instance
 */
void osd_check(OsdCore *osd_p, int score) {

	int ones, tens, hundreds;

	ones = score % 10;
	tens = (score / 10) % 10;
	hundreds = (score / 100);

   osd_p->set_color(0xf00, 0x001); // dark gray/green

   osd_p->bypass(0);

   osd_p->wr_char(0, 0, 'S');
   osd_p->wr_char(1, 0, 'C');
   osd_p->wr_char(2, 0, 'O');
   osd_p->wr_char(3, 0, 'R');
   osd_p->wr_char(4, 0, 'E');
   osd_p->wr_char(5, 0, '=');
   osd_p->wr_char(6, 0, hundreds + '0' );
   osd_p->wr_char(7, 0, tens + '0');
   osd_p->wr_char(8, 0, ones + '0');

   /*
   osd_p->wr_char(6, 0, 'C');
   osd_p->wr_char(7, 0, 'O');
   osd_p->wr_char(8, 0, 'R');
   osd_p->wr_char(9, 0, 'E');
   osd_p->wr_char(10, 0, '=');
   osd_p->wr_char(11, 0, ' ');
*/
/*
   for (int i = 0; i < 64; i++) {
      osd_p->wr_char(8 + i, 20, i);


      osd_p->wr_char(8 + i, 21, 64 + i, 1);
      sleep_ms(100);
   }
   */
   sleep_ms(3000);
}


/**
 * test frame buffer core
 * @param frame_p pointer to frame buffer instance
 */
void frame_check(FrameCore *frame_p) {
   int x, y, color;

   frame_p->bypass(0);
   for (int i = 0; i < 10; i++) {
      frame_p->clr_screen(0x008);  // dark green
      for (int j = 0; j < 20; j++) {
         x = rand() % 640;
         y = rand() % 480;
         color = rand() % 512;
         frame_p->plot_line(400, 200, x, y, color);
      }
      sleep_ms(300);
   }
   sleep_ms(3000);
}

/**
 * test ghost sprite
 * @param ghost_p pointer to mouse sprite instance
 */
/*
void mouse_check(SpriteCore *carL1, SpriteCore *carL2, SpriteCore *carR1, SpriteCore *carR2, XadcCore *adc_p) {
   int xR, y0, y1, y2, y3, xL;
   double scalar = 500.0;
   double reading = (adc_p -> read_adc_in(0)) * scalar;


   carL1->bypass(0);
   carL2->bypass(0);
   carR1->bypass(0);
   carR2->bypass(0);



   // slowly move mouse pointer
   xR = 0;
   xL = 640;

   y0 = 125;
   y1 = 267;

   y2 = 197;
   y3 = 339;

   for (int i = 0; i < 160; i++) {
	   reading = (adc_p->read_adc_in(0)) * scalar;
	   uart.disp(reading);
	   map(reading, 0.0, 1.2, 40, 60);

      carL1->move_xy(xL, y0);
      carL2->move_xy(xL, y1);

      carR1->move_xy(xR, y2);
      carR2->move_xy(xR, y3);
      sleep_ms(reading);

      xR = xR + 4;
      xL = xL - 4;

      if (i == 159)
      {
    	  i = 0;

    	  xL = 640;
    	  xR = 0;
      }

   }

   sleep_ms(3000);
}
*/

void ghost_check (SpriteCore *ghost_p, Ps2Core *ps2_p)
{


	   ghost_p->bypass(0);
	   char key;
	   int xGhost, yGhost;
	   ps2_p->init();

	   xGhost = 340;
	   yGhost = 0;


	   ghost_p -> move_xy(340, 0);
	   ghost_p -> wr_ctrl(0x11); //looking down // orange

	   if (ps2_p -> get_kb_ch(&key))
	   {
	           if (key == 'w') //forward (W)
	           {
	               yGhost = yGhost - 28;
	               if (yGhost < 0)
	               {
	                   yGhost = yGhost + 28;
	               }

	               ghost_p -> wr_ctrl(0x14); //looking down; orange
	               ghost_p -> move_xy(xGhost, yGhost);
	           }

	           if (key == 'a') //left (A)
	           {
	               xGhost = xGhost - 40;
	               if (xGhost < 0 )
	               {
	                   xGhost = xGhost + 40;
	               }
	               ghost_p -> wr_ctrl(0x13); //looking left; orange
	               ghost_p -> move_xy(xGhost, yGhost);

	           }
	           if (key == 's') //backwards (S)
	           {
	               yGhost = yGhost + 28;

	               if (yGhost > 480)
	               {
	                   yGhost = yGhost - 28;
	               }
	               ghost_p -> wr_ctrl(0x11); //looking down; orange
	               ghost_p -> move_xy(xGhost, yGhost);
	           }
	           if (key == 'd') //right (D)
	           {
	               xGhost = xGhost + 40;
	               if (xGhost > 640 )
	               {
	                   xGhost = xGhost - 40;
	               }
	               ghost_p -> wr_ctrl(0x10); //looking down; orange
	               ghost_p -> move_xy(xGhost, yGhost);

	           }
	       }
}

void mouse_check(SpriteCore *carL1, SpriteCore *carL2, SpriteCore *carR1, SpriteCore *carR2, XadcCore *adc_p, SpriteCore *ghost_p, Ps2Core *ps2_p, OsdCore *osd_p) {
   int xR, y0, y1, y2, y3, xL;

   int flag1 = 0;
   int flag2 = 0;

	int ones, tens, hundreds;
	int score = 0;

   double scalar = 100.0;
   double reading = (adc_p -> read_adc_in(0)) * scalar;

   carL1->bypass(0);
   carL2->bypass(0);
   carR1->bypass(0);
   carR2->bypass(0);
   ghost_p->bypass(0);

   char key;
   int xGhost, yGhost;
   ps2_p->init();
   xGhost = 340;
   yGhost = 0;
   ghost_p -> move_xy(340, 0);
   ghost_p -> wr_ctrl(0x11); //looking down // orange




   // slowly move mouse pointer
   xR = 0; //x Right Car
   xL = 640; //x Left Car


    //y axis of all cars
   y0 = 110;
   y1 = 252;
   y2 = 182;
   y3 = 324;



   for (int i = 0; i < 160; i++) {
    //XADC CORE potentiometer reading + map

		ones = score % 10;
		tens = (score / 10) % 10;
		hundreds = (score / 100);

	   osd_p->set_color(0xf00, 0x001); // dark gray/green

	   osd_p->bypass(0);

	   osd_p->wr_char(0, 0, 'S');
	   osd_p->wr_char(1, 0, 'C');
	   osd_p->wr_char(2, 0, 'O');
	   osd_p->wr_char(3, 0, 'R');
	   osd_p->wr_char(4, 0, 'E');
	   osd_p->wr_char(5, 0, '=');
	   osd_p->wr_char(6, 0, hundreds + '0' );
	   osd_p->wr_char(7, 0, tens + '0');
	   osd_p->wr_char(8, 0, ones + '0');

	   reading = (adc_p->read_adc_in(0)) * scalar;
	    //move L&R Cars
	      carL1->move_xy(xL, y0);
	      carL2->move_xy(xL, y1);

	      carR1->move_xy(xR, y2);
	      carR2->move_xy(xR, y3);
	    //Delay
	      sleep_ms(reading);

	    //Increment X axis
	      xR = xR + 4;
	      xL = xL - 4;

	      if (ps2_p -> get_kb_ch(&key))
		   {
				   if (key == 'w') //forward (W)
				   {
					   yGhost = yGhost - 25;
					   if (yGhost < 0)
					   {
						   yGhost = yGhost + 25;
					   }

					   ghost_p -> wr_ctrl(0x13); //looking down; orange
					   ghost_p -> move_xy(xGhost, yGhost);
				   }

				   if (key == 'a') //left (A)
				   {
					   xGhost = xGhost - 40;
					   if (xGhost < 0 )
					   {
						   xGhost = xGhost + 40;
					   }
					   ghost_p -> wr_ctrl(0x12); //looking left; orange
					   ghost_p -> move_xy(xGhost, yGhost);

				   }
				   if (key == 's') //backwards (S)
				   {
					   yGhost = yGhost + 27;

					   if (yGhost > 480)
					   {
						   yGhost = yGhost - 27;
					   }
					   ghost_p -> wr_ctrl(0x11); //looking down; orange
					   ghost_p -> move_xy(xGhost, yGhost);
				   }
				   if (key == 'd') //right (D)
				   {
					   xGhost = xGhost + 40;
					   if (xGhost > 640 )
					   {
						   xGhost = xGhost - 40;
					   }
					   ghost_p -> wr_ctrl(0x10); //looking down; orange
					   ghost_p -> move_xy(xGhost, yGhost);

				   }
			   }

	      if (yGhost > 420 )
	      {
	    	  score++;
	    	  flag1 = 0;
	    	  flag2 = 0;
	    	  ghost_p -> bypass(1);
	    	  xGhost = 340;
	    	  yGhost = 0;
	    	  ghost_p -> move_xy(xGhost,yGhost);
	    	  ghost_p -> bypass(0);
	      }
	      if (((xGhost == xR - 16) || (xGhost == xR + 16)) &&  (( yGhost >= y2 - 16 ) && (yGhost <= y2 + 16)))
	      {
	          score = 0;
	          flag1 =0;
			  flag2 = 0;
	          ghost_p ->bypass(1);
	          xGhost = 340;
	          yGhost = 0;
	          ghost_p -> move_xy(xGhost, yGhost);
	          ghost_p ->bypass(0);
	      }
	      if (((xGhost == xR - 16) || (xGhost == xR + 16)) && ( yGhost >= y3 - 16 ) && (yGhost <= y3 + 16) )
	      {
	          score = 0;
	          flag1 =0;
			  flag2 = 0;
	          ghost_p ->bypass(1);
	          xGhost = 340;
	          yGhost = 0;
	          ghost_p -> move_xy(xGhost, yGhost);
	          ghost_p ->bypass(0);
	      }
	      if (((xGhost == xL + 16) || (xGhost == xL - 16)) && (( yGhost >= y0 - 16 ) && (yGhost <= y0 + 16)))
	      {
	          score = 0;
	          flag1 =0;
	          flag2 = 0;
	          ghost_p ->bypass(1);
	          xGhost = 340;
	          yGhost = 0;
	          ghost_p -> move_xy(xGhost, yGhost);
	          ghost_p ->bypass(0);
	      }
	      if (((xGhost == xL + 16) || (xGhost == xL - 16)) && (( yGhost >= y1 - 16 ) && (yGhost <= y1 + 16)))
	      {
	          score = 0;
	          flag1 =0;
	         flag2 = 0;
	          ghost_p ->bypass(1);
	          xGhost = 340;
	          yGhost = 0;
	          ghost_p -> move_xy(xGhost, yGhost);
	          ghost_p ->bypass(0);
	      }

	      if(xGhost >= 0 && xGhost <= 640 && yGhost + 16 >= 0 && yGhost + 16 <= 179 && yGhost >= 142 && flag1 == 0)
	      {
	    	  score++;
	    	  flag1 = 1;
	      }

	      if(xGhost >= 0 && xGhost <= 640 && yGhost + 16 >= 0 && yGhost + 16 >= 284 && yGhost <= 321 && flag2 == 0)
	      {
	    	  score++;
	    	  flag2 = 1;
	      }



	    //If at the end of loop, then reset
	      if (i == 159)
	      {
	    	  i = 0;

	    	  xL = 640;
	    	  xR = 0;
	      }
   }

}

/**
 * test ghost sprite
 * @param ghost_p pointer to ghost sprite instance
 */
void ghost_check1(SpriteCore *ghost_p) {
   int x, y;

   // slowly move mouse pointer
   ghost_p->bypass(0);
   ghost_p->wr_ctrl(0x1c);  //animation; blue ghost
   x = 0;
   y = 100;
   for (int i = 0; i < 156; i++) {
      ghost_p->move_xy(x, y);
      sleep_ms(100);
      x = x + 4;
      if (i == 80) {
         // change to red ghost half way
         ghost_p->wr_ctrl(0x04);
      }
   }
   sleep_ms(3000);
}

// external core instantiation
GpoCore led(get_slot_addr(BRIDGE_BASE, S2_LED));
GpiCore sw(get_slot_addr(BRIDGE_BASE, S3_SW));
FrameCore frame(FRAME_BASE);
Ps2Core ps2(get_slot_addr(BRIDGE_BASE, S11_PS2));
GpvCore bar(get_sprite_addr(BRIDGE_BASE, V7_BAR));
SpriteCore carL1(get_sprite_addr(BRIDGE_BASE, V6_GRAY), 1024);
SpriteCore carL2(get_sprite_addr(BRIDGE_BASE, V5_USER5), 1024);
SpriteCore carR1(get_sprite_addr(BRIDGE_BASE, V4_USER4), 1024);
SpriteCore carR2(get_sprite_addr(BRIDGE_BASE, V1_MOUSE), 1024);
XadcCore adc(get_slot_addr(BRIDGE_BASE, S5_XDAC));
SpriteCore ghost(get_sprite_addr(BRIDGE_BASE, V3_GHOST), 1024);
OsdCore osd(get_sprite_addr(BRIDGE_BASE, V2_OSD));
SsegCore sseg(get_slot_addr(BRIDGE_BASE, S8_SSEG));

int main() {
   while (1) {
	   //int score;

      //test_start(&led);
      // bypass all cores
      //frame.bypass(1);
      bar.bypass(1);
      //gray.bypass(1);
      //ghost.bypass(1);
      osd.bypass(1);
      carL1.bypass(1);
      carL2.bypass(1);
      carR1.bypass(1);
      carR2.bypass(1);
      ghost.bypass(1);
      sleep_ms(3000);

      // enable cores one by one
      //frame_check(&frame);
      bar_check(&bar);
      //gray_check(&gray);

      //osd_check(&osd);
      mouse_check(&carL1, &carL2, &carR1, &carR2, &adc, &ghost, &ps2, &osd);


     // ghost_check(&ghost, &ps2);
     // while (sw.read(0)) {
         // test composition with different overlays if sw(0) is 1
     //    carL1.bypass(sw.read(1));
         //osd.bypass(sw.read(2));
         //ghost.bypass(sw.read(3));
         //gray.bypass(sw.read(6));
         //bar.bypass(sw.read(7));
         //frame.bypass(sw.read(8));
         // set osd background color to transparent
         //osd.set_color(0x0f0, sw.read(9));
         // set color/animation of ghost sprite
         //ghost.wr_ctrl(sw.read() >> 11);
      //} //while
   } // while
} //main
