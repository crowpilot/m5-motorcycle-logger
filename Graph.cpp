#include<M5Stack.h>
#include "Graph.h"

Graph::Graph(int x,int y,int w,int h,TFT_eSprite *graph){
  _graph=graph;
  _x=x;
  _y=y;
  _w=w;
  _h=h;
}

void Graph::init(){
  _graph->setColorDepth(3);
  _graph->createSprite(_w,_h);
  _graph->fillSprite(TFT_BLUE);
  _graph->pushSprite(_x,_y);
  _center=_h/2;
}

void Graph::centerPlot(int val){
  _graph->pushSprite(_x,_y);
  _graph->scroll(1,(val-_lastval));
  _lastval=val;
  _graph->drawPixel(0,15,TFT_GREEN);
}
