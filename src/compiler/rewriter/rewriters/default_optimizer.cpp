#include "compiler/rewriter/framework/rule_driver.h"
#include "compiler/rewriter/rules/ruleset.h"
#include "compiler/rewriter/rewriters/default_optimizer.h"

namespace zorba {

class TestRuleDriver : public RuleMajorDriver {
  public:
    TestRuleDriver()
    {
      m_rules.push_back(rule_ptr_t(new EchoNodeRule()));
    }
};

class NodeidRuleDriver : public RuleMajorDriver {
  public:
    NodeidRuleDriver()
    {
      m_rules.push_back(rule_ptr_t(new EliminateDocOrderSortRule()));
    }
};

DefaultOptimizer::DefaultOptimizer()
{
  //m_childRewriters.push_back(rewriter_ptr_t(new TestRuleDriver()));
  //m_childRewriters.push_back(rewriter_ptr_t(new NodeidRuleDriver()));
}

DefaultOptimizer::~DefaultOptimizer() throw ()
{
}

}
