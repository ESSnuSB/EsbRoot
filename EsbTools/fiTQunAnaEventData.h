#ifndef ESBROOT_ESBTOOLS_FITQUNANAEVENTDATA_H
#define ESBROOT_ESBTOOLS_FITQUNANAEVENTDATA_H

#include <cstddef>

class TTree;

namespace esbroot {
namespace tools {

struct fiTQunAnaEventData
{
	static constexpr size_t max_fqnse = 100;
	
	int fqnse;

	int fqnhitpmt[max_fqnse];
	float fqtotq[max_fqnse];
	float fq0rnll[max_fqnse];

	int fq1rpcflg[max_fqnse][7];
	float fq1rnll[max_fqnse][7];
	float fq1rmom[max_fqnse][7];
	float fq1rt0[max_fqnse][7];
	float fq1rpos[max_fqnse][7][3];
	float fq1rdir[max_fqnse][7][3];

	float fqpi0nll[2];
	float fqpi0mass[2];
	
	void ConnectToTree(TTree* fitqun_tree);
};


}
}

#endif //ESBROOT_ESBTOOLS_FITQUNANAEVENTDATA_H
