#ifndef _GTP_GOGUI_H_
#define _GTP_GOGUI_H_

#include <string>
#include <map>
#include <boost/bind.hpp>

#include "gtp.h"

namespace Gtp {
namespace Gogui {

using namespace std;

// -----------------------------------------------------------------------------

class Analyze : public Repl {
public:
  Analyze ();

  void RegisterGfx (const string& name, const string& params, Callback command);

  template <class T>
  void RegisterGfx (const string& name,
                    const string& params,
                    T* object,
                    void(T::*member)(Io&));

  template <typename T>
  void RegisterParam (const string& cmd_name, const string& param_name, T* param);

private:
  void CAnalyze (Io&);
  void CParam (const string& cmd_name, Io& io);

private:
  stringstream analyze_list;
  map <string, map <string, Callback> > params; // params [cmd_name] [param_name]
};

// -----------------------------------------------------------------------------
// internal

template <class T>
void Analyze::RegisterGfx (const string& name,
                           const string& params,
                           T* object,
                           void(T::*member)(Io&))
{
  RegisterGfx (name, params, OfMethod(object, member));
}

template <typename T>
void Analyze::RegisterParam (const string& cmd_name,
                             const string& param_name,
                             T* param)
{
  params [cmd_name] [param_name] = GetSetCommand (param);
  if (IsCommand (cmd_name)) return;
  analyze_list << "param/" << cmd_name << "/" << cmd_name << endl; // NOTE: factor out
  Register (cmd_name, boost::bind (&Analyze::CParam, this, cmd_name, _1));
}

} // namespace Gogui
} // namespace Gtp

#endif
