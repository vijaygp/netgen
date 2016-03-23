#ifndef TOPOLOGY
#define TOPOLOGY

/**************************************************************************/
/* File:   topology.hh                                                    */
/* Author: Joachim Schoeberl                                              */
/* Date:   27. Apr. 01                                                    */
/**************************************************************************/

/*
    Mesh topology
    (Elements, Faces, Edges, Vertices
*/

namespace netgen
{
  
struct T_EDGE
{
  int orient:1;
  int nr:31;    // 0-based
};

struct T_FACE
{
  int forient:3;
  int fnr:29;    // 0-based
};


class MeshTopology
{
  const Mesh & mesh;
  bool buildedges;
  bool buildfaces;

  Array<INDEX_2> edge2vert;
  Array<INDEX_4> face2vert;
  Array<T_EDGE[12]> edges;
  Array<T_FACE[6]> faces;
  Array<T_EDGE[4]> surfedges;
  Array<T_EDGE> segedges;
  Array<T_FACE> surffaces;
  Array<INDEX_2> surf2volelement;
  Array<int> face2surfel;
  TABLE<ElementIndex,PointIndex::BASE> *vert2element;
  TABLE<SurfaceElementIndex,PointIndex::BASE> *vert2surfelement;
  TABLE<SegmentIndex,PointIndex::BASE> *vert2segment;
  int timestamp;
public:
  int GetNSurfedges() const {return surfedges.Size();}

  MeshTopology (const Mesh & amesh);
  ~MeshTopology ();

  void SetBuildEdges (bool be)
  { buildedges = be; }
  void SetBuildFaces (bool bf)
  { buildfaces = bf; }

  bool HasEdges () const
  { return buildedges; }
  bool HasFaces () const
  { return buildfaces; }

  void Update();


  int GetNEdges () const { return edge2vert.Size(); }
  int GetNFaces () const { return face2vert.Size(); }

  static inline short int GetNVertices (ELEMENT_TYPE et);
  static inline short int GetNPoints (ELEMENT_TYPE et);
  static inline short int GetNEdges (ELEMENT_TYPE et);
  static inline short int GetNFaces (ELEMENT_TYPE et);

  static const Point3d * GetVertices (ELEMENT_TYPE et);
  inline static const ELEMENT_EDGE * GetEdges1 (ELEMENT_TYPE et);
  inline static const ELEMENT_EDGE * GetEdges0 (ELEMENT_TYPE et);
  inline static const ELEMENT_FACE * GetFaces1 (ELEMENT_TYPE et);
  inline static const ELEMENT_FACE * GetFaces0 (ELEMENT_TYPE et);
  
  int GetSegmentEdge (int segnr) const { return segedges[segnr-1].nr+1; }
  int GetEdge (SegmentIndex segnr) const { return segedges[segnr].nr; }

  void GetSegmentEdge (int segnr, int & enr, int & orient) const
  {
    enr = segedges.Get(segnr).nr+1;
    orient = segedges.Get(segnr).orient;
  }

  void GetElementEdges (int elnr, Array<int> & edges) const;
  void GetElementFaces (int elnr, Array<int> & faces, bool withorientation = false) const;
  void GetElementEdgeOrientations (int elnr, Array<int> & eorient) const;
  void GetElementFaceOrientations (int elnr, Array<int> & forient) const;

  int GetElementEdges (int elnr, int * edges, int * orient) const;
  int GetElementFaces (int elnr, int * faces, int * orient) const;

  void GetFaceVertices (int fnr, Array<int> & vertices) const;
  void GetFaceVertices (int fnr, int * vertices) const;
  void GetEdgeVertices (int enr, int & v1, int & v2) const;
  void GetEdgeVertices (int enr, PointIndex & v1, PointIndex & v2) const;
  const int * GetEdgeVerticesPtr (int enr) const { return &edge2vert[enr][0]; }
  const int * GetFaceVerticesPtr (int fnr) const { return &face2vert[fnr][0]; }
  void GetFaceEdges (int fnr, Array<int> & edges, bool withorientation = false) const;

  ELEMENT_TYPE GetFaceType (int fnr) const;

