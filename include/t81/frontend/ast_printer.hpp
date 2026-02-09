#ifndef T81_FRONTEND_AST_PRINTER_HPP
#define T81_FRONTEND_AST_PRINTER_HPP

#include "t81/frontend/ast.hpp"

#include <any>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace t81::frontend {

class CanonicalAstPrinter : public ExprVisitor, public StmtVisitor {
public:
    std::string print(const Stmt& stmt);
    std::string print(const Expr& expr);

    std::any visit(const ExpressionStmt& stmt) override;
    std::any visit(const VarStmt& stmt) override;
    std::any visit(const LetStmt& stmt) override;
    std::any visit(const BlockStmt& stmt) override;
    std::any visit(const IfStmt& stmt) override;
    std::any visit(const WhileStmt& stmt) override;
    std::any visit(const LoopStmt& stmt) override;
    std::any visit(const ReturnStmt& stmt) override;
    std::any visit(const BreakStmt& stmt) override;
    std::any visit(const ContinueStmt& stmt) override;
    std::any visit(const FunctionStmt& stmt) override;
    std::any visit(const ModuleDecl& stmt) override;
    std::any visit(const ImportDecl& stmt) override;
    std::any visit(const TypeDecl& stmt) override;
    std::any visit(const RecordDecl& stmt) override;
    std::any visit(const EnumDecl& stmt) override;

    std::any visit(const BinaryExpr& expr) override;
    std::any visit(const UnaryExpr& expr) override;
    std::any visit(const LiteralExpr& expr) override;
    std::any visit(const GroupingExpr& expr) override;
    std::any visit(const VariableExpr& expr) override;
    std::any visit(const CallExpr& expr) override;
    std::any visit(const AssignExpr& expr) override;
    std::any visit(const MatchExpr& expr) override;
    std::any visit(const VectorLiteralExpr& expr) override;
    std::any visit(const FieldAccessExpr& expr) override;
    std::any visit(const RecordLiteralExpr& expr) override;
    std::any visit(const EnumLiteralExpr& expr) override;
    std::any visit(const SimpleTypeExpr& expr) override;
    std::any visit(const GenericTypeExpr& expr) override;

private:
    std::string parenthesize(std::string_view name, const std::vector<const Expr*>& exprs);
    std::string parenthesize(std::string_view name, const std::vector<std::any>& parts);
};

} // namespace t81::frontend

#endif // T81_FRONTEND_AST_PRINTER_HPP
