with Ada.Calendar;
with Ada.Numerics.Float_Random;

package body Random_Seeds is

   function Make_Seeds (Count : Positive) return Seed_Array_Type is
      Seeds : Seed_Array_Type(1 .. Count);
      Base_Seed : Integer;
      G : Ada.Numerics.Float_Random.Generator;
   begin
      -- **BASE SEED**: Use current time for randomization
      Base_Seed := Integer(Ada.Calendar.Seconds(Ada.Calendar.Clock));
      
      -- **SEED DIVERSIFICATION**: Create unique seeds for each task
      Ada.Numerics.Float_Random.Reset(G, Base_Seed);
      
      for I in Seeds'Range loop
         Seeds(I) := Base_Seed + I * 17 + Integer(1000.0 * Ada.Numerics.Float_Random.Random(G));
      end loop;
      
      return Seeds;
   end Make_Seeds;

end Random_Seeds;
