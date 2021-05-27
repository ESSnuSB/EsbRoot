#ifndef ESBROOT_ESBTOOLS_GENIEANAEVENTDATA_H
#define ESBROOT_ESBTOOLS_GENIEANAEVENTDATA_H

#include <cstddef>
#include "Rtypes.h"

class TObjString;
class TTree;

namespace esbroot {
namespace tools {

struct GenieAnaEventData
{
	static constexpr size_t npmax = 1000;

	int EvtNum;
	TObjString* EvtCode = nullptr;
	int StdHepN;
	int StdHepPdg[npmax];
	int StdHepStatus[npmax];
	double StdHepP4[npmax][4];
	int StdHepFd[npmax];
	int StdHepLd[npmax];
	int StdHepFm[npmax];
	int StdHepLm[npmax];

	void ConnectToTree(TTree* gtrac_tree);
	void PrintEvent();
	
	ClassDef(GenieAnaEventData, 2)
};

}
}

#endif //ESBROOT_ESBTOOLS_GENIEANAEVENTDATA_H
