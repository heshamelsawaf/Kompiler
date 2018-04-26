//
// Created by hesham on 3/21/18.
//

#include "rexparser.h"
#include <sstream>
#include <algorithm>
#include "rexplib.h"
#include <iostream>


std::string replace_escape (std::string s);

// trim from start (in place)
static inline void ltrim (std::string &s)
{
  s.erase (s.begin (), std::find_if (s.begin (), s.end (), [] (int ch)
  {
    return !std::isspace (ch);
  }));
}

// trim from end (in place)
static inline void rtrim (std::string &s)
{
  s.erase (std::find_if (s.rbegin (), s.rend (), [] (int ch)
  {
    return !std::isspace (ch);
  }).base (), s.end ());
}

// trim from both ends (in place)
static inline void trim (std::string &s)
{
  ltrim (s);
  rtrim (s);
}

// trim from both ends (copying)
static inline std::string trim_copy (std::string s)
{
  trim (s);
  return s;
}

machine rexparser::rules2nfa (const std::string rules)
{
  std::stringstream iss (rules);

  while (iss.good ())
    {
      std::string SingleLine;
      getline (iss, SingleLine, '\n');
      process_line (trim_copy (SingleLine));
    }

  machine nfa = machine_ops::oring (regex);
  //Process NFA first;
  return nfa;
}

void rexparser::process_line (const std::string line)
{
  if (line.empty ())
    return;

  /*Identify line type; supported types are:
   *
   * Regular definitions are lines in the form LHS = RHS.
   * Regular expressions are lines in the form LHS: RHS.
   * Keywords are enclosed by { } in separate lines.
   * Punctuations are enclosed by [ ] in separate lines.
   */

  if ((line.find_first_of ("{") == (size_t) 0) &&
      (line.find_last_of ("}") == (line.size () - 1)))
    {
      handler_reserved (trim_copy (line.substr (1, line.size () - 2)));
    }
  else if ((line.find_first_of ("[") == (size_t) 0) &&
           (line.find_last_of ("]") == (line.size () - 1)))
    {
      handler_reserved (trim_copy (line.substr (1, line.size () - 2)));
    }
  else if (line.find_first_of ("=:") != std::string::npos)
    {
      size_t index = line.find_first_of ("=:");
      std::string machine_identifier = trim_copy (line.substr (0, index));
      machine m = handler_regular (trim_copy (line.substr (index + 1)));
      m.set_machine_identifier (machine_identifier);
      machines.insert (std::make_pair (machine_identifier, m));
      if (line.at (index) == ':')
        {
          m.set_token_class (machine_identifier);
          regex.push_back (m);
        }
    }
  else
    {
      //ERROR!
      //TODO:: Handle errors gracefully
    }

  return;
}

void
rexparser::push_to_stack (std::string s, std::stack<machine_stacks> &stk, std::map<std::string, machine> &_machines)
{
  machine_stacks ms;
  ms.is_operator = false;
  if (stk.empty () || stk.top ().is_operator)
    {
      machine m = get_hashed_machine (s, _machines);
      ms.identifier = s;
    }
  else
    {
      machine_stacks bottom = stk.top ();
      stk.pop ();
      machine m = machine_ops::concat (get_hashed_machine (bottom
                                                             .identifier, _machines), get_hashed_machine (s, _machines));
      ms.identifier = bottom.identifier + s;
      _machines.insert (std::make_pair (ms.identifier, m));
    }
  stk.push (ms);
}

