// DO NOT EDIT THIS FILE - it is machine generated -*- c++ -*-

#ifndef __Render__
#define __Render__

#pragma interface

#include <java/lang/Object.h>
#include <gcj/array.h>

extern "Java"
{
  class Render;
  class VERTEX;
  class MyTexture;
  class TLVERTEX;
  class SCANLINE;
  class Matrix;
}

class Render : public ::java::lang::Object
{
public:
  Render (jint, jint);
  static void SetTexture (::MyTexture *);
  static void Clear ();
  static void SetTransform (::Matrix *);
  static void DrawIndexedPrimitive (jint, JArray< ::VERTEX *> *, jshortArray);
  static void TransformAndLighting (JArray< ::VERTEX *> *);
private:
  static void DrawPolygon (::TLVERTEX *, ::TLVERTEX *, ::TLVERTEX *, ::TLVERTEX *);
  static void ScanEdge (::TLVERTEX *, ::TLVERTEX *);
public: // actually package-private
  static const jint PRIMITIVE_POLYGON = 1L;
  static ::Matrix *mMaster;
  static jintArray pbuf;
private:
  static jintArray zbuf;
  static jintArray tbuf;
  static jint BUFFERW;
  static jint BUFFERH;
  static jint TEXTUREW;
  static jint TEXTUREH;
  static jint TEXMASKW;
  static jint TEXMASKH;
  static jint TEXSHIFT;
  static JArray< ::SCANLINE *> *min;
  static JArray< ::SCANLINE *> *max;
  static JArray< ::TLVERTEX *> *tvtx;
public:

  static ::java::lang::Class class$;
};

#endif /* __Render__ */