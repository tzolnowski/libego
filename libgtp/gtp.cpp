#include <boost/algorithm/string/trim.hpp>
#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>
#include <iostream>
#include "gtp.h"

namespace Gtp {

Io::Io (const string& params_) : params (params_), success(true), quit_gtp (false) { 
}

void Io::SetError (const string& message) {
  out.str (message);
  success = false;
}

void Io::CheckEmpty() {
  string s;
  in >> s;
  if (in) {
    SetError ("too many parameters");
    throw Repl::Return();
  }
  in.clear();
}

bool Io::IsEmpty() {
  string s;
  std::streampos pos = in.tellg();
  in >> s;
  bool ok = in;
  in.seekg(pos);
  in.clear();
  return !ok;
}

void Io::PrepareIn () {
  in.clear();
  in.str (params);
}

string Io::Report () const {
  return boost::trim_right_copy (out.str()); // remove bad endl in msg
}

// -----------------------------------------------------------------------------

// IMPROVE: can't Static Aux be anonymous function

void StaticAux(const string& ret, Io& io) {
  io.CheckEmpty ();
  io.out << ret;
}

Repl::Callback StaticCommand (const string& ret) {
  return boost::bind(StaticAux, ret, _1);
}

// -----------------------------------------------------------------------------

Repl::Repl () {
  Register ("list_commands", this, &Repl::CListCommands);
  Register ("help",          this, &Repl::CListCommands);
  Register ("known_command", this, &Repl::CKnownCommand);
  Register ("quit",          this, &Repl::CQuit);
}

void Repl::Register (const string& name, Callback callback) {
  callbacks[name].push_back (callback);
}

void Repl::RegisterStatic (const string& name, const string& response) {
  Register (name, StaticCommand(response));
}

void ParseLine (const string& line, int* id, string* command, string* rest) {
  stringstream ss;
  BOOST_FOREACH (char c, line) {
    if (false) {}
    else if (c == '\t') ss << ' ';
    else if (c == '\n') assert (false);
    else if (c <= 31 || c == 127) continue;
    else if (c == '#') break;  // remove comments
    else ss << c;
  }
  *id = -1;
  *command = "";
  ss >> *id;
  if (!ss) {
    ss.clear ();
    ss.seekg (ios_base::beg);
  }
  ss >> *command;
  getline (ss, *rest);
}

void Repl::Run (istream& in, ostream& out) {
  in.clear();
  while (true) {
    int id;
    string line, command, params;

    if (!getline (in, line)) break;
    ParseLine (line, &id, &command, &params);
    Io io(params);

    if (command == "") {
      continue;
    } else if (IsCommand (command)) {
      // Callback call with optional fast return.
      BOOST_FOREACH (Callback& cmd, callbacks [command]) {
        io.PrepareIn();
        try { cmd (io); } catch (Return) { }
      }
    } else {
      io.SetError ("unknown command: \"" + command + "\"");
    }

    out << (io.success ? "=" : "?") << " "
        << io.Report ()
        << endl << endl;

    if (io.quit_gtp) return;
  }
}

void Repl::CListCommands (Io& io) {
  io.CheckEmpty();
  pair<string, list<Callback> > p;
  BOOST_FOREACH(p, callbacks) {
    io.out << p.first << endl;
  }
}

void Repl::CKnownCommand (Io& io) {
  io.out << boolalpha << IsCommand(io.Read<string> ());
  io.CheckEmpty();
}

void Repl::CQuit (Io& io) {
  io.CheckEmpty();
  io.out << "bye";
  io.quit_gtp = true;
}

bool Repl::IsCommand (const string& name) {
  return callbacks.find(name) != callbacks.end();
}

} // end of namespace Gtp
