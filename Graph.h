#ifndef Graph_h
  #define Graph_h

#include<M5Stack.h>

class Graph{
  public:
    Graph(uint8_t x,uint8_t y,uint8_t w,uint8_t h,uint8_t mesh=0);
    void plotV(int val);
    void plotH(int val);
    void centerPlot(int val);
  private:
    int _val;
    int _lastval;
    uint8_t _center;
    uint8_t _x;
    uint8_t _y;
    uint8_t _w;
    uint8_t _h;
    TFT_eSprite _graph=TFT_eSprite(&M5.Lcd);
    uint8_t _mesh;
    uint8_t _count;
};
#endif
