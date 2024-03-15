#ifndef LLVM_TRANSFORMS_TEST_PASS_H
#define LLVM_TRANSFORMS_TEST_PASS_H

#include "llvm/IR/PassManager.h"

namespace llvm {
	class TestPass : public PassInfoMixin<TestPass> {
		public:
			PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
	};
}

#endif
