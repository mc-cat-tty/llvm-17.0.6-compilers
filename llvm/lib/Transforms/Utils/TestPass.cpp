#include "llvm/Transforms/Utils/TestPass.h"

using namespace llvm;

PreservedAnalyses TestPass::run(Function &F, FunctionAnalysisManager &AM) {
	errs() << "Funzione chiamata: " << F.getName() << "\n";
	return PreservedAnalyses::all();
}
	
