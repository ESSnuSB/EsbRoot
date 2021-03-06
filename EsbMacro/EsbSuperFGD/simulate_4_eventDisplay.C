/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

/*
  
  How to use:
  .L eventDisplay.C
  FairEventManager* man = eventDisplay();
  man->NextEvent();

 */

FairEventManager* simulate_4_eventDisplay(Bool_t addTracks=kTRUE, 
			       Bool_t addPoints=kTRUE /* kFALSE*/ )
{

  using namespace esbroot;
  
  TString inFile  = "evetest.root";
  TString parFile = "params.root";
  TString outFile = "out.root";

  // -----   Reconstruction run   -------------------------------------------
  FairRunAna *fRun= new FairRunAna();
  FairFileSource *fFileSource = new FairFileSource(inFile);
  fRun->SetSource(fFileSource);
 
  fRun->SetSink(new FairRootFileSink(outFile));

  // -----  Parameter database   --------------------------------------------
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();

  FairParRootFileIo* parIo1 = new FairParRootFileIo();
	parIo1->open(parFile);
	rtdb->setFirstInput(parIo1);
  // ------------------------------------------------------------------------



  FairEventManager   *fMan      = new FairEventManager   ();
  FairMCTracks       *Track     = new FairMCTracks       ("MCTrack");
  FairMCPointDraw    *EsbPoints = new FairMCPointDraw    ("FgdDetectorPoint",kBlue , kFullSquare);

  if(addTracks)
    fMan->AddTask(Track);
  if(addPoints)
    fMan->AddTask(EsbPoints);


  fMan->Init();
  return fMan;
}