  void GetSurfaceElementEdges (int elnr, Array<int> & edges) const;
  int GetSurfaceElementFace (int elnr) const;
  void GetSurfaceElementEdgeOrientations (int elnr, Array<int> & eorient) const;
  int GetSurfaceElementFaceOrientation (int elnr) const;
  void GetEdges (SurfaceElementIndex elnr, Array<int> & edges) const;
  int GetFace (SurfaceElementIndex elnr) const;

  int GetSurfaceElementEdges (int elnr, int * edges, int * orient) const;

  const T_EDGE * GetElementEdgesPtr (int elnr) const { return &edges[elnr][0]; }
  const T_EDGE * GetSurfaceElementEdgesPtr (int selnr) const { return &surfedges[selnr][0]; }
  const T_EDGE * GetSegmentElementEdgesPtr (int selnr) const { return &segedges[selnr]; }

  const T_FACE * GetElementFacesPtr (int elnr) const { return &faces[elnr][0]; }
  const T_FACE * GetSurfaceElementFacesPtr (int selnr) const { return &surffaces[selnr]; }


  void GetSurface2VolumeElement (int selnr, int & elnr1, int & elnr2) const
  { 
    elnr1 = surf2volelement.Get(selnr)[0];
    elnr2 = surf2volelement.Get(selnr)[1];
  }

  int GetFace2SurfaceElement (int fnr) const { return face2surfel[fnr-1]; }
  
  void GetVertexElements (int vnr, Array<ElementIndex> & elements) const;
  FlatArray<ElementIndex> GetVertexElements (int vnr) const
  { return (*vert2element)[vnr]; }

  void GetVertexSurfaceElements( int vnr, Array<SurfaceElementIndex>& elements ) const;
  FlatArray<SurfaceElementIndex> GetVertexSurfaceElements (int vnr) const
  { return (*vert2surfelement)[vnr]; }

  FlatArray<SegmentIndex> GetVertexSegments (int vnr) const
  { return (*vert2segment)[vnr]; }
  
