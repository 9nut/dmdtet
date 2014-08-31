/**********************************************************/
/*                                                        */
/* Copyright (C) 1993 Fariborz ``Skip'' Tavakkoian        */
/*                                                        */
/**********************************************************/

static char *RCSid = "$Id: drawing.c,v 1.3 93/06/06 11:11:38 fst Exp $";

/*
 * Description:
 *
 * $Log: drawing.c,v $
 * Revision 1.3  93/06/06  11:11:38  fst
 * Added the current game piece to argument list for Show_Full_Rows and
 * Clear_Full_Rows.  The two functions were changed to check those rows
 * which are effected by the last of move of the current game piece.
 * 
 * Revision 1.2  93/06/05  22:11:32  fst
 * Changed the RCS ``Header'' with ``Id''.
 * 
 * Revision 1.1  93/06/05  22:05:24  fst
 * Initial revision
 * 
 */

#include <dmd.h>
#include <dmdio.h>
#include "tetris.h"

Texture16 *colors[] =
{
  &T_darkgrey, 
  (Texture16*) 0, /* black */
  (Texture16*) 0, /* grey */
  (Texture16*) 0, /* grey2 */
  (Texture16*) 0, /* checks */
  (Texture16*) 0, /* background */
  &T_lightgrey,
  (Texture16*) 0,
};

Texture16 hashmark =
{
  0x318C, 0x6318, 0xC631, 0x8C63,
  0x18C6, 0x318C, 0x6318, 0xC631,
  0x8C63, 0x18C6, 0x318C, 0x6318,
  0xC631, 0x8C63, 0x18C6, 0x318C,
};

static Word brick_bits[] =
{
  0xFFFE, 0xFFFE, 0xFFFE, 0xFFFE,
  0xFFFE, 0xFFFE, 0xFFFE, 0xFFFE,
  0xFFFE, 0xFFFE, 0xFFFE, 0xFFFE,
  0xFFFE, 0xFFFE, 0xFFFE, 0x0000,
};

static Bitmap a_brick =
{
  (Word *) brick_bits,
  1,
  { {0, 0}, {16, 16} },
  (char *) 0,
};

static Word shadow_bits[64];
static Bitmap shadow =
{
  (Word *) shadow_bits,
  1,    /* this changes at runtime */
  { {0, 0}, {64, 16} },  /* so does corner.x  */
  (char *) 0,
};

static Word game_bits[BOARD_HEIGHT];
static Bitmap game_bitmap =
{
  (Word *) game_bits,
  1,
  { {0, 0}, {BOARD_WIDTH+1, BOARD_HEIGHT+1} },
  (char *) 0,
};

Board game_board =
{
  { 0, 0, },
  &game_bitmap,
};

/* ================= J - piece ================ */

/* -------------------  North ----------------- */
static Word xJ_n[] =
{
  0x4000,
  0x4000,
  0xC000,
};

static Bitmap xJ_north =
{
  (Word *) xJ_n,
  1,
  { {0, 0}, {2, 3} },
  (char *) 0,
};

static Word J_n[96];

static Bitmap J_north =
{
  (Word *) J_n,
  2,
  { {0, 0}, {32, 48} },
  (char *) 0,
};

/* ------------------  East ----------------- */
static Word xJ_e[] =
{
  0x8000,
  0xE000,
};

static Bitmap xJ_east =
{
  (Word *) xJ_e,
  1,
  { {0, 0}, {3, 2} },
  (char *) 0,
};

static Word J_e[96];

static Bitmap J_east =
{
  (Word*) J_e,
  3,
  { {0, 0}, {48, 32} },
  (char *) 0,
};

/* ------------------- South ------------------ */
static Word xJ_s[] =
{
  0xC000,
  0x8000,
  0x8000,
};

static Bitmap xJ_south =
{
  (Word *) xJ_s,
  1,
  { {0, 0}, {2, 3} },
  (char *) 0,
};

static Word J_s[96];

static Bitmap J_south =
{
  (Word*) J_s,
  2,
  { {0, 0}, {32, 48} },
  (char *) 0,
};


/* ---------------------- West ------------------ */
static Word xJ_w[] =
{
  0xE000,
  0x2000,
};

static Bitmap xJ_west =
{
  (Word *) xJ_w,
  1,
  { {0, 0}, {3, 2} },
  (char *) 0,
};

