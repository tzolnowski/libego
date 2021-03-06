#ifndef SGF_GTP_H_
#define SGF_GTP_H_

#include "full_board.hpp"
#include "sgf.hpp"
#include "gtp.hpp"

class SgfGtp {
public:
  SgfGtp (Gtp::Repl& _gtp, SgfTree& _sgf_tree, FullBoard& _base_board);
  virtual ~SgfGtp () {}
  void exec_embedded_gtp_rec (SgfNode* current_node, ostream& response);

 private:
  void CLoad (Gtp::Io& io);
  void CSave (Gtp::Io& io);
  void CGtpExec (Gtp::Io& io);
  
  SgfTree&   sgf_tree;
  Gtp::Repl& gtp;
  FullBoard& base_board;
};

#endif
