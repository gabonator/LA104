// http://archive.gamedev.net/archive/reference/articles/article359.html

struct star_t {
  int x, y, z;
};

struct pos_t {
  int x, y;
};

constexpr int Num = 250;
star_t stars[Num];
pos_t clear[2][Num];

void Init()
{
  for (int i=0; i<Num; i++)
  {
    do 
    {  
      stars[i].x = (rand()%128)-64;
      stars[i].y = (rand()%64)-32;
      stars[i].z = i+1; 
    } while (stars[i].x == 0 && stars[i].y == 0);
  }
}

void Calcstars()
{
  for (int i=0; i<Num; i++)
  {
    int x = (stars[i].x << 7) / stars[i].z + 64;
    int y = (stars[i].y << 7) / stars[i].z + 32;
    clear[0][i].x = x;
    clear[0][i].y = y;
  }
}

void Drawstars()
{
  for (int i=0; i<Num; i++)
  {
    int x = clear[0][i].x;
    int y = clear[0][i].y;
    if (x >= 0 && x < 128 && y >= 0 && y < 64)
    {
      putpixel(x, y, 1);
    }
  }   
}

void Clearstars()
{
  for (int i=0; i<Num; i++)
  {
    int x = clear[1][i].x;
    int y = clear[1][i].y;
    if (x >= 0 && x < 128 && y >= 0 && y < 64)
    {
      putpixel(x, y, 0);
    }
  }  
}

void Movestars()
{
  for (int i=0; i<Num; i++)
  {
    stars[i].z -= 2;
    if (stars[i].z < 1)
      stars[i].z += Num;
  }
}

void Draw()
{
  Calcstars();
  Drawstars();
  Movestars();
}