class EdgeTracker  {
    public:
        EdgeTracker();
        void activate();
        void deactivate();
        bool is_active();
        bool is_not_active();
        bool just_activated();

    private:
        bool active;
        bool justActive;
};



