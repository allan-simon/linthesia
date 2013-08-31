// -*- mode: c++; coding: utf-8 -*-

// Linthesia

// Copyright (c) 2007 Nicholas Piegdon
// Adaptation to GNU/Linux by Oscar Aceña
// See COPYING for license information

#include "TextWriter.h"
#include "Renderer.h"
#include "LinthesiaError.h"
#include "OSGraphics.h"
#include "UserSettings.h"

#include <map>

using namespace std;

TextWriter::TextWriter(int in_x, int in_y, Renderer &in_renderer,
                       bool in_centered, int in_size, string fontname) :
  x(in_x),
  y(in_y),
  size(in_size),
  original_x(0),
  last_line_height(0),
  centered(in_centered),
  renderer(in_renderer) {

  x += renderer.m_xoffset;
  original_x = x;

  y += renderer.m_yoffset;
  point_size = size;

}

int TextWriter::get_point_size() {
  return point_size;
}

TextWriter& TextWriter::next_line() {
  y += max(last_line_height, get_point_size());
  x = original_x;

  last_line_height = 0;
  return *this;
}

TextWriter& Text::operator<<(TextWriter& tw) const {
  int draw_x;
  int draw_y;
  calculate_position_and_advance_cursor(tw, &draw_x, &draw_y);

  string narrow(m_text.begin(), m_text.end());

  glBindTexture(GL_TEXTURE_2D, 0);

  glPushMatrix();
  tw.renderer.SetColor(m_color);
  //glListBase( );
  glRasterPos2i(draw_x, draw_y + tw.size);
  glCallLists(static_cast<int>(narrow.length()), GL_UNSIGNED_BYTE, narrow.c_str());
  glPopMatrix();

  // TODO: Should probably delete these on shutdown.
  //glDeleteLists(1000, 128);

  return tw;
}

void Text::calculate_position_and_advance_cursor(TextWriter &tw, int *out_x, int *out_y) const  {

  Glib::RefPtr<Pango::Layout> layout = Pango::Layout::create(tw.renderer.m_pangocontext);
  layout->set_text(m_text);
  //layout->set_font_description( );

  Pango::Rectangle drawing_rect = layout->get_pixel_logical_extents();
  tw.last_line_height = drawing_rect.get_height();

  if (tw.centered)
    *out_x = tw.x - drawing_rect.get_width() / 2;

  else {
    *out_x = tw.x;
    // FIXME: this size offset is not updated correctly (it differs from GL text width)
    tw.x += drawing_rect.get_width();
  }

  *out_y = tw.y;
}

TextWriter& operator<<(TextWriter& tw, const Text& t) {
  return t.operator<<(tw);
}

TextWriter& newline(TextWriter& tw) {
  return tw.next_line();
}

TextWriter& operator<<(TextWriter& tw, const string& s)        { return tw << Text(s, White); }
TextWriter& operator<<(TextWriter& tw, const int& i)           { return tw << Text(i, White); }
TextWriter& operator<<(TextWriter& tw, const unsigned int& i)  { return tw << Text(i, White); }
TextWriter& operator<<(TextWriter& tw, const long& l)          { return tw << Text(l, White); }
TextWriter& operator<<(TextWriter& tw, const unsigned long& l) { return tw << Text(l, White); }