static Word J_w[96];

static Bitmap J_west =
{
  (Word*) J_w,
  3,
  { {0, 0}, {48, 32} },
  (char *) 0,
};

/* =================== L - piece ================== */

/* ---------------------  North  ------------------ */
static Word xL_n[] =
{
  0x8000,
  0x8000,
  0xC000,
};

static Bitmap xL_north =
{
  (Word *) xL_n,
  1,
  { {0, 0}, {2, 3} },
  (char *) 0,
};

static Word L_n[96];

static Bitmap L_north =
{
  (Word*) L_n,
  2,
  { {0, 0}, {32, 48} },
  (char *) 0,
};

/* ---------------------  East  ------------------ */
static Word xL_e[] =
{
  0xE000,
  0x8000,
};

static Bitmap xL_east =
{
  (Word *) xL_e,
  1,
  { {0, 0}, {3, 2} },
  (char *) 0,
};

static Word L_e[96];

static Bitmap L_east =
{
  (Word*) L_e,
  3,
  { {0, 0}, {48, 32} },
  (char *) 0,
};

/* --------------------  South  ------------------ */
static Word xL_s[] =
{
  0xC000,
  0x4000,
  0x4000,
};

static Bitmap xL_south =
{
  (Word *) xL_s,
  1,
  { {0, 0}, {2, 3} },
  (char *) 0,
};

static Word L_s[96];

static Bitmap L_south =
{
  (Word*) L_s,
  2,
  { {0, 0}, {32, 48} },
  (char *) 0,
};

/* ---------------------  West  ------------------ */
static Word xL_w[] =
{
  0x2000,
  0xE000,
};

static Bitmap xL_west =
{
  (Word *) xL_w,
  1,
  { {0, 0}, {3, 2} },
  (char *) 0,
};

static Word L_w[96];

static Bitmap L_west =
{
  (Word*) L_w,
  3,
  { {0, 0}, {48, 32} },
  (char *) 0,
};

/* ================= S - peices ===================== */

/* ------------------ North / South ----------------- */
static Word xS_n[] =
{
  0x8000,
  0xC000,
  0x4000,
};

static Bitmap xS_north =
{
  (Word *) xS_n,
  1,
  { {0, 0}, {2, 3} },
  (char *) 0,
};

static Word S_n[96];

static Bitmap S_north =
{
  (Word *) S_n,
  2,
  { {0, 0}, {32, 48} },
  (char *) 0,
};

/* ----------------- East / West ------------------ */
static Word xS_e[] =
{
  0x6000,
  0xC000,
};

static Bitmap xS_east =
{
  (Word *) xS_e,
  1,
  { {0, 0}, {3, 2} },
  (char *) 0,
};

static Word S_e[96];

static Bitmap S_east = 
{
  (Word *) S_e,
  3,
  { {0, 0}, {48, 32} },
  (char *) 0,
};

/* =========================== Z - piece ======================= */

/* -------------------------- North/South ---------------------- */
static Word xZ_n[] =
{
  0x4000,
  0xC000,
  0x8000,
};

static Bitmap xZ_north =
{
  (Word *) xZ_n,
  1,
  { {0, 0}, {2, 3} },
  (char *) 0,
};

static Word Z_n[96];
static Bitmap Z_north =
{
  (Word *) Z_n,
  2,
  { {0, 0}, {32, 48} },
  (char *) 0,
};

/* -------------------------- East/West ---------------------- */
static Word xZ_e[] =
{
  0xC000,
  0x6000,
};

static Bitmap xZ_east =
{
  (Word *) xZ_e,
  1,
  { {0, 0}, {3, 2} },
  (char *) 0,
};

static Word Z_e[96];
static Bitmap Z_east = 
{
  (Word *) Z_e,
  3,
  { {0, 0}, {48, 32} },
  (char *) 0,
};  

/* ============================ T - piece ======================= */
/* ---------------- North -------------- */
static Word xT_n[] =
{
  0x8000,
  0xC000,
  0x8000,
};

static Bitmap xT_north = 
{
  (Word *) xT_n,
  1,
  { {0, 0}, {2, 3} },
  (char *) 0,
};

static Word T_n[96];
static Bitmap T_north = 
{
  (Word *) T_n,
  2,
  { {0, 0}, {32, 48} },
  (char *) 0,
};

