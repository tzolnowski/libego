//
// Copyright 2006 and onwards, Lukasz Lew
//

#ifndef PLAYER_H_
#define PLAYER_H_

class Player : public Nat <Player> { // TODO check is check always checked in constructors
public:

  // Constructors.

  Player() {} // TODO remove it after Gtp::Io::Read <Player> problem is resolved.

  static Player Black ();
  static Player White ();
  static Player OfGtpString (const std::string& s);

  // Utilities.

  Player Other() const;
  int ToScore () const;   // ToScore (Black()) == 1, ToScore (White()) == -1
  string ToGtpString () const;

  template <typename T>
  T SubjectiveScore (const T& score) const;

  static const uint kBound = 2;

 private:
  friend class Nat <Player>;
  explicit Player (uint raw);
};

// TODO move this to GTP implementation.
istream& operator>> (istream& in, Player& pl);

// TODO move this to player-impl.h
// -----------------------------------------------------------------------------
// internal

template <typename T>
T Player::SubjectiveScore (const T& score) const {
  T tab[2];
  tab[0] = score;
  tab[1] = - score;
  return tab [GetRaw()];
}

#endif
