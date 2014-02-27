/*

   Copyright 2013 John Bailey

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Lex/Lexer.h"
#include "clang/Tooling/CompilationDatabase.h"
#include "clang/Tooling/Refactoring.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/ADT/OwningPtr.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/raw_ostream.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Rewrite/Frontend/Rewriters.h"


#include "clang/Frontend/TextDiagnosticPrinter.h"

#include "MetricMatcher.hpp"

#include <string>


using namespace clang;
using namespace clang::tooling;
using namespace llvm;

// Set up the command line options
cl::opt<std::string> BuildPath(
  cl::Positional,
  cl::desc("<build-path>"));

#if 0
cl::opt<std::string> ProjectRoot(
  "r",
  cl::desc("Set project root directory"),
  cl::value_desc("filename"));
#endif

cl::opt<std::string> SourcePaths(
  cl::Positional,
  cl::desc("<source>"),
  cl::Required);

MetricUnit topUnit("Global");


class MetricFrontendAction : public ASTFrontendAction {
public:
    virtual ASTConsumer *CreateASTConsumer(CompilerInstance &CI, StringRef file) {
		// TODO: More elegant way of getting topUnit in.
		return new MetricASTConsumer(&CI.getASTContext(),&topUnit); // pass CI pointer to ASTConsumer
    }
};

int main(int argc, const char **argv) {
  llvm::sys::PrintStackTraceOnErrorSignal();
  llvm::OwningPtr<CompilationDatabase> Compilations(
        FixedCompilationDatabase::loadFromCommandLine(argc, argv));
  cl::ParseCommandLineOptions(argc, argv);
  if (!Compilations) {  // Couldn't find a compilation DB from the command line
    std::string ErrorMessage;
    Compilations.reset(
      !BuildPath.empty() ?
        CompilationDatabase::autoDetectFromDirectory(BuildPath, ErrorMessage) :
        CompilationDatabase::autoDetectFromSource(SourcePaths, ErrorMessage)
      );

    //  Still no compilation DB? - bail.
    if (!Compilations)
    {
      llvm::report_fatal_error(ErrorMessage);
    }
  }
  ClangTool Tool(*Compilations, SourcePaths);

  int Result = Tool.run(newFrontendActionFactory<MetricFrontendAction>());

  topUnit.dump( std::cout );

  return Result;
}
