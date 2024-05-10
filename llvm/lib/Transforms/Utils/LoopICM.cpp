#include  "llvm/Transforms/Utils/LoopICM.h"
#include "llvm/IR/Dominators.h"
#include "llvm/ADT/DepthFirstIterator.h"
#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SetOperations.h"
#include "llvm/Support/GenericLoopInfo.h"
#include <unordered_set>

using namespace llvm;

void bulletPoint(const std::string &&name, const BasicBlock *bb, bool printBB = false) {
  errs() << "- " << name << " [first instr; last instr]:";
  bb->front().print(errs());
  errs() << ";";
  bb->back().print(errs());
  if (printBB) bb->print(errs());
  errs() << "\n";
}

void infoLog(Loop &l) {
  errs() << "Loop is " << (l.isLoopSimplifyForm() ? "" : "not ") << "in normal form\n";

  errs() << "Loop blocks: \n";
  if (auto preheader = l.getLoopPreheader()) bulletPoint("preheader", preheader);
  if (auto header = l.getHeader()) bulletPoint("header", header);
  if (auto tail = l.getBlocks().back()) bulletPoint("tail", tail);
  errs() << "Goes through " << std::distance(l.block_begin(), l.block_end()) << " blocks:\n";

  for (auto i = 0u; const auto loopBB : l.blocks()) {
    bulletPoint("BB" + std::to_string(i++), loopBB);
  }
}

void licmOptimize(Loop &l) {
  using namespace std;

  auto loopInvariantInstructions = unordered_set<Instruction*>{};

  // Be aware that this lambda marks phi-instructions as loop-invariant
  auto isOperandLI = [&l, &loopInvariantInstructions] (const Use &usee) {
    if (dyn_cast<Constant>(usee)) return true;
    if (dyn_cast<Argument>(usee)) return true;
    
    decltype(auto) i = dyn_cast<Instruction>(usee);
    
    if (i and loopInvariantInstructions.count(i)) return true;
    if (i and not l.contains(i)) return true;

    return false;
  };
  
  // Extract loop-invariant instructions with iterative algo
  for (auto *bb : l.blocks()) {
    for (auto &i : *bb) {
      bool isInstrLI = true;
      for (const auto &usee : i.operands()) isInstrLI &= isOperandLI(usee);

      if (isInstrLI) loopInvariantInstructions.insert(&i);
    }
  }


  for (const auto *i : loopInvariantInstructions) {
    i->print(errs());
    errs() << "\n";
  }

  // Extract exit blocks from loop
  using Edge = pair<BasicBlock*, BasicBlock*>;
  auto exitEdges = SmallVector<Edge>{};
  l.getExitEdges(exitEdges);
  const auto parentFunction = l.getBlocks()[0]->getParent();
  const auto dominators = DominatorTree(*parentFunction);;
  auto doesDominateExitPoint = [&exitEdges, &dominators] (Instruction *instr) {
    bool dominate = true;
    for (const auto edge : exitEdges) dominate &= dominators.dominates(instr, edge.second);
    return dominate;
  };
}

PreservedAnalyses LoopICM::run(Loop &l, LoopAnalysisManager &lam, LoopStandardAnalysisResults &lar, LPMUpdater &lu) {
  // infoLog(l);

  licmOptimize(l);

  return PreservedAnalyses::all();
}
