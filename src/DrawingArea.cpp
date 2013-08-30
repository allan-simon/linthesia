#include <map>
#include "DrawingArea.h"
#include "EdgeTracker.h"
#include "GameState.h"
#include "TitleState.h"
#include "MidiComm.h"

typedef std::map<int,sigc::connection> ConnectMap;
ConnectMap pressed;



// FIXME: use user settings to do this mapping
int keyToNote(GdkEventKey* event) {
  const unsigned short oct = 4;

  switch(event->keyval) {
  /* no key for C :( */
  case GDK_masculine:  return 12*oct + 1;      /* C# */
  case GDK_Tab:        return 12*oct + 2;      /* D  */
  case GDK_1:          return 12*oct + 3;      /* D# */
  case GDK_q:          return 12*oct + 4;      /* E  */
  case GDK_w:          return 12*oct + 5;      /* F  */
  case GDK_3:          return 12*oct + 6;      /* F# */
  case GDK_e:          return 12*oct + 7;      /* G  */
  case GDK_4:          return 12*oct + 8;      /* G# */
  case GDK_r:          return 12*oct + 9;      /* A  */
  case GDK_5:          return 12*oct + 10;     /* A# */
  case GDK_t:          return 12*oct + 11;     /* B  */

  case GDK_y:          return 12*(oct+1) + 0;  /* C  */
  case GDK_7:          return 12*(oct+1) + 1;  /* C# */
  case GDK_u:          return 12*(oct+1) + 2;  /* D  */
  case GDK_8:          return 12*(oct+1) + 3;  /* D# */
  case GDK_i:          return 12*(oct+1) + 4;  /* E  */
  case GDK_o:          return 12*(oct+1) + 5;  /* F  */
  case GDK_0:          return 12*(oct+1) + 6;  /* F# */
  case GDK_p:          return 12*(oct+1) + 7;  /* G  */
  case GDK_apostrophe: return 12*(oct+1) + 8;  /* G# */
  case GDK_dead_grave: return 12*(oct+1) + 9;  /* A  */
  case GDK_exclamdown: return 12*(oct+1) + 10; /* A# */
  case GDK_plus:       return 12*(oct+1) + 11; /* B  */
  }

  return -1;
}



bool __sendNoteOff(int note) {

  ConnectMap::iterator it = pressed.find(note);
  if (it == pressed.end())
   return false;

  sendNote(note, false);
  pressed.erase(it);

  return true;
}



/**
 *
 */
DrawingArea::DrawingArea(
    const Glib::RefPtr<const Gdk::GL::Config>& config
) :Gtk::GL::DrawingArea(config) {

    windowState = new EdgeTracker();
    state_manager = new GameStateManager(
        Compatible::GetDisplayWidth(),
        Compatible::GetDisplayHeight()
    );



    set_events(
        Gdk::POINTER_MOTION_MASK |
        Gdk::BUTTON_PRESS_MASK   |
        Gdk::BUTTON_RELEASE_MASK |
        Gdk::KEY_PRESS_MASK      |
        Gdk::KEY_RELEASE_MASK
    );

    set_can_focus();

    signal_motion_notify_event().connect(
        sigc::mem_fun(
            *this,
            &DrawingArea::on_motion_notify
        )
    );
    signal_button_press_event().connect(
        sigc::mem_fun(
            *this,
            &DrawingArea::on_button_press
        )
    );
    signal_button_release_event().connect(
        sigc::mem_fun(
            *this,
            &DrawingArea::on_button_press
        )
    );
    signal_key_press_event().connect(
        sigc::mem_fun(
            *this,
            &DrawingArea::on_key_press
        )
    );
    signal_key_release_event().connect(
        sigc::mem_fun(
            *this,
            &DrawingArea::on_key_release
        )
    );


}

/**
 *
 */
DrawingArea::~DrawingArea() {
    windowState->deactivate();
    delete windowState;

}

