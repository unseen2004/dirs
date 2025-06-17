with Ada.Calendar;
package body Random_Seeds is
   function Make_Seeds(Count : Integer) return Seed_Array_Type is
      Seeds : Seed_Array_Type(1..Count);
      Base_Seed : Integer := Integer(Ada.Calendar.Seconds(Ada.Calendar.Clock));
   begin
      for I in Seeds'Range loop
         Seeds(I) := Base_Seed + I * 1000;
      end loop;
      return Seeds;
   end Make_Seeds;
end Random_Seeds;
