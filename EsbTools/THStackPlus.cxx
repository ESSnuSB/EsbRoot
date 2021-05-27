#include "EsbTools/THStackPlus.h"

#include <cstdlib>
#include <iostream>
#include <cstring>
#include <set>
#include <string>
#include <functional>

#include "TH1D.h"

#include "TList.h"
#include "TObject.h"
#include "TDirectory.h"
#include "TClass.h"
#include "TKey.h"
#include "TROOT.h"
#include "TPad.h"
#include "TObjString.h"
#include "THashList.h"
#include "TBuffer.h"

namespace esbroot {
namespace tools {

int THStackPlus::save_as_thstack_ = false;

THStackPlus::~THStackPlus() {
  delete protoHisto_;
}

void THStackPlus::common_constructor() {
  SetProtoHisto(NULL);
  gDirectory->Add(this);
}


TH1D *THStackPlus::GetCreate1D(const char* name, const char* title) {
	TH1D *tmpHisto = Get1D(name);
  
  if(tmpHisto != NULL) {
		if( std::strcmp(tmpHisto->GetTitle(), title) ) {
			std::cerr << __PRETTY_FUNCTION__ << " FATAL: Wrong title.";
			exit(EXIT_FAILURE);
		}
    return(tmpHisto);
	} else {
    if(GetProtoHisto() == NULL) {
      std::cerr << __PRETTY_FUNCTION__ << " FATAL: " << name << " does not exist in "
        << GetName() << " and there is no proto histogram. Aborting." << std::endl;
        exit(EXIT_FAILURE);
    }
		if(GetProtoHisto()->IsA() == TH1D::Class()) {
			TH1D *protoHisto = static_cast<TH1D*>(GetProtoHisto());
			TH1D *newHisto = new TH1D(*protoHisto);
			newHisto->SetName(name);
      newHisto->SetTitle(title);
			Add(newHisto);
			return(newHisto);
		} else {
			std::cerr << __PRETTY_FUNCTION__ << " FATAL: prototype histogram not TH1D. Aborting." << std::endl;
			exit(EXIT_FAILURE);
		}
	}

}

void THStackPlus::WriteToDirectory(TDirectory *outDir) {
  if(GetHists() == NULL) return;
  
  {
    TIterator *it = GetHists()->MakeIterator();
    while(TH1 *histo = static_cast<TH1*>(it->Next())) {
      histo->SetDirectory(outDir);
      histo->Write();
    }
    delete it;
  }
}

void THStackPlus::ReadFromDirectory(TDirectory *inDir) {
  if(GetHists() != NULL) {
    std::cerr << __PRETTY_FUNCTION__ << " FATAL: Trying to read into a stack which is not empty. Aborting." << std::endl;
    exit(EXIT_FAILURE);
  }
  
  TList *objs = inDir->GetList();
  if(objs != NULL) {
    TIterator *it = objs->MakeIterator();
    while(TObject *obj = it->Next()) {
      if(obj->IsA()->InheritsFrom(TH1::Class())) {
        Add(static_cast<TH1*>(obj));
      }
    }
    delete it;
  }
  
  TList *keys = inDir->GetListOfKeys();
  if(keys != NULL) {
    TIterator *it = keys->MakeIterator();
    while(TKey *key = static_cast<TKey *>(it->Next())) {
      TClass *tmpClass = gROOT->GetClass(key->GetClassName());
      if(tmpClass->InheritsFrom(TH1::Class())) {
        Add(static_cast<TH1*>(key->ReadObj()));
      }
    }
  }
  
  
}

TH1D* THStackPlus::Get1D(const char *name) {
	TObject *tmpObject;
  
  if(GetHists() == NULL) {
    tmpObject = NULL;
  } else {
    tmpObject = GetHists()->FindObject(name);
  }
  
  if(tmpObject != NULL) {
		if(tmpObject->IsA() == TH1D::Class()) {
			return(static_cast<TH1D*>(tmpObject));
		} else {
			std::cerr << __PRETTY_FUNCTION__ << " FATAL: " << name << "exists in " << GetName() << "and is not TH1D! Aborting." << std::endl;
			exit(EXIT_FAILURE);
		}
  } else return(NULL);
}

void THStackPlus::Add(TH1* h, Option_t* option /* = "" */) {
  if(h==NULL) return;
  h->SetDirectory(NULL);
  THStack::Add(h, option);
}

void THStackPlus::Draw(Option_t* chopt /* = "" */) {
  THStack::Draw(chopt);
  static_cast<TPad*>(gPad)->BuildLegend();
}

void THStackPlus::SetProtoHisto(TH1* protoHisto) {
  protoHisto_ = protoHisto;
  if(protoHisto_ != NULL) protoHisto_->SetDirectory(NULL);
}

void THStackPlus::SyncTextLabels() { //Because ROOT is crap
  //Make a set of all labels
  if(GetHists() == NULL) return;
  
  std::set<std::string> known_labels;
  TIterator *iter = GetHists()->MakeIterator();
  while(TH1D* hist = dynamic_cast<TH1D*>(iter->Next())) {
    TIterator *iter1 = hist->GetXaxis()->GetLabels()->MakeIterator();
    while( TObjString* label = dynamic_cast<TObjString*>(iter1->Next()) ) {
      known_labels.insert(label->String().Data());
    }
    delete(iter1);
  }
  delete(iter);
  
  //Add missing labels to histograms
  iter = GetHists()->MakeIterator();
  while(TH1D* hist = dynamic_cast<TH1D*>(iter->Next())) {
    for(auto it = known_labels.begin(); it != known_labels.end(); ++it) {
      hist->GetXaxis()->FindBin(it->c_str());
    }
    hist->LabelsDeflate("x");
    hist->LabelsOption("a", "x");
  }
  delete(iter);
}

THStackPlus* THStackPlus::DrawNormalized(Option_t* chopt /* = "" */) {
  TString clone_name = this->GetName();
  clone_name += "_normalized";
  
  THStackPlus *ts_clone = dynamic_cast<THStackPlus *>(this->Clone(clone_name));
  
  double total_norm = 0.0;
  TIterator *iter = ts_clone->GetHists()->MakeIterator();
  while( TH1D* hist = dynamic_cast<TH1D*>(iter->Next()) ) {
    total_norm += hist->Integral();
  }
  iter->Reset();
  while( TH1D* hist = dynamic_cast<TH1D*>(iter->Next()) ) {
    hist->Scale(1.0/total_norm);
  }
  delete(iter);
  
  ts_clone->Draw(chopt);
  return(ts_clone);
  
}

THStack *THStackPlus::MakeTHStack(const char* name /*= NULL*/, const char* title /*= NULL*/) const
{
  const char *new_name, *new_title;
  new_name = (name == NULL) ? GetName() : name;
  new_title = (title == NULL) ? GetTitle() : title;
  
  THStack *retval = new THStack(new_name, new_title);
  if(GetHists() == NULL) return(retval);
  
  {
    TIterator *it = GetHists()->MakeIterator();
    while(TH1 *histo = dynamic_cast<TH1*>(it->Next())) {
      retval->Add(histo);
    }
    delete(it);
  }
  
  return(retval);
}

/*virtual*/ Int_t THStackPlus::Write(const char *name /*= 0*/, Int_t option /*= 0*/, Int_t	bufsize /*= 0*/) const
{
  if(GetSaveAsTHStack()) {
    THStack *ths = MakeTHStack();
    Int_t retval = ths->Write(name, option, bufsize);
    delete(ths);
    return(retval);
  } else {
    return(TObject::Write(name, option, bufsize));
  }
}

void THStackPlus::ApplyToAll(std::function<void(TH1D*)> const& func) {
	for(auto&& obj : *GetHists()) {
		func(dynamic_cast<TH1D*>(obj));
	}
}


} //namespace tools
} //namespace esbroot
