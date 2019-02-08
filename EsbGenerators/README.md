This directory contains ESSnuSB interaction generators, i.e. implementations of FairGenerator abstract class. Pythia generators are left as an example.


In order to run install Genie the following steps should be done:


1./ During installation if it cannot find Mathmore library 
check its existence in the "root" folder. It should be :

$ROOTSYS/lib/root is located libMathMore.so (on the current version with FairRoot installation)

in the document in looks lie:
update Genie/configure 
$mathmore_lib = "$ROOTSYS/lib/libMathMore.so";
  if( ! -f $mathmore_lib ) {
     die ("*** Error *** ROOT needs to be built with GSL/MathMore enabled.");
  }

to
$mathmore_lib = "$ROOTSYS/lib/root/libMathMore.so";

2./ run the configuration file with 
$configure --disable-lhapdf5
to disable lhapdf5 since we do not have it installed

3./ Donwload cross_sections.xml for genie
and  set the following variable
export CSFILE=/path_to_cross_sections.xml
e.g.
export CSFILE="/home/georgi/Downloads/genie_xsec/v3_00_02/NULL/G0000a00000-k500-e1000/data/gxspl-FNALsmall.xml"

4./ Add to config.sh the following
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$GENIE"/lib"
export GXMLPATH=$GENIE"/config"
export CSFILE="/home/georgi/Downloads/genie_xsec/v3_00_02/NULL/G0000a00000-k500-e1000/data/gxspl-FNALsmall.xml"
export GXMLPATHS=$GXMLPATH"/G00_00a":$GXMLPATH"/G00_00b":$GXMLPATH"/G00_00z":$GXMLPATH"/G18_01a":GXMLPATH"/G18_01b":GXMLPATH"/G18_02a":GXMLPATH"/G18_02b":GXMLPATH"/G18_10a":GXMLPATH"/G18_10b":GXMLPATH"/G18_10i":GXMLPATH"/G18_10j":GXMLPATH"/GDM18_00a":GXMLPATH"/GDM18_00b":GXMLPATH"/GDM18_01a"

5./ edint ~/build/config.sh -> the path of the Genie libraries has to be added 
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$GENIE"/lib"
(NOTE this should be updated in the CMake file to set it automatically)


6./ For the moment, root cannot find its libraries.
So when running through 
$root 
the following libraries have to be loaded manually

gSystem->Load("libGFwMsg");
gSystem->Load("libGFwReg");
gSystem->Load("libGFwAlg");
gSystem->Load("libGFwParDat");
gSystem->Load("libPhysics");
gSystem->Load("libGFwInt");
gSystem->Load("libGFwGHEP");
gSystem->Load("libxml2");
gSystem->Load("libTree");
gSystem->Load("libGFwEG");
gSystem->Load("libGFwNtp");
gSystem->Load("libHist");
gSystem->Load("libGFwNum");
gSystem->Load("libGFwUtl");
gSystem->Load("liblog4cpp");
gSystem->Load("libGTlFlx");
gSystem->Load("libGeom");
gSystem->Load("libGTlGeo");

