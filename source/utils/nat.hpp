//
// Copyright 2006 and onwards, Lukasz Lew
//

#ifndef NAT_H_
#define NAT_H_

#include "utils.hpp"

// -----------------------------------------------------------------------------
// For a use case look in player.h

template <class T>
class Nat {
 public:
  // Constructors.

  Nat ();  // TODO replace default constructor with Invalid.
  static T OfRaw (uint raw);
  static T Invalid();

  // Utils.

  bool MoveNext();
  uint GetRaw() const;
  bool IsValid() const;
  bool operator == (const Nat& other) const;
  bool operator != (const Nat& other) const;

 protected:
  explicit Nat (uint raw);

 private:
  uint raw;
  const static bool kCheckAsserts = false;
};

#define ForEachNat(T, var) for (T var = T::Invalid(); var.MoveNext(); )

// -----------------------------------------------------------------------------
// NatMap<Nat, Elt> is an array that can be indexed by Nat

template <typename Nat, typename Elt>
class NatMap {
 public:
  NatMap (const Elt& init = Elt());
  Elt& operator[] (Nat nat);
  const Elt& operator[] (Nat nat) const;
  void SetAll (const Elt& elt);
  void SetAllToZero (); // Assumes POD of Elt, uses memset
  void Scale (Elt min_val, Elt max_val);
  void ScalePositive ();
  void Load (const NatMap& other);

 private:
  Elt tab [Nat::kBound];
  const static bool kCheckAsserts = false;
};

// -----------------------------------------------------------------------------

#include "nat-inl.hpp"

#endif // NAT_H_
