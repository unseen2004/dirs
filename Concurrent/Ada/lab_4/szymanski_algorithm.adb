-- Szymanski_Algorithm.adb
-- Implementation of Szymanski's mutual exclusion algorithm

with Ada.Text_IO; use Ada.Text_IO;
with Ada.Integer_Text_IO; use Ada.Integer_Text_IO;

package body Szymanski_Algorithm is

   -- Protected object implementation
   protected body Atomic_Flags is
   
      procedure Set_Flag (Id : Process_Id; Value : Flag_Value) is
      begin
         Flags(Id) := Value;
      end Set_Flag;
      
      function Get_Flag (Id : Process_Id) return Flag_Value is
      begin
         return Flags(Id);
      end Get_Flag;
      
      function Get_All_Flags return Flags_Array is
      begin
         return Flags;
      end Get_All_Flags;
      
      -- await(all f[j] for j != i in {0,1,4})
      function All_Flags_In_Set (Exclude : Process_Id; Valid_Set : Flag_Value) return Boolean is
      begin
         for J in Process_Id loop
            if J /= Exclude then
               if Valid_Set = 0 then
                  -- Check if flag is in {0,1,4}
                  if Flags(J) /= 0 and Flags(J) /= 1 and Flags(J) /= 4 then
                     return False;
                  end if;
               elsif Valid_Set = 1 then
                  -- Check if flag is in {0,1}
                  if Flags(J) /= 0 and Flags(J) /= 1 then
                     return False;
                  end if;
               end if;
            end if;
         end loop;
         return True;
      end All_Flags_In_Set;
      
      -- await(any f[j] for j > i in {2,3})
      function Any_Flag_In_Set (Exclude : Process_Id; Valid_Set : Flag_Value) return Boolean is
      begin
         for J in Process_Id loop
            if J > Exclude then
               if Flags(J) = 2 or Flags(J) = 3 then
                  return True;
               end if;
            end if;
         end loop;
         return False;
      end Any_Flag_In_Set;
      
      -- await(all f[j] for j > i in {0,1})
      function All_Higher_Flags_In_Set (Id : Process_Id; Valid_Set : Flag_Value) return Boolean is
      begin
         for J in Process_Id loop
            if J > Id then
               if Flags(J) /= 0 and Flags(J) /= 1 then
                  return False;
               end if;
            end if;
         end loop;
         return True;
      end All_Higher_Flags_In_Set;
      
   end Atomic_Flags;

   -- Process task implementation
   task body Process_Task is
      Current_State : Process_State := Local_Section;
      Local_Flags : Flags_Array;
      Loop_Counter : Natural := 0;
   begin
      
      loop
         case Current_State is
         
            when Local_Section =>
               -- Display current state
               Local_Flags := Shared_Flags.Get_All_Flags;
               Display_State(Id, Current_State, Local_Flags);
               
               -- Simulate local work
               delay 2.0;
               
               -- Start entry protocol
               Current_State := Entry_Protocol_1;
               
            when Entry_Protocol_1 =>
               -- f[i] := 1
               Shared_Flags.Set_Flag(Id, 1);
               Local_Flags := Shared_Flags.Get_All_Flags;
               Display_State(Id, Current_State, Local_Flags);
               
               Current_State := Entry_Protocol_2;
               
            when Entry_Protocol_2 =>
               -- await(all f[j] for j != i in {0,1,4})
               -- Implementation with loop
               loop
                  if Shared_Flags.All_Flags_In_Set(Id, 0) then
                     exit;
                  end if;
                  delay 0.1; -- Small delay to avoid busy waiting
               end loop;
               
               -- f[i] := 2
               Shared_Flags.Set_Flag(Id, 2);
               Local_Flags := Shared_Flags.Get_All_Flags;
               Display_State(Id, Current_State, Local_Flags);
               
               Current_State := Entry_Protocol_3;
               
            when Entry_Protocol_3 =>
               -- f[i] := 3
               Shared_Flags.Set_Flag(Id, 3);
               Local_Flags := Shared_Flags.Get_All_Flags;
               Display_State(Id, Current_State, Local_Flags);
               
               -- await(any f[j] for j > i in {2,3}) or (all f[j] for j > i in {0,1})
               -- Implementation with loop
               loop
                  if Shared_Flags.Any_Flag_In_Set(Id, 2) or 
                     Shared_Flags.All_Higher_Flags_In_Set(Id, 1) then
                     exit;
                  end if;
                  delay 0.1;
               end loop;
               
               Current_State := Entry_Protocol_4;
               
            when Entry_Protocol_4 =>
               -- f[i] := 4
               Shared_Flags.Set_Flag(Id, 4);
               Local_Flags := Shared_Flags.Get_All_Flags;
               Display_State(Id, Current_State, Local_Flags);
               
               -- await(all f[j] for j != i in {0,1})
               -- Implementation with loop
               loop
                  if Shared_Flags.All_Flags_In_Set(Id, 1) then
                     exit;
                  end if;
                  delay 0.1;
               end loop;
               
               Current_State := Critical_Section;
               
            when Critical_Section =>
               Local_Flags := Shared_Flags.Get_All_Flags;
               Display_State(Id, Current_State, Local_Flags);
               
               -- Simulate critical section work
               Put_Line("*** Process" & Process_Id'Image(Id) & " in CRITICAL SECTION ***");
               delay 1.0;
               
               Current_State := Exit_Protocol;
               
            when Exit_Protocol =>
               -- Exit protocol: wait until we can change f[i] from 4 to 0
               -- In this simple version, we can immediately set to 0
               Shared_Flags.Set_Flag(Id, 0);
               Local_Flags := Shared_Flags.Get_All_Flags;
               Display_State(Id, Current_State, Local_Flags);
               
               Put_Line("Process" & Process_Id'Image(Id) & " exited critical section");
               
               Current_State := Local_Section;
               
               -- Increment loop counter and exit after a few iterations
               Loop_Counter := Loop_Counter + 1;
               exit when Loop_Counter >= 3;
               
         end case;
         
         delay 0.5; -- Small delay between state transitions
         
      end loop;
      
      Put_Line("Process" & Process_Id'Image(Id) & " finished");
      
   end Process_Task;

   -- Display current state procedure
   procedure Display_State (Id : Process_Id; State : Process_State; Flags : Flags_Array) is
   begin
      Put("Process" & Process_Id'Image(Id) & " - " & Process_State'Image(State) & " | Flags: [");
      for I in Process_Id loop
         Put(Flag_Value'Image(Flags(I)));
         if I < Process_Id'Last then
            Put(",");
         end if;
      end loop;
      Put_Line("]");
   end Display_State;

end Szymanski_Algorithm;