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

#define FLOAT2X(f)      ((int) ( (f) * (65536)))
#define f2vt(f)     FLOAT2X(f)

using namespace std;


inline cairo_t* create_cairo_context (
    int width,
    int height,
    int channels,
    cairo_surface_t** surf,
    unsigned char** buffer
) {
    cairo_t* cr;

    /* create cairo-surface/context to act as OpenGL-texture source */
    *buffer = (unsigned char*)calloc (channels * width * height, sizeof (unsigned char));
    if (!*buffer) {
        printf ("create_cairo_context() - Couldn't allocate surface-buffer\n");
        return NULL;
    }

    *surf = cairo_image_surface_create_for_data (
        *buffer,
        CAIRO_FORMAT_ARGB32,
        width,
        height,
        channels * width
    );
    if (cairo_surface_status (*surf) != CAIRO_STATUS_SUCCESS) {
        free (*buffer);
        printf ("create_cairo_context() - Couldn't create surface\n");
        return NULL;
    }

    cr = cairo_create (*surf);
    if (cairo_status (cr) != CAIRO_STATUS_SUCCESS) {
        free (*buffer);
        printf ("create_cairo_context() - Couldn't create context\n");
        return NULL;
    }

    return cr;
}


inline int draw_text(
    int x,
    int y,
    int width,
    int height,
    const char *string,
    const Color &textColor
) {
    cairo_surface_t* surface = NULL;
    cairo_t* cr;
    unsigned char* surfData;
    GLuint textureId;

    /* create cairo-surface/context to act as OpenGL-texture source */
    cr = create_cairo_context (
        width,
        height,
        4,
        &surface,
        &surfData
    );

    /* clear background */
    cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
    cairo_set_source_rgba(
        cr,
        0,
        0,
        0,
        0
    );
    cairo_paint (cr);

    cairo_move_to(
        cr,
        x,
        y
    );

    // TODO replace this by a parameter
    cairo_set_font_size(cr, 12);
    cairo_select_font_face(
        cr,
        "sans",
        CAIRO_FONT_SLANT_NORMAL,
        CAIRO_FONT_WEIGHT_NORMAL
    );
    cairo_set_source_rgba(
        cr,
        textColor.r,
        textColor.g,
        textColor.b,
        textColor.a
    );
    cairo_show_text(cr, string);

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        surfData
    );
    //TestEGLError("glTexImage2D");

    free (surfData);
    cairo_destroy (cr);

    return textureId;

}

/**
 *
 */
void TextWriter::write(
    int x,
    int y,
    string text,
    const Color &textColor

) {
    GLfloat textureCoord[] = {
        f2vt(0.0f), f2vt(0.0f),
        f2vt(1.0f), f2vt(0.0f),
        f2vt(0.0f), f2vt(1.0f),
        f2vt(1.0f), f2vt(1.0f)
    };


    // WORK IN PROGRESS 
    // for the moment we just try a dirty hacky 
    // display of a 256*24 surface
    GLint  rect[] = {
        0,0,
       256,0,
       0,24,
       256,24
        
    };

    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    GLuint textureId = draw_text(
        x,
        y,
        256,//width,
        24,//height,
        text.c_str(),
        textColor
    );

    glVertexPointer(2, GL_INT, 0, rect);
    glTexCoordPointer(2, GL_FLOAT, 0, textureCoord);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glDeleteTextures(1, &textureId); 

}



/**
 *
 */
TextWriter::TextWriter(
    int in_x,
    int in_y,
    Renderer &in_renderer,
    bool in_centered,
    int in_size,
    string fontname
) :
  x(in_x),
  y(in_y),
  size(in_size),
  original_x(0),
  last_line_height(0),
  centered(in_centered),
  renderer(in_renderer) 
{

  x += renderer.m_xoffset;
  original_x = x;

  y += renderer.m_yoffset;
  point_size = size;

}

/**
 *
 */
int TextWriter::get_point_size() {
  return point_size;
}

/**
 *
 */
TextWriter& TextWriter::next_line() {
  y += max(last_line_height, get_point_size());
  x = original_x;

  last_line_height = 0;
  return *this;
}

/**
 *
 */
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
