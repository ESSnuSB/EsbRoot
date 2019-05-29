#ifndef ESBROOT_ESBGEOMETRY_SUPERFGD_CUBE_SCINTILATOR_CONSTRUCTION_H
#define ESBROOT_ESBGEOMETRY_SUPERFGD_CUBE_SCINTILATOR_CONSTRUCTION_H 1

#include <G4ThreeVector.hh>
   
#include <globals.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4VisAttributes.hh>
#include <G4PVPlacement.hh>

#include <G4RotationMatrix.hh>

#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4Trap.hh>
#include <G4SubtractionSolid.hh>

#include "G4SDManager.hh"

// used to keep a list of SD logical volumes
#include "G4RegionStore.hh"
#include <G4Region.hh> 

///
/// This class is taken and adapted from /nd280mc/ND280ExtrudedScintConstructor
///
/// Construct a cubic extruded scintillator cube.  The cube
/// are constructed as rectangular bars with edges of same size
/// with the Z axis along the length of the bar, the base is
/// along the X axis, and the height along the Y axis. 
/// 
/// By default, the bars are made with polystyrene, and have "coextruded"
/// TiO2+Polystyrene outer coating.  If the coating thickness can be set to
/// zero with SetCoatingThickness() but will still be constructed.  The bars
/// have a hole for the fiber (default: 0.9 mm), and a fiber (default: 0.5 mm).
/// If the hole radius is less than the fiber radius then neither the fiber
/// nor the hole are constructed.
///

class G4LogicalVolume;

namespace esbroot {
namespace geometry {
namespace superfgd {

class ND280CubeScintConstructor
{

public:

    /** Constructor
     *@param name - name of the constructed cube scintilator
     **/
    ND280CubeScintConstructor(G4String name);

    /** Destructor **/
    virtual ~ND280CubeScintConstructor();

    /** Get a bar of scintillator **/
    virtual G4LogicalVolume *GetPiece(void);

    /** Get the length of the bar to be constructed. **/
    virtual double GetLength(void) {return fLength;};

    /** Set the length of the bar to be constructed.
     *@param len - lenght
     **/
    virtual void SetLength(double len) {fLength = len;};

    /** Get the base width of the extruded bar. **/
    virtual double GetBase(void) {return fBase;};

    /** Set the base width of the extruded bar.
     *@param base - width
     **/
    virtual void SetBase(double base) {fBase = base;}
  
  /** Set the radius of the hole.
     *@param radius - radius
     **/
  virtual void SetHoleRadius(double radius) {fHoleRadius = radius;};
  
  /** Get the radius of the hole. **/
  virtual double GetHoleRadius(void) {return fHoleRadius;};

  /** Set the Fiber Hole position along X axis
     *@param pos - position along X axis
     **/
  virtual void SetHolePosition_X(G4ThreeVector pos) {fHolePositionX = pos;};

  /** Get the Fiber Hole position along X axis **/
  virtual G4ThreeVector GetHolePosition_X(void) {return fHolePositionX;};

  /** Set the Fiber Hole position along Y axis
     *@param pos - position along Y axis
     **/
  virtual void SetHolePosition_Y(G4ThreeVector pos) {fHolePositionY = pos;};

  /** Get the Fiber Hole position along Y axis **/
  virtual G4ThreeVector GetHolePosition_Y(void) {return fHolePositionY;};

  /** Set the Fiber Hole position along Z axis
     *@param pos - position along Z axis
     **/
  virtual void SetHolePosition_Z(G4ThreeVector pos) {fHolePositionZ = pos;};

  /** Get the Fiber Hole position along Z axis **/
  virtual G4ThreeVector GetHolePosition_Z(void) {return fHolePositionZ;};


  /** Set the WLS fiber radius
     *@param rad - radius
     **/
  virtual void SetFiberRadius(double rad){fFiberRadius = rad;};

  /** Get the WLS fiber radius **/
  double GetFiberRadius(){return fFiberRadius;};

  /** Set the WLS fiber material
     *@param mat - material
     **/
  virtual void SetFiberMaterial(std::string mat){fFiberMaterial = mat;};

  /** Get the WLS fiber material **/
  std::string GetFiberMaterial(){return fFiberMaterial;};


  /** Set rotation for the Fiber Hole along X axis
     *@param rot - rotation along X axis
     **/
  virtual void SetHoleRot_X(G4RotationMatrix rot) {fHoleRotX = rot;};
  /** Get rotation for the Fiber Hole along X axis **/
  virtual G4RotationMatrix GetHoleRot_X(void) {return fHoleRotX;};

