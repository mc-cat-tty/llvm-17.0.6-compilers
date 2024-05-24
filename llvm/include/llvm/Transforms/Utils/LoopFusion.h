#pragma once
#include "llvm/IR/PassManager.h"

using namespace llvm;

struct LoopFusion : public PassInfoMixin<LoopFusion> {
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};