/* ------------------ East ----------------- */
static Word xT_e[] =
{
  0xE000,
  0x4000,
};
static Bitmap xT_east =
{
  (Word *) xT_e,
  1,
  { {0, 0}, {3, 2} },
  (char *) 0,
};

static Word T_e[96];
static Bitmap T_east = 
{
  (Word *) T_e,
  3,
  { {0, 0}, {48, 32} },
  (char *) 0,
};

/* ------------------ South ---------------- */
static Word xT_s[] =
{
  0x4000,
  0xC000,
  0x4000,
};

static Bitmap xT_south = 
{
  (Word *) xT_s,
  1,
  { {0, 0}, {2, 3} },
  (char *) 0,
};

static Word T_s[96];
static Bitmap T_south = 
{
  (Word *) T_s,
  2,
  { {0, 0}, {32, 48} },
  (char *) 0,
};

/* -------------------- West ----------------- */  
static Word xT_w[] =
{
  0x4000,
  0xE000,
};
static Bitmap xT_west =
{
  (Word *) xT_w,
  1,
  { {0, 0}, {3, 2} },
  (char *) 0,
};

static Word T_w[96];
static Bitmap T_west = 
{
  (Word *) T_w,
  3,
  { {0, 0}, {48, 32} },
  (char *) 0,
};

/* ========================= I - piece ===================== */

/* ------------------------- North / South ----------------- */
static Word xI_n[] =
{
  0x8000,
  0x8000,
  0x8000,
  0x8000,
};
static Bitmap xI_north = 
{
  (Word *) xI_n,
  1,
  { {0, 0}, {1, 4} },
  (char *) 0,
};

static Word I_n[96];
static Bitmap I_north =
{
  (Word *) I_n,
  1,
  { {0, 0}, {16, 64} },
  (char *) 0,
};

/* ------------------------- East / West ------------------- */
static Word xI_e[] =
{
  0xF000,
};
static Bitmap xI_east = 
{
  (Word *) xI_e,
  1,
  { {0, 0}, {4, 1} },
  (char *) 0,
};

static Word I_e[96];
static Bitmap I_east =
{
  (Word *) I_e,
  4,
  { {0, 0}, {64, 16} },
};

/* ======================== Square piece ====================== */
/* -------------------- North / South / East / West =========== */
static Word xO_n[] =
{
  0xC000,
  0xC000,
};
static Bitmap xO_north =
{
  (Word *) xO_n,
  1,
  { {0, 0}, {2, 2} },
  (char *) 0,
};

static Word O_n[96];
static Bitmap O_north =
{
  (Word *) O_n,
  2,
  { {0, 0}, {32, 32} },
  (char *) 0,
};
    

/* ============================================================ */

static GamePiece J_Piece =
{
  0, (Texture16 *) 0,
  { 7, 6, 7, 6, },
  { &xJ_north, &xJ_east, &xJ_south, &xJ_west, },
  { &J_north, &J_east, &J_south, &J_west, },
};

static GamePiece L_Piece =
{
  0, (Texture16 *) 0,
  { 7, 6, 7, 6, },
  { &xL_north, &xL_east, &xL_south, &xL_west, },
  { &L_north, &L_east, &L_south, &L_west, },
};

static GamePiece S_Piece =
{
  0, (Texture16 *) 0,
  { 7, 6, 7, 6, },
  { &xS_north, &xS_east, &xS_north, &xS_east, },
  { &S_north, &S_east, &S_north, &S_east, },
};

static GamePiece Z_Piece =
{
  0, (Texture16 *) 0,
  { 7, 6, 7, 6, },
  { &xZ_north, &xZ_east, &xZ_north, &xZ_east, },
  { &Z_north, &Z_east, &Z_north, &Z_east, },
};

static GamePiece T_Piece =
{
  0, (Texture16 *) 0,
  { 5, 6, 5, 5, },
  { &xT_north, &xT_east, &xT_south, &xT_west, },
  { &T_north, &T_east, &T_south, &T_west, },
};

static GamePiece I_Piece = 
{
  0, (Texture16 *) 0,
  { 8, 5, 8, 5, },
  { &xI_north, &xI_east, &xI_north, &xI_east, },
  { &I_north, &I_east, &I_north, &I_east, },
};

