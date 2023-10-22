# How to compile

<br/>

### Requirements / Dependencies

- C++ 20
- Qt6 *(Framework 6.2+, Linguist, Widgets, Charts, Sql, Network)*
- CMake
- gcc / clang / msvc<br/><br/>

<br/>

### Compilation steps

- Prepare a build folder:
  <br/>`mkdir build && cd build`<br/><br/>
- Prepare **CMake**'s build files:
  <br/>`cmake ../logdoctor -DCMAKE_BUILD_TYPE=MinSizeRel`<br/><br/>
- Use **CMake** to compile the entire project:
  <br/>`cmake --build ./ --target all`<br/><br/>

#### Additional steps

- **Linux** / **BSD**:
  - No additional steps. Once compiling is done, you can move the executable file wherever you want and execute it from there.<br/><br/>

- **Windows**:
  - Create a new folder and move the executable in it:
    <br/>`mkdir LogDoctor && move LogDoctor.exe .\LogDoctor`<br/><br/>
  - Add the needed libraries to the executable's folder:<br/>
    - You need to know the path of your Qt installation, default is *C:\Qt*<br/>
    - You need to know which compiler you used
      <br/>`cd C:\<path>\<to>\Qt\<version>\<compiler>\bin`<br/><br/>
    - Deploy Qt's libraries using Qt's additional tool:
      <br/>`windeployqt.exe C:\<path>\<to>\<LogDoctor>`<br/><br/>
    - Deploy C++ libraries by copying them:
      <br/>`copy "libstdc++-6.dll" C:\<path>\<to>\<LogDoctor>`
      <br/>`copy "libwinpthread-1.dll" C:\<path>\<to>\<LogDoctor>`
      <br/>`copy "libgcc_s_seh-1.dll" C:\<path>\<to>\<LogDoctor>`<br/><br/>
  - You can now move the executable's folder wherever you want and execute LogDoctor from there.<br/><br/>

- **Mac OS**:
  - No additional steps. Once compiling is done, you can use the app bundle to execute LogDoctor.

<br/>
