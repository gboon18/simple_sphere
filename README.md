# How to run
### Note: Geant4 simulation is much slower than OpenMC.
```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX=/usr/local \
      -DGeant4_DIR=/usr/local/lib/cmake/Geant4 \
      -DOPENGL_GLES2_INCLUDE_DIR=/usr/include \
      -DOPENGL_GLES3_INCLUDE_DIR=/usr/include \
      -DPTL_DIR=/usr/local/lib/cmake/Geant4/PTL \
      -DQt5Core_DIR=/usr/lib/x86_64-linux-gnu/cmake/Qt5Core \
      -DQt5Gui_DIR=/usr/lib/x86_64-linux-gnu/cmake/Qt5Gui \
      -DQt5OpenGL_DIR=/usr/lib/x86_64-linux-gnu/cmake/Qt5OpenGL \
      -DQt5Widgets_DIR=/usr/lib/x86_64-linux-gnu/cmake/Qt5Widgets \
      -DQt5_DIR=/usr/lib/x86_64-linux-gnu/cmake/Qt5 \
      -DVTK_DIR=/usr/local/lib/cmake/vtk-9.3 \
      -DWITH_GEANT4_VIS=ON \
      /path/to/source
```
Change the paths in cmake for the user's case.
Currently, the simulation uses multi-threading. It is not necessary to use it.
```
make -j2
```
This will create `gdml_det` executable.
```
./gdml_det sphere_hdpe.gdml
```
let's you see the geometry of `sphere_hdpe.gdml` geometry (HDPE material sphere) in the simulation.
```
./gdml_det sphere_hdep.gdml run.mac output_file_name.csv
```
runs the simulation using the `run.mac` macro: 1e5 x 2.45 MeV neutron beams.
See `run.sh` as an example macro to run multiple simulation in a row.
Energy deposition histogram will be save named as `output_file_name_h1_edep.csv` in csv format.

## Opening the histogram
The histogram binning is defined in `src/RunAction.cc` line 57.
```
  analysisManager->CreateH1("edep" ,"edep", 1e4, 0., 20*MeV);
```
`1e4` defines the number of bins, `0., 20*MeV` defines the energy range.
The output .csv file has four columns. Use "entires" (the first column) to plot the histogram.
You would need to convert the each bin number to corresponding energy.
For example, the first bin would represent the energy range of [0*20/10000, 1*20/10000) MeV.
The i-th bin would represent the energy range of [(i-1)*20/10000, i*20/10000) MeV.
