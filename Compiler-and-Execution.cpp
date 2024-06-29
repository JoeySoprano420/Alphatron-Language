#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Verifier.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

LLVMContext Context;
Module *ModuleOb = new Module("alphatron", Context);
IRBuilder<> Builder(Context);
std::unique_ptr<ExecutionEngine> EE;

void InitializeLLVM() {
    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();
    InitializeNativeTargetAsmParser();
    std::string ErrStr;
    EE.reset(EngineBuilder(std::unique_ptr<Module>(ModuleOb)).setErrorStr(&ErrStr).create());
    if (!EE) {
        errs() << "Failed to construct ExecutionEngine: " << ErrStr << "\n";
        exit(1);
    }
}

Function* CreateFunction(const std::string &Name) {
    FunctionType *FT = FunctionType::get(Type::getVoidTy(Context), false);
    Function *F = Function::Create(FT, Function::ExternalLinkage, Name, ModuleOb);
    BasicBlock *BB = BasicBlock::Create(Context, "entry", F);
    Builder.SetInsertPoint(BB);
    return F;
}

void CompileAndRun() {
    ModuleOb->print(llvm::outs(), nullptr);
    EE->finalizeObject();
    void (*FP)() = (void (*)())EE->getPointerToFunction(ModuleOb->getFunction("main"));
    FP();
}

int
int main() {
    InitializeLLVM();

    // Example code to demonstrate compilation and execution
    Function *MainFunction = CreateFunction("main");

    // Insert code generation here for Alphatron programs

    Builder.CreateRetVoid();

    CompileAndRun();

    return 0;
}
