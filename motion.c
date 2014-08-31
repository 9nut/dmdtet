/**********************************************************/
/*                                                        */
/* Copyright (C) 1993 Fariborz ``Skip'' Tavakkolian       */
/*                                                        */
/**********************************************************/

static char *RCSid = "$Id: motion.c,v 1.2 93/06/05 22:11:28 fst Exp Locker: fst $";

/*
 * Description:
 *
 * $Log: motion.c,v $
 * Revision 1.2  93/06/05  22:11:28  fst
 * Changed the RCS ``Header'' with ``Id''.
 * 
 * Revision 1.1  93/06/05  22:06:11  fst
 * Initial revision
 * 
 */

#include <dmd.h>
#include <dmdio.h>
#include <font.h>
#include "tetris.h"

void Reshape_Window()
{
  lprintf("Please Reshape Window");

  request(RESHAPED);
  do
    {
      wait(RESHAPED);
      if (P->state & MOVED)
 {
   P->state &= ~(MOVED|RESHAPED);
 }
      else if ((P->state & RESHAPED) && !(P->state & MOVED))
 {
   P->state &= ~RESHAPED;
   P->state |= NO_RESHAPE;
   break;
 }
    }
  while (1);
}

/*
 * Initialize the game pieces.
 */
void Init_Pieces()
{
  int i, j;
  extern Texture16 *colors[];

  for (i = 0; i < PIECES_COUNT; i++)
    for (j = 0; j < 4; j++)
      {
 Pieces[i]->color = colors[i];
 Populate_Bitmap(Pieces[i]->gmap_arr[j], Pieces[i]->smap_arr[j]);
 Set_Color(Pieces[i]->smap_arr[j], colors[i]);
      }

  srand((unsigned) realtime()); /* seed the random number generator */
}

Rectangle Grect, Srect, Mrect;

/*
 * Set default size for the window.
 */
Point default_size(x, y, p)
     int x,y;
     struct Proc *p;
{
  Point q;

  Srect.origin = fPt(TET_INSET + BBW, TET_INSET + BBW);

  Grect.origin = add(Srect.origin, Pt(TEXTWIDTH+2*BBW+TET_INSET, 0));
  Grect.corner = add(Grect.origin, Pt(GBW, GBH));

  Mrect.origin = add(Grect.origin, Pt(0, 2*BBW+GBH+TET_INSET));
  Mrect.corner = add(Mrect.origin, Pt(GBW, 16));

  Srect.corner = fPt(Srect.origin.x+TEXTWIDTH, Mrect.corner.y);

  q.x = Grect.corner.x - Srect.origin.x + 2*TET_INSET + 2*BBW + 2*INSET;
  q.y = Srect.corner.y - Srect.origin.y + 2*TET_INSET + 2*BBW + 2*INSET;

  return q;
}

/*
 * Initialize the default size function, colors and pieces.
 */
void Initialize()
{
  colors[1] = &T_white;
  colors[2] = &T_grey;
  colors[3] = &T_grey2;
  colors[4] = &T_checks;
  colors[5] = &T_background;

  Init_Pieces();

  P->ctob = default_size;
  P->state |= NOCURSEXPAND;
}

int Check_Bounds(origin, corner)
     Point origin, corner;
{
  /*
  fprintf(stderr, "Check_Bounds origin(%d,%d), corner(%d,%d)\n",
   origin.x, origin.y, corner.x, corner.y
   );
  */

  if ((! ptinrect(origin, game_board.map->rect)) ||
      (! ptinrect(corner, game_board.map->rect)))
    return 0;

  return 1;
}

