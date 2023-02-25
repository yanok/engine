// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/flow/paint_utils.h"

#include <stdlib.h>

#include "third_party/skia/include/core/SkBitmap.h"
#include "third_party/skia/include/core/SkPaint.h"
#include "third_party/skia/include/core/SkShader.h"

namespace flutter {

namespace {

sk_sp<SkShader> CreateCheckerboardShader(SkColor c1, SkColor c2, int size) {
  SkBitmap bm;
  bm.allocN32Pixels(2 * size, 2 * size);
  bm.eraseColor(c1);
  bm.eraseArea(SkIRect::MakeLTRB(0, 0, size, size), c2);
  bm.eraseArea(SkIRect::MakeLTRB(size, size, 2 * size, 2 * size), c2);
  return bm.makeShader(SkTileMode::kRepeat, SkTileMode::kRepeat,
                       SkSamplingOptions());
}

}  // anonymous namespace

void DrawCheckerboard(DlCanvas* canvas, const SkRect& rect) {
  // Draw a checkerboard
  canvas->Save();
  canvas->ClipRect(rect, DlCanvas::ClipOp::kIntersect, false);

  // Secure random number generation isn't needed here.
  // NOLINTBEGIN(clang-analyzer-security.insecureAPI.rand)
  auto checkerboard_color =
      SkColorSetARGB(64, rand() % 256, rand() % 256, rand() % 256);
  // NOLINTEND(clang-analyzer-security.insecureAPI.rand)

  DlPaint paint;
  paint.setColorSource(DlColorSource::From(
      CreateCheckerboardShader(checkerboard_color, 0x00000000, 12)));
  canvas->DrawPaint(paint);
  canvas->Restore();

  // Stroke the drawn area
  DlPaint debug_paint;
  debug_paint.setStrokeWidth(8);
  debug_paint.setColor(SkColorSetA(checkerboard_color, 255));
  debug_paint.setDrawStyle(DlDrawStyle::kStroke);
  canvas->DrawRect(rect, debug_paint);
}

}  // namespace flutter