#pragma once

#include "llvm/IR/PassManager.h"

namespace llvm {
  struct TestStrengthReduction : public PassInfoMixin<TestStrengthReduction> {
    PreservedAnalyses run(Function&, FunctionAnalysisManager&);
  };
}