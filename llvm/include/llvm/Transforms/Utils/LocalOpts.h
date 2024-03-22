#pragma once

#include <llvm/IR/Constants.h>
#include <llvm/IR/PassManager.h>

namespace llvm {
  struct LocalOpts : public PassInfoMixin<LocalOpts> {
    PreservedAnalyses run(Module&, ModuleAnalysisManager&);
  };
}