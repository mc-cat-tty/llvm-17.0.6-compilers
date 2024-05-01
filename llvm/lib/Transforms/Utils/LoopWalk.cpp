#include  "llvm/Transforms/Utils/LoopWalk.h"

using namespace llvm;

void bulletPoint(const std::string &&name, const Instruction &first, const Instruction &last) {
  errs() << "- " << name << " [first instr; last instr]:";
  first.print(errs());
  errs() << ";";
  last.print(errs());
  errs() << "\n";
}

PreservedAnalyses LoopWalk::run(Loop &l, LoopAnalysisManager &lam, LoopStandardAnalysisResults &lar, LPMUpdater &lu) {
  errs() << "Loop is " << (l.isLoopSimplifyForm() ? "" : "not ") << "in normal form\n";

  errs() << "Loop blocks: \n";
  if (auto preheader = l.getLoopPreheader()) bulletPoint("preheader", preheader->front(), preheader->back());
  if (auto header = l.getHeader()) bulletPoint("header", header->front(), header->back());
  if (auto tail = l.getBlocks().back()) bulletPoint("tail", tail->front(), tail->back());
  errs() << "Goes through " << std::distance(l.block_begin(), l.block_end()) << " blocks\n";

  return PreservedAnalyses::all();
}