  int GetVerticesEdge ( int v1, int v2) const;
  void GetSegmentVolumeElements ( int segnr, Array<ElementIndex> & els ) const;
  void GetSegmentSurfaceElements ( int segnr, Array<SurfaceElementIndex> & els ) const;
};










inline short int MeshTopology :: GetNVertices (ELEMENT_TYPE et)
{
  switch (et)
    {
    case SEGMENT:
    case SEGMENT3:
      return 2;

    case TRIG:
    case TRIG6:
      return 3;

    case QUAD:
    case QUAD6:
    case QUAD8:
      return 4;

    case TET:
    case TET10:
      return 4;

    case PYRAMID:
      return 5;

    case PRISM:
    case PRISM12:
      return 6;

    case HEX:
      return 8;

      // default:
      // cerr << "Ng_ME_GetNVertices, illegal element type " << et << endl;
    }
  return 0;
}


inline short int MeshTopology :: GetNPoints (ELEMENT_TYPE et)
{
  switch (et)
    {
    case SEGMENT:
      return 2;
    case SEGMENT3:
      return 3;

    case TRIG:
      return 3;
    case TRIG6:
      return 6;

    case QUAD:
    case QUAD6:
    case QUAD8:
      return 4;

    case TET:
      return 4;
    case TET10:
      return 10;

    case PYRAMID:
      return 5;

    case PRISM:
    case PRISM12:
      return 6;

    case HEX:
      return 8;

      // default:
      // cerr << "Ng_ME_GetNVertices, illegal element type " << et << endl;
    }
  return 0;
}



inline short int MeshTopology :: GetNEdges (ELEMENT_TYPE et)
{
  __assume(et >= SEGMENT && et <= HEX);
  switch (et)
    {
    case SEGMENT:
    case SEGMENT3:
      return 1;

    case TRIG:
    case TRIG6:
      return 3;

    case QUAD:
    case QUAD6:
    case QUAD8:
      return 4;

    case TET:
    case TET10:
      return 6;

    case PYRAMID:
      return 8;

    case PRISM:
    case PRISM12:
      return 9;

    case HEX:
      return 12;
    default:
      return 0;
      // default:
      // cerr << "Ng_ME_GetNEdges, illegal element type " << et << endl;
    }
  // return 0;
}


inline short int MeshTopology :: GetNFaces (ELEMENT_TYPE et)
{
  __assume(et >= SEGMENT && et <= HEX);  
  switch (et)
    {
    case SEGMENT:
    case SEGMENT3:
      return 0;

    case TRIG:
    case TRIG6:
      return 1;

    case QUAD:
    case QUAD6:
    case QUAD8:
      return 1;

    case TET:
    case TET10:
      return 4;

    case PYRAMID:
      return 5;

    case PRISM:
    case PRISM12:
      return 5;

    case HEX:
      return 6;

    default:
      return -99;
      // default:
      // cerr << "Ng_ME_GetNVertices, illegal element type " << et << endl;
    }
}






const ELEMENT_EDGE * MeshTopology :: GetEdges1 (ELEMENT_TYPE et)
{
  static int segm_edges[1][2] =
    { { 1, 2 }};

  static int trig_edges[3][2] =
    { { 3, 1 },
      { 2, 3 },        
      { 1, 2 }};

  static int quad_edges[4][2] =
    { { 1, 2 },
      { 3, 4 },
      { 4, 1 },
      { 2, 3 }};


  static int tet_edges[6][2] =
    { { 4, 1 },
      { 4, 2 },
      { 4, 3 }, 
      { 1, 2 },
      { 1, 3 },
      { 2, 3 }};

  static int prism_edges[9][2] =
    { { 3, 1 },
      { 1, 2 },
      { 3, 2 },
      { 6, 4 },
      { 4, 5 },
      { 6, 5 },
      { 3, 6 },
      { 1, 4 },
      { 2, 5 }};

  static int pyramid_edges[8][2] =
    { { 1, 2 },
      { 2, 3 },
      { 1, 4 },
      { 4, 3 },
      { 1, 5 },
      { 2, 5 },
      { 3, 5 },
      { 4, 5 }};

  static int hex_edges[12][2] =
    {
      { 1, 2 },
      { 3, 4 },
      { 4, 1 },
      { 2, 3 },
      { 5, 6 },
      { 7, 8 },
      { 8, 5 },
      { 6, 7 },
      { 1, 5 },
      { 2, 6 },
      { 3, 7 },
      { 4, 8 },
    };

  switch (et)
    {
    case SEGMENT:
    case SEGMENT3:
      return segm_edges;

    case TRIG:
    case TRIG6:
      return trig_edges;

    case QUAD:
    case QUAD6:
    case QUAD8:
      return quad_edges;

    case TET:
    case TET10:
      return tet_edges;

    case PYRAMID:
      return pyramid_edges;

    case PRISM:
    case PRISM12:
      return prism_edges;

    case HEX:
      return hex_edges;
      // default:
      // cerr << "Ng_ME_GetEdges, illegal element type " << et << endl;
    }
   return 0;  
}



const ELEMENT_EDGE * MeshTopology :: GetEdges0 (ELEMENT_TYPE et)
{
  static int segm_edges[1][2] =
    { { 0, 1 }};

  static int trig_edges[3][2] =
    { { 2, 0 },
      { 1, 2 },        
      { 0, 1 }};

  static int quad_edges[4][2] =
    { { 0, 1 },
      { 2, 3 },
      { 3, 0 },
      { 1, 2 }};


  static int tet_edges[6][2] =
    { { 3, 0 },
      { 3, 1 },
      { 3, 2 }, 
      { 0, 1 },
      { 0, 2 },
      { 1, 2 }};

  static int prism_edges[9][2] =
    { { 2, 0 },
      { 0, 1 },
      { 2, 1 },
      { 5, 3 },
      { 3, 4 },
      { 5, 4 },
      { 2, 5 },
      { 0, 3 },
      { 1, 4 }};

  static int pyramid_edges[8][2] =
    { { 0, 1 },
      { 1, 2 },
      { 0, 3 },
      { 3, 2 },
      { 0, 4 },
      { 1, 4 },
      { 2, 4 },
      { 3, 4 }};

  static int hex_edges[12][2] =
    {
      { 0, 1 },
      { 2, 3 },
      { 3, 0 },
      { 1, 2 },
      { 4, 5 },
      { 6, 7 },
      { 7, 4 },
      { 5, 6 },
      { 0, 4 },
      { 1, 5 },
      { 2, 6 },
      { 3, 7 },
    };

  switch (et)
    {
    case SEGMENT:
    case SEGMENT3:
      return segm_edges;

    case TRIG:
    case TRIG6:
      return trig_edges;

    case QUAD:
    case QUAD6:
    case QUAD8:
      return quad_edges;

    case TET:
    case TET10:
      return tet_edges;

    case PYRAMID:
      return pyramid_edges;

    case PRISM:
    case PRISM12:
      return prism_edges;

    case HEX:
      return hex_edges;
      // default:
      // cerr << "Ng_ME_GetEdges, illegal element type " << et << endl;
    }
   return 0;  
}










inline const ELEMENT_FACE * MeshTopology :: GetFaces1 (ELEMENT_TYPE et)
{
  static const int trig_faces[1][4] = 
    { { 1, 2, 3, 0 } };
  static const int quad_faces[1][4] = 
    { { 1, 2, 3, 4 } };

  static const int tet_faces[4][4] =
    { { 4, 2, 3, 0 },
      { 4, 3, 1, 0 },
      { 4, 1, 2, 0 },
      { 1, 3, 2, 0 } };
  
  static const int prism_faces[5][4] =
    {
      { 1, 3, 2, 0 },
      { 4, 5, 6, 0 },
      { 3, 1, 4, 6 },
      { 1, 2, 5, 4 },
      { 2, 3, 6, 5 } 
    };

  static const int pyramid_faces[5][4] =
    {
      { 1, 2, 5, 0 },
      { 2, 3, 5, 0 },
      { 3, 4, 5, 0 },
      { 4, 1, 5, 0 },
      { 1, 4, 3, 2 } 
    };

  static const int hex_faces[6][4] =
    {
      { 1, 4, 3, 2 },
      { 5, 6, 7, 8 },
      { 1, 2, 6, 5 },
      { 2, 3, 7, 6 },
      { 3, 4, 8, 7 },
      { 4, 1, 5, 8 }
    };


  
  switch (et)
    {
    case TRIG:
    case TRIG6:
      return trig_faces;

    case QUAD:
    case QUAD6:
    case QUAD8:
      return quad_faces;


    case TET:
    case TET10:
      return tet_faces;

    case PRISM:
    case PRISM12:
      return prism_faces;

    case PYRAMID:
      return pyramid_faces;

    case SEGMENT:
    case SEGMENT3:

    case HEX:
      return hex_faces;

      // default:
      // cerr << "Ng_ME_GetVertices, illegal element type " << et << endl;
    }
  return 0;
}





inline const ELEMENT_FACE * MeshTopology :: GetFaces0 (ELEMENT_TYPE et)
{
  static const int trig_faces[1][4] = 
    { { 0, 1, 2, -1 } };
  static const int quad_faces[1][4] = 
    { { 0, 1, 2, 3 } };

  static const int tet_faces[4][4] =
    { { 3, 1, 2, -1 },
      { 3, 2, 0, -1 },
      { 3, 0, 1, -1 },
      { 0, 2, 1, -1 } };
  
  static const int prism_faces[5][4] =
    {
      { 0, 2, 1, -1 },
      { 3, 4, 5, -1 },
      { 2, 0, 3, 5 },
      { 0, 1, 4, 3 },
      { 1, 2, 5, 4 } 
    };

  static const int pyramid_faces[5][4] =
    {
      { 0, 1, 4, -1 },
      { 1, 2, 4, -1 },
      { 2, 3, 4, -1 },
      { 3, 0, 4, -1 },
      { 0, 3, 2, 1 } 
    };

  static const int hex_faces[6][4] =
    {
      { 0, 3, 2, 1 },
      { 4, 5, 6, 7 },
      { 0, 1, 5, 4 },
      { 1, 2, 6, 5 },
      { 2, 3, 7, 6 },
      { 3, 0, 4, 7 }
    };


  
  switch (et)
    {
    case TRIG:
    case TRIG6:
      return trig_faces;

    case QUAD:
    case QUAD6:
    case QUAD8:
      return quad_faces;


    case TET:
    case TET10:
      return tet_faces;

    case PRISM:
    case PRISM12:
      return prism_faces;

    case PYRAMID:
      return pyramid_faces;

    case SEGMENT:
    case SEGMENT3:

    case HEX:
      return hex_faces;

      // default:
      // cerr << "Ng_ME_GetVertices, illegal element type " << et << endl;
    }
  return 0;
}

}

#endif
