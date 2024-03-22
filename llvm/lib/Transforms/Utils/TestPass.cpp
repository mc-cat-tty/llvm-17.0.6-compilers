#include "llvm/Transforms/Utils/TestPass.h"

using namespace llvm;

PreservedAnalyses TestPass::run(Function &f, FunctionAnalysisManager &am) {
  auto bbCounter = 0u, instrCounter = 0u, callInstrCounter = 0u;

  for (const auto &bb : f) {
    bbCounter++;
    for (const auto &i : bb) {
      instrCounter++;
      if (dyn_cast<CallInst>(&i)) {
        callInstrCounter++;
      }
    }
  }

  errs()
    << "Found function: " << f.getName()
    << "( #args: " << std::distance(f.arg_begin(), f.arg_end()) << (f.isVarArg() ? "+*" : "") << " )"
    << "{ #bb: " << bbCounter << ", #instr: " << instrCounter << ", #calls: " << callInstrCounter << " }"
    << "\n";
  
	return PreservedAnalyses::all();
}

PreservedAnalyses TestPass::run(Module &m, ModuleAnalysisManager &am) {
  for (const auto &gv : m.globals()) {
    errs() << gv.getName() << " is a global var\n";
  }
  errs() << "#functions: " << std::distance(m.begin(), m.end()) << "\n";
	return PreservedAnalyses::all();
}
	
