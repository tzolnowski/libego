//
// Copyright 2006 and onwards, Lukasz Lew
//

#ifndef HASH_H_
#define HASH_H_

#include "utils.hpp"
#include "fast_random.hpp"
#include "move.hpp"

class Hash {
public:
  Hash ();
  uint Index () const;
  uint Lock  () const;

  void Randomize (FastRandom& fr);
  void SetZero();

  bool operator== (const Hash& other) const;
  void operator^= (const Hash& other);

private:  
  uint64 hash;
};

// -----------------------------------------------------------------------------

class Zobrist {
public:
  Zobrist();
  Hash OfMove (Move m) const;
  Hash OfPlayerVertex (Player pl,  Vertex v) const;

private:
  NatMap<Move, Hash> hashes;
};


// -----------------------------------------------------------------------------


class Hash3x3 : public Nat <Hash3x3> {
public:
  explicit Hash3x3 () : Nat <Hash3x3>() {};

  
  // ataris have to be marked manually
  Hash3x3 OfBoard (const NatMap <Vertex, Color>& color_at, Vertex v) {
    // bit mask from least significant
    // N, E, S, W, NW, NE, SE, SW, aN, aE, aS, sW
    // 2  2  2  2   2   2   2   2   1   1   1   1
    uint raw = 0;
    ForEachNat (Dir, dir) {
      raw |= color_at [v.Nbr(dir)].GetRaw() << (2*dir.GetRaw());
    }

    return Hash3x3::OfRaw (raw);
  }

  Color ColorAt (Dir dir) const {
    return Color::OfRaw (GetRaw() & (3 << (2*dir.GetRaw())));
  }

  void SetColorAt (Dir dir, Color color) {
    raw &= ~(3 << (2*dir.GetRaw()));
    raw |= color.GetRaw() << (2*dir.GetRaw());
  }

  // TODO atari stuff

  static const uint kBound = 1 << 20;
private:

  friend class  Nat <Hash3x3>;
  explicit Hash3x3 (uint raw) : Nat <Hash3x3> (raw) {}
  const static bool kCheckAsserts = false;
};

#endif
