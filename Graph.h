#ifndef Graph_h
  #define Graph_h

class Graph{
  public:
    Graph(int x,int y,int w,int h,TFT_eSprite *graph);
    void init();
    void plot(int val);
    void centerPlot(int val);
    void pushH(void);
    void pushV(void);
  private:
    int _val;
    int _lastval;
    int _center;
    int _x;
    int _y;
    int _w;
    int _h;
    TFT_eSprite *_graph;
};
#endif
