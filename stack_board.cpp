/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                           *
 *  This file is part of Library of Effective GO routines - EGO library      *
 *                                                                           *
 *  Copyright 2006, 2007 Lukasz Lew                                          *
 *                                                                           *
 *  EGO library is free software; you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by     *
 *  the Free Software Foundation; either version 2 of the License, or        *
 *  (at your option) any later version.                                      *
 *                                                                           *
 *  EGO library is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *  GNU General Public License for more details.                             *
 *                                                                           *
 *  You should have received a copy of the GNU General Public License        *
 *  along with EGO library; if not, write to the Free Software               *
 *  Foundation, Inc., 51 Franklin St, Fifth Floor,                           *
 *  Boston, MA  02110-1301  USA                                              *
 *                                                                           *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


// class stack_board_t


class stack_board_t {

public:

  board_t   stack [max_stack_size];
  board_t*  stack_top;

public: 
  
  void check () {
    if (!stack_board_ac) return;
    assert (stack_top < stack + max_stack_size);
  }

  stack_board_t () {
    clear ();
  }

  void clear () {
    stack_top = stack;  
    stack_top->clear ();
  }

  board_t const* act_board () const { 
    return stack_top; 
  }

  bool try_play (player_t player, vertex_t v) {

    record_state (); // for undo

    if (v == vertex_pass) return true;

    if (v == vertex_resign) return true;

    if (stack_top->color_at [v] != color_empty) 
      { revert_state (); return false; }

    if (stack_top->play_no_pass (player, v) != play_ok)
      { revert_state (); return false; }

    if (is_hash_repeated ())    // superko test
      { revert_state (); return false; }

    return true;
  }

  bool try_undo () {
    if (stack_top <= stack) return false;
    revert_state ();
    return true;
  }

  bool is_legal (player_t pl, vertex_t v) {            // slow function
    bool undo_res;
    if (!try_play (pl, v)) return false;
    undo_res = try_undo ();
    assertc (stack_board_ac, undo_res == true);

    return true;
  }

  void record_state () {
    (stack_top+1)->load (stack_top); // for undo-ing
    stack_top++;
    check ();
  }

  void revert_state () {
    stack_top--;
    check ();
  }

  bool is_hash_repeated () const {
    for (board_t* b = stack_top - 1; b >= stack; b--)
      if (b->hash == stack_top->hash) return true;
    return false;
  }

  void set_komi (float komi) {  // TODO is it proper semantics ?
    for (board_t* b = stack; b <= stack_top; b++) b->set_komi (komi); 
  }

  bool load (istream& ifs) {
    board_t tmp[1];
    if (!tmp->load (ifs)) return false;
    clear ();
    stack_top->load (tmp);
    return true;
  }

};

