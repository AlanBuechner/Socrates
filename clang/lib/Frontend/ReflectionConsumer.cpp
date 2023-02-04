#include "clang/Frontend/ReflectionConsumer.h"
#include "clang/AST/ASTMutationListener.h"
#include "clang/AST/DeclGroup.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "llvm/Support/FileSystem.h"
#include <iostream>

bool clang::ReflectionConsumer::s_StripReflect = false;

namespace clang {

std::string GetAnnotations(Attr const *attr) {
  std::string str;
  llvm::raw_string_ostream os(str);
  LangOptions langopts;
  PrintingPolicy policy(langopts);
  attr->printPretty(os, policy);
  return str.substr(26, str.size() - 4 - 26);
}

std::string GetAnnotations(const AttrVec &attrs) {
  std::string attributes;
  for (uint32_t i = 0; i < attrs.size(); i++) {
    auto &attr = attrs[i];
    if (attr->getKind() == attr::Annotate) {
      attributes += GetAnnotations(attr);
    }
    if (i != attrs.size() - 1)
      attributes += ",";
  }
  return attributes;
}


// ---------------- Reflected Class ---------------- //
ReflectedClass::ReflectedClass(CXXRecordDecl const *rec, const std::string& attrs) : m_record(rec), m_attrs(attrs) {}

void ReflectedClass::Generate(ASTContext *ctx, raw_ostream &os) {

  std::string type = m_record->getQualifiedNameAsString();

  os << "class<" << type << ">" << m_attrs << "\n";

  for (const auto &field : m_record->fields()) {
    std::string attrs = GetAnnotations(field->getAttrs());
    if (StringRef(attrs).starts_with("reflect-property")) {
      std::string t = field->getType()->getAsCXXRecordDecl()->getQualifiedNameAsString();
      os << "\tprop<" << t << ";" << field->getName() << ">" << attrs << "\n";
    }
  }
  
}

// ---------------- Class Finder ---------------- //

void ClassFinder::run(MatchFinder::MatchResult const &result) {
  m_context = result.Context;
  m_sourceman = result.SourceManager;

  CXXRecordDecl const *record = result.Nodes.getNodeAs<clang::CXXRecordDecl>("id");
  if (record) return FoundRecord(record);
}

void ClassFinder::onStartOfTranslationUnit() {}

void ClassFinder::onEndOfTranslationUnit() {
  /* For stdout output. */
  /* raw_fd_ostream os(1, false); */
  if (m_classes.size() != 0) {

    for (auto &f : m_classes) {
      fs::path file = f.first;

      std::cout << "found classes: " << m_classes.size() << " in file: " << file << std::endl;

      std::error_code ec;
      std::filesystem::path generatedPath = fs::path("generated") / (file.filename().string() + ".reflected");
      std::cout << "Generating reflection file: " << generatedPath << std::endl;
      llvm::raw_fd_ostream os(generatedPath.string(), ec, llvm::sys::fs::CD_CreateAlways);
      assert(!ec && "error opening file");
      os << "include<" << file.string() << ">\n";
      for (auto &ref : f.second)
        ref.Generate(m_context, os);

      os.flush();
      os.close();
    }
  }
}


std::unordered_map<fs::path, bool>& ClassFinder::getProcessedDirs() {
  return m_processedDirs;
}

void ClassFinder::FoundRecord(CXXRecordDecl const *record) {

  fs::path file = fs::path(m_sourceman->getFilename(record->getLocation()).str());

  if (record->getAttrs().size() != 0) {
    std::string attrs = GetAnnotations(record->getAttrs());
    if (StringRef(attrs).starts_with("reflect-class")) {
      m_classes[file].emplace_back(ReflectedClass(record, attrs));
      m_processedDirs[file] = true;
    }
  }

  if (m_processedDirs[file] == false) // add the file
    m_processedDirs[file] = false;
}

// ---------------- Reflection Consumer---------------- //
ReflectionConsumer::ReflectionConsumer(fs::path file)
    : FilePath(file) {}

ReflectionConsumer::~ReflectionConsumer() {}

void ReflectionConsumer::HandleTranslationUnit(ASTContext &Ctx) {
  if (s_StripReflect)
    return;

  std::filesystem::create_directory("generated");
  ClassFinder classFinder;
  MatchFinder finder;

  DeclarationMatcher classMatcher = cxxRecordDecl(decl().bind("id"));

  finder.addMatcher(classMatcher, &classFinder);

  finder.matchAST(Ctx);

  for (auto& d : classFinder.getProcessedDirs()) {
    if (d.second == false) {
      fs::path p = fs::path("generated") / (d.first.filename().string() + ".reflected");
      if (fs::exists(p)) {
        fs::remove(p);
      }
    }
  }
}


void ReflectionConsumer::StripReflect() { s_StripReflect = true; }

} // namespace clang