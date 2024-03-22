#ifndef LLVM_TRANSFORMS_TEST_PASS_H
#define LLVM_TRANSFORMS_TEST_PASS_H

#include "llvm/IR/PassManager.h"
#include "llvm/IR/Instructions.h"

namespace llvm {
	struct TestPass : public PassInfoMixin<TestPass> {
    PreservedAnalyses run(Function&, FunctionAnalysisManager&);
    PreservedAnalyses run(Module&, ModuleAnalysisManager&);
	};
}

#endif