gSystem->Load("libASImageGui.so")
gSystem->Load("libASImage.so")
gSystem->Load("libCling.so")
gSystem->Load("libcomplexDict.so")
gSystem->Load("libCore.so")
gSystem->Load("libdequeDict.so")
gSystem->Load("libEGPythia6.so")
gSystem->Load("libEGPythia8.so")
gSystem->Load("libEG.so")
gSystem->Load("libEve.so")
gSystem->Load("libFitPanel.so")
gSystem->Load("libFoam.so")
gSystem->Load("libforward_listDict.so")
gSystem->Load("libFTGL.so")
gSystem->Load("libFumili.so")
gSystem->Load("libGdml.so")
gSystem->Load("libGed.so")
gSystem->Load("libGenetic.so")
gSystem->Load("libGenVector.so")
gSystem->Load("libGeomBuilder.so")
gSystem->Load("libGeomPainter.so")
gSystem->Load("libGeom.so")
gSystem->Load("libGLEW.so")
gSystem->Load("libGpad.so")
gSystem->Load("libGraf3d.so")
gSystem->Load("libGraf.so")
gSystem->Load("libGuiBld.so")
gSystem->Load("libGuiHtml.so")
gSystem->Load("libGui.so")
gSystem->Load("libGviz3d.so")
gSystem->Load("libGX11.so")
gSystem->Load("libGX11TTF.so")
gSystem->Load("libHistFactory.so")
gSystem->Load("libHistPainter.so")
gSystem->Load("libHist.so")
gSystem->Load("libHtml.so")
gSystem->Load("libImt.so")
gSystem->Load("libJupyROOT.so")
gSystem->Load("liblistDict.so")
gSystem->Load("libmap2Dict.so")
gSystem->Load("libmapDict.so")
gSystem->Load("libMathCore.so")
gSystem->Load("libMathMore.so")
gSystem->Load("libMatrix.so")
gSystem->Load("libMemStat.so")
gSystem->Load("libMinuit2.so")
gSystem->Load("libMinuit.so")
gSystem->Load("libMLP.so")
gSystem->Load("libmultimap2Dict.so")
gSystem->Load("libmultimapDict.so")
gSystem->Load("libMultiProc.so")
gSystem->Load("libmultisetDict.so")
gSystem->Load("libNet.so")
gSystem->Load("libNetxNG.so")
gSystem->Load("libNetx.so")
gSystem->Load("libNew.so")
gSystem->Load("libPhysics.so")
gSystem->Load("libPostscript.so")
gSystem->Load("libProofBench.so")
gSystem->Load("libProofDraw.so")
gSystem->Load("libProofPlayer.so")
gSystem->Load("libProof.so")
gSystem->Load("libProofx.so")
gSystem->Load("libPyROOT.so")
gSystem->Load("libQuadp.so")
gSystem->Load("libRecorder.so")
gSystem->Load("libRGL.so")
gSystem->Load("libRHTTP.so")
gSystem->Load("libRint.so")
gSystem->Load("libRIO.so")
gSystem->Load("libRooFitCore.so")
gSystem->Load("libRooFit.so")
gSystem->Load("libRooStats.so")
gSystem->Load("libRootAuth.so")
gSystem->Load("libSessionViewer.so")
gSystem->Load("libsetDict.so")
gSystem->Load("libSmatrix.so")
gSystem->Load("libSpectrumPainter.so")
gSystem->Load("libSpectrum.so")
gSystem->Load("libSPlot.so")
gSystem->Load("libSQLIO.so")
gSystem->Load("libSrvAuth.so")
gSystem->Load("libtbbmalloc_proxy.so")
gSystem->Load("libtbbmalloc.so")
gSystem->Load("libtbb.so")
gSystem->Load("libThread.so")
gSystem->Load("libTMVAGui.so")
gSystem->Load("libTMVA.so")
gSystem->Load("libTreePlayer.so")
gSystem->Load("libTree.so")
gSystem->Load("libTreeViewer.so")
gSystem->Load("libUnfold.so")
gSystem->Load("libunordered_mapDict.so")
gSystem->Load("libunordered_multimapDict.so")
gSystem->Load("libunordered_multisetDict.so")
gSystem->Load("libunordered_setDict.so")
gSystem->Load("libvalarrayDict.so")
gSystem->Load("libvectorDict.so")
gSystem->Load("libVMC.so")
gSystem->Load("libX3d.so")
gSystem->Load("libXMLIO.so")
gSystem->Load("libXMLParser.so")
gSystem->Load("libXrdProofd.so")

gSystem->Load("libGPhXSIg.so")
gSystem->Load("libGPhNuclSt.so")
gSystem->Load("libGPhNuElXS.so")
gSystem->Load("libGPhQELXS.so")
gSystem->Load("libGPhHadTransp.so")
gSystem->Load("libGPhPDF.so")
gSystem->Load("libGPhCmn.so")
gSystem->Load("libGFwAlg.so")
gSystem->Load("libGFwEG.so")
gSystem->Load("libGFwGHEP.so")
gSystem->Load("libGFwInt.so")
gSystem->Load("libGFwMsg.so")
gSystem->Load("libGFwNtp.so")
gSystem->Load("libGFwNum.so")
gSystem->Load("libGFwParDat.so")
gSystem->Load("libGFwReg.so")
gSystem->Load("libGFwUtl.so")
gSystem->Load("libGPhAMNGEG.so")
gSystem->Load("libGPhAMNGXS.so")
gSystem->Load("libGPhChmXS.so")
gSystem->Load("libGPhCohEG.so")
gSystem->Load("libGPhCohXS.so")
gSystem->Load("libGPhDcy.so")
gSystem->Load("libGPhDeEx.so")
gSystem->Load("libGPhDfrcEG.so")
gSystem->Load("libGPhDfrcXS.so")
gSystem->Load("libGPhHadnz.so")
gSystem->Load("libGPhDISEG.so")
gSystem->Load("libGPhDISXS.so")
gSystem->Load("libGPhGlwResEG.so")
gSystem->Load("libGPhGlwResXS.so")
gSystem->Load("libGPhQELEG.so")
gSystem->Load("libGPhNuElEG.so")

gSystem->Load("libGPhIBDEG.so")
gSystem->Load("libGPhIBDXS.so")
gSystem->Load("libGPhMEL.so")
gSystem->Load("libGPhMNucEG.so")
gSystem->Load("libGPhMNucXS.so")
gSystem->Load("libGPhResEG.so")
gSystem->Load("libGPhResXS.so")
gSystem->Load("libGPhStrEG.so")
gSystem->Load("libGPhStrXS.so")
gSystem->Load("libGTlFlx.so")
gSystem->Load("libGTlGeo.so")

























