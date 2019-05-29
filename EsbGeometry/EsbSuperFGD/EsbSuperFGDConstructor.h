#ifndef ESBROOT_ESBGEOMETRY_SUPERFGD_FGD_CONSTRUCTION_H
#define ESBROOT_ESBGEOMETRY_SUPERFGD_FGD_CONSTRUCTION_H 1

#include <G4ThreeVector.hh>
#include <G4Material.hh>
class G4LogicalVolume;


/// Create a bounding box to contain the off axis detector.  The method get
/// GetPiece() constructs a new unplaced magnet that is facing up along the Z
/// axis.  The calling code should then rotate the object into position and
/// and place the object into a large mother volume.

namespace esbroot {
namespace geometry {
namespace superfgd {

class ND280SuperFGDConstructor  
{
public:

    /** Constructor
     *@param name - name of the constructed fgd detector
     **/
    ND280SuperFGDConstructor(G4String name);

    /** Destructor **/
    virtual ~ND280SuperFGDConstructor();
  
    /** Construct and return a G4 volume for the object.  This is a pure
    * virtual function, which means it must be implemented by the inheriting
    * classes.  This returns an unplaced logical volume which faces along
    * the Z axis.**/
    virtual G4LogicalVolume* GetPiece(void);
  

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


    /** Set the name of the repetition along XYZ
     *@param name - name along XYZ
     **/
    virtual void SetNameRepXYZ(G4String name){fNameRepXYZ=name;};

    /** Set the name of the repetition along XZ
     *@param name - name along XZ
     **/
    virtual void SetNameRepXZ (G4String name){fNameRepXZ =name;};

    /** Set the name of the repetition along Z
     *@param name - name along Z
     **/
    virtual void SetNameRepZ  (G4String name){fNameRepZ  =name;};

    /** Set the name of the cube
     *@param name - cube name
     **/
    virtual void SetNameCube  (G4String name){fNameCube  =name;};

    /** Get the name of the repetition along XYZ **/
    virtual G4String GetNameRepXYZ(){return fNameRepXYZ;};

    /** Get the name of the repetition along XZ **/
    virtual G4String GetNameRepXZ (){return fNameRepXZ;};

    /** Get the name of the repetition along Z **/
    virtual G4String GetNameRepZ  (){return fNameRepZ;};

    /** Get the name of the cube **/
    virtual G4String GetNameCube  (){return fNameCube;};

    /** Get the name of the constructor **/
    G4String GetName() {return fname;};

    /** Set the name of the constructor
     *@param name - name
     **/
    void  SetName(G4String name){fname = name;}

    /** Set Geant4 visibility
     *@param vis - visibility
     **/
    virtual void SetVisibility(bool vis) {fIsVisible = vis;}

    /** Get current visibility **/ 
    virtual bool GetVisibility(void) {return fIsVisible;}

    /** Define the used material in the geometry **/ 
    void DefineMaterials();

    /** Return the defined material for the requested name **/ 
    static G4Material* FindMaterial(G4String material);

private:
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
  
    /// The name of the material to use for the fiber.
    std::string fFiberMaterial;
  
    // Detector names
    G4String fNameRepXYZ; // Replica of layers along Y
    G4String fNameRepXZ;  // Replica of rows along X
    G4String fNameRepZ;   // Replica of cubes along Z
    G4String fNameCube;   // Single cube

    G4String fname;   // this name
    G4Material* fDefaultMaterial;

    void Init(void);
};

}   //superfgd
}   //geometry
}   //esbroot

#endif
