#include  "llvm/Transforms/Utils/LoopWalk.h"

using namespace llvm;

void bulletPoint(const std::string &&name, const BasicBlock *bb, bool printBB = false) {
  errs() << "- " << name << " [first instr; last instr]:";
  bb->front().print(errs());
  errs() << ";";
  bb->back().print(errs());
  if (printBB) bb->print(errs());
  errs() << "\n";
}

PreservedAnalyses LoopWalk::run(Loop &l, LoopAnalysisManager &lam, LoopStandardAnalysisResults &lar, LPMUpdater &lu) {
  errs() << "Loop is " << (l.isLoopSimplifyForm() ? "" : "not ") << "in normal form\n";

  errs() << "Loop blocks: \n";
  if (auto preheader = l.getLoopPreheader()) bulletPoint("preheader", preheader);
  if (auto header = l.getHeader()) bulletPoint("header", header);
  if (auto tail = l.getBlocks().back()) bulletPoint("tail", tail);
  errs() << "Goes through " << std::distance(l.block_begin(), l.block_end()) << " blocks:\n";

  for (auto i = 0u; const auto loopBB : l.blocks()) {
    bulletPoint("BB" + std::to_string(i++), loopBB);
  }

  return PreservedAnalyses::all();
}
