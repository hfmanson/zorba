#ifndef XQP_RULESET_H
#define XQP_RULESET_H

#include "compiler/expression/expr.h"
#include "compiler/rewriter/framework/rewriter_context.h"
#include "compiler/rewriter/framework/rewrite_rule.h"

namespace zorba {

#define RULE(name) \
  class name : public RewriteRule { \
    public: \
      name() { } \
      ~name() { } \
      expr_t rewritePre(expr *node, RewriterContext& rCtx); \
      expr_t rewritePost(expr *node, RewriterContext& rCtx); \
  }

RULE(EchoNodeRule);

// Node id rules
RULE(MarkNodesWithNodeIdPropertiesRule);
RULE(EliminateDocOrderSortRule);

#undef RULE

#define RULE_REWRITE_PRE(name) expr_t name::rewritePre(expr *node, RewriterContext& rCtx)
#define RULE_REWRITE_POST(name) expr_t name::rewritePost(expr *node, RewriterContext& rCtx)

}

#endif /* XQP_REWRITE_RULE_H */
/* vim:set ts=2 sw=2: */
