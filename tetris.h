/**********************************************************/
/*                                                        */
/* Copyright (C) 1993 Fariborz ``Skip'' Tavakkolian       */
/* All rights reserved                                    */
/*                                                        */
/**********************************************************/

#ident "$Id: tetris.h,v 1.3 93/06/13 12:29:25 fst Exp $"

/*
 * Description:
 *
 * $Log: tetris.h,v $
 * Revision 1.3  93/06/13  12:29:25  fst
 * Added forward declarations, and some external function declarations.
 * 
 * Revision 1.2  93/06/05  22:11:25  fst
 * Changed the RCS ``Header'' with ``Id''.
 * 
 * Revision 1.1  93/06/05  22:04:29  fst
 * Initial revision
 * 
 */

#ifndef _tetris_h
#define _tetris_h

/* DMD Library Routines */
extern void bitblt();
extern void lprintf();
extern alarm();
extern int request();
extern int own();
extern int wait();
extern Point sPtCurrent();


#define DIM(x) (sizeof(x)/sizeof((x)[0]))

/* NOTE:
   Do not set BOARD_WIDTH greater than 16
   */
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 30
#define PIECES_COUNT 7

#define TET_INSET 4
#define BBW  1
#define CHARCOUNT 18
#define TEXTWIDTH (FONTWIDTH(largefont)*CHARCOUNT)
#define GBW (BOARD_WIDTH * 16)
#define GBH (BOARD_HEIGHT* 16)

/* 1 tick = 1/60 of a second */
#define FALL_SPEED 30

/*
  FULL_ROW_MASK greater than FFFF wont work
  (see drawing.c Collapse_Full_Rows)
  */
#define FULL_ROW_MASK 0xFFC0

extern Texture16 hashmark;

enum MoveDirection
{
  FirstMove,
  Fall,
  Left,
  Right,
  Down,
  Clockwise,
  CounterClockwise,
};

enum Boolean
{
  False = 0,
  True,
};

typedef struct
{
  Point curpt;
  Bitmap *map;
} Board;

extern Board game_board;

typedef struct
{
  int cur;
  Texture16 *color; /* color for this game piece */
  int score[4];  /* points scored for each orientation of the piece */
  Bitmap *gmap_arr[4]; /* the game bitmap (one bit per brick)     */
  Bitmap *smap_arr[4]; /* shape bit map.  This is built from gmap    */
} GamePiece;

extern GamePiece *Pieces[];
extern Texture16 *colors[];
extern Rectangle Srect, Grect, Mrect;
extern Point nploc;

#ifdef _STDC_
# define __(x) x
#else
# define __(x) ()
#endif

/* dmdtetris functions */
extern GamePiece *Get_Next_Piece();

extern void Clear_Game_Board ();
extern void Draw_Window      ();
extern void Clear_Shadow_Area();
extern void Reshape_Window   ();
extern void Clear_Full_Rows  ();
extern void Init_Pieces      ();
extern void Initialize       ();

extern void Show_Next_Piece    __((GamePiece* ));
extern void Bitblit_Game_Board_XOR __((GamePiece* ));
extern void Bitblit_Display_XOR    __((GamePiece* ));
extern void Bitblit_Piece_XOR    __((GamePiece* ));
extern void Set_Color     __((Bitmap*, Texture16* ));
extern void Populate_Bitmap    __((Bitmap*, Bitmap* ));

extern int Show_Full_Rows();

extern Point default_size __((int x, int y, struct Proc* ));

#endif /* _tetris_h */