static GamePiece O_Piece =
{
  0, (Texture16 *) 0,
  { 6, 6, 6, 6, },
  { &xO_north, &xO_north, &xO_north, &xO_north, },
  { &O_north, &O_north, &O_north, &O_north, },
};

GamePiece *Pieces[] =
{
  &O_Piece,
  &I_Piece,
  &J_Piece,
  &L_Piece,
  &S_Piece,
  &Z_Piece,
  &T_Piece,
};

/*
 * Pick a game piece. any piece...
 */
GamePiece *Get_Next_Piece()
{
  return Pieces[rand() % 7];
}

/*
 * Set the color of a piece to a texture given.
 */
void Set_Color(smap, col)
     Bitmap *smap;
     Texture16 *col;
{
  texture(smap, smap->rect, col, F_OR);
  texture(smap, smap->rect, col, F_XOR);
}

int Show_Full_Rows(gp)
     GamePiece *gp; /* the game piece that might have caused full rows */
{
  int y, xmax, ymax, ymin, rcount;
  Word *w;
  Bitmap *gbmap = game_board.map;

  if (gp)
    {
      Bitmap *gpmap = gp->gmap_arr[gp->cur];

      ymin = game_board.curpt.y;
      ymax = game_board.curpt.y + gpmap->rect.corner.y;
    }
  else
    {
      ymin = gbmap->rect.origin.y;
      ymax = gbmap->rect.corner.y;
    }
      
  xmax = gbmap->rect.corner.x - 1;

  for (rcount = 0, y = ymin; y < ymax; y++)
    {
      w = addr(gbmap, Pt(0, y));
      if (*w == FULL_ROW_MASK)
 {
   Rectangle r;

   rcount++;
   r.origin = add(add(Drect.origin, Grect.origin), mul(Pt(0, y), 16));
   r.corner = add(r.origin, mul(Pt(xmax, 1), 16));
   texture(&physical, r, &hashmark, F_STORE);
   sleep(30);
 }
    }

  sleep(30);
  return rcount;
}

void Clear_Full_Rows(rcount, gp)
     int rcount;
     GamePiece *gp;
{
  int y, xmax, ymax, ymin;
  Word *w;
  Bitmap *gbmap = game_board.map;

  if (gp)
    {
      Bitmap *gpmap = gp->gmap_arr[gp->cur];

      ymin = game_board.curpt.y;
      ymax = game_board.curpt.y + gpmap->rect.corner.y;
    }
  else
    {
      ymin = gbmap->rect.origin.y;
      ymax = gbmap->rect.corner.y;
    }

  xmax = gbmap->rect.corner.x - 1;
  while (rcount--)
    {
      for (y = ymax-1; y >= ymin; y--)
 {
   w = addr(gbmap, Pt(0,y));
   if (*w == FULL_ROW_MASK)
     {
       Point o;
       Rectangle r;

       /* move game rows down by 1 row */
       bitblt(gbmap,Rpt(Pt(0,0), Pt(xmax, y)), gbmap, Pt(0,1), F_STORE);
       /* clear the top game row */
       rectf(gbmap, Rpt(Pt(0,0), Pt(xmax, 1)), F_CLR);

       /* move displayed rows down by 1 row */
       r.origin = add(Drect.origin, Grect.origin);
       r.corner = add(r.origin, mul(Pt(xmax, y), 16));
       o = add(r.origin, mul(Pt(0, 1), 16));
       bitblt(&physical, r, &physical, o, F_STORE);

       /* clear the top display row */
       r.origin = add(Drect.origin, Grect.origin);
       r.corner = add(r.origin, mul(Pt(xmax, 1), 16));
       rectf(&physical, r, F_CLR);
       break;
     }
 }
    }
}

/*
 * Using the game bitmap create the displayed piece shape.
 */
