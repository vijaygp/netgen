configure_file(__init__.py ${CMAKE_CURRENT_BINARY_DIR}/__init__.py @ONLY)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/__init__.py
        meshing.py csg.py geom2d.py stl.py gui.py NgOCC.py
        DESTINATION ${NG_INSTALL_DIR_PYTHON}/${NG_INSTALL_SUFFIX}
        COMPONENT netgen
        )
