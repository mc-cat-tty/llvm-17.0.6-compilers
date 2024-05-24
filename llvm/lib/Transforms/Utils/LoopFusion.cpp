#include "llvm/Transforms/Utils/LoopFusion.h"

using namespace llvm;

PreservedAnalyses LoopFusion::run(Function &F, FunctionAnalysisManager &AM) {
  return PreservedAnalyses::none();
}