#pragma once
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Transforms/Scalar/LoopPassManager.h"
#include "llvm/Analysis/LoopAnalysisManager.h"

using namespace llvm;

struct LoopWalk : public PassInfoMixin<LoopWalk> {
  PreservedAnalyses run(Loop&, LoopAnalysisManager&, LoopStandardAnalysisResults&, LPMUpdater&);
};