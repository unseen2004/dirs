package Random_Seeds is
   type Seed_Array_Type is array (Integer range <>) of Integer;
   function Make_Seeds(Count : Integer) return Seed_Array_Type;
end Random_Seeds;
