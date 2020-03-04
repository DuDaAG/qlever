// Copyright 2011, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Author: Björn Buchhold (buchhold@informatik.uni-freiburg.de)

#pragma once

#include <memory>
#include <string>
#include <vector>
#include "../global/Constants.h"
#include "../index/Index.h"
#include "../util/Cache.h"
#include "../util/Log.h"
#include "./Engine.h"
#include "./ResultTable.h"
#include "QueryPlanningCostFactors.h"
#include "RuntimeInformation.h"

using std::shared_ptr;
using std::string;
using std::vector;

struct CacheValue {
  CacheValue() : _resTable(std::make_shared<ResultTable>()), _runtimeInfo() {}
  std::shared_ptr<ResultTable> _resTable;
  RuntimeInformation _runtimeInfo;
  [[nodiscard]] size_t size() const {
    return _resTable ? _resTable-> size() * _resTable->width() : 0;
  }
};

typedef ad_utility::LRUCache<string, CacheValue> SubtreeCache;

// Execution context for queries.
// Holds references to index and engine, implements caching.
class QueryExecutionContext {
 public:
  QueryExecutionContext(const Index& index, const Engine& engine,
                        SubtreeCache* const cache,
                        const bool pinSubtrees = false)
      : pin(pinSubtrees),
        _index(index),
        _engine(engine),
        _subtreeCache(cache),
        _costFactors() {}

  SubtreeCache& getQueryTreeCache() { return *_subtreeCache; }

  const Engine& getEngine() const { return _engine; }

  const Index& getIndex() const { return _index; }

  void clearCache() { getQueryTreeCache().clear(); }

  void readCostFactorsFromTSVFile(const string& fileName) {
    _costFactors.readFromFile(fileName);
  }

  float getCostFactor(const string& key) const {
    return _costFactors.getCostFactor(key);
  };

  const bool pin;

 private:
  const Index& _index;
  const Engine& _engine;
  SubtreeCache* const _subtreeCache;
  QueryPlanningCostFactors _costFactors;
};
