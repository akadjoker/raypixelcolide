/*******************************************************************************************
*
*   raylib [textures] example - Load textures from raw data
*
*   NOTE: Images are loaded in CPU memory (RAM); textures are loaded in GPU memory (VRAM)
*
*   This example has been created using raylib 1.3 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2015 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#include <stdlib.h>         // Required for: malloc() and free()


#include "assert.h"

/*returns maximum or minimum of number*/
#define COLLIDE_MAX(a,b)	((a > b) ? a : b)
#define COLLIDE_MIN(a,b)	((a < b) ? a : b)

Color CollideTransparentPixel(Image surface , int x , int y)
{
    if (x < 0 ) return WHITE;
    if (y < 0 ) return WHITE;
    if (x > surface.width ) return WHITE;
    if (y > surface.height ) return WHITE;

    Color *pixels = LoadImageColors(surface);
    Color r = pixels[y*surface.width + x];
    MemFree (pixels);
    return r;
}


int getPixel(Color *pixels,int width,int height, int x , int y)
{
    if (x < 0  ) return false;
    if (y < 0 ) return false;
    if (x > width ) return false;
    if (y > height ) return false;


    Color r = pixels[y*width + x];

    return (int)r.a;

}

bool getTransparentPixel(Color *pixels,int width,int height, int x , int y)
{
    if (x < 0  ) return false;
    if (y < 0 ) return false;
    if (x > width ) return false;
    if (y > height ) return false;


    Color r = pixels[y*width + x];

    return r.a!=0;

}

bool IntersectsPixel(Rectangle hitbox1, Color* pixels1,
                     Rectangle hitbox2, Color* pixels2)
{


    int top    =0;//COLLIDE_MAX(hitbox1.y, hitbox2.y);
    int bottom = COLLIDE_MIN(hitbox1.height, hitbox2.height);
    int right  = COLLIDE_MAX(hitbox1.width, hitbox2.width);
    int left   =0;// COLLIDE_MIN(hitbox1.x, hitbox2.x);


    DrawRectangleLines(left,top,right,bottom,RED);

    for(int y = top; y< bottom; y++)
    {
        for(int x = left; x < right; x++)
        {
      //  Color color1 = pixels1[(int)((x - hitbox1.x) + (y - hitbox1.y) * hitbox1.width)];


            Color color1 = pixels1[(int)((x - hitbox1.x) + (y - hitbox1.y) * hitbox1.width)];
            Color color2 = pixels2[(int)((x - hitbox2.x) + (y - hitbox2.y) * hitbox2.width)];

           // DrawCircle( x - hitbox1.x , y - hitbox1.y,5,color1);
            DrawCircle( x , y,5,color2);

            if (color1.a != 0 && color2.a != 0)
                return true;


        }
    }
        return false;
}

/*
  static bool PerPixelCollision(Sprite a, Sprite b)
    {
        // Get Color data of each Texture
        Color[] bitsA = new Color[a.Width * a.Height];
        a.Texture.GetData(0, a.CurrentFrameRectangle, bitsA, 0, a.Width * a.Height);
        Color[] bitsB = new Color[b.Width * b.Height];
        b.Texture.GetData(0, b.CurrentFrameRectangle, bitsB, 0, b.Width * b.Height);

        // Calculate the intersecting rectangle
        int x1 = (int)Math.Floor(Math.Max(a.Bounds.X, b.Bounds.X));
        int x2 = (int)Math.Floor(Math.Min(a.Bounds.X + a.Bounds.Width, b.Bounds.X + b.Bounds.Width));

        int y1 = (int)Math.Floor(Math.Max(a.Bounds.Y, b.Bounds.Y));
        int y2 = (int)Math.Floor(Math.Min(a.Bounds.Y + a.Bounds.Height, b.Bounds.Y + b.Bounds.Height));

        // For each single pixel in the intersecting rectangle
        for (int y = y1; y < y2; ++y)
        {
            for (int x = x1; x < x2; ++x)
            {
                // Get the color from each texture
                Color colorA = bitsA[(x - (int)Math.Floor(a.Bounds.X)) + (y - (int)Math.Floor(a.Bounds.Y)) * a.Texture.Width];
                Color colorB = bitsB[(x - (int)Math.Floor(b.Bounds.X)) + (y - (int)Math.Floor(b.Bounds.Y)) * b.Texture.Width];

                if (colorA.A != 0 && colorB.A != 0) // If both colors are not transparent (the alpha channel is not 0), then there is a collision
                {
                    return true;
                }
            }
        }
        //If no collision occurred by now, we're clear.
        return false;
    }
    */
