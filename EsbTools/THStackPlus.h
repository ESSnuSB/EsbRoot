#ifndef ESBROOT_ESBTOOLS_THSTACKPLUS_H
#define ESBROOT_ESBTOOLS_THSTACKPLUS_H

#include "THStack.h"

#include <TString.h>

#include <functional>

class TH1;
class TH1D;
class TDirectory;

namespace esbroot {
namespace tools {

class THStackPlus : public THStack {
	public:
	THStackPlus() : protoHisto_(NULL) {};
	THStackPlus(const char* name, const char* title) :
    THStack(name, title) {common_constructor();};
  THStackPlus(const char* name, const char* title, TH1* proto) :
    THStack(name, title) {common_constructor(); proto->SetName(TString(name) + "-proto"); SetProtoHisto(proto);}
	THStackPlus(const char* name, const char* title, TH1 const& proto) :
    THStack(name, title) {common_constructor(); SetProtoHisto(dynamic_cast<TH1*>(proto.Clone(TString(name) + "-proto")));}
  virtual ~THStackPlus();
	  
	TH1 *GetProtoHisto() {return(protoHisto_);};
	void SetProtoHisto(TH1* protoHisto);
	
  TH1D *Get1D(const char *name);

	TH1D *GetCreate1D(const char* name, const char* title);
	TH1D *GetCreate1D(const char* name) {return(GetCreate1D(name, name));};
  
  void WriteToDirectory(TDirectory *outDir);
  void ReadFromDirectory(TDirectory *outDir);

  virtual void Add(TH1* h, Option_t* option = "");
 
  virtual void Draw(Option_t* chopt = "");
  virtual void DrawClone(Option_t* chopt = "") {dynamic_cast<THStackPlus*>(this->Clone())->Draw(chopt);};
  virtual THStackPlus* DrawNormalized(Option_t* chopt = "");

	THStackPlus* SmartClone(const char* newname = "") {return(dynamic_cast<THStackPlus*>(this->Clone()));};

  virtual Int_t Write(const char *name = 0, Int_t	option = 0, Int_t	bufsize = 0) const;

  void SyncTextLabels(); //Because ROOT is crap

  THStack *MakeTHStack(const char* name = NULL, const char* title = NULL) const;

	void ApplyToAll(std::function<void(TH1D*)> const& func);

  static void SetSaveAsTHStack(int option) {save_as_thstack_ = option;};
  static int GetSaveAsTHStack() {return(save_as_thstack_);};

  protected:
  void common_constructor();
  //~ Color_t GetNewColor();
	
	private:
	TH1 *protoHisto_;
  //~ Color_t currentColor_;
	
  static int save_as_thstack_;
  
	ClassDef(THStackPlus, 3)
};

} //namespace tools
} //namespace esbroot
#endif //ESBROOT_ESBTOOLS_THSTACKPLUS_H
