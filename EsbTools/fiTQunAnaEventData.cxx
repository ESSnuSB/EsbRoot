#include "EsbTools/fiTQunAnaEventData.h"

#include <TTree.h>

namespace esbroot {
namespace tools {

void fiTQunAnaEventData::ConnectToTree(TTree* fitqun_tree) {
	fitqun_tree->SetBranchAddress("fqnse", &fqnse);

	fitqun_tree->SetBranchAddress("fqnhitpmt", fqnhitpmt);
	fitqun_tree->SetBranchAddress("fqtotq", fqtotq);
	fitqun_tree->SetBranchAddress("fq0rnll", fq0rnll);

	fitqun_tree->SetBranchAddress("fq1rpcflg", fq1rpcflg);
	fitqun_tree->SetBranchAddress("fq1rnll", fq1rnll);
	fitqun_tree->SetBranchAddress("fq1rmom", fq1rmom);
	fitqun_tree->SetBranchAddress("fq1rt0", fq1rt0);
	fitqun_tree->SetBranchAddress("fq1rpos", fq1rpos);
	fitqun_tree->SetBranchAddress("fq1rdir", fq1rdir);
	
	fitqun_tree->SetBranchAddress("fqpi0nll", fqpi0nll);	
	fitqun_tree->SetBranchAddress("fqpi0mass", fqpi0mass);	
}

}
}