int CollidePixel(Color* as , Image a, int ax , int ay ,
                       Color* bs ,Image b, int bx , int by, int skip)
{
	/*a - bottom right co-ordinates in world space*/
	int ax1 = ax + a.width - 1;
	int ay1 = ay + a.height - 1;

	/*b - bottom right co-ordinates in world space*/
	int bx1 = bx + b.width - 1;
	int by1 = by + b.height- 1;





	/*check if bounding boxes intersect*/
	if((bx1 < ax) || (ax1 < bx))
		return 0;
	if((by1 < ay) || (ay1 < by))
		return 0;


	int xstart = COLLIDE_MAX(ax,bx);
	int xend = COLLIDE_MIN(ax1,bx1);

	int ystart = COLLIDE_MAX(ay,by);
	int yend = COLLIDE_MIN(ay1,by1);





	for(int y = ystart ; y <= yend ; y += skip)
	{
		for(int x = xstart ; x <= xend ; x += skip)
		{
			bool ca = getTransparentPixel(as, a.width,a.height,x-ax , y-ay);
			bool cb = getTransparentPixel(bs, b.width,b.height,x-bx , y-by);
			if (ca && cb) return 1;
		}
	}
	return 0;
}

int CollideRect(Color* as , Image a,  Color* bs ,Image b, Rectangle r, int skip)
{

	int xstart = (int)r.x;
	int xend   = (int)r.width;
	int ystart = (int)r.y;
	int yend   = (int)r.height;

	for(int y = ystart ; y <= yend ; y += skip)
	{
		for(int x = xstart ; x <= xend ; x += skip)
		{

			bool ca = getTransparentPixel(as, a.width,a.height, x , y);
			bool cb = getTransparentPixel(bs, b.width,b.height, x , y);
			if (ca && cb) return 1;
		}
	}
	return 0;
}

int Collide(Image as , int ax , int ay ,
                       Image bs , int bx , int by, int skip)
{
	/*a - bottom right co-ordinates in world space*/
	int ax1 = ax + as.width - 1;
	int ay1 = ay + as.height - 1;

	/*b - bottom right co-ordinates in world space*/
	int bx1 = bx + bs.width - 1;
	int by1 = by + bs.height- 1;


	/*check if bounding boxes intersect*/
	if((bx1 < ax) || (ax1 < bx))
		return 0;
	if((by1 < ay) || (ay1 < by))
		return 0;


	int xstart = COLLIDE_MAX(ax,bx);
	int xend = COLLIDE_MIN(ax1,bx1);

	int ystart = COLLIDE_MAX(ay,by);
	int yend = COLLIDE_MIN(ay1,by1);


	DrawLine(xstart,ystart,xend,yend,RED);

	for(int y = ystart ; y <= yend ; y += skip)
	{
		for(int x = xstart ; x <= xend ; x += skip)
		{
			/*compute offsets for surface
			before pass to TransparentPixel test*/
			//if(!SDL_CollideTransparentPixel(as , x-ax , y-ay)
			//&& !SDL_CollideTransparentPixel(bs , x-bx , y-by))
			//	return 1;
			Color a = CollideTransparentPixel(as,x-ax , y-ay);
			Color b = CollideTransparentPixel(bs,x-bx , y-by);



			DrawCircle( x-ax , y-ay,5,a);
			DrawCircle( x-bx , y-by,5,b);
		}
	}

	return 0;
}


