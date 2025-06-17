-- Szymanski_Algorithm.ads
-- Specification for Szymanski's mutual exclusion algorithm demonstration

package Szymanski_Algorithm is

   -- Number of processes
   N : constant := 3;
   
   -- Process identifiers
   type Process_Id is range 1 .. N;
   
   -- Process states with flag values in names
   type Process_State is (
      Local_Section,
      Entry_Protocol_1,    -- f[i] = 1
      Entry_Protocol_2,    -- f[i] = 2
      Entry_Protocol_3,    -- f[i] = 3
      Entry_Protocol_4,    -- f[i] = 4
      Critical_Section,
      Exit_Protocol
   );
   
   -- Flag values for the algorithm
   type Flag_Value is range 0 .. 4;
   
   -- Shared flags array
   type Flags_Array is array (Process_Id) of Flag_Value;
   
   -- Protected object for atomic operations on flags
   protected type Atomic_Flags is
      procedure Set_Flag (Id : Process_Id; Value : Flag_Value);
      function Get_Flag (Id : Process_Id) return Flag_Value;
      function Get_All_Flags return Flags_Array;
      
      -- Helper functions for await conditions
      function All_Flags_In_Set (Exclude : Process_Id; Valid_Set : Flag_Value) return Boolean;
      function Any_Flag_In_Set (Exclude : Process_Id; Valid_Set : Flag_Value) return Boolean;
      function All_Higher_Flags_In_Set (Id : Process_Id; Valid_Set : Flag_Value) return Boolean;
      
   private
      Flags : Flags_Array := (others => 0);
   end Atomic_Flags;
   
   -- Process task type
   task type Process_Task (Id : Process_Id; Shared_Flags : access Atomic_Flags);
   
   -- Procedure to display current state
   procedure Display_State (Id : Process_Id; State : Process_State; Flags : Flags_Array);
   
end Szymanski_Algorithm;