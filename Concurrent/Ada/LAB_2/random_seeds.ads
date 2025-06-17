-- **RANDOM SEED UTILITIES**: Provides unique seeds for concurrent tasks
-- This ensures each task has independent random number generation

package Random_Seeds is

   type Seed_Array_Type is array (Positive range <>) of Integer;

   -- **SEED GENERATION**: Creates array of unique seeds
   -- Used to initialize random generators for travelers and field tasks
   function Make_Seeds (Count : Positive) return Seed_Array_Type;

end Random_Seeds;
