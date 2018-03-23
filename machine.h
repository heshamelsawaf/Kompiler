//
// Created by hesham on 3/18/18.
// Initial definion to the Machine strcture
//

#ifndef KOMPILAR_MACHINE_H
#define KOMPILAR_MACHINE_H

#include <map>
#include <vector>
#include <string>
#include <set>
#define EPS 0x01

typedef int sid_t;

class machine {

    class state {

        class transition {
        private:
            sid_t to_identifier;
            char on_input;
        public:
            transition(sid_t, char);

            sid_t get_to_identifier() const;

            char get_transition_char() const;
        };

    private:
        std::map<char, std::vector<transition> > transitions;
        std::string token_class;
        std::string key;
    public:
        state();

        state(std::string _token_class);

        state(std::string _token_class, std::string _key);

        std::string get_token_class() const;

        bool add_new_transition(sid_t to_id, char on_input = EPS);

        void set_key(std::string key);

        void set_token_class(std::string token_class);

        std::string get_key() const;

        std::vector<sid_t> get_transitions_for(char input);

    };

private:
    std::string machine_identifier;
    std::vector<state> states;
    std::set<sid_t> accepting;
    std::set<char> language;
    sid_t starting;

public:
    machine();

    machine(std::string _machine_identifier);

    sid_t add_new_state(std::string key, std::string token_class = "",
                      bool is_starting = false, bool is_accepting = false);

    sid_t add_new_state(bool is_starting = false, bool is_accepting = false);

    sid_t add_new_state(std::string token_class = "", bool is_starting = false,
                        bool is_accepting = false);

    bool add_new_transition(sid_t from_id, sid_t to_id, char on_input = EPS);

    bool set_starting_state(sid_t _starting_id);

    sid_t add_starting_state(std::string key, std::string token_class = "",
                             bool is_accepting = false);

    sid_t get_starting_state() const;

    sid_t merge(machine other);

    std::string get_machine_identifier() const;

    std::set<sid_t> get_accepting_states() const;

    std::vector<sid_t> get_transitions(sid_t id, char input);

    std::string get_token_class(sid_t id) const;

    std::string get_key_for(sid_t id) const;

    void set_key_for(sid_t id, std::string new_key);


    std::set<char> get_language() const;

    sid_t get_states_count() const;

    bool set_accepting(sid_t id);

    bool set_token_class(sid_t id, std::string new_token_class);

    bool set_token_class(std::string new_token_class);

    void set_machine_identifier (std::string identifier);

    bool is_accepting(sid_t id);

    bool is_starting(sid_t id);

    void clear_accepting_states();

    void print_machine();

    friend std::ostream &operator <<(std::ostream &os, machine &m);

    friend std::istream &operator >>(std::istream &is, machine &m);
};


#endif //KOMPILAR_MACHINE_H
