#ifndef PLUGINS_PATTERN_H
#define PLUGINS_PATTERN_H


#include <vector>
#include <Common.h>


namespace brainlib {

    // FIXME TODO allow for pattern expansion and sub-pattern removal

    class Pattern {
        typedef std::vector<Unit> pattern_t;
        typedef pattern_t::iterator pattern_it;

    public:
        Pattern(Unit *p, Unit r, int l) :
        pattern(l), resp(r), length(l) {
            for (int i = 0; i < length; ++i) { pattern[i] = p[i]; }
        }

        int size() {
            return length;
        }

		Unit &operator[](int i) {
            return pattern[i];
        }

		Unit get(int i) {
            return pattern[i];
        }

		Unit response() {
            return resp;
        }

    private:
        pattern_t pattern;
        Unit      resp;
        int       length;
    };



    bool operator==(Pattern &a, Pattern &b) {
        if (a.size() == b.size()) {
            int i = a.size() - 1;
            while ( (a[i] == b[i]) && (i-- >= 0) ) {}
            return (i == 0);
        } else {
            return false;
        }
    }


};


#endif
