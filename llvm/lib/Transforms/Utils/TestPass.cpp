#include "llvm/Transforms/Utils/TestPass.h"
#include <array>
#include <set>

using namespace llvm;

void walkDUChain(const User *i, unsigned spaces = 0, bool offsetted = false) {
  for (unsigned j = 0; j < spaces and not offsetted; j++) errs() << " ";
  const auto prevIndicatorSize = errs().tell();
  errs() << " ->"; i->print(errs());
  const auto indicatorIncrement = errs().tell() - prevIndicatorSize;

  if (i->user_empty()) return;

  offsetted = true;
  for (const User *user : i->users()) {
    walkDUChain(user, spaces + indicatorIncrement, offsetted);
    errs() << "\n";
    offsetted = false;
  }
}

void run(const Instruction &i) {
  errs() << "\n";
  if (not i.getType()->isVoidTy()) {
    i.printAsOperand(errs(), true);
    errs() << "\n";
  }
  else {
    i.print(errs());
    errs() << "\n";
  }

  if (i.hasNUsesOrMore(1)) {
    errs() << "Users: \n";
    for (const User *user : i.users()) {
      user->print(errs());
      errs() << "\n";
    }
  }
  else {
    errs() << "No Users\n";
  }

  std::array<const Use*, 2> operands;
  unsigned idx = 0;

  for (const Use &usee : i.operands()) {
    if (dyn_cast<Instruction>(usee)) operands[idx++] = &usee;
  }

  if (idx != 0) {
    errs() << "Usees: \n";
    for (unsigned j=0; j<idx; j++) {
      operands[j]->get()->print(errs());
      errs() << "\n";
    }
    errs() << "Use chain:\n"; walkDUChain(&i); errs() << "\n";
  }
  else {
    errs() << "No instruction operands\n";
  }
}

void cfgWalk(BasicBlock &bb, std::set<BasicBlock*> &visited) {
  if (visited.count(&bb)) return;

  decltype(auto) branchInstr = bb.getTerminator();
  if (not branchInstr) return;

  visited.emplace(&bb);

  errs() << "[";
  bb.front().print(errs());
  errs() << "; ";
  bb.back().print(errs());
  errs() << "] -> ";

  for (int i=0, N = branchInstr->getNumSuccessors(); i<N; ++i) {
    decltype(auto) successor = branchInstr->getSuccessor(i);
    errs() << "[";
    successor->front().print(errs());
    errs() << "; ";
    successor->back().print(errs());
    errs() << "] ";
  }

  errs() << "\n";

  for (int i=0, N = branchInstr->getNumSuccessors(); i<N; ++i) {
    cfgWalk(*branchInstr->getSuccessor(i), visited);
  }
}

PreservedAnalyses TestPass::run(Function &f, FunctionAnalysisManager &am) {
  auto bbCounter = 0u, instrCounter = 0u, callInstrCounter = 0u;

  errs() << "Function " << f.getName() << "\n";

  errs() << "PRINTING UD-DU CHAINS\n";
  for (const auto &bb : f) {
    bbCounter++;
    for (const auto &i : bb) {
      ::run(i);
      instrCounter++;
      if (dyn_cast<CallInst>(&i)) {
        callInstrCounter++;
      }
    }
  }

  errs() << "PRINTING CFG WALK\n";
  auto visited = std::set<BasicBlock*>{};
  ::cfgWalk(f.getEntryBlock(), visited);

  errs()
  << "Stats for function " << f.getName() << ": "
  << "( #args: " << std::distance(f.arg_begin(), f.arg_end()) << (f.isVarArg() ? "+*" : "") << " )"
  << "{ #bb: " << bbCounter << ", #instr: " << instrCounter << ", #calls: " << callInstrCounter << " }"
  << "\n";
  
	return PreservedAnalyses::all();
}

PreservedAnalyses TestPass::run(Module &m, ModuleAnalysisManager &am) {
  for (const auto &gv : m.globals()) {
    errs() << gv.getName() << " is a global var\n";
  }
  errs() << "#functions: " << std::distance(m.begin(), m.end()) << "\n";
	return PreservedAnalyses::all();
}
	
