/**********************************************************/
/*                                                        */
/* Copyright (C) 1993 Fariborz ``Skip'' Tavakkolian       */
/*                                                        */
/**********************************************************/

static char *RCSid = "$Id: tetris.c,v 1.4 93/06/13 12:28:25 fst Exp $";

/*
 * Description:
 *
 * $Log: tetris.c,v $
 * Revision 1.4  93/06/13  12:28:25  fst
 * Added casts to satisfy link.
 * 
 * Revision 1.3  93/06/06  11:07:13  fst
 * Added the current game piece to the arguments passed to Collapse_Full_Rows
 * to allow for checking of only those rows which are effected by the last
 * move.
 * 
 * Revision 1.2  93/06/05  22:11:16  fst
 * Changed the RCS ``Header'' with ``Id''.
 * 
 * Revision 1.1  93/06/05  22:03:58  fst
 * Initial revision
 * 
 */

#include <dmd.h>
#include <dmdio.h>
#include <object.h>
#include <font.h>
#include "tetris.h"

/* Library Routines and associated manual page. */
void bitblt();
void lprintf();
void sleep();
Point sPtCurrent();
int wait();


/* tetris functions */
extern char *Gen_Menu_Items();

/* local variables */
static int running = 0;
static int new_piece = 1;
static int fall_speed = FALL_SPEED;
static int p_count = 0;
static int score = 0;

#define ADJUST_SPEED()     \
{       \
  if (score > 50)     \
    {       \
      fall_speed = FALL_SPEED - (score / 25);  \
      fall_speed = (fall_speed <= 10) ? 10 : fall_speed;\
    }       \
}

Menu top_level_menu =
{
  0, 0, 0, Gen_Menu_Items,
};
  
char *Gen_Menu_Items(i)
     int i;
{
  switch (i)
    {
    case 0:
      return "New Game";
    case 1:
      return ((running) ? "Stop" : "Start");
    case 2:
      return "Quit";
    default:
      return NULL;
    }
}
  
#define WAIT_TIME 5

GamePiece *cgp, *ngp;


Do_First_Move()
{
  int middle_x;

  if (! running)
    return 0;

  middle_x = BOARD_WIDTH/2 -
    (cgp->gmap_arr[cgp->cur]->rect.corner.x -
     cgp->gmap_arr[cgp->cur]->rect.origin.x)/2;
      
  game_board.curpt = fPt(middle_x, 0);
  /*
  fprintf(stderr, "Do_First_Move(): game_board.curpt is (%d,%d)\n",
   game_board.curpt.x, game_board.curpt.y
   );
  */
  if (! Move_Piece(cgp, FirstMove)) /* first move and wont fit */
    {
      return 0;
    }

  return 1;
}

static Point scoreloc, countloc;
Point nploc;

void Show_Score()
{
  char buff[16];

  /* itoa((long) score, buff); */
  Sprintf(buff, "%4d", score);
  string(&largefont, buff, &display, add(Drect.origin, scoreloc), F_STORE);

  /* itoa((long) p_count, buff); */
  Sprintf(buff, "%4d", p_count);
  string(&largefont, buff, &display, add(Drect.origin, countloc), F_STORE);
}

void New_Game()
{
  Point q;

  Clear_Game_Board();
  Draw_Window();
  q = add(Drect.origin, Srect.origin);
  scoreloc = string(&largefont, " SCORE   ", &display, q, F_STORE);
  scoreloc = sub(scoreloc, Drect.origin);

  q = add(q, Pt(0, FONTHEIGHT(largefont)));
  countloc = string(&largefont, " PIECES  ", &display, q, F_STORE);
  countloc = sub(countloc, Drect.origin);

  q = add(q, Pt(0, FONTHEIGHT(largefont)));
  nploc = string(&largefont, " NEXT PIECE ", &display, q, F_STORE);
  nploc = sub(nploc, Drect.origin);

  ngp = Get_Next_Piece();
  new_piece = 1;
  running = 0;
  p_count = 0;
  score = 0;
  fall_speed = FALL_SPEED;

  Show_Score();
}

void Collapse_Full_Rows()
{
  int n;

  if (n = Show_Full_Rows(cgp))
    Clear_Full_Rows(n, cgp);
}

main()
{
  extern GamePiece *Get_Next_Piece();
  int bonus;

  Initialize();
  /* cache((char *) 0, A_NO_BOOT & ~A_SHARED); */
  Reshape_Window();

  request(MOUSE);
  New_Game();

  while (1)
    {
      alarm(0);
      request(MOUSE);

      while (! running)
 {
   wait(MOUSE);
   if ((own() & MOUSE) && button2())
     {
       int m = menuhit(&top_level_menu, 2);
       switch (m)
  {
  default:
    break;
  case 0:
    New_Game();
    break;

  case 1:
    running = 1;
    break;

  case 2:
    exit();
  }
     }
 }


      alarm(fall_speed);
      request(MOUSE|KBD|ALARM);
      
      while (running)
 {
   Show_Score();
   if (new_piece)
     {
       alarm(0);
       Collapse_Full_Rows();
       Clear_Shadow_Area();
       cgp = ngp;
       ngp = Get_Next_Piece();
       cgp->cur = 0;
       Show_Next_Piece(ngp);

       if (! Do_First_Move())
  {
    running = 0;
    new_piece = 1;
    break;
  }
       p_count++;
       new_piece = 0;
       ADJUST_SPEED();
       alarm(fall_speed);
     }

   wait(ALARM|MOUSE|KBD);
   if ((own() & MOUSE) && button2())
     {
       int m = menuhit(&top_level_menu, 2);
       switch (m)
  {
  default:
    break;
  case 0:
    New_Game();
    break;

  case 1:
    running = 0;
    break;

  case 2:
    exit();
  }

       if (! running)
  break;
     }

   if (own() & ALARM)
     {
       alarm(0);
       if (Move_Piece(cgp, Down) < 0) /* resting */
  {
    score += cgp->score[cgp->cur];
    new_piece = 1;
    continue;
  }

       alarm(fall_speed);
     }

   if (own() & KBD)
     {
       switch (kbdchar())
  {
  default:
    break;

  case 0xE2: /* right arrow */
  case 'l':
    (void) Move_Piece(cgp, Right);
    break;

  case 0xE3: /* left arrow */
  case 'h':
    (void) Move_Piece(cgp, Left);
    break;

  case 0xE4: /* home key */
  case 'j':
    (void) Move_Piece(cgp, Clockwise);
    break;

  case 0xE0: /* up arrow */
  case 'k':
    (void) Move_Piece(cgp, CounterClockwise);
    break;

  case 0xE1: /* down arrow */
  case ' ':
    bonus = BOARD_HEIGHT - game_board.curpt.y;
    (void) Move_Piece(cgp, Fall);
    score += cgp->score[cgp->cur];
    score += bonus/2; /* give them half the fall distance */

    /* after a fall, the piece is always resting */
    new_piece = 1;
    break;
  }
     }
 }
    }
}

