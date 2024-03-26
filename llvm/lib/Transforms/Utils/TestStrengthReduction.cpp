#include "llvm/Transforms/Utils/TestStrengthReduction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Constants.h"


using namespace llvm;

bool optimizeMul(Instruction &i) {
  if (i.getOpcode() != Instruction::Mul) return false;

  Value *op1 = i.getOperand(0);
  Value *op2 = i.getOperand(1);
  ConstantInt *c;

  auto isSuitable = [&c](auto &op) mutable {
    return (c = dyn_cast<ConstantInt>(op)) and c->getValue().isPowerOf2();
  };

  if (isSuitable(op1)) std::swap(op1, op2);
  if (not isSuitable(op2)) return false;

  // If we reach this point, op2 is always a constant integer, power of 2
  // The following code in hence invariant wrt operands order

  errs() << "Found constant power of 2: " << c->getValue() << "\n";

  auto *base = op1;
  auto *exp = ConstantInt::get(c->getType(), c->getValue().logBase2());

  auto *ni = BinaryOperator::CreateShl(base, exp);
  
  ni->insertAfter(&i);
  i.replaceAllUsesWith(ni);
  return true;
}


PreservedAnalyses TestStrengthReduction::run(Function &f, FunctionAnalysisManager &fm) {
  bool modified = false;

  for (BasicBlock &bb : f) {
    for (Instruction &i : bb) {
      modified = optimizeMul(i) | modified;
    }
  }

  return modified ? PreservedAnalyses::all() : PreservedAnalyses::none();
}