bool DrawingArea::GameLoop() {

    if (windowState->is_not_active()) {
        return true;
    }

    state_manager->Update(windowState->just_activated());
    Renderer rend(
        get_gl_context(),
        get_pango_context()
    );
    state_manager->Draw(rend);

    return true;
}


bool DrawingArea::on_key_release(GdkEventKey* event) {

  // if is a note...
  int note = keyToNote(event);
  if (note >= 0) {

    // setup a timeout with Note-Off
    pressed[note] = Glib::signal_timeout().connect(
        sigc::bind<int>(
            sigc::ptr_fun(&__sendNoteOff),
            note
        ),
        20
    );
    return true;
  }

  return false;
}

bool DrawingArea::on_expose_event(GdkEventExpose* event) {

  Glib::RefPtr<Gdk::GL::Window> glwindow = get_gl_window();
  if (!glwindow->gl_begin(get_gl_context()))
    return false;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glCallList(1);

  Renderer rend(get_gl_context(), get_pango_context());
  state_manager->Draw(rend);

  // swap buffers.
  if (glwindow->is_double_buffered())
     glwindow->swap_buffers();
  else
     glFlush();

  glwindow->gl_end();
  return true;
}



bool DrawingArea::on_configure_event(GdkEventConfigure* event) {

  Glib::RefPtr<Gdk::GL::Window> glwindow = get_gl_window();
  if (!glwindow->gl_begin(get_gl_context()))
    return false;

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_TEXTURE_2D);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  glShadeModel(GL_SMOOTH);

  glViewport(0, 0, get_width(), get_height());
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, get_width(), 0, get_height());

  state_manager->Update(
    windowState->just_activated()
  );

  glwindow->gl_end();
  return true;
}


bool DrawingArea::on_key_press(GdkEventKey* event) {

  // if is a note...
  int note = keyToNote(event);
  if (note >= 0) {

    // if first press, send Note-On
    ConnectMap::iterator it = pressed.find(note);
    if (it == pressed.end())
      sendNote(note, true);

    // otherwise, cancel emission of Note-off
    else
      it->second.disconnect();

    return true;
  }

  switch (event->keyval) {
  case GDK_Up:       state_manager->KeyPress(KeyUp);      break;
  case GDK_Down:     state_manager->KeyPress(KeyDown);    break;
  case GDK_Left:     state_manager->KeyPress(KeyLeft);    break;
  case GDK_Right:    state_manager->KeyPress(KeyRight);   break;
  case GDK_space:    state_manager->KeyPress(KeySpace);   break;
  case GDK_Return:   state_manager->KeyPress(KeyEnter);   break;
  case GDK_Escape:   state_manager->KeyPress(KeyEscape);  break;

  // show FPS
  case GDK_F6:       state_manager->KeyPress(KeyF6);      break;

  // show key names
  case GDK_F7:       state_manager->KeyPress(KeyF7);      break;

  // increase/decrease octave
  case GDK_greater:  state_manager->KeyPress(KeyGreater); break;
  case GDK_less:     state_manager->KeyPress(KeyLess);    break;

  default:
    return false;
  }

  return true;
}


bool DrawingArea::on_button_press(GdkEventButton* event) {

  MouseButton b;

  // left and right click allowed
  if (event->button == 1)
    b = MouseLeft;
  else if (event->button == 3)
    b = MouseRight;

  // ignore other buttons
  else
    return false;

  // press or release?
  if (event->type == GDK_BUTTON_PRESS)
    state_manager->MousePress(b);
  else if (event->type == GDK_BUTTON_RELEASE)
    state_manager->MouseRelease(b);
  else
    return false;

  return true;
}

bool DrawingArea::on_motion_notify(GdkEventMotion* event) {

  state_manager->MouseMove(event->x, event->y);
  return true;
}

/**
 *
 */
void DrawingArea::init_state_manager(
    SharedState state
) {
    state_manager->SetInitialState(new TitleState(state));
}


