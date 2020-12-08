#include<M5Stack.h>
#include "Graph.h"

Graph::Graph(uint8_t x,uint8_t y,uint8_t w,uint8_t h,uint8_t mesh){
  //pos x, pos y,width,height,mesh-interval
  _x=x;
  _y=y;
  _w=w;
  _h=h;
  _mesh=mesh;
  _count=0;
  _graph.setColorDepth(3);
  _graph.createSprite(_w,_h);
  _graph.fillSprite(TFT_BLUE);
  _center=_h/2;
}

void Graph::init(){
  _graph.pushSprite(_x,_y);
}

void Graph::centerPlot(int val){
  _graph.pushSprite(_x,_y);
  _graph.scroll(1,(val-_lastval));
  _lastval=val;
  _graph.drawPixel(0,15,TFT_GREEN);
}

void Graph::plotV(int val){
  _graph.pushSprite(_x,_y);
  _graph.scroll(0,1);
  for(int i=0;i<=_w;i=i+_mesh){
    _graph.drawPixel(i,0,TFT_BLUE);
  }
  if(_count==_mesh){
    _count=0;
    _graph.drawFastHLine(0,0,_w,TFT_BLUE);
  }
  _graph.drawPixel(val,0,TFT_GREEN);
  _count++;
}

void Graph::plotH(int val){
  _graph.pushSprite(_x,_y);
  _graph.scroll(1,0);
  for(int i=0;i<=_h;i=i+_mesh){
    _graph.drawPixel(0,i,TFT_BLUE);
  }
  if(_count==_mesh){
    _count=0;
    _graph.drawFastVLine(0,0,_h,TFT_BLUE);
  }
  _graph.drawPixel(0,val,TFT_GREEN);
  _count++;
}
