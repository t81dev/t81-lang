#include "t81/frontend/ast_printer.hpp"

namespace t81::frontend {

std::string CanonicalAstPrinter::print(const Stmt& stmt) {
    return std::any_cast<std::string>(stmt.accept(*this));
}

std::string CanonicalAstPrinter::print(const Expr& expr) {
    return std::any_cast<std::string>(expr.accept(*this));
}

std::any CanonicalAstPrinter::visit(const ExpressionStmt& stmt) {
    return parenthesize(";", {&stmt.expression});
}

std::any CanonicalAstPrinter::visit(const VarStmt& stmt) {
    std::string name = "var " + std::string(stmt.name.lexeme);
    if (stmt.type) {
        name += ": " + print(*stmt.type);
    }
    if (stmt.initializer) {
        return parenthesize(name, {&stmt.initializer});
    }
    return std::string("(" + name + ")");
}

std::any CanonicalAstPrinter::visit(const LetStmt& stmt) {
    std::string name = "let " + std::string(stmt.name.lexeme);
    if (stmt.type) {
        name += ": " + print(*stmt.type);
    }
    name += " =";
    return parenthesize(name, {&stmt.initializer});
}

std::any CanonicalAstPrinter::visit(const BlockStmt& stmt) {
    std::stringstream ss;
    ss << "(block";
    for (const auto& statement : stmt.statements) {
        ss << " " << print(*statement);
    }
    ss << ")";
    return ss.str();
}

std::any CanonicalAstPrinter::visit(const IfStmt& stmt) {
    if (stmt.else_branch) {
        return parenthesize("if-else", {&stmt.condition, &stmt.then_branch, &stmt.else_branch});
    }
    return parenthesize("if", {&stmt.condition, &stmt.then_branch});
}

std::any CanonicalAstPrinter::visit(const WhileStmt& stmt) {
    return parenthesize("while", {&stmt.condition, &stmt.body});
}

std::any CanonicalAstPrinter::visit(const LoopStmt& stmt) {
    std::stringstream ss;
    ss << "(loop";
    if (stmt.bound_kind == LoopStmt::BoundKind::Infinite) {
        ss << " @bounded(infinite)";
    } else if (stmt.bound_kind == LoopStmt::BoundKind::Static) {
        ss << " @bounded(" << stmt.bound_value.value_or(0) << ")";
    }
    ss << " (block";
    for (const auto& statement : stmt.body) {
        ss << " " << print(*statement);
    }
    ss << "))";
    return ss.str();
}

std::any CanonicalAstPrinter::visit(const ReturnStmt& stmt) {
    if (stmt.value) {
        return parenthesize("return", {&stmt.value});
    }
    return std::string("(return)");
}

std::any CanonicalAstPrinter::visit(const BreakStmt&) {
    return std::string("(break)");
}

std::any CanonicalAstPrinter::visit(const ContinueStmt&) {
    return std::string("(continue)");
}

std::any CanonicalAstPrinter::visit(const FunctionStmt& stmt) {
    std::stringstream ss;
    ss << "(fn";
    if (stmt.attributes.is_effectful) {
        ss << " @effect";
    }
    if (stmt.attributes.tier.has_value()) {
        ss << " @tier(" << *stmt.attributes.tier << ")";
    }
    ss << " " << stmt.name.lexeme;
    ss << " (";
    for (size_t i = 0; i < stmt.params.size(); ++i) {
        ss << stmt.params[i].name.lexeme << ": " << print(*stmt.params[i].type);
        if (i < stmt.params.size() - 1) {
            ss << " ";
        }
    }
    ss << " )";
    if (stmt.return_type) {
        ss << " -> " << print(*stmt.return_type);
    }
    ss << " ";
    ss << "(block";
    for (const auto& statement : stmt.body) {
        ss << " " << print(*statement);
    }
    ss << ")";
    ss << ")";
    return ss.str();
}

std::any CanonicalAstPrinter::visit(const ModuleDecl& stmt) {
    return std::string("(module " + stmt.path + ")");
}

std::any CanonicalAstPrinter::visit(const ImportDecl& stmt) {
    return std::string("(import " + stmt.path + ")");
}

std::any CanonicalAstPrinter::visit(const TypeDecl& stmt) {
    std::stringstream ss;
    ss << "(type " << stmt.name.lexeme << " [";
    for (size_t i = 0; i < stmt.params.size(); ++i) {
        ss << stmt.params[i].lexeme;
        if (i + 1 < stmt.params.size()) {
            ss << ", ";
        }
    }
    ss << "] = " << print(*stmt.alias);
    ss << ")";
    return ss.str();
}

std::any CanonicalAstPrinter::visit(const RecordDecl& stmt) {
    std::stringstream ss;
    ss << "(record " << stmt.name.lexeme;
    for (const auto& field : stmt.fields) {
        ss << " " << field.name.lexeme << ": ";
        if (field.type) {
            ss << print(*field.type);
        } else {
            ss << "<unknown>";
        }
    }
    ss << ")";
    return ss.str();
}

std::any CanonicalAstPrinter::visit(const EnumDecl& stmt) {
    std::stringstream ss;
    ss << "(enum " << stmt.name.lexeme;
    for (const auto& variant : stmt.variants) {
        ss << " " << variant.name.lexeme;
        if (variant.payload) {
            ss << "(" << print(*variant.payload) << ")";
        }
    }
    ss << ")";
    return ss.str();
}

std::any CanonicalAstPrinter::visit(const BinaryExpr& expr) {
    return parenthesize(expr.op.lexeme, {std::any(&expr.left), std::any(&expr.right)});
}

std::any CanonicalAstPrinter::visit(const UnaryExpr& expr) {
    return parenthesize(expr.op.lexeme, {&expr.right});
}

std::any CanonicalAstPrinter::visit(const LiteralExpr& expr) {
    return std::string(expr.value.lexeme);
}

std::any CanonicalAstPrinter::visit(const GroupingExpr& expr) {
    return parenthesize("group", {&expr.expression});
}

std::any CanonicalAstPrinter::visit(const VariableExpr& expr) {
    return std::string(expr.name.lexeme);
}

std::any CanonicalAstPrinter::visit(const CallExpr& expr) {
    std::vector<std::any> parts;
    parts.push_back(&expr.callee);
    for (const auto& arg : expr.arguments) {
        parts.push_back(&arg);
    }
    return parenthesize("call", parts);
}

std::any CanonicalAstPrinter::visit(const AssignExpr& expr) {
    return parenthesize("= " + std::string(expr.name.lexeme), {&expr.value});
}

std::any CanonicalAstPrinter::visit(const MatchExpr& expr) {
    std::stringstream ss;
    ss << "(match " << print(*expr.scrutinee);
    for (const auto& arm : expr.arms) {
        ss << " (" << arm.keyword.lexeme;
        switch (arm.pattern.kind) {
            case MatchPattern::Kind::Identifier:
                if (!arm.pattern.binding_is_wildcard) {
                    ss << " " << arm.pattern.identifier.lexeme;
                }
                break;
            case MatchPattern::Kind::Tuple:
                for (const auto& binding : arm.pattern.tuple_bindings) {
                    ss << " " << binding.lexeme;
                }
                break;
            case MatchPattern::Kind::Record:
                ss << " {";
                for (const auto& binding : arm.pattern.record_bindings) {
                    ss << binding.first.lexeme;
                    if (!(binding.second.lexeme == binding.first.lexeme)) {
                        ss << ":" << binding.second.lexeme;
                    }
                    ss << ",";
                }
                ss << " }";
                break;
            default:
                break;
        }
        if (arm.guard) {
            ss << " if " << print(*arm.guard);
        }
        ss << " => " << print(*arm.expression);
        ss << ")";
    }
    ss << ")";
    return ss.str();
}

std::any CanonicalAstPrinter::visit(const VectorLiteralExpr& expr) {
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < expr.elements.size(); ++i) {
        ss << print(*expr.elements[i]);
        if (i + 1 < expr.elements.size()) {
            ss << ", ";
        }
    }
    ss << "]";
    return ss.str();
}

