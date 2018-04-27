#ifndef KOMPILER_PARSETABLE_H
#define KOMPILER_PARSETABLE_H

#include <unordered_map>
#include <string>
#include "cfg.h"
#include "parsetable.pb.h"

#define EPS_STR "\\EPS"

class parsetable {

public:
    class entry {
    public:
        enum States {
            ERROR, SYNC, PROD
        };
        States state;
        std::vector<std::string> productions;

        entry();

        entry(States _state);
    };

    parsetable(cfg);

    parsetable();

    entry get_entry(std::string nonterm, std::string next_input);

    std::string get_starting_symbol_key() const;

    bool is_nonterm(std::string symbol);

    bool has_sync(std::string symbol);

    bool serialize(std::string file_name);

    bool deserialize(std::string file_name);

    friend std::ostream &operator<<(std::ostream &stream, const parsetable &t);

private:
    std::unordered_map<std::string, std::unordered_map<std::string, entry>> table;
    std::string starting_symbol_key;
};


#endif //KOMPILER_PARSETABLE_H