int Game_Board_Spot_Empty(gmap, pt)
     Bitmap *gmap; /* game piece bitmap */
     Point pt;  /* origin of game piece in game board bitmap */
{
  Point gpt, ept;
  int x, y, maxx, maxy, bit;
  Word *w;
  extern Word *addr();

  ept = add(pt, sub(gmap->rect.corner, gmap->rect.origin));
  if (! Check_Bounds(pt, ept))
    {
      /*fprintf(stderr, "Game_Board_Spot_Empty(): Check_Bounds() failed\n");*/
      return 0;
    }

  maxy = gmap->rect.corner.y;
  maxx = gmap->rect.corner.x;

  /*
    For every bit set in the game piece bitmap
    if corresponding bit in the game board bitmap, then fail
    if all bits in game board are free, succeed.
    */
    
  for (y = gmap->rect.origin.y; y < maxy; y++)
    for (x = gmap->rect.origin.x; x < maxx; x++)
      {
 w = addr(gmap, Pt(x, y));

 bit = FIRSTBIT >> (x % WORDSIZE);

 if ((*w & bit) == bit)
   {
     gpt = add(pt, Pt(x, y));

     w = addr(game_board.map, gpt);
     bit = FIRSTBIT >> (gpt.x % WORDSIZE);

     if ((*w & bit) == bit)
       {
  /*
  fprintf(stderr,
   "Game_Board_Spot_Empty(): point(%d,%d) occupied\n",
   gpt.x, gpt.y
   );
  */
  return 0;
       }
   }
      }

  /*
  fprintf(stderr,
   "Game_Board_Spot_Empty(): rectangle([%d,%d],[%d,%d]) is open\n",
   pt.x, pt.y, ept.x, ept.y
   );
  */
  return 1;
}

int Can_Move(gp, d, orgpt)
     GamePiece *gp;
     enum MoveDirection d;
     Point *orgpt;
{
  switch ((int) d)
    {
    case Left:
      *orgpt = sub(game_board.curpt, Pt(1, 0));
      break;
    case Right:
      *orgpt = add(game_board.curpt, Pt(1, 0));
      break;
    case Down:
      *orgpt = add(game_board.curpt, Pt(0, 1));
      break;

    default:
      return 0;
    }

  return Game_Board_Spot_Empty(gp->gmap_arr[gp->cur], *orgpt);
}

int Can_Rotate(gp, d, orgpt, orient)
     GamePiece *gp;
     enum MoveDirection d;
     Point *orgpt;
     int *orient;
{
  Point center;

  switch ((int) d)
    {
    case Clockwise:
      *orient = ((gp->cur + 1 > 3) ? 0 : gp->cur + 1);
      break;
    case CounterClockwise:
      *orient = ((gp->cur - 1 < 0) ? 3 : gp->cur - 1);
      break;
    default:
      return 0;
    }


  /* Center calculations */
  center = add(game_board.curpt,
        div(
     sub(gp->gmap_arr[gp->cur]->rect.corner,
         gp->gmap_arr[gp->cur]->rect.origin
         ),
     2)
        );

  /* New origin calculations */
  *orgpt = sub(center,
        div(
     sub(gp->gmap_arr[*orient]->rect.corner,
         gp->gmap_arr[*orient]->rect.origin
         ),
     2)
        );

  return Game_Board_Spot_Empty(gp->gmap_arr[*orient], *orgpt);
}

int Move_Piece(gp, d)
     GamePiece *gp;
     enum MoveDirection d;
{
  int new_orientation;
  Point new_origin;
  int retval;

  switch ((int) d)
    {
    case FirstMove:
      if (! Game_Board_Spot_Empty(gp->gmap_arr[gp->cur], game_board.curpt))
 return 0;

      break;

    case Left:
    case Right:
    case Down:
      Bitblit_Game_Board_XOR(gp); /* first free current location */
      if (! Can_Move(gp, d, &new_origin))
 {
   Bitblit_Game_Board_XOR(gp); /* put it back */
   return ((d == Down) ? -1 : 0);
 }

      Bitblit_Display_XOR(gp);  /* erase old impression */
      game_board.curpt = new_origin;
      break;

    case Clockwise:
    case CounterClockwise:
      Bitblit_Game_Board_XOR(gp);
      if (! Can_Rotate(gp, d, &new_origin, &new_orientation))
 {
   Bitblit_Game_Board_XOR(gp); /* put it back */
   return 0;
 }

      Bitblit_Display_XOR(gp);
      gp->cur = new_orientation;
      game_board.curpt = new_origin;
      break;

    case Fall:
      while ((retval = Move_Piece(gp, Down)) > 0)
 /* sleep(2) */;

      return retval;
    }

  /*
  fprintf(stderr,
   "Move_Piece(): drawing (0x%x) at game_board location (%d,%d)\n",
   gp, game_board.curpt.x, game_board.curpt.y
   );
  */
  Bitblit_Piece_XOR(gp);
  return 1;
}

  