std::any CanonicalAstPrinter::visit(const FieldAccessExpr& expr) {
    return parenthesize("field " + std::string(expr.field.lexeme),
                        std::vector<const Expr*>{expr.object.get()});
}

std::any CanonicalAstPrinter::visit(const RecordLiteralExpr& expr) {
    std::stringstream ss;
    ss << "(recordlit " << expr.type_name.lexeme;
    for (const auto& field : expr.fields) {
        ss << " " << field.first.lexeme;
        if (field.second) {
            ss << ": " << print(*field.second);
        }
    }
    ss << ")";
    return ss.str();
}

std::any CanonicalAstPrinter::visit(const EnumLiteralExpr& expr) {
    std::stringstream ss;
    ss << "(enumlit " << expr.enum_name.lexeme << "." << expr.variant.lexeme;
    if (expr.payload) {
        ss << " " << print(*expr.payload);
    }
    ss << ")";
    return ss.str();
}

std::any CanonicalAstPrinter::visit(const SimpleTypeExpr& expr) {
    return std::string(expr.name.lexeme);
}

std::any CanonicalAstPrinter::visit(const GenericTypeExpr& expr) {
    std::vector<const Expr*> params;
    for (size_t i = 0; i < expr.param_count; ++i) {
        params.push_back(expr.params[i].get());
    }
    return parenthesize("generic " + std::string(expr.name.lexeme), params);
}

std::string CanonicalAstPrinter::parenthesize(std::string_view name, const std::vector<const Expr*>& exprs) {
    std::stringstream ss;
    ss << "(" << name;
    for (const auto& expr : exprs) {
        ss << " " << print(*expr);
    }
    ss << ")";
    return ss.str();
}

std::string CanonicalAstPrinter::parenthesize(std::string_view name, const std::vector<std::any>& parts) {
    std::stringstream ss;
    ss << "(" << name;
    for (const auto& part : parts) {
        ss << " ";
        if (part.type() == typeid(const std::unique_ptr<Expr>*)) {
            ss << print(**std::any_cast<const std::unique_ptr<Expr>*>(part));
        } else if (part.type() == typeid(const std::unique_ptr<Stmt>*)) {
            ss << print(**std::any_cast<const std::unique_ptr<Stmt>*>(part));
        }
    }
    ss << ")";
    return ss.str();
}

} // namespace t81::frontend
