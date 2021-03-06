#ifdef NG_PYTHON
#ifdef OCCGEOMETRY

#include <../general/ngpython.hpp>

#include <meshing.hpp>
#include <occgeom.hpp>

using namespace netgen;

namespace netgen
{
  extern std::shared_ptr<NetgenGeometry> ng_geometry;
}


DLL_HEADER void ExportNgOCC(py::module &m) 
{
  py::class_<OCCGeometry, shared_ptr<OCCGeometry>> (m, "OCCGeometry")
    .def(py::init<>())
      ;
    m.def("LoadOCCGeometry",FunctionPointer([] (const string & filename)
           {
             cout << "load OCC geometry";
             ifstream ist(filename);
             OCCGeometry * instance = new OCCGeometry();
             instance = LoadOCC_STEP(filename.c_str());
             return shared_ptr<OCCGeometry>(instance, NOOP_Deleter);
           }));
  m.def("GenerateMesh", FunctionPointer([] (shared_ptr<OCCGeometry> geo, MeshingParameters &param)
					{
					  auto mesh = make_shared<Mesh>();
					  SetGlobalMesh(mesh);
					  mesh->SetGeometry(geo);
					  ng_geometry = geo;
					  try
					    {
					      geo->GenerateMesh(mesh,param);
					    }
					  catch (NgException ex)
					    {
					      cout << "Caught NgException: " << ex.What() << endl;
					    }
					  return mesh;
					}))
    ;
}

PYBIND11_PLUGIN(libNgOCC) {
  py::module m("NgOCC", "pybind NgOCC");
  ExportNgOCC(m);
  return m.ptr();
}

#endif // OCCGEOMETRY
#endif // NG_PYTHON
