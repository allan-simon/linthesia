#include "OSGraphics.h"
#include "SharedState.h"

class EdgeTracker;
class GameStateManager;

class DrawingArea : public Gtk::GL::DrawingArea {
    public:
        DrawingArea(
            const Glib::RefPtr<
                const Gdk::GL::Config
            >& config
        );

        ~DrawingArea();

        bool GameLoop();
        virtual void init_state_manager(SharedState state);

    protected:
        virtual bool on_configure_event(GdkEventConfigure* event);
        virtual bool on_expose_event(GdkEventExpose* event);

        virtual bool on_motion_notify(GdkEventMotion* event);
        virtual bool on_button_press(GdkEventButton* event);
        virtual bool on_key_press(GdkEventKey* event);
        virtual bool on_key_release(GdkEventKey* event);


    private:
        EdgeTracker* windowState;
        GameStateManager* state_manager;

};


