#ifndef LLVM_CLANG_FRONTEND_REFLECTIONXCONSUMER_H
#define LLVM_CLANG_FRONTEND_REFLECTIONXCONSUMER_H

#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/LLVM.h"
#include "clang/Sema/SemaConsumer.h"
#include "clang/Serialization/ASTDeserializationListener.h"
#include "clang/Tooling/Tooling.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"

/* Declares llvm::cl::extrahelp. */
#include "llvm/Support/CommandLine.h"

#include <filesystem>
#include <memory>
#include <vector>
#include <algorithm>
#include <string>
#include <unordered_map>

using namespace clang;
using namespace clang::ast_matchers;
namespace fs = std::filesystem;


namespace clang {

std::string GetAnnotations(Attr const *attr);
std::string GetAnnotations(const AttrVec &attrs);

// --------------------------- Reflected Class --------------------------- //
class ReflectedClass {
public:
  ReflectedClass(CXXRecordDecl const *rec, const std::string& attrs);

  void Generate(ASTContext *ctx, raw_ostream &os);


private:
  CXXRecordDecl const *const m_record;
  std::string m_attrs;
};

class ClassFinder : public MatchFinder::MatchCallback {
public:

  virtual void run(MatchFinder::MatchResult const &result) override;

  virtual void onStartOfTranslationUnit() override;

  virtual void onEndOfTranslationUnit() override;

  std::unordered_map<fs::path, bool>& getProcessedDirs();

protected:
  void FoundRecord(CXXRecordDecl const *record);

protected:
  ASTContext *m_context;
  SourceManager *m_sourceman;
  std::unordered_map<fs::path, std::vector<ReflectedClass>> m_classes;
  std::unordered_map<fs::path, bool> m_processedDirs;
};

// Has a list of ASTConsumers and calls each of them. Owns its children.
class ReflectionConsumer : public ASTConsumer {
public:
  // Takes ownership of the pointers in C.
  ReflectionConsumer(fs::path file);
  ~ReflectionConsumer() override;

  // ASTConsumer
  virtual void HandleTranslationUnit(ASTContext &Ctx) override;

  static void StripReflect();

private:
  fs::path FilePath;
  static bool s_StripReflect;
};
} // namespace clang

#endif