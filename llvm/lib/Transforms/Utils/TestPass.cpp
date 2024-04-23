#include "llvm/Transforms/Utils/TestPass.h"
#include <array>

using namespace llvm;

void definitionUsageChain(const User *i, unsigned spaces = 0, bool offsetted = false) {
  for (unsigned j = 0; j < spaces and not offsetted; j++) outs() << " ";
  const auto prevIndicatorSize = outs().tell();
  outs() << " ->"; i->print(outs());
  const auto indicatorIncrement = outs().tell() - prevIndicatorSize;

  if (i->user_empty()) return;

  offsetted = true;
  for (const User *user : i->users()) {
    definitionUsageChain(user, spaces + indicatorIncrement, offsetted);
    outs() << "\n";
    offsetted = false;
  }
}

void run(const Instruction &i) {
  outs() << "\n";
  if (not i.getType()->isVoidTy()) {
    i.printAsOperand(outs(), true);
    outs() << "\n";
  }
  else {
    i.print(outs());
    outs() << "\n";
  }

  if (i.hasNUsesOrMore(1)) {
    outs() << "Users: \n";
    for (const User *user : i.users()) {
      user->print(outs());
      outs() << "\n";
    }
  }
  else {
    outs() << "No Users\n";
  }

  std::array<const Use*, 2> operands;
  unsigned idx = 0;

  for (const Use &usee : i.operands()) {
    if (dyn_cast<Instruction>(usee)) operands[idx++] = &usee;
  }

  if (idx != 0) {
    outs() << "Usees: \n";
    for (unsigned j=0; j<idx; j++) {
      operands[j]->get()->print(outs());
      outs() << "\n";
    }
    outs() << "Use chain:\n"; definitionUsageChain(&i); outs() << "\n";
  }
  else {
    outs() << "No instruction operands\n";
  }
}

PreservedAnalyses TestPass::run(Function &f, FunctionAnalysisManager &am) {
  auto bbCounter = 0u, instrCounter = 0u, callInstrCounter = 0u;

  errs()
    << "Found function: " << f.getName()
    << "( #args: " << std::distance(f.arg_begin(), f.arg_end()) << (f.isVarArg() ? "+*" : "") << " )"
    << "{ #bb: " << bbCounter << ", #instr: " << instrCounter << ", #calls: " << callInstrCounter << " }"
    << "\n";

  outs() << "PRINTING UD-DU CHAINS\n";
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
  
	return PreservedAnalyses::all();
}

PreservedAnalyses TestPass::run(Module &m, ModuleAnalysisManager &am) {
  for (const auto &gv : m.globals()) {
    errs() << gv.getName() << " is a global var\n";
  }
  errs() << "#functions: " << std::distance(m.begin(), m.end()) << "\n";
	return PreservedAnalyses::all();
}
	