void Populate_Bitmap(gmap, smap)
     Bitmap *gmap, *smap;
{
  int x, y;
  int bit;
  Word *w;

  if (! eqpt(gmap->rect.origin, Pt(0,0)))
    {
      /* fprintf(stderr, "Populate_Bitmap(): Internal error\n"); */
      sleep(2);
      exit();
    }
  if (gmap->rect.corner.x > 4 || gmap->rect.corner.y > 4)
    {
      /* fprintf(stderr, "Populate_Bitmap(): Internal error\n"); */
      sleep(2);
      exit();
    }

  for (y = gmap->rect.origin.y; y < gmap->rect.corner.y; y++)
    for (x = gmap->rect.origin.x; x < gmap->rect.corner.x; x++)
    {
      w = addr(gmap, Pt(x,y));

      bit = FIRSTBIT >> (x % WORDSIZE);

      /*
       * If the bit is set fill the corresponding block
       * in game piece's shape bitmap
       */
      if ((*w & bit) == bit)
 bitblt(&a_brick, a_brick.rect, smap, Pt((x*16),(y*16)), F_STORE);
    }
}

void Clear_Shadow_Area()
{
  rectf(&physical, raddp(Mrect, Drect.origin), F_CLR);
}
  
void Display_Shadow_XOR(gp)
     GamePiece *gp;
{
  Point origin;
  Bitmap *smap = gp->smap_arr[gp->cur];

  /* Display the shadow */

  origin = add(Drect.origin, Mrect.origin);
  origin = add(origin, Pt(game_board.curpt.x*16, 0));

  shadow.rect.corner.x = smap->rect.corner.x;
  shadow.width = smap->width;

  texture(&shadow, shadow.rect, (Texture16*) brick_bits, F_STORE);
  texture(&shadow, shadow.rect, gp->color, F_XOR);
  bitblt(&shadow, shadow.rect, &physical, origin, F_XOR);
}

void Bitblit_Piece_XOR(gp)
     GamePiece *gp;
{
  Point origin;
  Bitmap *smap = gp->smap_arr[gp->cur];
  Bitmap *gmap = gp->gmap_arr[gp->cur];

  /* Game board bitmap */
  origin = game_board.curpt;
  bitblt(gmap, gmap->rect, game_board.map, origin, F_XOR);
  
  /* Display bitmap */

  origin = fPt(add(add(Drect.origin, Grect.origin), mul(origin, 16)));
  bitblt(smap, smap->rect, &physical, origin, F_XOR);
  Display_Shadow_XOR(gp);
}

void Bitblit_Display_XOR(gp)
     GamePiece *gp;
{
  Point origin;
  Bitmap *smap = gp->smap_arr[gp->cur];

  origin = fPt(add(add(Drect.origin, Grect.origin),mul(game_board.curpt, 16)));

  bitblt(smap, smap->rect, &physical, origin, F_XOR);
  Display_Shadow_XOR(gp);
}

void Bitblit_Game_Board_XOR(gp)
     GamePiece *gp;
{
  Point origin;
  Bitmap *gmap = gp->gmap_arr[gp->cur];

  /* Game board bitmap */
  origin = game_board.curpt;
  bitblt(gmap, gmap->rect, game_board.map, origin, F_XOR);
}

void Draw_Window()
{
  Rectangle r;

  r.origin = sub(Srect.origin, Pt(1, 1));
  r.corner = add(Srect.corner, Pt(1, 1));
  r = raddp(r, Drect.origin);
  box(&display, r, F_STORE);

  r.origin = sub(Grect.origin, Pt(1, 1));
  r.corner = add(Grect.corner, Pt(1, 1));
  r = raddp(r, Drect.origin);
  box(&display, r, F_STORE);

  r.origin = sub(Mrect.origin, Pt(1, 1));
  r.corner = add(Mrect.corner, Pt(1, 1));
  r = raddp(r, Drect.origin);
  box(&display, r, F_STORE);
}

void Clear_Game_Board()
{
  extern Rectangle Grect;
  Rectangle r;

  r.origin = add(Drect.origin, Grect.origin);
  r.corner = add(Drect.origin, Grect.corner);

  /* rectf(&display, Drect, F_CLR); */
  rectf(&display, r, F_CLR);
  rectf(game_board.map, game_board.map->rect, F_CLR);
  /* sleep(2); */
}

void Show_Next_Piece(np)
     GamePiece *np;
{
  Bitmap *smap = np->smap_arr[np->cur];
  Rectangle r;

  r.origin = add(Drect.origin, nploc);
  r.corner = add(r.origin, Pt(64, 64));
  rectf(&physical, r, F_CLR);
  bitblt(smap, smap->rect, &physical, r.origin, F_STORE);
}