bool intersect(Rectangle rectangle1, Rectangle rectangle2, Rectangle* intersection)
{
    if (CheckCollisionRecs(rectangle1,rectangle2))
    {
        intersection->x = COLLIDE_MAX(rectangle1.x, rectangle2.x);
        intersection->width = COLLIDE_MIN(rectangle1.x + rectangle1.width, rectangle2.x + rectangle2.width) - intersection->x;
        intersection->y = COLLIDE_MAX(rectangle1.y, rectangle2.y);
        intersection->height = COLLIDE_MIN(rectangle1.y + rectangle1.height, rectangle2.y + rectangle2.height) - intersection->y;
        return true;
    }
    return false;
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    int h = 0;
	int playerx = 0, playery = 0;

    InitWindow(screenWidth, screenHeight, "raylib [textures] example - texture from raw data");
    SetTargetFPS(30);

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)

    // Load RAW image data (512x512, 32bit RGBA, no file header)
    //Image fudesumiRaw = LoadImageRaw("/media/djoker/code/linux/cpp/raylib/raylib/raylib/examples/textures/resources/wabbit_alpha.png", 384, 512, UNCOMPRESSED_R8G8B8A8, 0);
    Image image = LoadImage("assets/wabbit_alpha.png");
    Texture2D sprite = LoadTextureFromImage(image);  // Upload CPU (RAM) image to GPU (VRAM)


    Image image2 = LoadImage("assets/wabbit_alpha.png");
    Texture2D sprite2 = LoadTextureFromImage(image2);

    // Generate a checked texture by code
    int width = image.width;
    int height = image.height;


    Color *colora = LoadImageColors(image);
    Color *colorb = LoadImageColors(image2);


    // Dynamic memory allocation to store pixels data (Color type)
    Color *pixels = (Color *)malloc(width*height*sizeof(Color));



    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
           // if (((x/32+y/32)/1)%2 == 0) pixels[y*width + x] = ORANGE;
           // else pixels[y*width + x] = GOLD;

        }
    }



    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------


        int deltax = 0, deltay = 0;

		if (IsKeyDown(KEY_UP))
			deltay -= 1;
		if (IsKeyDown(KEY_DOWN))
			deltay += 1;

		if (IsKeyDown(KEY_LEFT))
			deltax -= 1;
		if (IsKeyDown(KEY_RIGHT))
			deltax += 1;





			playerx += deltax;
			playery += deltay;

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLUE);

            //DrawTexture(sprite, screenWidth/2 , screenHeight/2 , WHITE);

            //DrawTexture(sprite, 0,0, WHITE);
        //     DrawTexture(sprite,0,0, WHITE);
           DrawTexture(sprite2,  playerx, playery, WHITE);

            Vector2 v = GetMousePosition();
            int mouse_x=(int)v.x;
            int mouse_y=(int)v.y;


            DrawTexture(sprite,mouse_x,mouse_y , WHITE);

         //    DrawTexture(sprite2,  mouse_x,mouse_y, WHITE);
          //  int px=mouse_x - screenWidth/2  + sprite.width/2 ;
           // int py=mouse_y - screenHeight/2 + sprite.height/2;

          //  int c = getPixel(colora,image.width,image.height,px,py);

          //  Rectangle a=(Rectangle){screenWidth/2, screenHeight/2, sprite.width, sprite.height};
            Rectangle a=(Rectangle){playerx,playery, sprite.width, sprite.height};
          //  Rectangle a=(Rectangle){0,0, sprite.width, sprite.height};
            Rectangle b=(Rectangle){mouse_x,mouse_y, sprite.width, sprite.height};


       //     Rectangle r = GetCollisionRec(a,b);

            //intersect(a,b,&r);




            DrawRectangleLinesEx(a,2,LIME);
            DrawRectangleLinesEx(b,2,LIME);
        //    DrawRectangleLinesEx(r,2,RED);

            if (
		     CollidePixel(colora,image, playerx, playery,      	 colorb,image2,mouse_x,mouse_y,1))

		   //  CollideRect(colora,image,	 colorb,image2,r,1))

            {
            DrawText("Colide", 100,50,12, RED);

            } else
            {
            DrawText("not Colide", 100,50,12, RED);
            }


//              DrawText(TextFormat("Pixel : %d ",c), 10,50,12, RED);
//              DrawText(TextFormat(" %d : %d ",px,py), 10,90,12, RED);


         //   DrawTexture(fudesumi, 430, -30, WHITE);

//         CollidePixel(image, playerx+deltax, playery+deltay,image2,screenWidth/2 - sprite.width/2, screenHeight/2 - sprite.height/2,4);

            DrawText("(c) djokersoft", 310, screenHeight - 20, 10, BROWN);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    MemFree (colora);
    MemFree (colorb);
        UnloadImage(image);                                // Unload CPU (RAM) image data
    UnloadImage(image2);

    UnloadTexture(sprite);    // Texture unloading
    UnloadTexture(sprite2);


    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