machine rexparser::handler_regular (const std::string line)
{
  std::stack<machine_stacks> st_regex;
  std::map<std::string, machine> line_machines;
  size_t index = -1;
  int matching_parentheses = 0;
  std::string holder;

  while (++index != line.size ())
    {
      char current = line.at (index);
      if (isseparator (current))
        {
          if (!holder.empty ())
            {
              push_to_stack (holder, st_regex, line_machines);
              holder = "";
            }

          if (isspace (current))
            continue;
        }
      if (current == '(')
        {
          machine_stacks ms;
          ms.identifier = "(", ms.is_operator = true;
          st_regex.push (ms), matching_parentheses++;
        }
      else if (current == ')')
        {

          if (!matching_parentheses || st_regex.top ().is_operator)
            {
              throw std::invalid_argument ("Closing parenthesis location mismatched\n(" + line + ")");
            }
          matching_parentheses--;
          std::vector<std::string> possible_result;
          std::vector<machine> or_machines;
          std::string new_machine_identifier;

          while (!st_regex.empty ())
            {
              machine_stacks _top = st_regex.top ();
              st_regex.pop ();
              if (!_top.identifier.compare ("("))
                break;
              else if (_top.is_operator && !_top.identifier.compare ("|"))
                continue;
              possible_result.push_back (_top.identifier);
            }

          for (int i = possible_result.size () - 1; i >= 0; i--)
            {
              or_machines.push_back (get_hashed_machine (possible_result.at (i), line_machines));
              new_machine_identifier += possible_result.at (i);
              if (i != 0)
                new_machine_identifier += "|";
            }

          if (!or_machines.empty ())
            {
              line_machines.insert (std::make_pair (new_machine_identifier, machine_ops::oring (or_machines)));
              holder = new_machine_identifier;
            }
        }
      else if (current == '|')
        {
          machine_stacks ms;
          ms.identifier = "|", ms.is_operator = true;
          if (!st_regex.empty () && st_regex.top ().is_operator)
            {
              throw std::invalid_argument ("'|'-OR Can not be applied to operators\n(" + line + ")");
            }
          st_regex.push (ms);
        }
      else if (current == '-')
        {
          if ((index + 1 == line.size ()) || holder.size () != 1)
            {
              throw std::invalid_argument ("error encounterd in range operator(-)\n(" + line + ")");
            }
          index++;
          std::string identifier = holder + "-" + line.at (index);
          line_machines.insert (std::make_pair (identifier, machine_ops::char_range (holder.at (0), line.at (index))));
          machine_stacks ms;
          ms.identifier = identifier, ms.is_operator = false;
          push_to_stack (identifier, st_regex, line_machines);
          holder = "";
        }
      else if (current == '*' || current == '+')
        {
          if (holder.empty ())
            {
              throw std::invalid_argument (
                "[*+] operators must be preceded directly by a valid definition\n(" + line + ")");
            }
          std::string identifier = holder + current;
          if (current == '*')
            line_machines
              .insert (std::make_pair (identifier, machine_ops::star (get_hashed_machine (holder, line_machines))));
          else if (current == '+')
            line_machines
              .insert (std::make_pair (identifier, machine_ops::plus (get_hashed_machine (holder, line_machines))));
          machine_stacks ms;
          ms.identifier = identifier, ms.is_operator = false;
          push_to_stack (identifier, st_regex, line_machines);
          holder = "";
        }
      else if (current == '\\')
        {
          if (index + 1 == line.size ())
            {
              throw std::invalid_argument ("Invalid escape character at the end of the line\n(" + line + ")");
            }
          index++;
          if (line.at (index) == 'L')
              holder += EPS_CHAR;
          else
            holder += line.at (index);
        }
      else
        {
          holder += current;
        }
    }

  if (!holder.empty ())
    {
      push_to_stack (holder, st_regex, line_machines);
      holder = "";
    }

  std::vector<std::string> possible_result;
  std::vector<machine> or_machines;

  // Create Final Machine
  while (!st_regex.empty ())
    {
      machine_stacks _top = st_regex.top ();
      st_regex.pop ();
      if (_top.is_operator && !_top.identifier.compare ("|"))
        continue;
      possible_result.push_back (_top.identifier);
    }

  for (int i = possible_result.size () - 1; i >= 0; i--)
    {
      or_machines.push_back (get_hashed_machine (possible_result.at (i), line_machines));
    }
  return machine_ops::oring (or_machines);
}

machine rexparser::get_hashed_machine (std::string key, std::map<std::string, machine> &_machines)
{
  if (machines.count (key))
    return machines.find (key)->second;
  else if (_machines.count (key))
    return _machines.find (key)->second;
  _machines.insert (std::make_pair (key, machine_ops::string_concat (key)));
  return _machines.find (key)->second;
}

bool rexparser::isseparator (char c)
{
  return isspace (c) || c == '|' || c == '(' || c == ')';
}

void rexparser::handler_reserved (const std::string line)
{
  std::stringstream iss (line);

  while (iss.good ())
    {
      std::string SingleLine;
      getline (iss, SingleLine, ' ');
      SingleLine = trim_copy (SingleLine);
      machine m = handler_regular (SingleLine);
      SingleLine = replace_escape (SingleLine);
      m.set_token_class (SingleLine);
      m.set_machine_identifier (SingleLine);
      machines.insert (std::make_pair (SingleLine, m));
      regex.insert (regex.begin (), m);
    }

}

std::string replace_escape (std::string s){
  std::string r = "";

  for (size_t i = 0; i < s.size (); i++){
      if (i != s.size () - 1 && s.at (i) == '\\')
        r += s.at (++i);
      else
        r += s.at (i);
    }
  return r;
}
