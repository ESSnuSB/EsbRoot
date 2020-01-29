#ifndef ESBROOT_ESBGEOMETRY_SUPERFGD_CUBE_SCINTILATOR_CONSTRUCTION_H
#define ESBROOT_ESBGEOMETRY_SUPERFGD_CUBE_SCINTILATOR_CONSTRUCTION_H 1

#include "TObject.h"
#include "TGeoVolume.h"
#include "TGeoManager.h"
#include "TGeoMaterial.h"
#include "TGeoMedium.h"

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
/// NOTE: The geometry has been rewritten to use ROOT geometry classes


namespace esbroot {
namespace geometry {
namespace superfgd {

 struct ThreeVector{

   ThreeVector():X(0),Y(0),Z(0) {}

   ThreeVector(double x, double y, double z):X(x),Y(y),Z(z) {}

   ~ThreeVector(){}

   double X;
   double Y;
   double Z;
   };

struct RotMatrix{

   RotMatrix():rotateX(0),rotateY(0),rotateZ(0) {}

   RotMatrix(double rotx, double roty, double rotz):rotateX(rotx),rotateY(roty),rotateZ(rotz) {}

   ~RotMatrix(){}

   double rotateX;
   double rotateY;
   double rotateZ;
   };

class CubeScintConstructor : public TObject
{

public:
   
      /** Constructor **/
      CubeScintConstructor();

      /** Destructor **/
      virtual ~CubeScintConstructor();

      /** Get the length of the bar to be constructed. **/
      double GetLength(void) {return fLength;};

      /** Set the length of the bar to be constructed.
      *@param len - lenght
      **/
      void SetLength(double len) {fLength = len;};

      /** Get the base width of the extruded bar. **/
      double GetBase(void) {return fBase;};

      /** Set the base width of the extruded bar.
      *@param base - width
      **/
      void SetBase(double base) {fBase = base;}
  
      /** Set the radius of the hole.
      *@param radius - radius
      **/
      void SetHoleRadius(double radius) {fHoleRadius = radius;};
  
      /** Get the radius of the hole. **/
      double GetHoleRadius(void) {return fHoleRadius;};

      /** Set the Fiber Hole position along X axis
      *@param pos - position along X axis
      **/
      void SetHolePosition_X(const ThreeVector& pos) {fHolePositionX = pos;};

      /** Get the Fiber Hole position along X axis **/
      const ThreeVector& GetHolePosition_X(void) {return fHolePositionX;};

      /** Set the Fiber Hole position along Y axis
      *@param pos - position along Y axis
      **/
      void SetHolePosition_Y(const ThreeVector& pos) {fHolePositionY = pos;};

      /** Get the Fiber Hole position along Y axis **/
      const ThreeVector& GetHolePosition_Y(void) {return fHolePositionY;};

      /** Set the Fiber Hole position along Z axis
      *@param pos - position along Z axis
      **/
      void SetHolePosition_Z(const ThreeVector& pos) {fHolePositionZ = pos;};

      /** Get the Fiber Hole position along Z axis **/
      const ThreeVector& GetHolePosition_Z(void) {return fHolePositionZ;};


      /** Set the WLS fiber radius
      *@param rad - radius
      **/
      void SetFiberRadius(double rad){fFiberRadius = rad;};

      /** Get the WLS fiber radius **/
      double GetFiberRadius(){return fFiberRadius;};

      /** Set the WLS fiber material
      *@param mat - material
      **/
      void SetFiberMaterial(std::string mat){fFiberMaterial = mat;};

      /** Get the WLS fiber material **/
      std::string GetFiberMaterial(){return fFiberMaterial;};


      /** Set rotation for the Fiber Hole along X axis
      *@param rot - rotation along X axis
      **/
      void SetHoleRot_X(const RotMatrix& rot) {fHoleRotX = rot;};
      /** Get rotation for the Fiber Hole along X axis **/
      RotMatrix GetHoleRot_X(void) {return fHoleRotX;};

