#ifndef ESBROOT_ESBGEOMETRY_SUPERFGD_FGD_CONSTRUCTION_H
#define ESBROOT_ESBGEOMETRY_SUPERFGD_FGD_CONSTRUCTION_H 1

#include "TObject.h"
#include "TGeoVolume.h"
#include "TGeoManager.h"
#include "TGeoMaterial.h"
#include "TGeoMedium.h"

#include "FairGeoLoader.h"

/// Create a bounding box to contain the geometry of the detector 

namespace esbroot {
namespace geometry {
namespace superfgd {

class SuperFGDConstructor  : public TObject
{
public:

    /** Constructor
     *@param name - name of the constructed fgd detector
     *@param gGeoMan - class containing detector materials
     **/
    SuperFGDConstructor(std::string name, FairGeoLoader *geoLoad);

    /** Destructor **/
    ~SuperFGDConstructor();
  
    /** Return a Root volume **/
    TGeoVolume* GetPiece(void);

    /** Set size of cube edges  
     *@param w - width
     **/
    void SetEdge(double w) {fEdge = w;}
    /** Get size of cube edge **/
    double GetEdge(void) {return fEdge;}

    /** Set the width of the SuperFGD detector bounding box "along" the X axis 
     *@param w - width
     **/
    void SetWidth(double w) {fWidth = w;}
    /** Get the width of the SuperFGD detector **/
    double GetWidth(void) {return fWidth;}
 
    /** Set the width of the SuperFGD detector bounding box "along" the Y axis 
     *@param h - Height
     **/
    void SetHeight(double h) {fHeight = h;}
    /** Get the height of the SuperFGD detector **/
    double GetHeight(void) {return fHeight;}
    
    /** Set the length of the SuperFGD detector bounding box "along" the beam axis (the
    * Z axis
    *@param l - Length
     **/
    void SetLength(double l) {fLength = l;}
    /** Get the length of the SuperFGD detector **/
    double GetLength(void) {return fLength;}

    /** Set number of cubes along X
     *@param num - number of cubes along X
     **/
    void SetCubeNumX(int num) {fCubeNumX = num;}
    /** Get number of cubes along X **/
    int GetCubeNumX() {return fCubeNumX;}

    /** Set number of cubes along Y
     *@param num - number of cubes along Y
     **/
    void SetCubeNumY(int num) {fCubeNumY = num;}
     /** Get number of cubes along Y **/
    int GetCubeNumY() {return fCubeNumY;}

    /** Set number of cubes along Z
     *@param num - number of cubes along Z
     **/
    void SetCubeNumZ(int num) {fCubeNumZ = num;}
    /** Get number of cubes along Z **/
    int GetCubeNumZ() {return fCubeNumZ;}


    /** Set the X position of the SuperFGD detector 
     *@param pos - position along X
     **/    
    void SetPosX(double pos) {fPosX = pos;}
    /** Get the X position of the SuperFGD detector  **/
    double GetPosX(void) {return fPosX;}
  
    /** Set the Y position of the SuperFGD detector 
     *@param pos - position along Y
     **/      
    void SetPosY(double pos) {fPosX = pos;}
    /** Get the Y position of the SuperFGD detector **/    
    double GetPosY(void) {return fPosY;}
  
    /** Set the Z position of the SuperFGD detector 
     *@param pos - position along Z
     **/ 
    void SetPosZ(double pos) {fPosZ = pos;}
    /** Get the Z position of the SuperFGD detector **/  
    double GetPosZ(void) {return fPosZ;}

    /** Set the radius of the WLS fiber.
     *@param radius - radius
     **/ 
    virtual void SetFiberRadius(double radius) {fFiberRadius = radius;};
  
    /** Get the radius of the WLS fiber. **/
    virtual double GetFiberRadius(void) {return fFiberRadius;};

  
    /** The name of the material to use for the fiber. 
     *  [Default: FiberCore]
     *@param mat - fiber  material
     **/
    virtual void SetFiberMaterial(const char* mat) {fFiberMaterial = mat;}
    /** Get the fiber material **/
    const char* GetFiberMaterial() const { return fFiberMaterial.c_str();}


    /** Set the name of the cube
     *@param name - cube name
     **/
    virtual void SetNameCube  (std::string name){fNameCube  =name;};

    /** Get the name of the cube **/
    virtual std::string GetNameCube  (){return fNameCube;};

    /** Get the name of the constructor **/
    std::string GetName() {return fname;};

    /** Set the name of the constructor
     *@param name - name
     **/
    void  SetName(std::string name){fname = name;}

    /** Set Geant4 visibility
     *@param vis - visibility
     **/
    virtual void SetVisibility(bool vis) {fIsVisible = vis;}

    /** Get current visibility **/ 
    virtual bool GetVisibility(void) {return fIsVisible;}

    /** Define the used material in the geometry **/ 
    void DefineMaterials();

private:

    /** Constructs the TgeoVolume of the detector**/
    void Construct();

    // Size of the SuperFGD
    double fWidth;
    double fHeight;
    double fLength;

    bool fIsVisible;

    /// The edge of a cube of the SuperFGD detector 
    double fEdge;

    // Number of cubes along each axis 
    int fCubeNumX;
    int fCubeNumY;
    int fCubeNumZ;
  
    // Position of the center of the SuperFGD detector
    double fPosX;
    double fPosY;
    double fPosZ;

    /// The radius of the WLS fiber.
    double fFiberRadius;

    /** Class to hold the materials **/
    FairGeoLoader* fgeoLoad;
  
    /// The name of the material to use for the fiber.
    std::string fFiberMaterial;
  
    // Detector names
    std::string fNameCube;   // Single cube

    std::string fname;   // this name

    TGeoVolume* fVolume;

    void Init(void);

    ClassDef(SuperFGDConstructor,2)
};

}   //superfgd
}   //geometry
}   //esbroot

#endif
