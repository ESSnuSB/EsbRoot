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

  Peter comment 21/1-2019:
  Does not work for me with the points. Also the tracks are mainly shown for cherenkov photons and not for muons and electrons...

 */

FairEventManager* eventDisplay(Bool_t addTracks=kTRUE, 
			       Bool_t addPoints=kFALSE)
{

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
  FairMCPointDraw    *EsbPoints = new FairMCPointDraw    ("EsbWCDetectorPoint",kBlue , kFullSquare);

  if(addTracks)
    fMan->AddTask(Track);
  if(addPoints)
    fMan->AddTask(EsbPoints);

  fMan->Init();
  return fMan;
}