      /** Set rotation for the Fiber Hole along Y axis
      *@param rot - rotation along Y axis
      **/
      void SetHoleRot_Y(const RotMatrix& rot) {fHoleRotY = rot;};
      /** Get rotation for the Fiber Hole along Y axis **/
      RotMatrix GetHoleRot_Y(void) {return fHoleRotY;};

      /** Set rotation for the Fiber Hole along Z axis
      *@param rot - rotation along Z axis
      **/
      void SetHoleRot_Z(const RotMatrix& rot) {fHoleRotZ = rot;};
      /** Get rotation for the Fiber Hole along Z axis **/
      RotMatrix GetHoleRot_Z(void) {return fHoleRotZ;};


      /** Get the thickness of the reflective coating.**/
      double GetCoatingThickness(void) {return fCoatingThickness;}
  
      /** Set the thickness of the reflective coating.
      *@param t - thickness
      **/
      void SetCoatingThickness(double t) {fCoatingThickness=t;}


      /** Set the gap between the bars which corresponds to the space taken by
      *   whatever material is around the extrusion.  E.G. This is the space
      *   taken by the glue that is holding the bars together.
      *@param g - gap
      **/
      void SetGap(double g) {fGap = g;};

      /** Get the gap between the bars which corresponds to the space taken by
      whatever material is around the extrusion. **/ 
      double GetGap(void) {return fGap;};


      /** Get the height of the extruded bar.  The default value will be by a
      call to SetBase(). **/ 
      double GetHeight(void) {return fHeight;};

      /** Set the height of the extruded bar.
      *@param height - height
      **/
      void SetHeight(double height) { fHeight=height;};


      /** The name of the material to use for the scintillator coating.
      *  [Default: ScintillatorCoating]
      *@param mat - coating material
      **/
      void SetCoatingMaterial(const char* mat) {fCoatingMaterial = mat;}
      /** Get the coating material **/ 
      const char* GetCoatingMaterial() const {return fCoatingMaterial.c_str();}

      /** The name of the material to use for the scintillator. 
      *  [Default: ScintillatorCoating]
      *@param mat - scintilator  material
      **/
      void SetScintillatorMaterial(const char* mat) {fScintillatorMaterial = mat;}
      /** Get the scintilator material **/ 
      const char* GetScintillatorMaterial() const {return fScintillatorMaterial.c_str();}

      /** Set Geant4 visibility
      *@param vis - visibility
      **/
      void SetVisibility(bool vis) {fIsVisible = vis;}

      /** Get current visibility **/ 
      bool GetVisibility(void) {return false; /*fIsVisible;*/}

      /** Construct the TGeoVolume geometry for the cube **/
      TGeoVolume* Construct(void);

      /** Return a pointer to the cube geometry **/
      TGeoVolume* GetCubeVolume() {return fcubeTVol;}

      /** Return a pointer to the sensitive volume of the cube**/
      TGeoVolume* GetSensitiveVolume(){return fcubeScntVol;}

private:

      /// The length of the scintillator bar.
      double fLength;

      /// The width of the base of the scintillator.  The base is along the X
      /// axis.
      double fBase;
    
      /// The height of the scintillator.  If the bar is triangular this will be
      /// calculated when the base is set, but may be overridden.  
      double fHeight;

      /// The radius of the hole.
      double fHoleRadius;

      // Fiber Hole position along X, Y and Z axes
      ThreeVector fHolePositionX;
      ThreeVector fHolePositionY;
      ThreeVector fHolePositionZ;

      // Rotation for the Fiber Hole along X, Y and Z axes
      RotMatrix fHoleRotX;
      RotMatrix fHoleRotY;
      RotMatrix fHoleRotZ;
  
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

      /// The WLS fiber material
      std::string fFiberMaterial;

      /// The WLS fiber radius
      double fFiberRadius;

      //! pointer to the constructed cubeVolume
      TGeoVolume* fcubeTVol;

      //! pointer to the scintilator (sensitive) volume of the cube
      TGeoVolume* fcubeScntVol;

      ClassDef(CubeScintConstructor,2)
};

}   //superfgd
}   //geometry
}   //esbroot

#endif