  /** Set rotation for the Fiber Hole along Y axis
     *@param rot - rotation along Y axis
     **/
  virtual void SetHoleRot_Y(G4RotationMatrix rot) {fHoleRotY = rot;};
  /** Get rotation for the Fiber Hole along Y axis **/
  virtual G4RotationMatrix GetHoleRot_Y(void) {return fHoleRotY;};

  /** Set rotation for the Fiber Hole along Z axis
     *@param rot - rotation along Z axis
     **/
  virtual void SetHoleRot_Z(G4RotationMatrix rot) {fHoleRotZ = rot;};
  /** Get rotation for the Fiber Hole along Z axis **/
  virtual G4RotationMatrix GetHoleRot_Z(void) {return fHoleRotZ;};


  /** Get the thickness of the reflective coating.**/
  virtual double GetCoatingThickness(void) {return fCoatingThickness;}
  
  /** Set the thickness of the reflective coating.
     *@param t - thickness
     **/
    virtual void SetCoatingThickness(double t) {fCoatingThickness=t;}


    /** Set the gap between the bars which corresponds to the space taken by
    *   whatever material is around the extrusion.  E.G. This is the space
    *   taken by the glue that is holding the bars together.
     *@param g - gap
     **/
  virtual void SetGap(double g) {fGap = g;};

    /** Get the gap between the bars which corresponds to the space taken by
         whatever material is around the extrusion. **/ 
    virtual double GetGap(void) {return fGap;};


    /** Get the height of the extruded bar.  The default value will be by a
         call to SetBase(). **/ 
    virtual double GetHeight(void) {return fHeight;};

    /** Set the height of the extruded bar.
     *@param height - height
     **/
    virtual void SetHeight(double height) { fHeight=height;};


    /** The name of the material to use for the scintillator coating.
     *  [Default: ScintillatorCoating]
     *@param mat - coating material
     **/
    virtual void SetCoatingMaterial(const char* mat) {fCoatingMaterial = mat;}
    /** Get the coating material **/ 
    const char* GetCoatingMaterial() const {return fCoatingMaterial.c_str();}

    /** The name of the material to use for the scintillator. 
     *  [Default: ScintillatorCoating]
     *@param mat - scintilator  material
     **/
    virtual void SetScintillatorMaterial(const char* mat) {fScintillatorMaterial = mat;}
    /** Get the scintilator material **/ 
    const char* GetScintillatorMaterial() const {return fScintillatorMaterial.c_str();}

    /** Set Geant4 visibility
     *@param vis - visibility
     **/
    virtual void SetVisibility(bool vis) {fIsVisible = vis;}

    /** Get current visibility **/ 
    virtual bool GetVisibility(void) {return false; /*fIsVisible;*/}

    /** Set scintilator name
     *@param name - name
     **/
    virtual void SetName(G4String name) {fname = name;}
    /** Get current name **/ 
    virtual G4String GetName(void) {return fname;}

  
private:

    void Init(void);

    /// The length of the scintillator bar.
    double fLength;

    G4String fname;

    /// The width of the base of the scintillator.  The base is along the X
    /// axis.
    double fBase;
    
    /// The height of the scintillator.  If the bar is triangular this will be
    /// calculated when the base is set, but may be overridden.  
    double fHeight;

    /// The radius of the hole.
    double fHoleRadius;

    // Fiber Hole position along X, Y and Z axes
    G4ThreeVector fHolePositionX;
    G4ThreeVector fHolePositionY;
    G4ThreeVector fHolePositionZ;

    // Rotation for the Fiber Hole along X, Y and Z axes
    G4RotationMatrix fHoleRotX;
    G4RotationMatrix fHoleRotY;
    G4RotationMatrix fHoleRotZ;
  
    /// The thickness of the material that is wrapping the scintillator (this
    /// may be extruded).
    double fCoatingThickness;

    /// The inner corner radius of the extruded wrapping for square FGD
    /// scintillators
    //double fCoatingRadius;

    /// Space outside of the bar to allow for glueing and mounting.
    double fGap;

    /// Flag if the created bar should be set to visible for G4 visualization. 
    bool fIsVisible;

    /// The name of the material to use for the coating.
    std::string fCoatingMaterial;

    /// The name of the material to use for the scintillator.
    std::string fScintillatorMaterial;

    /// Flag if we want to use FGD scintillator material for this bar.
    bool fUseFGDScint;

    /// The sensitive detector for this extruded bar.
    G4VSensitiveDetector* fSensitiveDetector;

    /// The WLS fiber material
    std::string fFiberMaterial;

    /// The WLS fiber radius
    double fFiberRadius;
};

}   //superfgd
}   //geometry
}   //esbroot

#endif
