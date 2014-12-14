#ifndef LINTHESIA_SCORE_H
#define LINTHESIA_SCORE_H

namespace linthesia {
    struct Score {
        unsigned missed = 0;
        unsigned soso = 0;
        unsigned good = 0;
        unsigned excellent = 0;
        unsigned perfect = 0;

        /**
         *
         */
        void reset() {
            missed = 0;
            soso = 0;
            good = 0;
            excellent = 0;
            perfect = 0;
        }

        /**
         * Mark a note base on its delta time (i.e
         * difference between the time at which it was supposed to
         * be played and time at which it has been played)
         */
        void markNote(int delta) {
            if (delta < 0) {
                missed += 1;
                return;
            }

            // 100ms
            if (delta < 100*1000) {
                perfect += 1 ;
                return;
            }

            // 200ms
            if (delta < 200*1000) {
                excellent += 1; 
                return;
            }

            // 300ms
            if (delta < 300*1000) {
                good += 1; 
                return;
            }

            // other (too late/too early are already handled)
            soso += 1;
        }
    };

}

#endif

