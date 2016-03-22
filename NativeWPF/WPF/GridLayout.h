#pragma once

#include "View.h"
#include <map>
#include <vector>

using namespace std;

class GridLayout {
private:
  ViewBase* m_masterView;
  map<ViewBase*, pair<int, int>> m_subViews;

  vector<pair<string, int>> m_rowWidths;
  vector<pair<string, int>> m_colHeights;

  void UpdateWidth() {
    int viewWidth = m_masterView->GetWidth();
    int viewHeight = m_masterView->GetHeight();

    // step1: set the fix width
    int ttlWidth = 0;
    for (auto w : m_rowWidths) {
      if (w.first[w.first.length() - 1] != '*') {
        w.second = atoi(w.first.c_str());
        ttlWidth += w.second;
      }
    }

    // step2: set the ratio width (which is end with a '*')
    //        will only have values when the view's width is bigger than
    //        the fix width column's total width.
    if (viewWidth > ttlWidth) {
      list<pair<int, int>> col_ratios;
      int colidx = 0;
      int leftWidth = viewWidth - ttlWidth;
      for (auto w : m_rowWidths) {
        if (w.first.length() > 1 && w.first[w.first.length() - 1] == '*') {
          string ratiostr = w.first.substr(0, w.first.length() - 1);
          col_ratios.push_back(make_pair(colidx, atoi(ratiostr.c_str())));
        }
        else if (w.first == "*") {
          col_ratios.push_back(make_pair(colidx, 1));
        }
        colidx++;
      }

      if (!col_ratios.empty()) {
        int ratioSum = 0;
        for (auto w : col_ratios) {
          ratioSum += w.second;
        }

        int lastIdx = 0;
        int usedWidthSum = 0;
        for (auto w : col_ratios) {
          int setIdx = w.first;
          lastIdx = setIdx;
          int ratio = w.second;
          m_rowWidths[setIdx].second = ratio * leftWidth / ratioSum;
          usedWidthSum += m_rowWidths[setIdx].second;
        }

        if (usedWidthSum != leftWidth) {
          m_rowWidths[lastIdx].second += (leftWidth - usedWidthSum);
        }
      }
    }
    else {
      // update all the ratio column's width to zero.
      for (auto w : m_rowWidths) {
        if (w.first[w.first.length() - 1] == '*') {
          w.second = 0;
        }
      }
    }
  }

public:

  GridLayout() {
    m_subViews = map<ViewBase*, pair<int, int>>();
    m_rowWidths = vector<pair<string, int>>();
    m_colHeights = vector<pair<string, int>>();
  }

  void AddSubView(ViewBase* v, int rowIdx, int colIdx) {
    m_subViews.insert(make_pair(v, make_pair(rowIdx, colIdx)));
  }

  void AddRow(string width) {
    m_rowWidths.push_back(make_pair(width, 0));
  }

  void AddCol(string height) {
    m_colHeights.push_back(make_pair(height, 0));
  }

  pair<int, int> GetCoord(ViewBase * v) {
    return make_pair(0, 0);
  }
};
